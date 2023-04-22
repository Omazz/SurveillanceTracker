#include <QCoreApplication>
#include "AirplaneHandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SettingsTracker::initialization();
    qDebug() << "\nProgram successfully start with settings!"
             << "\n\nANTETNNA SYSTEM PARAMETERS"
             << "\nScan period, msecs: " << SettingsTracker::SCAN_MSECS
             << "\nWait information, msecs: " << SettingsTracker::WAIT_INFO_MSECS
             << "\n\nLOCK PARAMETERS"
             << "\nNumber plots to lock: " << SettingsTracker::NUMBER_PLOTS_TO_LOCK
             << "\nNumber scans to lock: " << SettingsTracker::NUMBER_SCANS_TO_LOCK
             << "\n\nHOLD PARAMETERS"
             << "\nNumber missing plots to remove track: " << SettingsTracker::NUMBER_MISSING_PLOTS
             << "\nMin velocity, m/secs: " << SettingsTracker::MIN_VELOCITY_M_SECS
             << "\nMax velocity, m/secs: " << SettingsTracker::MAX_VELOCITY_M_SECS
             << "\nMax difference angle, deg: " << SettingsTracker::MAX_ANGLE_DEG
             << "\nCoefficient increase strobe of hold: " << SettingsTracker::COEF_STROBE_HOLD
             << "\n\nMANEUVER PARAMETERS"
             << "\nVelocity, m/s: " << SettingsTracker::MANEUVER_VELOCITY_M_SECS
             << "\nAngle, deg: " << SettingsTracker::MANEUVER_ANGLE_DEG
             << "\n\nEnd settings!\n";

    AirplaneHandler ah;


    return a.exec();
}
