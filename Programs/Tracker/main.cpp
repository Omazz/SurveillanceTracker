#include <QCoreApplication>
#include "AirplaneHandler.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SettingsTracker::initialization();

    qDebug() << "\nProgram successfully start with settings!"
             << "\n\nANTETNNA SYSTEM PARAMETERS"
             << "\nScan period, msecs: " << SettingsTracker::SCAN_MSECS
             << "\nWait information, msecs: " << SettingsTracker::WAIT_INFO_MSECS
             << "\n\nLOCK PARAMETERS"
             << "\nMin velocity, m/secs: " << SettingsTracker::MIN_VELOCITY_M_SECS
             << "\nMax velocity, m/secs: " << SettingsTracker::MAX_VELOCITY_M_SECS
             << "\nNumber of plots to lock: " << SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK
             << "\nNumber of scans to lock: " << SettingsTracker::NUMBER_OF_SCANS_TO_LOCK
             << "\n\nHOLD PARAMETERS"
             << "\nMean deviation of distance, m: " << SettingsTracker::MEAN_DEVIATION_RHO_M
             << "\nMean deviation of angle, angular minutes: " << qRound(60.0 * qRadiansToDegrees(SettingsTracker::MEAN_DEVIATION_ANGLE_RAD))
             << "\nNumber of missing plots to remove track: " << SettingsTracker::NUMBER_OF_MISSING_PLOTS
             << "\nCoefficient increase strobe of hold: " << SettingsTracker::COEF_STROBE_HOLD
             << "\n\nMANEUVER PARAMETERS"
             << "\nVelocity, m/s: " << SettingsTracker::MANEUVER_VELOCITY_M_SECS
             << "\nAngle, deg: " << SettingsTracker::MANEUVER_ANGLE_DEG
             << "\n\nEnd settings!\n";

    AirplaneHandler ah;


    return a.exec();
}
