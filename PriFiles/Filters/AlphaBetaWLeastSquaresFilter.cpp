#include "AlphaBetaWLeastSquaresFilter.h"


AlphaBetaWLeastSquaresFilter::AlphaBetaWLeastSquaresFilter(quint16 maximumNumberOfSteps,
                                                           quint16 numberOfTargetsToExtrapolation,
                                                           qreal noiseRho_m, qreal noiseAngle_rad) {
    m_maximumNumberOfSteps = maximumNumberOfSteps;
    m_numberOfTargetsToExtrapolation = numberOfTargetsToExtrapolation;
    m_noiseRho_m = noiseRho_m;
    m_noiseAngle_rad = noiseAngle_rad;
}

void AlphaBetaWLeastSquaresFilter::initialization(QVector<Target> array) {
    m_velocity = (array.last().coordinate - array.first().coordinate) /
                    (array.last().time - array.first().time);

    m_numberOfSteps = array.size();

    m_alpha = static_cast<qreal>(2.0 * ((2.0 * m_numberOfSteps) - 1.0) / (m_numberOfSteps * (m_numberOfSteps + 1.0)));
    m_beta = static_cast<qreal>(6.0 / (m_numberOfSteps * (m_numberOfSteps + 1.0)));

    for(int i = 0; i < array.size(); i++) {
        //if(m_targets.size() < m_numberOfTargetsToExtrapolation) {
            m_targets.enqueue(array[i]);
        //}
    }

    m_filteredTarget = array.last();
}

Target AlphaBetaWLeastSquaresFilter::filterMeasuredValue(Target measurement) {
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
//    if(m_targets.size() > m_numberOfTargetsToExtrapolation) {
//        m_targets.dequeue();
//    }

    return m_filteredTarget;
}

Target AlphaBetaWLeastSquaresFilter::updateExtrapolation(qreal time) {
    return filterMeasuredValue(extrapolateOnTime(time));
}

Target AlphaBetaWLeastSquaresFilter::extrapolateOnTime(qreal time) const {
    qreal sumT = 0;
    qreal sumT2 = 0;
    QPointF sumCoords(0, 0);
    QPointF sumCoordsT(0, 0);

    qreal sumWeightCoefs = 0;

    qreal T = m_targets.last().time - m_targets.head().time;
    for(int i = 0; i < m_targets.size(); ++i) {
        qreal t = m_targets.last().time - m_targets[i].time;
        qreal weightCoef = qExp(-t / T);
        //qreal weightCoef = 1. - qAbs(t / T);
        //qreal distance = qSqrt(qPow(m_targets[i].coordinate.x(), 2) + qPow(m_targets[i].coordinate.y(), 2));
        //qreal disp = qPow(m_noiseRho_m, 2) + qPow(distance * m_noiseAngle_rad, 2);
        //qreal weightCoef = 1 / disp;
        sumWeightCoefs += weightCoef;
        sumT += weightCoef * (m_targets[i].time - m_targets.head().time);
        sumT2 += weightCoef * (qPow(m_targets[i].time - m_targets.head().time, 2));
        sumCoords += weightCoef * m_targets[i].coordinate;
        sumCoordsT += weightCoef * (m_targets[i].coordinate * (m_targets[i].time - m_targets.head().time));
    }
    sumT /= sumWeightCoefs;
    sumT2 /= sumWeightCoefs;
    sumCoords /= sumWeightCoefs;
    sumCoordsT /= sumWeightCoefs;

//    QPointF extrapolatedVelocity =
//            ((m_targets.size() * sumCoordsT) - (sumCoords * sumT)) / ((m_targets.size() * sumT2) - (sumT * sumT));

//    QPointF firstCoord = (sumCoords - (extrapolatedVelocity * sumT)) / m_targets.size();

//    QPointF extrapolatedCoord = firstCoord + (extrapolatedVelocity * (time - m_targets.head().time));

    QPointF extrapolatedVelocity =
            (sumCoordsT - (sumCoords * sumT)) / (sumT2 - (sumT * sumT));
    if(qIsNaN(extrapolatedVelocity.x()) || qIsNaN(extrapolatedVelocity.y())) {
        qDebug() << "fail!";
    }

    QPointF firstCoord = (sumCoords - (extrapolatedVelocity * sumT));

    QPointF extrapolatedCoord = firstCoord + (extrapolatedVelocity * (time - m_targets.head().time));

    return Target(extrapolatedCoord, time);
}

QPointF AlphaBetaWLeastSquaresFilter::getVelocity() const {
    return m_velocity;
}
