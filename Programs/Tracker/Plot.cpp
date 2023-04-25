#include "Plot.h"

Plot::Plot() {

}

Plot::Plot(qreal distance, qreal angle, qreal frequencyDoppler, qreal amplitude, qreal time)
    : mRange(std::move(distance)),
      mAngle(std::move(angle)),
      mFrequencyDoppler(std::move(frequencyDoppler)),
      mAmplitude(std::move(amplitude)),
      mTime(std::move(time)) {

    mCartesianCoords = QPointF(mRange * qSin(qDegreesToRadians(mAngle)),
                               mRange * qCos(qDegreesToRadians(mAngle)));
}

Plot::Plot(QPointF cartesianCoords, qreal frequencyDoppler, qreal amplitude, qreal time)
    : mCartesianCoords(std::move(cartesianCoords)),
      mFrequencyDoppler(std::move(frequencyDoppler)),
      mAmplitude(std::move(amplitude)),
      mTime(std::move(time)) {

    std::pair<qreal, qreal> polarCoords = fromDecartToPolar(mCartesianCoords.x(), mCartesianCoords.y());
    mRange = polarCoords.first;
    mAngle = polarCoords.second;
}

qreal Plot::time() const {
    return mTime;
}

qreal Plot::x() const {
    return mCartesianCoords.x();
}

qreal Plot::y() const {
    return mCartesianCoords.y();
}

QPointF Plot::cartesianCoords() const {
    return mCartesianCoords;
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
    return mRange;
}

qreal Plot::angle() const {
    return mAngle;
}

qreal Plot::frequencyDoppler() const
{
    return mFrequencyDoppler;
}

qreal Plot::amplitude() const
{
    return mAmplitude;
}
