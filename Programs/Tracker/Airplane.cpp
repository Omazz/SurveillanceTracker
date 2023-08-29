#include "Airplane.h"

Airplane::Airplane(QObject *parent)
    : QObject{parent}
{

}

Airplane::Airplane(QVector<Plot> initPlots, quint16 trackNumber, QObject* parent)
    : QObject(parent), m_trackNumber(trackNumber) {

    m_velocity = (initPlots.last().cartesianCoords() - initPlots.first().cartesianCoords()) /
                    (initPlots.last().time() - initPlots.first().time());

    m_directionAngle = qDegreesToRadians(Plot::fromDecartToPolar(m_velocity.y(), m_velocity.x()).second);

    m_oldVelocity = velocity();

    m_filteredPlot = initPlots.last();

    m_oldAngle = m_filteredPlot.angle();

    initializationMNK(initPlots);

    m_measuredPlot = initPlots.last();

    m_numberSteps = 3;
    m_numberStepsMax = 7;

    QTimer* timer = new QTimer(this);
    m_timer = QSharedPointer<QTimer>(timer);
    connect(m_timer.data(), &QTimer::timeout, this, &Airplane::onTimeout);
    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setSingleShot(true);
    m_timer->setInterval(SettingsTracker::SCAN_MSECS + SettingsTracker::WAIT_INFO_MSECS);
    m_timer->start();
}


Airplane::~Airplane() {

}

void Airplane::setTrack(Plot measuredPlot) {
    m_counterExtrapolations = 0;

    m_measuredPlot = measuredPlot;

    if(m_numberSteps < m_numberStepsMax) {
        m_alpha = static_cast<qreal>((2.0 * ((2.0 * m_numberSteps) - 1.0)) / (m_numberSteps * (m_numberSteps + 1.0)));
        m_beta = static_cast<qreal>(6.0 / (m_numberSteps * (m_numberSteps + 1.0)));
    }

    m_numberSteps++;

    m_oldVelocity = velocity();
    m_oldAngle = m_filteredPlot.angle();
    qreal oldTime = m_filteredPlot.time();
    qreal currentTime = static_cast<qreal>(QDateTime::currentMSecsSinceEpoch() / 1000.0);

    std::pair<Plot, QPointF> resultExtrapolation = doExtrapolation(currentTime);
    Plot extrapolatedPlot = resultExtrapolation.first;
    QPointF extrapolatedVelocity = resultExtrapolation.second;

    m_filteredPlot = Plot(extrapolatedPlot.cartesianCoords() +
                            (m_alpha * (measuredPlot.cartesianCoords() - extrapolatedPlot.cartesianCoords())),
                         measuredPlot.frequencyDoppler(),
                         measuredPlot.amplitude(),
                         measuredPlot.time(),
                         measuredPlot.asterixPlot());

    m_velocity = extrapolatedVelocity + (m_beta / (currentTime - oldTime)
                                        * (measuredPlot.cartesianCoords() - extrapolatedPlot.cartesianCoords()));

    m_directionAngle = qDegreesToRadians(Plot::fromDecartToPolar(m_velocity.y(), m_velocity.x()).second);

    replaceQueue(m_filteredPlot);
}

bool Airplane::isManeuverAngle() const {
    qreal differenceAngles = qAbs(m_filteredPlot.angle() - m_oldAngle);
    if(differenceAngles > 180.0) {
       differenceAngles = 360.0 - differenceAngles;
    }
    return qAbs(differenceAngles) >= SettingsTracker::MANEUVER_ANGLE_DEG;
}

bool Airplane::isManeuverVelocity() const {
    return qAbs(velocity() - m_oldVelocity) >= SettingsTracker::MANEUVER_VELOCITY_M_SECS;
}

void Airplane::initializationMNK(QVector<Plot> initPlots) {
    int i = (initPlots.size() == 4) ? 1 : 0;
    for(; i < initPlots.size(); ++i) {
        m_coordsMNK.enqueue(initPlots[i]);
    }
}

void Airplane::replaceQueue(Plot plot) {
    if(m_coordsMNK.size() == 3) {
        m_coordsMNK.dequeue();
        m_coordsMNK.append(plot);
    } else {
        m_coordsMNK.append(plot);
    }
}

std::pair<Plot, QPointF> Airplane::doExtrapolation(qreal currentTimeSecs) {

    qreal sumT = 0, sumT2 = 0;
    QPointF sumCoords(0, 0), sumCoordsT(0, 0);
    QVector<qreal> time;

    for(int i = 0; i < m_coordsMNK.size(); ++i) {
        time.append(m_coordsMNK[i].time() - m_coordsMNK[0].time());
        sumT += time[i];
        sumT2 += qPow(time[i], 2);
        sumCoords += m_coordsMNK[i].cartesianCoords();
        sumCoordsT += (m_coordsMNK[i].cartesianCoords() * time[i]);
    }

    QPointF extrapolatedVelocity = ((m_coordsMNK.size() * sumCoordsT) - (sumCoords * sumT)) /
                                        ((m_coordsMNK.size() * sumT2) - (sumT * sumT));
    QPointF firstCoord = (sumCoords - (extrapolatedVelocity * sumT)) / m_coordsMNK.size();
    QPointF extrapolatedCoord = firstCoord + (extrapolatedVelocity * (currentTimeSecs - m_coordsMNK[0].time()));
    Plot extrapolatedPlot = Plot(extrapolatedCoord,
                                 m_filteredPlot.frequencyDoppler(),
                                 m_filteredPlot.amplitude(),
                                 currentTimeSecs,
                                 m_filteredPlot.asterixPlot());

    return std::pair<Plot, QPointF>(extrapolatedPlot, extrapolatedVelocity);
}

void Airplane::onTimeout() {
    m_oldAngle = m_filteredPlot.angle();
    m_oldVelocity = velocity();
    qreal currentTime = static_cast<qreal>(QDateTime::currentMSecsSinceEpoch()) / 1000.0;

    std::pair<Plot, QPointF> resultExtrapolation = doExtrapolation(currentTime);
    Plot extrapolatedPlot = resultExtrapolation.first;
    QPointF extrapolatedVelocity = resultExtrapolation.second;

    m_filteredPlot = extrapolatedPlot;
    m_velocity = extrapolatedVelocity;

    m_counterExtrapolations++;

    replaceQueue(m_filteredPlot);

    emit skippingMeasurement();

    if(m_counterExtrapolations == SettingsTracker::NUMBER_OF_MISSING_PLOTS) {
        m_timer->stop();
        emit removeTrack();
        return;
    }

    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setSingleShot(true);
    m_timer->setInterval(SettingsTracker::SCAN_MSECS + SettingsTracker::WAIT_INFO_MSECS);
    m_timer->start();
}

void Airplane::onRestartTimer() {
    m_timer->stop();
    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setSingleShot(true);
    m_timer->setInterval(SettingsTracker::SCAN_MSECS + SettingsTracker::WAIT_INFO_MSECS);
    m_timer->start();
}

const Plot &Airplane::measuredPlot() const {
    return m_measuredPlot;
}

const Plot &Airplane::filteredPlot() const {
    return m_filteredPlot;
}

qreal Airplane::timeToNextPlot() const {
    return m_filteredPlot.time() + (SettingsTracker::SCAN_MSECS / 1000.0);
}


qreal Airplane::velocity() const {
    return qSqrt(qPow(m_velocity.x(), 2) + qPow(m_velocity.y(), 2));
}

qreal Airplane::velocityX() const {
    return m_velocity.x();
}

qreal Airplane::velocityY() const {
    return m_velocity.y();
}

qreal Airplane::directionAngle() const {
    return m_directionAngle;
}

quint16 Airplane::trackNumber() const {
    return m_trackNumber;
}

qreal Airplane::counterExtrapolations() const
{
    return m_counterExtrapolations;
}

QByteArray Airplane::getFilteredPlot() const {
    QByteArray data = m_filteredPlot.asterixPlot().Data;

    qreal x = m_filteredPlot.cartesianCoords().x() / 1852.0;
    qreal y = m_filteredPlot.cartesianCoords().y() / 1852.0;
    qreal velocity = this->velocity() / 1852.0;
    qreal angleVelocity = qRadiansToDegrees(m_directionAngle);

    quint16 coordX = x * 128.0;
    quint16 coordY = y * 128.0;
    quint16 groundspeed = velocity * (0x1 << 14);
    quint16 heading = angleVelocity * (0x1 << 16) / 360.0;

    int posTrackNumber = m_filteredPlot.asterixPlot().PosTrackNumber;

    if(((data[3] & 0x1) > 0) && ((data[4] & 0b00011100) > 0)) {
        data.remove(posTrackNumber + 1, 10);;
        data.insert(posTrackNumber + 1, (char) ((m_trackNumber >> 8) & 0xFF));
        data.insert(posTrackNumber + 1, (char) (m_trackNumber & 0xFF));
        data.insert(posTrackNumber + 2, (char) ((coordX >> 8) & 0xFF));
        data.insert(posTrackNumber + 3, (char) (coordX & 0xFF));
        data.insert(posTrackNumber + 4, (char) ((coordY >> 8) & 0xFF));
        data.insert(posTrackNumber + 5, (char) (coordY & 0xFF));
        data.insert(posTrackNumber + 6, (char) ((groundspeed >> 8) & 0xFF));
        data.insert(posTrackNumber + 7, (char) (groundspeed & 0xFF));
        data.insert(posTrackNumber + 8, (char) ((heading >> 8) & 0xFF));
        data.insert(posTrackNumber + 9, (char) (heading & 0xFF));
    } else if((data[3] & 0x1) == 0) {
        data[3] = data[3] | 0x1; // устанавливаем единичку которая говорит что дальше ещё есть что-то
        data.insert(4, 0b00011100);
        quint16 LEN = m_filteredPlot.asterixPlot().LEN;
        LEN += 1 + 2 + 4 + 4; // sizeof(NewFSPEC-oldFSPEC0 + sizeof(trackNumber) + sizeof(Coordinates) + sizeof(Velocity)
        data[1] = (LEN >> 8) & 0xFF;
        data[2] = LEN & 0xFF;
        posTrackNumber = data.size();
        data.append((char) ((m_trackNumber >> 8) & 0xFF));
        data.append((char) (m_trackNumber & 0xFF));
        data.append((char) ((coordX >> 8) & 0xFF));
        data.append((char) (coordX & 0xFF));
        data.append((char) ((coordY >> 8) & 0xFF));
        data.append((char) (coordY & 0xFF));
        data.append((char) ((groundspeed >> 8) & 0xFF));
        data.append((char) (groundspeed & 0xFF));
        data.append((char) ((heading >> 8) & 0xFF));
        data.append((char) (heading & 0xFF));
    } else {
        data[4] = data[4] | 0b00011100;
        quint16 LEN = m_filteredPlot.asterixPlot().LEN;
        LEN += 2 + 4 + 4; // sizeof(trackNumber) + sizeof(Coordinates) + sizeof(Velocity)
        data[1] = (LEN >> 8) & 0xFF;
        data[2] = LEN & 0xFF;
        data.insert(posTrackNumber, (char) ((m_trackNumber >> 8) & 0xFF));
        data.insert(posTrackNumber + 1, (char) (m_trackNumber & 0xFF));
        data.insert(posTrackNumber + 2, (char) ((coordX >> 8) & 0xFF));
        data.insert(posTrackNumber + 3, (char) (coordX & 0xFF));
        data.insert(posTrackNumber + 4, (char) ((coordY >> 8) & 0xFF));
        data.insert(posTrackNumber + 5, (char) (coordY & 0xFF));
        data.insert(posTrackNumber + 6, (char) ((groundspeed >> 8) & 0xFF));
        data.insert(posTrackNumber + 7, (char) (groundspeed & 0xFF));
        data.insert(posTrackNumber + 8, (char) ((heading >> 8) & 0xFF));
        data.insert(posTrackNumber + 9, (char) (heading & 0xFF));
    }

    return data;
}

QByteArray Airplane::getExtrapolationPlot() const {
    QByteArray data = m_filteredPlot.asterixPlot().Data;
    quint16 LEN = m_filteredPlot.asterixPlot().LEN;

    qreal x = m_filteredPlot.cartesianCoords().x() / 1852.0;
    qreal y = m_filteredPlot.cartesianCoords().y() / 1852.0;
    qreal velocity = this->velocity() / 1852.0;
    qreal angleVelocity = qRadiansToDegrees(m_directionAngle);

    quint16 coordX = x * 128.0;
    quint16 coordY = y * 128.0;
    quint16 groundspeed = velocity * (0x1 << 14);
    quint16 heading = angleVelocity * (0x1 << 16) / 360.0;

    int posTrackNumber = m_filteredPlot.asterixPlot().PosTrackNumber;

    if(((data[3] & 0x1) > 0) && ((data[4] & 0b00011100) > 0)) {
        data.remove(posTrackNumber + 1, 10);;
        data.insert(posTrackNumber + 1, (char) ((m_trackNumber >> 8) & 0xFF));
        data.insert(posTrackNumber + 1, (char) (m_trackNumber & 0xFF));
        data.insert(posTrackNumber + 2, (char) ((coordX >> 8) & 0xFF));
        data.insert(posTrackNumber + 3, (char) (coordX & 0xFF));
        data.insert(posTrackNumber + 4, (char) ((coordY >> 8) & 0xFF));
        data.insert(posTrackNumber + 5, (char) (coordY & 0xFF));
        data.insert(posTrackNumber + 6, (char) ((groundspeed >> 8) & 0xFF));
        data.insert(posTrackNumber + 7, (char) (groundspeed & 0xFF));
        data.insert(posTrackNumber + 8, (char) ((heading >> 8) & 0xFF));
        data.insert(posTrackNumber + 9, (char) (heading & 0xFF));
    } else if((data[3] & 0x1) == 0) {
        data[3] = data[3] | 0x1; // устанавливаем единичку которая говорит что дальше ещё есть что-то
        data.insert(4, 0b00011100);
        LEN += 1 + 2 + 4 + 4; // sizeof(NewFSPEC-oldFSPEC0 + sizeof(trackNumber) + sizeof(Coordinates) + sizeof(Velocity)
        posTrackNumber = data.size();
        data.append((char) ((m_trackNumber >> 8) & 0xFF));
        data.append((char) (m_trackNumber & 0xFF));
        data.append((char) ((coordX >> 8) & 0xFF));
        data.append((char) (coordX & 0xFF));
        data.append((char) ((coordY >> 8) & 0xFF));
        data.append((char) (coordY & 0xFF));
        data.append((char) ((groundspeed >> 8) & 0xFF));
        data.append((char) (groundspeed & 0xFF));
        data.append((char) ((heading >> 8) & 0xFF));
        data.append((char) (heading & 0xFF));
    } else {
        data[4] = data[4] | 0b00011100;
        LEN += 2 + 4 + 4; // sizeof(trackNumber) + sizeof(Coordinates) + sizeof(Velocity)
        data.insert(posTrackNumber, (char) ((m_trackNumber >> 8) & 0xFF));
        data.insert(posTrackNumber + 1, (char) (m_trackNumber & 0xFF));
        data.insert(posTrackNumber + 2, (char) ((coordX >> 8) & 0xFF));
        data.insert(posTrackNumber + 3, (char) (coordX & 0xFF));
        data.insert(posTrackNumber + 4, (char) ((coordY >> 8) & 0xFF));
        data.insert(posTrackNumber + 5, (char) (coordY & 0xFF));
        data.insert(posTrackNumber + 6, (char) ((groundspeed >> 8) & 0xFF));
        data.insert(posTrackNumber + 7, (char) (groundspeed & 0xFF));
        data.insert(posTrackNumber + 8, (char) ((heading >> 8) & 0xFF));
        data.insert(posTrackNumber + 9, (char) (heading & 0xFF));
    }

    quint16 i = m_filteredPlot.asterixPlot().TargetReportDescriptor.size();
    data[3] = data[3] & 0b11101111;
    if((data[3] & 0x1) == 0) {
        data.remove(9 + i, 4);
    } else if ((data[4] & 0x1) == 0){
        data.remove(10 + i, 4);
    } else if((data[5] & 0x1) == 0) {
        data.remove(11 + i, 4);
    } else {
        data.remove(12 + i, 4);
    }
    LEN -= 4;

    data[1] = (LEN >> 8) & 0xFF;
    data[2] = LEN & 0xFF;

    return data;
}

