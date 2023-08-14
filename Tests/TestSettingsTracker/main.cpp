#include <gtest/gtest.h>
#include "SettingsTracker.h"

TEST(initialization, checkField_SCAN_MSECS) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::SCAN_MSECS >= 1000) &&
                (SettingsTracker::SCAN_MSECS <= 10000));
}

TEST(initialization, checkField_WAIT_INFO_MSECS) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::WAIT_INFO_MSECS > 0) &&
             (SettingsTracker::WAIT_INFO_MSECS < 1000));
}

TEST(initialization, checkField_NUMBER_OF_PLOTS_TO_LOCK) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK >= 2) &&
                (SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK <= 4));
}

TEST(initialization, checkField_NUMBER_OF_SCANS_TO_LOCK) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::NUMBER_OF_SCANS_TO_LOCK >= 2) &&
                (SettingsTracker::NUMBER_OF_SCANS_TO_LOCK <= 4));
}

TEST(initialization, checkField_NUMBER_OF_MISSING_PLOTS) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::NUMBER_OF_MISSING_PLOTS >= 2) &&
                (SettingsTracker::NUMBER_OF_MISSING_PLOTS <= 6));
}

TEST(initialization, checkField_MIN_VELOCITY_M_SECS) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::MIN_VELOCITY_M_SECS >= 0) &&
                (SettingsTracker::MIN_VELOCITY_M_SECS <= 95));
}

TEST(initialization, checkField_MAX_VELOCITY_M_SECS) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::MAX_VELOCITY_M_SECS >= 95) &&
                (SettingsTracker::MAX_VELOCITY_M_SECS <= 350));
}


TEST(initialization, checkField_MEAN_DEVIATION_RHO_M) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::MEAN_DEVIATION_RHO_M >= 10) &&
                (SettingsTracker::MEAN_DEVIATION_RHO_M <= 150));
}

TEST(initialization, checkField_MEAN_DEVIATION_ANGLE_ARCMIN) {
    SettingsTracker::initialization();
    qreal angleDeviation = qRound(qRadiansToDegrees(SettingsTracker::MEAN_DEVIATION_ANGLE_RAD) * 60.0);
    ASSERT_TRUE((angleDeviation >= 3) &&
                (angleDeviation <= 48));
}

TEST(initialization, checkField_COEF_STROBE_HOLD) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::COEF_STROBE_HOLD >= 1) &&
                (SettingsTracker::COEF_STROBE_HOLD <= 2));
}

TEST(initialization, checkField_MANEUVER_VELOCITY_M_SECS) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::MANEUVER_VELOCITY_M_SECS > 0) &&
                (SettingsTracker::MANEUVER_VELOCITY_M_SECS <= 60));
}

TEST(initialization, checkField_MANEUVER_ANGLE_DEG) {
    SettingsTracker::initialization();
    ASSERT_TRUE((SettingsTracker::MANEUVER_ANGLE_DEG > 0) &&
                (SettingsTracker::MANEUVER_ANGLE_DEG <= 120));
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
