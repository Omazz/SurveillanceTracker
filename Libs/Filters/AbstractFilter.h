#ifndef ABSTRACTFILTER_H
#define ABSTRACTFILTER_H

#include <QtCore>

struct Target {
    QPointF coordinate;
    qreal time;

    Target():coordinate(QPointF(0, 0)), time(0) {};

    Target(QPointF coordinate, qreal time):coordinate(coordinate), time(time) {};
};

class AbstractFilter
{
public:
    virtual void initialization(QVector<Target> array) = 0;

    virtual Target filterMeasuredValue(Target measurement) = 0;

    virtual Target updateExtrapolation(qreal time) = 0;

    virtual Target extrapolateOnTime(qreal time) const = 0;

    virtual QPointF getVelocity() const = 0;

private:
};
#endif // ABSTRACTFILTER_H
