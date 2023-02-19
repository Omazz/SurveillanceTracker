#ifndef ALPHABETAFILTER_H
#define ALPHABETAFILTER_H

#include <QtCore>

class AlphaBetaFilter
{
public:
    AlphaBetaFilter();

    static QVector<QPointF> calculateFilteredTrajectory(QVector<QPointF> trajectoryWithNoise,
                                                        qreal updateTime,
                                                        uint16_t numberRecalcsCoefs);

    static QVector<QPointF> calculateFilteredTrajectory_MNK(QVector<QPointF> trajectoryWithNoise,
                                                            qreal updateTime,
                                                            uint16_t numberRecalcsCoefs, uint16_t numberValuesMNK);
};

#endif // ALPHABETAFILTER_H
