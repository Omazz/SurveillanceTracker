#include "test_kalmanfilter.h"

Test_KalmanFilter::Test_KalmanFilter(QObject *parent)
    : QObject{parent}
{

}

void Test_KalmanFilter::calculateKalmanFilter_CV_LinearWithoutNoise() {
    QVector<QPointF> inputTrajectory;

    for(int i = 0; i < 20; i++) {
        inputTrajectory.append(QPointF(5 * i, 10 * i));
    }

    QVector<QPointF> filteredTrajectory = KalmanFilter::calculateKalmanFilter_CV(inputTrajectory, 5, 3, 1, 0);

    QPointF averageDeviation(0, 0);
    for(int i = 0; i < inputTrajectory.size(); ++i) {
        averageDeviation += QPointF(qPow(filteredTrajectory[i].x() - inputTrajectory[i].x(), 2),
                                    qPow(filteredTrajectory[i].y() - inputTrajectory[i].y(), 2));
    }
    averageDeviation /= inputTrajectory.size();
    averageDeviation.setX(qSqrt(averageDeviation.x()));
    averageDeviation.setY(qSqrt(averageDeviation.y()));
    qreal result = qSqrt(qPow(averageDeviation.x(), 2) + qPow(averageDeviation.y(), 2));

    QCOMPARE(qFuzzyCompare(result, 0), true);
}

void Test_KalmanFilter::calculateKalmanFilter_CV_LinearWithNoise() {
    QVector<QPointF> inputTrajectory;


    for(int i = 0; i < 20; i++) {
        inputTrajectory.append(QPointF(5 * i + (3.0 * (2.0 * QRandomGenerator::global()->generateDouble() - 1.0)),
                                       10 * i+ (3.0 * (2.0 * QRandomGenerator::global()->generateDouble() - 1.0))));
    }

    QVector<QPointF> filteredTrajectory = KalmanFilter::calculateKalmanFilter_CV(inputTrajectory, 5, 3, 1, 0);

    QPointF averageDeviation(0, 0);
    for(int i = 0; i < inputTrajectory.size(); ++i) {
        averageDeviation += QPointF(qPow(filteredTrajectory[i].x() - inputTrajectory[i].x(), 2),
                                    qPow(filteredTrajectory[i].y() - inputTrajectory[i].y(), 2));
    }
    averageDeviation /= (qreal) inputTrajectory.size();
    averageDeviation.setX(qSqrt(averageDeviation.x()));
    averageDeviation.setY(qSqrt(averageDeviation.y()));
    qreal result = qSqrt(qPow(averageDeviation.x(), 2) + qPow(averageDeviation.y(), 2));

    QCOMPARE(result < 2, true);
}
