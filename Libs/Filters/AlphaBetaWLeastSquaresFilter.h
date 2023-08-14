#ifndef ALPHABETAWLEASTSQUARESFILTER_H
#define ALPHABETAWLEASTSQUARESFILTER_H

#include "AbstractFilter.h"

//enum MethodCalculatingWeightCoefs {
//    Linear = 0, Exponential
//};

class AlphaBetaWLeastSquaresFilter : public AbstractFilter
{
public:
    AlphaBetaWLeastSquaresFilter(quint16 maximumNumberOfSteps = 100,
                                 quint16 numberOfTargetsToExtrapolation = 100,
                                 qreal noiseRho_m = 50., qreal noiseAngle_rad = qDegreesToRadians(12. / 60.));

    void initialization(QVector<Target> array) override;

    Target filterMeasuredValue(Target measurement) override;

    Target updateExtrapolation(qreal time) override;

    Target extrapolateOnTime(qreal time) const override;

    QPointF getVelocity() const override;

private:
    Target m_filteredTarget;
    QPointF m_velocity;

    QQueue<Target> m_targets;
    quint16 m_numberOfTargetsToExtrapolation;

    qreal m_alpha;
    qreal m_beta;
    qreal m_numberOfSteps;
    qreal m_maximumNumberOfSteps;

    qreal m_noiseRho_m;
    qreal m_noiseAngle_rad;
};

#endif // ALPHABETAWLEASTSQUARESFILTER_H
