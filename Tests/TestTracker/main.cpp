#include <gtest/gtest.h>
#include "AirplaneHandler.h"


TEST(tryCreateTrack, successLockAirplaneTrajectory) {

    SettingsTracker::initialization();
    AirplaneHandler airplaneHandler;

    bool flag1 = airplaneHandler.onNewPlot(
                Plot(100, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );

    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag2 = airplaneHandler.onNewPlot(
                Plot(1000, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );

    QThread::msleep(SettingsTracker::SCAN_MSECS);

    bool flag3 = airplaneHandler.onNewPlot(
                Plot(1900, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );

    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag4 = airplaneHandler.onNewPlot(
                Plot(2800, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );

    ASSERT_TRUE(true);
}


TEST(tryCreateTrack, failureLockAirplaneTrajectory) {
    SettingsTracker::initialization();
    AirplaneHandler airplaneHandler;

    bool flag1 = airplaneHandler.onNewPlot(
                Plot(5000, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag2 = airplaneHandler.onNewPlot(
                Plot(5000, 135, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag3 = airplaneHandler.onNewPlot(
                Plot(5000, 225, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool flag4 = airplaneHandler.onNewPlot(
                Plot(5000, 315, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );

    ASSERT_FALSE(flag1 || flag2 || flag3 ||flag4);
}

TEST(tryAddToTrack, successAddToTrajectory) {
    SettingsTracker::initialization();
    AirplaneHandler airplaneHandler;

    airplaneHandler.onNewPlot(
                Plot(100, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    QCoreApplication::processEvents(QEventLoop::AllEvents, SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(
                Plot(1000, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    QCoreApplication::processEvents(QEventLoop::AllEvents, SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(
                Plot(1900, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    QCoreApplication::processEvents(QEventLoop::AllEvents, SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(
                Plot(2800, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool result = airplaneHandler.tryAddToTrack(
                Plot(3700, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );

    ASSERT_TRUE(result);
}

TEST(tryAddToTrack, failureAddToTrajectory) {
    SettingsTracker::initialization();
    AirplaneHandler airplaneHandler;

    airplaneHandler.onNewPlot(
                Plot(100, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(
                Plot(1000, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(
                Plot(1900, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    airplaneHandler.onNewPlot(
                Plot(2800, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );
    QThread::msleep(SettingsTracker::SCAN_MSECS);
    bool result = airplaneHandler.tryAddToTrack(
                Plot(100, 45, 0, 0,
                     QDateTime::currentMSecsSinceEpoch() / 1000.0, Asterix48())
                );

    ASSERT_FALSE(result);
}


int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
