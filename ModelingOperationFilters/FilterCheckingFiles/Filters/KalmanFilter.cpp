#include "KalmanFilter.h"

KalmanFilter::KalmanFilter()
{

}

QVector<QPointF> KalmanFilter::calculateKalmanFilter_CV(QVector<QPointF> airplaneTrajectory, qreal updateTime,
                                                        qreal sigmaCoord,
                                                        qreal sigmaVelocity,
                                                        qreal sigmaAcceleration) {

    QVector<QPointF> resultTrajectory;
    //QVector<QPointF> resultVelocity;

    QVector<qreal> time;
    for(int i = 0; i < (airplaneTrajectory.size() + 1); ++i) {
        time.append(i * updateTime);
    }

    QPointF startVelocity = (airplaneTrajectory[2] - airplaneTrajectory[0]) / (time[2] - time[0]);

    for(int i = 0; i < 3; ++i) {
        resultTrajectory.append(airplaneTrajectory[i]);
        //resultVelocity.append(sqrt(pow(startVelocityX, 2.0) + pow(startVelocityY, 2.0)));
    }

    // Start initialization filter
    // I -- единичная матрица 4х4
    Matrix I(4, 4);
    for(quint8 i = 0; i < 4; ++i) {
        I.set(i, i, 1);
    }

    // A -- матрица перехода
    Matrix A(4, 4);
    for(quint8 i = 0; i < 4; ++i) {
        A.set(i, i, 1);
    }
    A.set(0, 2, updateTime);
    A.set(1, 3, updateTime);

    // H -- взвешенная матрица
    Matrix H(2, 4);
    H.set(0, 0, 1);
    H.set(1, 1, 1);

    // R -- взвешенная матрица шума
    Matrix R(2, 2);
    R.set(0, 0, pow(sigmaCoord, 2.0));
    R.set(1, 1, pow(sigmaCoord, 2.0));

    // G -- вспомогательная матрица для вычисления Q
    Matrix G(4, 1);
    G.set(0, 0, pow(updateTime, 2.0) / 2.0);
    G.set(1, 0, pow(updateTime, 2.0) / 2.0);
    G.set(2, 0, updateTime);
    G.set(3, 0, updateTime);

    // Q -- ковариационная матрица шума
    Matrix Q(4, 4);
    Q = G.multiply(G.transpose());
    Q = Q.multiply(pow(sigmaAcceleration, 2.0));

    // x -- вектор состояния, x_predicted -- предсказанный вектор состояния
    Matrix x(4, 1);
    x.set(0, 0, airplaneTrajectory[2].x());
    x.set(1, 0, airplaneTrajectory[2].y());
    x.set(2, 0, startVelocity.x());
    x.set(3, 0, startVelocity.y());
    x = A.multiply(x);

    // P -- ковариационная матрица
    Matrix P(4, 4);
    P.set(0, 0, pow(sigmaCoord, 2.0));
    P.set(1, 1, pow(sigmaCoord, 2.0));
    P.set(2, 2, pow(sigmaVelocity, 2.0));
    P.set(3, 3, pow(sigmaVelocity, 2.0));
    P = A.multiply(P).multiply(A.transpose()).sum(Q);

    for(int i = 3; i < airplaneTrajectory.size(); ++i) {
        // измеренные v_x и v_y
        Matrix z(2, 1);
        z.set(0, 0, airplaneTrajectory[i].x());
        z.set(1, 0, airplaneTrajectory[i].y());

        Matrix K = P.multiply(H.transpose()).multiply(
                    (H.multiply(P).multiply(H.transpose()).sum(R)).calculateInverseMatrix()
                    );

        x = x.sum(K.multiply(z.difference(H.multiply(x))));

        resultTrajectory.append(QPointF(x.get(0, 0), x.get(1, 0)));
        //resultVelocity.append(sqrt(pow(x.get(2, 0), 2.0) + pow(x.get(3, 0), 2.0)));

        P = I.difference(K.multiply(H)).multiply(P);
        x = A.multiply(x);
        P = A.multiply(P).multiply(A.transpose()).sum(Q);
    }

    return resultTrajectory;
}
