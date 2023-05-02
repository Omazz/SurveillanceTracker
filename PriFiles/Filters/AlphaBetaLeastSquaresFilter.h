#ifndef ALPHABETALEASTSQUARESFILTER_H
#define ALPHABETALEASTSQUARESFILTER_H

#include "AbstractFilter.h"

class AlphaBetaLeastSquaresFilter : public AbstractFilter {
public:

    AlphaBetaLeastSquaresFilter(quint16 maximumNumberOfSteps, quint16 numberOfTargetsToExtrapolation);

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
    quint16 m_numberOfSteps;
    quint16 m_maximumNumberOfSteps;
};


#endif // ALPHABETALEASTSQUARESFILTER_H
