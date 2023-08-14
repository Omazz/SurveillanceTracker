#include "AlphaBetaLeastSquaresFilter.h"

AlphaBetaLeastSquaresFilter::AlphaBetaLeastSquaresFilter(quint16 maximumNumberOfSteps,
                                                         quint16 numberOfTargetsToExtrapolation) {
    m_maximumNumberOfSteps = maximumNumberOfSteps;
    m_numberOfTargetsToExtrapolation = numberOfTargetsToExtrapolation;
}

void AlphaBetaLeastSquaresFilter::initialization(QVector<Target> array) {
    m_velocity = (array.last().coordinate - array.first().coordinate) /
                    (array.last().time - array.first().time);

    m_numberOfSteps = array.size();

    m_alpha = static_cast<qreal>(2.0 * ((2.0 * m_numberOfSteps) - 1.0) / (m_numberOfSteps * (m_numberOfSteps + 1.0)));
    m_beta = static_cast<qreal>(6.0 / (m_numberOfSteps * (m_numberOfSteps + 1.0)));

    for(int i = 0; i < array.size(); i++) {
        if(m_targets.size() < m_numberOfTargetsToExtrapolation) {
            m_targets.enqueue(array[i]);
        }
    }

    m_filteredTarget = array.last();
}

Target AlphaBetaLeastSquaresFilter::filterMeasuredValue(Target measurement) {
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

    m_targets.enqueue(m_filteredTarget);
    if(m_targets.size() > m_numberOfTargetsToExtrapolation) {
        m_targets.dequeue();
    }

    return m_filteredTarget;
}

Target AlphaBetaLeastSquaresFilter::updateExtrapolation(qreal time) {
    return filterMeasuredValue(extrapolateOnTime(time));
}

Target AlphaBetaLeastSquaresFilter::extrapolateOnTime(qreal time) const {
    qreal sumT = 0;
    qreal sumT2 = 0;
    QPointF sumCoords(0, 0);
    QPointF sumCoordsT(0, 0);

    for(int i = 0; i < m_targets.size(); ++i) {
        sumT += m_targets[i].time - m_targets.head().time;
        sumT2 += qPow(m_targets[i].time - m_targets.head().time, 2);
        sumCoords += m_targets[i].coordinate;
        sumCoordsT += (m_targets[i].coordinate * (m_targets[i].time - m_targets.head().time));
    }

    QPointF extrapolatedVelocity =
            ((m_targets.size() * sumCoordsT) - (sumCoords * sumT)) / ((m_targets.size() * sumT2) - (sumT * sumT));

    QPointF firstCoord = (sumCoords - (extrapolatedVelocity * sumT)) / m_targets.size();

    QPointF extrapolatedCoord = firstCoord + (extrapolatedVelocity * (time - m_targets.head().time));

    return Target(extrapolatedCoord, time);
}

QPointF AlphaBetaLeastSquaresFilter::getVelocity() const {
    return m_velocity;
}

