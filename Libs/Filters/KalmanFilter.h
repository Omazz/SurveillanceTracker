#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <QtCore>
#include "Matrix.h"

class KalmanFilter
{
public:
    KalmanFilter();

    static QVector<QPointF> calculateKalmanFilter_CV(QVector<QPointF> airplaneTrajectory, qreal updateTime,
                                                     qreal sigmaCoord,
                                                     qreal sigmaVelocity,
                                                     qreal sigmaAcceleration);
};

#endif // KALMANFILTER_H
