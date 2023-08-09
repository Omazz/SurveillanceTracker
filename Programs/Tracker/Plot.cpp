#include "Plot.h"

Plot::Plot() {

}

Plot::Plot(qreal distance, qreal angle, qreal frequencyDoppler, qreal amplitude, qreal time, Asterix48 asterixPlot)
    : m_range(std::move(distance)),
      m_angle(std::move(angle)),
      m_frequencyDoppler(std::move(frequencyDoppler)),
      m_amplitude(std::move(amplitude)),
      m_time(std::move(time)) {
    m_asterixPlot = asterixPlot;
    m_cartesianCoords = QPointF(m_range * qSin(qDegreesToRadians(m_angle)),
                               m_range * qCos(qDegreesToRadians(m_angle)));
}

Plot::Plot(QPointF cartesianCoords, qreal frequencyDoppler, qreal amplitude, qreal time, Asterix48 asterixPlot)
    : m_cartesianCoords(std::move(cartesianCoords)),
      m_frequencyDoppler(std::move(frequencyDoppler)),
      m_amplitude(std::move(amplitude)),
      m_time(std::move(time)) {
    m_asterixPlot = asterixPlot;
    std::pair<qreal, qreal> polarCoords = fromDecartToPolar(m_cartesianCoords.x(), m_cartesianCoords.y());
    m_range = polarCoords.first;
    m_angle = polarCoords.second;
}

qreal Plot::time() const {
    return m_time;
}

qreal Plot::x() const {
    return m_cartesianCoords.x();
}

qreal Plot::y() const {
    return m_cartesianCoords.y();
}

QPointF Plot::cartesianCoords() const {
    return m_cartesianCoords;
}

std::pair<qreal, qreal> Plot::fromDecartToPolar(qreal x, qreal y) {
    std::pair<qreal, qreal> polar;
    qreal theta;
    if (qAbs(x) < 0.1e-5) {
        if (y > 0.0) {
            theta = 0;
        } else {
            theta = M_PI;
        }
    } else {
        theta = qAtan(x / y);
        if(y < 0) {
            theta += M_PI;
        }
    }
    if(theta > M_PI) {
        theta -= (2.0 * M_PI);
    }
    qreal rho = qSqrt(qPow(y, 2) + qPow(x, 2));
    polar.first = rho;
    polar.second = qRadiansToDegrees(theta);
    return polar;
}

qreal Plot::range() const {
    return m_range;
}

qreal Plot::angle() const {
    return m_angle;
}

qreal Plot::frequencyDoppler() const
{
    return m_frequencyDoppler;
}

qreal Plot::amplitude() const
{
    return m_amplitude;
}

Asterix48 Plot::asterixPlot() const {
    return m_asterixPlot;
}


