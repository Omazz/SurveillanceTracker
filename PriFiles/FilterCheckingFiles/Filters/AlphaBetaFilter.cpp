#include "AlphaBetaFilter.h"

AlphaBetaFilter::AlphaBetaFilter()
{

}

QVector<QPointF> AlphaBetaFilter::calculateFilteredTrajectory(QVector<QPointF> trajectoryWithNoise,
                                                              qreal updateTime,
                                                              uint16_t numberRecalcsCoefs) {
    QVector<QPointF> filteredTrajectory = {trajectoryWithNoise[0],
                                           trajectoryWithNoise[1],
                                           trajectoryWithNoise[2]};
    uint16_t k = 3;
    QPointF filteredVelocity = (trajectoryWithNoise[2] - trajectoryWithNoise[0]) / (2 * updateTime);
    QPointF extrapolatedVelocity = filteredVelocity;
    QPointF filteredCoord = trajectoryWithNoise[2];
    QPointF extrapolatedCoord = filteredCoord + (filteredVelocity * updateTime);
    qreal alpha, beta;

    while(k < trajectoryWithNoise.size()) {

        if(k < numberRecalcsCoefs) {
            alpha = 2.0 * ((2.0 * k) - 1.0) / (k * (k + 1.0));
            beta = 6.0 / (k * (k + 1.0));
        }

        /* Фильтрация */
        filteredCoord = extrapolatedCoord + (alpha * (trajectoryWithNoise[k] - extrapolatedCoord));
        filteredVelocity = extrapolatedVelocity + (beta * (trajectoryWithNoise[k] - extrapolatedCoord) / updateTime);

        /* Прогноз на следующий шаг */
        extrapolatedCoord = filteredCoord + (filteredVelocity * updateTime);
        extrapolatedVelocity = filteredVelocity;

        filteredTrajectory.append(filteredCoord);
        k++;
    }


    return filteredTrajectory;
}

/* Пока что пусть numberValuesMNK = 3 */
QVector<QPointF> AlphaBetaFilter::calculateFilteredTrajectory_MNK(QVector<QPointF> trajectoryWithNoise,
                                                                  qreal updateTime,
                                                                  uint16_t numberRecalcsCoefs,
                                                                  uint16_t numberValuesMNK) {
    QVector<QPointF> filteredTrajectory = {trajectoryWithNoise[0],
                                           trajectoryWithNoise[1],
                                           trajectoryWithNoise[2]};
    uint16_t k = 3;
    QPointF filteredVelocity = (trajectoryWithNoise[2] - trajectoryWithNoise[0]) / (2 * updateTime);
    QPointF filteredCoord = trajectoryWithNoise[2];
    qreal alpha, beta;

    /* Для экстраполяции */
    QQueue<QPointF> coordsMNK;

    coordsMNK.enqueue(trajectoryWithNoise[0]);
    coordsMNK.enqueue(trajectoryWithNoise[1]);
    coordsMNK.enqueue(trajectoryWithNoise[2]);

    QVector<qreal> time;
    for(int i = 0; i < numberValuesMNK; ++i) {
        time.append(updateTime * i);
    }
    qreal sumT = time[0] + time[1] + time[2];
    qreal sumT2 = qPow(time[0], 2) + qPow(time[1], 2) + qPow(time[2], 2);
    QPointF sumCoords = trajectoryWithNoise[0] + trajectoryWithNoise[1] + trajectoryWithNoise[2];
    QPointF sumCoordsT = (trajectoryWithNoise[0] * time[0])
                          + (trajectoryWithNoise[1] * time[1])
                          + (trajectoryWithNoise[2] * time[2]);

    QPointF extrapolatedVelocity = ((3.0 * sumCoordsT) - (sumCoords * sumT)) / ((3.0 * sumT2) - (sumT * sumT));
    QPointF firstCoord = (sumCoords - (extrapolatedVelocity * sumT)) / 3.0;
    QPointF extrapolatedCoord = firstCoord + (extrapolatedVelocity * (time[2] + updateTime));


    while(k < trajectoryWithNoise.size()) {
        if(k < numberRecalcsCoefs) {
            alpha = 2.0 * ((2.0 * k) - 1.0) / (k * (k + 1.0));
            beta = 6.0 / (k * (k + 1.0));
        }

        /* Фильтрация */
        filteredCoord = extrapolatedCoord + (alpha * (trajectoryWithNoise[k] - extrapolatedCoord));
        filteredVelocity = extrapolatedVelocity + (beta * (trajectoryWithNoise[k] - extrapolatedCoord) / updateTime);

        /* Прогноз на следующий шаг */
        if(coordsMNK.size() == numberValuesMNK) {
            coordsMNK.dequeue();
        }
        coordsMNK.enqueue(filteredCoord);

        sumCoords= QPointF(0,0);
        sumCoordsT= QPointF(0,0);
        sumT = 0;
        sumT2 = 0;

        for(int i = 0; i < coordsMNK.size(); ++i) {
            sumT += time[i];
            sumT2 += qPow(time[i], 2);
            sumCoords += coordsMNK[i];
            sumCoordsT += coordsMNK[i] * time[i];
        }

        extrapolatedVelocity = ((coordsMNK.size() * sumCoordsT) - (sumCoords * sumT)) / ((coordsMNK.size() * sumT2) - (sumT * sumT));
        firstCoord = (sumCoords - (extrapolatedVelocity * sumT)) / coordsMNK.size();
        extrapolatedCoord = firstCoord + (extrapolatedVelocity * (time[coordsMNK.size() - 1] + updateTime));


        filteredTrajectory.append(filteredCoord);
        k++;
    }


    return filteredTrajectory;
}
