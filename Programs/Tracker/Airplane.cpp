#include "Airplane.h"

Airplane::Airplane(QObject *parent)
    : QObject{parent}
{

}

Airplane::Airplane(QVector<Plot> initPlots, quint16 trackNumber, QObject* parent)
    : QObject(parent), mTrackNumber(trackNumber) {

    mVelocity = (initPlots.last().cartesianCoords() - initPlots.first().cartesianCoords()) /
                    (initPlots.last().time() - initPlots.first().time());

    mDirectionAngle = qDegreesToRadians(Plot::fromDecartToPolar(mVelocity.y(), mVelocity.x()).second);

    mOldVelocity = velocity();

    mFilteredPlot = initPlots.last();

    mOldAngle = mFilteredPlot.angle();

    initializationMNK(initPlots);

    mMeasuredPlot = initPlots.last();

    mNumberSteps = 3;
    mNumberStepsMax = 7;
}

void Airplane::setTrack(Plot measuredPlot) {
    mCounterExtrapolations = 0;

    mMeasuredPlot = measuredPlot;

    if(mNumberSteps < mNumberStepsMax) {
        mAlpha = static_cast<qreal>((2.0 * ((2.0 * mNumberSteps) - 1.0)) / (mNumberSteps * (mNumberSteps + 1.0)));
        mBeta = static_cast<qreal>(6.0 / (mNumberSteps * (mNumberSteps + 1.0)));
    }

    mNumberSteps++;

    mOldVelocity = velocity();
    mOldAngle = mFilteredPlot.angle();
    qreal oldTime = mFilteredPlot.time();
    qreal currentTime = static_cast<qreal>(QDateTime::currentMSecsSinceEpoch() / 1000.0);

    std::pair<Plot, QPointF> resultExtrapolation = doExtrapolation(currentTime);
    Plot extrapolatedPlot = resultExtrapolation.first;
    QPointF extrapolatedVelocity = resultExtrapolation.second;

    mFilteredPlot = Plot(extrapolatedPlot.cartesianCoords() +
                            (mAlpha * (measuredPlot.cartesianCoords() - extrapolatedPlot.cartesianCoords())),
                         measuredPlot.frequencyDoppler(),
                         measuredPlot.amplitude(),
                         measuredPlot.time(),
                         measuredPlot.asterixPlot());

    mVelocity = extrapolatedVelocity + (mBeta / (currentTime - oldTime)
                                        * (measuredPlot.cartesianCoords() - extrapolatedPlot.cartesianCoords()));

    mDirectionAngle = qDegreesToRadians(Plot::fromDecartToPolar(mVelocity.y(), mVelocity.x()).second);

    replaceQueue(mFilteredPlot);
}

bool Airplane:: isManeuverAngle() const {
    qreal differenceAngles = qAbs(mFilteredPlot.angle() - mOldAngle);
    if(differenceAngles > 180.0) {
       differenceAngles = 360.0 - differenceAngles;
    }
    return qAbs(differenceAngles) >= SettingsTracker::MANEUVER_ANGLE_DEG;
}

bool Airplane:: isManeuverVelocity() const {
    return qAbs(velocity() - mOldVelocity) >= SettingsTracker::MANEUVER_VELOCITY_M_SECS;
}

void Airplane::initializationMNK(QVector<Plot> initPlots) {
    int i = (initPlots.size() == 4) ? 1 : 0;
    for(; i < initPlots.size(); ++i) {
        coordsMNK.enqueue(initPlots[i]);
    }
}

void Airplane::replaceQueue(Plot plot) {
    if(coordsMNK.size() == 3) {
        coordsMNK.dequeue();
        coordsMNK.append(plot);
    } else {
        coordsMNK.append(plot);
    }
}

std::pair<Plot, QPointF> Airplane::doExtrapolation(qreal currentTimeSecs) {

    qreal sumT = 0, sumT2 = 0;
    QPointF sumCoords(0, 0), sumCoordsT(0, 0);
    QVector<qreal> time;

    for(int i = 0; i < coordsMNK.size(); ++i) {
        time.append(coordsMNK[i].time() - coordsMNK[0].time());
        sumT += time[i];
        sumT2 += qPow(time[i], 2);
        sumCoords += coordsMNK[i].cartesianCoords();
        sumCoordsT += (coordsMNK[i].cartesianCoords() * time[i]);
    }

    QPointF extrapolatedVelocity = ((coordsMNK.size() * sumCoordsT) - (sumCoords * sumT)) /
                                        ((coordsMNK.size() * sumT2) - (sumT * sumT));
    QPointF firstCoord = (sumCoords - (extrapolatedVelocity * sumT)) / coordsMNK.size();
    QPointF extrapolatedCoord = firstCoord + (extrapolatedVelocity * (currentTimeSecs - coordsMNK[0].time()));
    Plot extrapolatedPlot = Plot(extrapolatedCoord,
                                 mFilteredPlot.frequencyDoppler(),
                                 mFilteredPlot.amplitude(),
                                 currentTimeSecs,
                                 mFilteredPlot.asterixPlot());

    return std::pair<Plot, QPointF>(extrapolatedPlot, extrapolatedVelocity);
}

void Airplane::updateExtrapolationPlot() {
    mOldAngle = mFilteredPlot.angle();
    mOldVelocity = velocity();
    qreal currentTime = static_cast<qreal>(QDateTime::currentMSecsSinceEpoch()) / 1000.0;

    std::pair<Plot, QPointF> resultExtrapolation = doExtrapolation(currentTime);
    Plot extrapolatedPlot = resultExtrapolation.first;
    QPointF extrapolatedVelocity = resultExtrapolation.second;

    mFilteredPlot = extrapolatedPlot;
    mVelocity = extrapolatedVelocity;

    mCounterExtrapolations++;

    replaceQueue(mFilteredPlot);
}

const Plot &Airplane::measuredPlot() const {
    return mMeasuredPlot;
}

const Plot &Airplane::filteredPlot() const {
    return mFilteredPlot;
}

qreal Airplane::timeToNextPlot() const {
    return mFilteredPlot.time() + (SettingsTracker::SCAN_MSECS / 1000.0);
}


qreal Airplane::velocity() const {
    return qSqrt(qPow(mVelocity.x(), 2) + qPow(mVelocity.y(), 2));
}

qreal Airplane::velocityX() const {
    return mVelocity.x();
}

qreal Airplane::velocityY() const {
    return mVelocity.y();
}

qreal Airplane::directionAngle() const {
    return mDirectionAngle;
}

quint16 Airplane::trackNumber() const {
    return mTrackNumber;
}

qreal Airplane::counterExtrapolations() const
{
    return mCounterExtrapolations;
}

QByteArray Airplane::getFilteredPlot() const {
    QByteArray data = mFilteredPlot.asterixPlot().Data;

    qreal x = mFilteredPlot.cartesianCoords().x() / 1852.0;
    qreal y = mFilteredPlot.cartesianCoords().y() / 1852.0;
    qreal velocity = this->velocity() / 1852.0;
    qreal angleVelocity = qRadiansToDegrees(mDirectionAngle);

    quint16 coordX = x * 128.0;
    quint16 coordY = y * 128.0;
    quint16 groundspeed = velocity * (0x1 << 14);
    quint16 heading = angleVelocity * (0x1 << 16) / 360.0;

    int posTrackNumber = mFilteredPlot.asterixPlot().PosTrackNumber;


    if(((data[3] & 0x1) > 0) && ((data[4] & 0b00011100) > 0)) {
        data.remove(posTrackNumber + 1, 10);;
        data.insert(posTrackNumber + 1, (char) ((mTrackNumber >> 8) & 0xFF));
        data.insert(posTrackNumber + 1, (char) (mTrackNumber & 0xFF));
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
        quint16 LEN = mFilteredPlot.asterixPlot().LEN;
        LEN += 1 + 2 + 4 + 4; // sizeof(NewFSPEC-oldFSPEC0 + sizeof(trackNumber) + sizeof(Coordinates) + sizeof(Velocity)
        data[1] = (LEN >> 8) & 0xFF;
        data[2] = LEN & 0xFF;
        posTrackNumber = data.size();
        data.append((char) ((mTrackNumber >> 8) & 0xFF));
        data.append((char) (mTrackNumber & 0xFF));
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
        quint16 LEN = mFilteredPlot.asterixPlot().LEN;
        LEN += 2 + 4 + 4; // sizeof(trackNumber) + sizeof(Coordinates) + sizeof(Velocity)
        data[1] = (LEN >> 8) & 0xFF;
        data[2] = LEN & 0xFF;
        data.insert(posTrackNumber, (char) ((mTrackNumber >> 8) & 0xFF));
        data.insert(posTrackNumber + 1, (char) (mTrackNumber & 0xFF));
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
    QByteArray data = mFilteredPlot.asterixPlot().Data;
    quint16 LEN = mFilteredPlot.asterixPlot().LEN;
    quint32 timeOfDay = QTime::currentTime().msecsSinceStartOfDay() * 128;
    if((data[3] & 0x1) == 0) {
        data[6] = (timeOfDay >> 16) & 0xFF;
        data[7] = (timeOfDay >> 8) & 0xFF;
        data[8] = timeOfDay & 0xFF;
    } else if ((data[4] & 0x1) == 0){
        data[7] = (timeOfDay >> 16) & 0xFF;
        data[8] = (timeOfDay >> 8) & 0xFF;
        data[9] = timeOfDay & 0xFF;
    } else if((data[5] & 0x1) == 0) {
        data[8] = (timeOfDay >> 16) & 0xFF;
        data[9] = (timeOfDay >> 8) & 0xFF;
        data[10] = timeOfDay & 0xFF;
    } else {
        data[9] = (timeOfDay >> 16) & 0xFF;
        data[10] = (timeOfDay >> 8) & 0xFF;
        data[11] = timeOfDay & 0xFF;
    }

    quint16 i = mFilteredPlot.asterixPlot().TargetReportDescriptor.size();
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


    qreal x = mFilteredPlot.cartesianCoords().x() / 1852.0;
    qreal y = mFilteredPlot.cartesianCoords().y() / 1852.0;
    qreal velocity = this->velocity() / 1852.0;
    qreal angleVelocity = qRadiansToDegrees(mDirectionAngle);

    quint16 coordX = x * 128.0;
    quint16 coordY = y * 128.0;
    quint16 groundspeed = velocity * (0x1 << 14);
    quint16 heading = angleVelocity * (0x1 << 16) / 360.0;

    int posTrackNumber = mFilteredPlot.asterixPlot().PosTrackNumber;

    if(((data[3] & 0x1) > 0) && ((data[4] & 0b00011100) > 0)) {
        data.remove(posTrackNumber + 1, 10);;
        data.insert(posTrackNumber + 1, (char) ((mTrackNumber >> 8) & 0xFF));
        data.insert(posTrackNumber + 1, (char) (mTrackNumber & 0xFF));
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
        data.append((char) ((mTrackNumber >> 8) & 0xFF));
        data.append((char) (mTrackNumber & 0xFF));
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
        data.insert(posTrackNumber, (char) ((mTrackNumber >> 8) & 0xFF));
        data.insert(posTrackNumber + 1, (char) (mTrackNumber & 0xFF));
        data.insert(posTrackNumber + 2, (char) ((coordX >> 8) & 0xFF));
        data.insert(posTrackNumber + 3, (char) (coordX & 0xFF));
        data.insert(posTrackNumber + 4, (char) ((coordY >> 8) & 0xFF));
        data.insert(posTrackNumber + 5, (char) (coordY & 0xFF));
        data.insert(posTrackNumber + 6, (char) ((groundspeed >> 8) & 0xFF));
        data.insert(posTrackNumber + 7, (char) (groundspeed & 0xFF));
        data.insert(posTrackNumber + 8, (char) ((heading >> 8) & 0xFF));
        data.insert(posTrackNumber + 9, (char) (heading & 0xFF));
    }

    data[1] = (LEN >> 8) & 0xFF;
    data[2] = LEN & 0xFF;

    return data;
}

