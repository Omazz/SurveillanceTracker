#include "KalmanConstAccelerationFilter.h"


KalmanConstAccelerationFilter::KalmanConstAccelerationFilter(qreal coordinateMSE,
                                                     qreal velocityMSE,
                                                     qreal accelerationMSE) {
    m_coordinateMSE = coordinateMSE;
    m_velocityMSE = velocityMSE;
    m_accelerationMSE = accelerationMSE;

    // Start initialization filter
    // I -- единичная матрица 6х6
    m_I = Matrix(6, 6);
    for(quint8 i = 0; i < 6; ++i) {
        m_I.set(i, i, 1);
    }

    // H -- взвешенная матрица
    m_H = Matrix(2, 6);
    m_H.set(0, 0, 1);
    m_H.set(1, 1, 1);

    // R -- взвешенная матрица шума
    m_R = Matrix(2, 2);
    m_R.set(0, 0, qPow(coordinateMSE, 2));
    m_R.set(1, 1, qPow(coordinateMSE, 2));

}

void KalmanConstAccelerationFilter::initialization(QVector<Target> array) {
    m_velocity = (array.last().coordinate - array.first().coordinate) /
                    (array.last().time - array.first().time);

    // A -- матрица перехода
    m_A = Matrix(6, 6);
    for(quint8 i = 0; i < 4; ++i) {
        m_A.set(i, i, 1);
    }
    qreal difTime = (array.last().time - array.first().time) / array.size();
    m_A.set(0, 2, difTime);
    m_A.set(1, 3, difTime);
    m_A.set(2, 4, difTime);
    m_A.set(3, 5, difTime);
    m_A.set(0, 4, qPow(difTime, 2) / 2.0);
    m_A.set(1, 5, qPow(difTime, 2) / 2.0);

    // G -- вспомогательная матрица для вычисления Q
    m_G = Matrix(6, 1);
    m_G.set(0, 0, qPow(difTime, 3.0) / 6.0);
    m_G.set(1, 0, qPow(difTime, 3.0) / 6.0);
    m_G.set(2, 0, qPow(difTime, 2.0) / 2.0);
    m_G.set(3, 0, qPow(difTime, 2.0) / 2.0);
    m_G.set(4, 0, difTime);
    m_G.set(5, 0, difTime);

    // Q -- ковариационная матрица шума
    m_Q = Matrix(6, 6);
    m_Q = m_G.multiply(m_G.transpose());
    m_Q = m_Q.multiply(pow(m_accelerationMSE, 2.0));

    m_filteredTarget = array.last();

    // x -- вектор состояния, x_predicted -- предсказанный вектор состояния
    QPointF acceleration(0,0);
    if(array.size() == 3) {
        QPointF firstVelocity(0,0);
        QPointF secondVelocity(0, 0);
        firstVelocity = (array[1].coordinate - array[0].coordinate) /
                                            (array[1].time - array[0].time);
        secondVelocity = (array[2].coordinate - array[1].coordinate) /
                                            (array[2].time - array[1].time);
        acceleration = (secondVelocity - firstVelocity) / (array[1].time - array[0].time);
    }
    m_x = Matrix(6, 1);
    m_x.set(0, 0, m_filteredTarget.coordinate.x());
    m_x.set(1, 0, m_filteredTarget.coordinate.y());
    m_x.set(2, 0, m_velocity.x());
    m_x.set(3, 0, m_velocity.y());
    m_x.set(4, 0, acceleration.x());
    m_x.set(5, 0, acceleration.y());
    m_x = m_A.multiply(m_x);

    // P -- ковариационная матрица
    m_P = Matrix(6, 6);
    m_P.set(0, 0, pow(m_coordinateMSE, 2.0));
    m_P.set(1, 1, pow(m_coordinateMSE, 2.0));
    m_P.set(2, 2, pow(m_velocityMSE, 2.0));
    m_P.set(3, 3, pow(m_velocityMSE, 2.0));
    m_P.set(4, 4, pow(m_accelerationMSE, 2.0));
    m_P.set(5, 5, pow(m_accelerationMSE, 2.0));
    m_P = m_A.multiply(m_P).multiply(m_A.transpose()).sum(m_Q);
}

Target KalmanConstAccelerationFilter::filterMeasuredValue(Target measurement) {
    Matrix z(2, 1);
    z.set(0, 0, measurement.coordinate.x());
    z.set(1, 0, measurement.coordinate.y());

    qreal difTime = measurement.time - m_filteredTarget.time;
    m_A.set(0, 2, difTime);
    m_A.set(1, 3, difTime);
    m_A.set(2, 4, difTime);
    m_A.set(3, 5, difTime);
    m_A.set(0, 4, qPow(difTime, 2) / 2.0);
    m_A.set(1, 5, qPow(difTime, 2) / 2.0);

    m_x = m_A.multiply(m_x);
    m_P = m_A.multiply(m_P).multiply(m_A.transpose()).sum(m_Q);

    Matrix K = m_P.multiply(m_H.transpose()).multiply(
                (m_H.multiply(m_P).multiply(m_H.transpose()).sum(m_R)).calculateInverseMatrix()
                );

    m_x = m_x.sum(K.multiply(z.difference(m_H.multiply(m_x))));

    QPointF filteredCoordinate = QPointF(m_x.get(0, 0), m_x.get(1, 0));
    m_velocity = QPointF(m_x.get(2, 0), m_x.get(3, 0));

    m_P = m_I.difference(K.multiply(m_H)).multiply(m_P);

    m_filteredTarget = Target(filteredCoordinate, measurement.time);

    return m_filteredTarget;
}

Target KalmanConstAccelerationFilter::updateExtrapolation(qreal time) {
    return filterMeasuredValue(extrapolateOnTime(time));
}

Target KalmanConstAccelerationFilter::extrapolateOnTime(qreal time) const {
    return Target(m_filteredTarget.coordinate + (m_velocity * (time - m_filteredTarget.time)), time);
}

QPointF KalmanConstAccelerationFilter::getVelocity() const {
    return m_velocity;
}
