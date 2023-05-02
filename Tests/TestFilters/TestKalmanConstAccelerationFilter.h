#ifndef TESTKALMANCONSTACCELERATIONFILTER_H
#define TESTKALMANCONSTACCELERATIONFILTER_H


#include <QObject>
#include <QTest>
#include "../../PriFiles/Filters/KalmanConstAccelerationFilter.h"

class TestKalmanConstAccelerationFilter : public QObject
{
    Q_OBJECT
public:
    explicit TestKalmanConstAccelerationFilter(QObject *parent = nullptr);

signals:

private slots:
    void filterTrack_linearWithoutNoise();

    void filterTrack_linearWithNoise();

};

#endif // TESTKALMANCONSTACCELERATIONFILTER_H
