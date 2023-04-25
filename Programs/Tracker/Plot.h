#ifndef PLOT_H
#define PLOT_H

#include <QtCore>

class Plot
{
public:
    Plot();

    Plot(qreal distance, qreal angle, qreal frequencyDoppler, qreal amplitude, qreal time);

    Plot(QPointF cartesianCoords, qreal frequencyDoppler, qreal amplitude, qreal time);

    qreal time() const;

    qreal x() const;

    qreal y() const;

    QPointF cartesianCoords() const;

    static std::pair<qreal, qreal> fromDecartToPolar(qreal y, qreal x);

    bool operator ==(const Plot& other) const {
        return ((mRange == other.mRange) &&
                (mAngle == other.mAngle) &&
                (mTime == other.mTime) &&
                (mCartesianCoords == other.mCartesianCoords));
    }

    qreal range() const;

    qreal angle() const;

    qreal frequencyDoppler() const;

    qreal amplitude() const;

private:
    qreal mRange; ///< м
    qreal mAngle; ///< градусы
    QPointF mCartesianCoords; ///< прямоугольные координаты, м
    qreal mFrequencyDoppler; ///< Гц
    qreal mAmplitude;
    qreal mTime; ///< с
};

#endif // PLOT_H
