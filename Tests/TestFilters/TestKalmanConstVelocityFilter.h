#ifndef TESTKALMANCONSTVELOCITYFILTER_H
#define TESTKALMANCONSTVELOCITYFILTER_H

#include <QObject>
#include <QTest>
#include "../../PriFiles/Filters/KalmanConstVelocityFilter.h"

class TestKalmanConstVelocityFilter : public QObject
{
    Q_OBJECT
public:
    explicit TestKalmanConstVelocityFilter(QObject *parent = nullptr);

signals:

private slots:
    void filterTrack_linearWithoutNoise();

    void filterTrack_linearWithNoise();

};

#endif // TESTKALMANCONSTVELOCITYFILTER_H
