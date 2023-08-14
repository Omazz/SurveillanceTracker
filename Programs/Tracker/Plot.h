#ifndef PLOT_H
#define PLOT_H

#include <QtCore>
#include "AsterixReader.h"

class Plot
{
public:
    Plot();

    Plot(qreal distance, qreal angle, qreal frequencyDoppler, qreal amplitude, qreal time, Asterix48 asterixPlot);

    Plot(QPointF cartesianCoords, qreal frequencyDoppler, qreal amplitude, qreal time, Asterix48 asterixPlto);

    qreal time() const;

    qreal x() const;

    qreal y() const;

    QPointF cartesianCoords() const;

    static std::pair<qreal, qreal> fromDecartToPolar(qreal y, qreal x);

    bool operator ==(const Plot& other) const {
        return ((m_range == other.m_range) &&
                (m_angle == other.m_angle) &&
                (m_time == other.m_time) &&
                (m_cartesianCoords == other.m_cartesianCoords));
    }

    qreal range() const;

    qreal angle() const;

    qreal frequencyDoppler() const;

    qreal amplitude() const;

    Asterix48 asterixPlot() const;

private:
    qreal m_range; ///< м
    qreal m_angle; ///< градусы
    QPointF m_cartesianCoords; ///< прямоугольные координаты, м
    qreal m_frequencyDoppler; ///< Гц
    qreal m_amplitude;
    qreal m_time; ///< с

    Asterix48 m_asterixPlot;
};

#endif // PLOT_H
