#include "AdaptiveKalmanConstVelocityFilter.h"

AdaptiveKalmanConstVelocityFilter::AdaptiveKalmanConstVelocityFilter(quint16 maximumNumberOfSteps,
                                                                     quint16 numberTargetsToRecalculateR,
                                                                     qreal coordinateMSE,
                                                                     qreal velocityMSE) {
    m_numberTargetsToRecalculateR = numberTargetsToRecalculateR;
    m_maximumNumberOfSteps = maximumNumberOfSteps;
    m_coordinateMSE = coordinateMSE;
    m_velocityMSE = velocityMSE;

    // Start initialization filter
    // I -- единичная матрица 4х4
    m_I = Matrix(4, 4);
    for(quint8 i = 0; i < 4; ++i) {
        m_I.set(i, i, 1);
    }

    // H -- взвешенная матрица
    m_H = Matrix(2, 4);
    m_H.set(0, 0, 1);
    m_H.set(1, 1, 1);

    // R -- взвешенная матрица шума
    m_R = Matrix(2, 2);
    m_R.set(0, 0, qPow(coordinateMSE, 2));
    m_R.set(1, 1, qPow(coordinateMSE, 2));

}

void AdaptiveKalmanConstVelocityFilter::initialization(QVector<Target> array) {
    m_velocity = (array.last().coordinate - array.first().coordinate) /
                    (array.last().time - array.first().time);

    m_numberOfSteps = array.size();
    for(int i = 0; i < array.size(); ++i) {
        m_measurementsToRecalculateR.append(array[i].coordinate);
        m_filteredTargetsToRecalculateR.append(array[i].coordinate);
    }

    // A -- матрица перехода
    m_A = Matrix(4, 4);
    for(quint8 i = 0; i < 4; ++i) {
        m_A.set(i, i, 1);
    }
    qreal difTime = (array.last().time - array.first().time) / array.size();
    m_A.set(0, 2, difTime);
    m_A.set(1, 3, difTime);

    // G -- вспомогательная матрица для вычисления Q
    m_G = Matrix(4, 1);
    m_G.set(0, 0, qPow(difTime, 2.0) / 2.0);
    m_G.set(1, 0, qPow(difTime, 2.0) / 2.0);
    m_G.set(2, 0, difTime);
    m_G.set(3, 0, difTime);

    // Q -- ковариационная матрица шума
    m_Q = Matrix(4, 4);
    m_Q = m_G.multiply(m_G.transpose());
    m_Q = m_Q.multiply(pow(m_velocityMSE, 2.0));

    m_filteredTarget = array.last();

    // x -- вектор состояния, x_predicted -- предсказанный вектор состояния
    m_x = Matrix(4, 1);
    m_x.set(0, 0, m_filteredTarget.coordinate.x());
    m_x.set(1, 0, m_filteredTarget.coordinate.y());
    m_x.set(2, 0, m_velocity.x());
    m_x.set(3, 0, m_velocity.y());
    m_x = m_A.multiply(m_x);

    // P -- ковариационная матрица
    m_P = Matrix(4, 4);
    m_P.set(0, 0, pow(m_coordinateMSE, 2.0));
    m_P.set(1, 1, pow(m_coordinateMSE, 2.0));
    m_P.set(2, 2, pow(m_velocityMSE, 2.0));
    m_P.set(3, 3, pow(m_velocityMSE, 2.0));
    m_P = m_A.multiply(m_P).multiply(m_A.transpose()).sum(m_Q);
}

Target AdaptiveKalmanConstVelocityFilter::filterMeasuredValue(Target measurement) {
    m_numberOfSteps++;
    Matrix z(2, 1);
    z.set(0, 0, measurement.coordinate.x());
    z.set(1, 0, measurement.coordinate.y());

    m_A.set(0, 2, measurement.time - m_filteredTarget.time);
    m_A.set(1, 3, measurement.time - m_filteredTarget.time);

    m_x = m_A.multiply(m_x);

    if(m_numberOfSteps < m_maximumNumberOfSteps) {
        m_P = m_A.multiply(m_P).multiply(m_A.transpose()).sum(m_Q);
    }

    Matrix K = m_P.multiply(m_H.transpose()).multiply(
                (m_H.multiply(m_P).multiply(m_H.transpose()).sum(m_R)).calculateInverseMatrix()
                );

    m_x = m_x.sum(K.multiply(z.difference(m_H.multiply(m_x))));

    QPointF filteredCoordinate = QPointF(m_x.get(0, 0), m_x.get(1, 0));
    m_velocity = QPointF(m_x.get(2, 0), m_x.get(3, 0));

   if((m_numberOfSteps + 1) < m_maximumNumberOfSteps) {
        m_P = m_I.difference(K.multiply(m_H)).multiply(m_P);
    }

    m_filteredTarget = Target(filteredCoordinate, measurement.time);

    m_measurementsToRecalculateR.append(measurement.coordinate);
    m_filteredTargetsToRecalculateR.append(m_filteredTarget.coordinate);
    if(m_measurementsToRecalculateR.size() > m_numberTargetsToRecalculateR) {
        m_measurementsToRecalculateR.dequeue();
        m_filteredTargetsToRecalculateR.dequeue();
        recalculateR();
    }

    return m_filteredTarget;
}

Target AdaptiveKalmanConstVelocityFilter::updateExtrapolation(qreal time) {
    return filterMeasuredValue(extrapolateOnTime(time));
}

Target AdaptiveKalmanConstVelocityFilter::extrapolateOnTime(qreal time) const {
    return Target(m_filteredTarget.coordinate + (m_velocity * (time - m_filteredTarget.time)), time);
}

QPointF AdaptiveKalmanConstVelocityFilter::getVelocity() const {
    return m_velocity;
}

void AdaptiveKalmanConstVelocityFilter::recalculateR() {
    QPointF coordinateMSE(0, 0);
    for(int i = 0; i < m_measurementsToRecalculateR.size(); ++i) {
        QPointF temp = m_filteredTargetsToRecalculateR[i] - m_measurementsToRecalculateR[i];
        temp.setX(qPow(temp.x(), 2));
        temp.setY(qPow(temp.y(), 2));
        coordinateMSE += temp;
    }

    coordinateMSE /= m_filteredTargetsToRecalculateR.size();
    qreal mse = qSqrt(coordinateMSE.x() + coordinateMSE.y());
    m_R.set(0, 0, qSqrt(mse));
    m_R.set(1, 1, qSqrt(mse));
}

