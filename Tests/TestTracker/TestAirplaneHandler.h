#ifndef TESTAIRPLANEHANDLER_H
#define TESTAIRPLANEHANDLER_H

#include <QtTest>
#include "../../Programs/Tracker/AirplaneHandler.h"

class TestAirplaneHandler: public QObject
{
    Q_OBJECT
public:
    explicit TestAirplaneHandler(QObject *parent = nullptr);

private slots:

    void tryCreateTrack_successLockAirplaneTrajectory();

    void tryCreateTrack_failureLockAirplaneTrajectory();

    void tryAddToTrack_successAddToTrajectory();

    void tryAddToTrack_failureAddToTrajectory();
};

#endif // TESTAIRPLANEHANDLER_H
