#ifndef ALPHABETAFILTER_H
#define ALPHABETAFILTER_H

#include "AbstractFilter.h"

class AlphaBetaFilter : public AbstractFilter {
public:
    AlphaBetaFilter(quint16 maximumNumberOfSteps);

    void initialization(QVector<Target> array) override;

    Target filterMeasuredValue(Target measurement) override;

    Target updateExtrapolation(qreal time) override;

    Target extrapolateOnTime(qreal time) const override;

    QPointF getVelocity() const override;

private:
    Target m_filteredTarget;
    QPointF m_velocity;

    qreal m_alpha;
    qreal m_beta;
    quint16 m_numberOfSteps;
    quint16 m_maximumNumberOfSteps;
};

#endif // ALPHABETAFILTER_H
