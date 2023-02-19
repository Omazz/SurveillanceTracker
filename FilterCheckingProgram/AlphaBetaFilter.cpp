#include "AlphaBetaFilter.h"

AlphaBetaFilter::AlphaBetaFilter()
{

}

QVector<QPointF> AlphaBetaFilter::calculateFilteredTrajectory(QVector<QPointF> trajectoryWithNoise,
                                                              qreal updateTime,
                                                              uint16_t numberRecalcsCoefs) {
    QVector<QPointF> filteredTrajectory = {trajectoryWithNoise[0], trajectoryWithNoise[1]};
    uint16_t k = 1;
    QPointF filteredVelocity = (trajectoryWithNoise[1] - trajectoryWithNoise[0]) / updateTime;
    QPointF extrapolatedVelocity = filteredVelocity;
    QPointF filteredCoord = trajectoryWithNoise[1];
    QPointF extrapolatedCoord = filteredCoord + (filteredVelocity * updateTime);
    qreal alpha, beta;

    do {
        k++;

        if(k < numberRecalcsCoefs) {
            alpha = 2.0 * ((2.0 * k) - 1.0) / (k * (k + 1.0));
            beta = 6.0 / (k * (k + 1.0));
        }

        // фильтрация
        filteredCoord = extrapolatedCoord + (alpha * (trajectoryWithNoise[k] - extrapolatedCoord));
        filteredVelocity = extrapolatedVelocity + (beta * (trajectoryWithNoise[k] - extrapolatedCoord) / updateTime);

        // прогноз на следующий шаг
        extrapolatedCoord = filteredCoord + (filteredVelocity * updateTime);
        extrapolatedVelocity = filteredVelocity;

        filteredTrajectory.append(filteredCoord);
    } while(k < trajectoryWithNoise.size());


    return filteredTrajectory;
}

QVector<QPointF> AlphaBetaFilter::calculateFilteredTrajectory_MNK(QVector<QPointF> trajectoryWithNoise,
                                                                  qreal updateTime,
                                                                  uint16_t numberRecalcsCoefs, uint16_t numberValuesMNK) {
    QVector<QPointF> filteredTrajectory = {trajectoryWithNoise[0], trajectoryWithNoise[1]};
    uint16_t k = 1;
    QPointF filteredVelocity = (trajectoryWithNoise[1] - trajectoryWithNoise[0]) / updateTime;
    QPointF extrapolatedVelocity = filteredVelocity;
    QPointF filteredCoord = trajectoryWithNoise[1];
    QPointF extrapolatedCoord = filteredCoord + (filteredVelocity * updateTime);
    qreal alpha, beta;

    do {
        k++;

        if(k < numberRecalcsCoefs) {
            alpha = 2.0 * ((2.0 * k) - 1.0) / (k * (k + 1.0));
            beta = 6.0 / (k * (k + 1.0));
        }

        // фильтрация
        filteredCoord = extrapolatedCoord + (alpha * (trajectoryWithNoise[k] - extrapolatedCoord));
        filteredVelocity = extrapolatedVelocity + (beta * (trajectoryWithNoise[k] - extrapolatedCoord) / updateTime);

        // прогноз на следующий шаг (МНК)
        extrapolatedCoord = filteredCoord + (filteredVelocity * updateTime);
        extrapolatedVelocity = filteredVelocity;

        filteredTrajectory.append(filteredCoord);
    } while(k < trajectoryWithNoise.size());


    return filteredTrajectory;
}
