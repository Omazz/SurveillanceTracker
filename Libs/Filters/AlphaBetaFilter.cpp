#include "AlphaBetaFilter.h"

AlphaBetaFilter::AlphaBetaFilter(quint16 maximumNumberOfSteps) {
    m_maximumNumberOfSteps = maximumNumberOfSteps;
}

void AlphaBetaFilter::initialization(QVector<Target> array) {
    m_velocity = (array.last().coordinate - array.first().coordinate) /
                    (array.last().time - array.first().time);

    m_numberOfSteps = array.size();

    m_alpha = static_cast<qreal>(2.0 * ((2.0 * m_numberOfSteps) - 1.0) / (m_numberOfSteps * (m_numberOfSteps + 1.0)));
    m_beta = static_cast<qreal>(6.0 / (m_numberOfSteps * (m_numberOfSteps + 1.0)));

    m_filteredTarget = array.last();
}

Target AlphaBetaFilter::filterMeasuredValue(Target measurement) {
    Target extrapolation = extrapolateOnTime(measurement.time);

    if(m_numberOfSteps < m_maximumNumberOfSteps) {
        m_alpha = static_cast<qreal>(2.0 * ((2.0 * m_numberOfSteps) - 1.0) / (m_numberOfSteps * (m_numberOfSteps + 1.0)));
        m_beta = static_cast<qreal>(6.0 / (m_numberOfSteps * (m_numberOfSteps + 1.0)));
    }

    QPointF filteredCoordinate = extrapolation.coordinate +
            (m_alpha * (measurement.coordinate - extrapolation.coordinate));

    m_velocity = m_velocity +
            (m_beta * (measurement.coordinate - extrapolation.coordinate) /
                  (measurement.time - m_filteredTarget.time));

    m_filteredTarget = Target(filteredCoordinate, measurement.time);

    return m_filteredTarget;
}

Target AlphaBetaFilter::updateExtrapolation(qreal time) {
    return filterMeasuredValue(extrapolateOnTime(time));
}

Target AlphaBetaFilter::extrapolateOnTime(qreal time) const {
    return Target(m_filteredTarget.coordinate + (m_velocity * (time - m_filteredTarget.time)), time);
}

QPointF AlphaBetaFilter::getVelocity() const {
    return m_velocity;
}
