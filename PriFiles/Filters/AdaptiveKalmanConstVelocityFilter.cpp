#include "AdaptiveKalmanConstVelocityFilter.h"

AdaptiveKalmanConstVelocityFilter::AdaptiveKalmanConstVelocityFilter(quint16 numberRecalcsP,
                                                                     quint16 numberRecalcsR,
                                                                     qreal rhoMSE, qreal thetaMSE,
                                                                     qreal velocityMSE) {
    m_numberRecalcsP = numberRecalcsP;
    m_numberRecalcsR = numberRecalcsR;
    m_rhoMSE = rhoMSE;
    m_thetaMSE = thetaMSE;
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
}

void AdaptiveKalmanConstVelocityFilter::initialization(QVector<Target> array) {
    m_velocity = (array.last().coordinate - array.first().coordinate) /
                    (array.last().time - array.first().time);

    m_counterSteps = array.size();

    // R -- взвешенная матрица шума
    m_R = Matrix(2, 2);

    qreal rho = qSqrt(qPow(array.last().coordinate.x(), 2) + qPow(array.last().coordinate.y(), 2));
    qreal theta = qAtan2(array.last().coordinate.y(), array.last().coordinate.x());
    if(qIsNull(array.last().coordinate.y())) {
        theta = 0;
    }
    theta = M_PI_2 - theta;
    if(theta < 0) {
        theta += (2.0 * M_PI);
    }

    m_R.set(0, 0, qPow(m_rhoMSE*qSin(theta),2)+qPow(rho*m_thetaMSE*qCos(theta),2));
    m_R.set(1, 1, qPow(m_rhoMSE*qCos(theta),2)+qPow(rho*m_thetaMSE*qSin(theta),2));

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
    m_P.set(0, 0, qPow(m_rhoMSE*qSin(theta),2)+qPow(rho*m_thetaMSE*qCos(theta),2));
    m_P.set(1, 1, qPow(m_rhoMSE*qCos(theta),2)+qPow(rho*m_thetaMSE*qSin(theta),2));
    m_P.set(2, 2, pow(m_velocityMSE, 2.0));
    m_P.set(3, 3, pow(m_velocityMSE, 2.0));
    m_P = m_A.multiply(m_P).multiply(m_A.transpose()).sum(m_Q);
}

Target AdaptiveKalmanConstVelocityFilter::filterMeasuredValue(Target measurement) {
    m_counterSteps++;
    Matrix z(2, 1);
    z.set(0, 0, measurement.coordinate.x());
    z.set(1, 0, measurement.coordinate.y());

    m_A.set(0, 2, measurement.time - m_filteredTarget.time);
    m_A.set(1, 3, measurement.time - m_filteredTarget.time);

    m_x = m_A.multiply(m_x);

    if(m_counterSteps < m_numberRecalcsP) {
        m_P = m_A.multiply(m_P).multiply(m_A.transpose()).sum(m_Q);
    }

    Matrix K = m_P.multiply(m_H.transpose()).multiply(
                (m_H.multiply(m_P).multiply(m_H.transpose()).sum(m_R)).calculateInverseMatrix()
                );

    m_x = m_x.sum(K.multiply(z.difference(m_H.multiply(m_x))));

    QPointF filteredCoordinate = QPointF(m_x.get(0, 0), m_x.get(1, 0));
    m_velocity = QPointF(m_x.get(2, 0), m_x.get(3, 0));

   if((m_counterSteps + 1) < m_numberRecalcsP) {
        m_P = m_I.difference(K.multiply(m_H)).multiply(m_P);
    }

    m_filteredTarget = Target(filteredCoordinate, measurement.time);

    if(m_counterSteps < m_numberRecalcsR) {
        recalculateMatrixR();
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

void AdaptiveKalmanConstVelocityFilter::recalculateMatrixR() {
    qreal rho = qSqrt(qPow(m_filteredTarget.coordinate.x(), 2) + qPow(m_filteredTarget.coordinate.y(), 2));
    qreal theta = qAtan2(m_filteredTarget.coordinate.y(), m_filteredTarget.coordinate.x());
    if(qIsNull(m_filteredTarget.coordinate.y())) {
        theta = 0;
    }
    theta = M_PI_2 - theta;
    if(theta < 0) {
        theta += (2.0 * M_PI);
    }

    m_R.set(0, 0, qPow(m_rhoMSE*qSin(theta),2)+qPow(rho*m_thetaMSE*qCos(theta),2));
    m_R.set(1, 1, qPow(m_rhoMSE*qCos(theta),2)+qPow(rho*m_thetaMSE*qSin(theta),2));
}

