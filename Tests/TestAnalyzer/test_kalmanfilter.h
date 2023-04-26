#ifndef TEST_KALMANFILTER_H
#define TEST_KALMANFILTER_H

#include <QObject>
#include <QTest>
#include "../../PriFiles/FilterCheckingFiles/Filters/KalmanFilter.h"

class Test_KalmanFilter : public QObject
{
    Q_OBJECT
public:
    explicit Test_KalmanFilter(QObject *parent = nullptr);

signals:

private slots:

    void calculateKalmanFilter_CV_LinearWithoutNoise();

    void calculateKalmanFilter_CV_LinearWithNoise();
};

#endif // TEST_KALMANFILTER_H
