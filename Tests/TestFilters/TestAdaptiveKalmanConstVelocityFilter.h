#ifndef TESTADAPTIVEKALMANCONSTVELOCITYFILTER_H
#define TESTADAPTIVEKALMANCONSTVELOCITYFILTER_H


#include <QObject>
#include <QTest>
#include "../../PriFiles/Filters/AdaptiveKalmanConstVelocityFilter.h"

class TestAdaptiveKalmanConstVelocityFilter : public QObject
{
    Q_OBJECT
public:
    explicit TestAdaptiveKalmanConstVelocityFilter(QObject *parent = nullptr);

signals:

private slots:
    void filterTrack_linearWithoutNoise();

    void filterTrack_linearWithNoise();

};

#endif // TESTADAPTIVEKALMANCONSTVELOCITYFILTER_H
