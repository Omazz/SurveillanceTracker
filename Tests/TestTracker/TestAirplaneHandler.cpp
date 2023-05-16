#include "TestAirplaneHandler.h"



TestAirplaneHandler::TestAirplaneHandler(QObject *parent)
    : QObject{parent}
{

}

void TestAirplaneHandler::tryCreateTrack_successLockAirplaneTrajectory() {
    SettingsTracker::initialization();
    AirplaneHandler airplaneHandler;

    bool flag1 = airplaneHandler.onNewPlot(Plot(100, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag2 = airplaneHandler.onNewPlot(Plot(1000, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag3 = airplaneHandler.onNewPlot(Plot(1900, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag4 = airplaneHandler.onNewPlot(Plot(2800, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));

    QCOMPARE(flag1 || flag2 || flag3 ||flag4, true);
}

void TestAirplaneHandler::tryCreateTrack_failureLockAirplaneTrajectory() {
    SettingsTracker::initialization();
    AirplaneHandler airplaneHandler;

    bool flag1 = airplaneHandler.onNewPlot(Plot(5000, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag2 = airplaneHandler.onNewPlot(Plot(5000, 135, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag3 = airplaneHandler.onNewPlot(Plot(5000, 225, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag4 = airplaneHandler.onNewPlot(Plot(5000, 315, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));

    QCOMPARE(!(flag1 || flag2 || flag3 ||flag4), true);
}

void TestAirplaneHandler::tryAddToTrack_successAddToTrajectory() {
    SettingsTracker::initialization();
    AirplaneHandler airplaneHandler;

    airplaneHandler.onNewPlot(Plot(100, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(Plot(1000, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(Plot(1900, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(Plot(2800, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool result =
            airplaneHandler.tryAddToTrack(Plot(3700, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));

    QCOMPARE(result, true);
}

void TestAirplaneHandler::tryAddToTrack_failureAddToTrajectory() {
    SettingsTracker::initialization();
    AirplaneHandler airplaneHandler;

    airplaneHandler.onNewPlot(Plot(100, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(Plot(1000, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(Plot(1900, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(Plot(2800, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool result =
            airplaneHandler.tryAddToTrack(Plot(100, 45, 0, 0, QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48()));

    QCOMPARE(!result, true);
}
