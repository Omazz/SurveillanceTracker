#include "TestSettingsTracker.h"

TestSettingsTracker::TestSettingsTracker(QObject *parent)
    : QObject{parent}
{

}

void TestSettingsTracker::initialization_checkField_KEY(){
    SettingsTracker::initialization();
    QCOMPARE(!SettingsTracker::KEY.isEmpty(), true);
}

void TestSettingsTracker::initialization_checkField_INITIALIZING_VECTOR(){
    SettingsTracker::initialization();
    QCOMPARE(!SettingsTracker::INITIALIZING_VECTOR.isEmpty(), true);
}

void TestSettingsTracker::initialization_checkField_SCAN_MSECS(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::SCAN_MSECS >= 1000) && (SettingsTracker::SCAN_MSECS <= 10000), true);
}

void TestSettingsTracker::initialization_checkField_WAIT_INFO_MSECS(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::WAIT_INFO_MSECS > 0) && (SettingsTracker::WAIT_INFO_MSECS < 1000), true);
}

void TestSettingsTracker::initialization_checkField_NUMBER_OF_PLOTS_TO_LOCK(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK >= 2) && (SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK <= 4), true);
}

void TestSettingsTracker::initialization_checkField_NUMBER_OF_SCANS_TO_LOCK(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::NUMBER_OF_SCANS_TO_LOCK >= 2) && (SettingsTracker::NUMBER_OF_SCANS_TO_LOCK <= 4), true);
}

void TestSettingsTracker::initialization_checkField_NUMBER_OF_MISSING_PLOTS(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::NUMBER_OF_MISSING_PLOTS >= 2) && (SettingsTracker::NUMBER_OF_MISSING_PLOTS <= 6), true);
}

void TestSettingsTracker::initialization_checkField_MIN_VELOCITY_M_SECS(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::MIN_VELOCITY_M_SECS >= 0) && (SettingsTracker::MIN_VELOCITY_M_SECS <= 95), true);
}

void TestSettingsTracker::initialization_checkField_MAX_VELOCITY_M_SECS(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::MAX_VELOCITY_M_SECS >= 95) && (SettingsTracker::MAX_VELOCITY_M_SECS <= 350), true);
}

void TestSettingsTracker::initialization_checkField_MAX_ANGLE_DEG(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::MAX_ANGLE_DEG >= 5) && (SettingsTracker::MAX_ANGLE_DEG <= 120), true);
}

void TestSettingsTracker::initialization_checkField_COEF_STROBE_HOLD(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::COEF_STROBE_HOLD >= 1) && (SettingsTracker::COEF_STROBE_HOLD <= 2), true);
}

void TestSettingsTracker::initialization_checkField_MANEUVER_VELOCITY_M_SECS(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::MANEUVER_VELOCITY_M_SECS > 0) && (SettingsTracker::MANEUVER_VELOCITY_M_SECS <= 60), true);
}

void TestSettingsTracker::initialization_checkField_MANEUVER_ANGLE_DEG(){
    SettingsTracker::initialization();
    QCOMPARE((SettingsTracker::MANEUVER_ANGLE_DEG > 0) && (SettingsTracker::MANEUVER_ANGLE_DEG <= 120), true);
}
