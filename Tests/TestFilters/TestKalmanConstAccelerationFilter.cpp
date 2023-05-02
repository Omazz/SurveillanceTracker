#include "TestKalmanConstAccelerationFilter.h"

TestKalmanConstAccelerationFilter::TestKalmanConstAccelerationFilter(QObject *parent)
    : QObject{parent}
{

}

void TestKalmanConstAccelerationFilter::filterTrack_linearWithoutNoise() {

    QVector<Target> inputTrajectory;

    for(int i = 0; i < 20; i++) {
        inputTrajectory.append(Target(QPointF(5 * i, 10 * i), 5 * i));
    }

    KalmanConstAccelerationFilter filter(15, 5, 3);
    filter.initialization({inputTrajectory[0], inputTrajectory[1], inputTrajectory[2]});
    QVector<Target> filteredTrajectory;
    filteredTrajectory += inputTrajectory[0];
    filteredTrajectory += inputTrajectory[1];
    filteredTrajectory += inputTrajectory[2];
    for(int i = 3; i < inputTrajectory.size(); ++i) {
        filteredTrajectory += filter.filterMeasuredValue(inputTrajectory[i]);
    }

    QPointF mse(0, 0);

    for(int i = 0; i < inputTrajectory.size(); ++i) {
        mse += QPointF(qPow(filteredTrajectory[i].coordinate.x() - inputTrajectory[i].coordinate.x(), 2),
                       qPow(filteredTrajectory[i].coordinate.y() - inputTrajectory[i].coordinate.y(), 2));
    }

    mse /= inputTrajectory.size();
    mse.setX(qSqrt(mse.x()));
    mse.setY(qSqrt(mse.y()));
    qreal result = qSqrt(qPow(mse.x(), 2) + qPow(mse.y(), 2));
    QCOMPARE(result < 0.5, true);
}

void TestKalmanConstAccelerationFilter::filterTrack_linearWithNoise() {

    QVector<Target> inputTrajectory;

    for(int i = 0; i < 20; i++) {
        inputTrajectory.append(
                    Target(QPointF(5 * i + (3.0 * (2.0 * QRandomGenerator::global()->generateDouble() - 1.0)),
                                   10 * i+ (3.0 * (2.0 * QRandomGenerator::global()->generateDouble() - 1.0))),
                           5 * i));
    }

    KalmanConstAccelerationFilter filter(15, 5, 3);
    filter.initialization({inputTrajectory[0], inputTrajectory[1], inputTrajectory[2]});
    QVector<Target> filteredTrajectory;
    filteredTrajectory += inputTrajectory[0];
    filteredTrajectory += inputTrajectory[1];
    filteredTrajectory += inputTrajectory[2];
    for(int i = 3; i < inputTrajectory.size(); ++i) {
        filteredTrajectory += filter.filterMeasuredValue(inputTrajectory[i]);
    }

    QPointF mse(0, 0);

    for(int i = 0; i < inputTrajectory.size(); ++i) {
        mse += QPointF(qPow(filteredTrajectory[i].coordinate.x() - inputTrajectory[i].coordinate.x(), 2),
                       qPow(filteredTrajectory[i].coordinate.y() - inputTrajectory[i].coordinate.y(), 2));
    }

    mse /= inputTrajectory.size();
    mse.setX(qSqrt(mse.x()));
    mse.setY(qSqrt(mse.y()));
    qreal result = qSqrt(qPow(mse.x(), 2) + qPow(mse.y(), 2));
    QCOMPARE(result < 5, true);
}
