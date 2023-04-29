#include "SettingsTracker.h"

QSettings* SettingsTracker::_settings;

/* ENCRYPTION PARAMETERS */
QString SettingsTracker::KEY;
QString SettingsTracker::INITIALIZING_VECTOR;

/* ANTENNA SYSTEM PARAMETERS*/
qreal SettingsTracker::SCAN_MSECS;
qreal SettingsTracker::WAIT_INFO_MSECS;

/* LOCK PARAMETERS */
quint8 SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK;
quint8 SettingsTracker::NUMBER_OF_SCANS_TO_LOCK;

/* HOLD PARAMETERS */
quint8 SettingsTracker::NUMBER_OF_MISSING_PLOTS;
qreal SettingsTracker::MIN_VELOCITY_M_SECS;
qreal SettingsTracker::MAX_VELOCITY_M_SECS;
qreal SettingsTracker::MAX_ANGLE_DEG;
qreal SettingsTracker::COEF_STROBE_HOLD;

/* MANEUVER PARAMETERS */
qreal SettingsTracker::MANEUVER_VELOCITY_M_SECS;
qreal SettingsTracker::MANEUVER_ANGLE_DEG;


SettingsTracker::SettingsTracker()
{

}

void SettingsTracker::initialization() {
    _settings = new QSettings("PetProjects", "SurveillanceTracker");

    if(_settings->childGroups().isEmpty()) {

        /* ENCRYPTION PARAMETERS */
        KEY = DEFAULT_KEY;
        INITIALIZING_VECTOR = DEFAULT_INITIALIZING_VECTOR;

        /* ANTENNA SYSTEM PARAMETERS*/
        SCAN_MSECS = DEFAULT_SCAN_MSECS;
        WAIT_INFO_MSECS = DEFAULT_WAIT_INFO_MSECS;

        /* LOCK PARAMETERS */
        NUMBER_OF_PLOTS_TO_LOCK = DEFAULT_NUMBER_OF_PLOTS_TO_LOCK;
        NUMBER_OF_SCANS_TO_LOCK = DEFAULT_NUMBER_OF_SCANS_TO_LOCK;

        /* HOLD PARAMETERS */
        NUMBER_OF_MISSING_PLOTS = DEFAULT_NUMBER_OF_MISSING_PLOTS;
        MIN_VELOCITY_M_SECS = DEFAULT_MIN_VELOCITY_M_SECS;
        MAX_VELOCITY_M_SECS = DEFAULT_MAX_VELOCITY_M_SECS;
        MAX_ANGLE_DEG = DEFAULT_MAX_ANGLE_DEG;
        COEF_STROBE_HOLD = DEFAULT_COEF_STROBE_HOLD;

        /* MANEUVER PARAMETERS */
        MANEUVER_VELOCITY_M_SECS = DEFAULT_MANEUVER_VELOCITY_M_SECS;
        MANEUVER_ANGLE_DEG = DEFAULT_MANEUVER_ANGLE_DEG;

        /* ENCRYPTION PARAMETERS */
        _settings->beginGroup("ENCRYPTION_PARAMETERS");
        _settings->setValue("KEY", KEY);
        _settings->setValue("INITIALIZING_VECTOR", INITIALIZING_VECTOR);
        _settings->endGroup();

        /* ANTENNA SYSTEM PARAMETERS*/
        _settings->beginGroup("ANTENNA_SYSTEM_PARAMETERS");
        _settings->setValue("SCAN_MSECS", SCAN_MSECS);
        _settings->setValue("WAIT_INFO_MSECS", WAIT_INFO_MSECS);
        _settings->endGroup();

        /* LOCK PARAMETERS */
        _settings->beginGroup("LOCK_PARAMETERS");
        _settings->setValue("NUMBER_OF_PLOTS_TO_LOCK", NUMBER_OF_PLOTS_TO_LOCK);
        _settings->setValue("NUMBER_OF_SCANS_TO_LOCK", NUMBER_OF_SCANS_TO_LOCK);
        _settings->endGroup();

        /* HOLD PARAMETERS */
        _settings->beginGroup("HOLD_PARAMETERS");
        _settings->setValue("NUMBER_OF_MISSING_PLOTS", NUMBER_OF_MISSING_PLOTS);
        _settings->setValue("MIN_VELOCITY_M_SECS", MIN_VELOCITY_M_SECS);
        _settings->setValue("MAX_VELOCITY_M_SECS", MAX_VELOCITY_M_SECS);
        _settings->setValue("MAX_ANGLE_DEG", MAX_ANGLE_DEG);
        _settings->setValue("COEF_STROBE_HOLD", COEF_STROBE_HOLD);
        _settings->endGroup();

        /* MANEUVER PARAMETERS */
        _settings->beginGroup("MANEUVER_PARAMETERS");
        _settings->setValue("MANEUVER_VELOCITY_M_SECS", MANEUVER_VELOCITY_M_SECS);
        _settings->setValue("MANEUVER_ANGLE_DEG", MANEUVER_ANGLE_DEG);
        _settings->endGroup();
    } else {

        /* ENCRYPTION PARAMETERS */
        _settings->beginGroup("ENCRYPTION_PARAMETERS");
        KEY = _settings->value("KEY", DEFAULT_KEY).toString();
        INITIALIZING_VECTOR = _settings->value("INITIALIZING_VECTOR", DEFAULT_INITIALIZING_VECTOR).toString();
        _settings->endGroup();

        /* ANTENNA SYSTEM PARAMETERS*/
        _settings->beginGroup("ANTENNA_SYSTEM_PARAMETERS");
        SCAN_MSECS = _settings->value("SCAN_MSECS", DEFAULT_SCAN_MSECS).toDouble();
        WAIT_INFO_MSECS = _settings->value("WAIT_INFO_MSECS", DEFAULT_WAIT_INFO_MSECS).toDouble();
        _settings->endGroup();

        /* LOCK PARAMETERS */
        _settings->beginGroup("LOCK_PARAMETERS");
        NUMBER_OF_PLOTS_TO_LOCK = _settings->value("NUMBER_OF_PLOTS_TO_LOCK", DEFAULT_NUMBER_OF_PLOTS_TO_LOCK).toUInt();
        NUMBER_OF_SCANS_TO_LOCK = _settings->value("NUMBER_OF_SCANS_TO_LOCK", DEFAULT_NUMBER_OF_SCANS_TO_LOCK).toUInt();
        _settings->endGroup();

        /* HOLD PARAMETERS */
        _settings->beginGroup("HOLD_PARAMETERS");
        NUMBER_OF_MISSING_PLOTS = _settings->value("NUMBER_OF_MISSING_PLOTS", DEFAULT_NUMBER_OF_MISSING_PLOTS).toUInt();
        MIN_VELOCITY_M_SECS = _settings->value("MIN_VELOCITY_M_SECS", DEFAULT_MIN_VELOCITY_M_SECS).toDouble();
        MAX_VELOCITY_M_SECS = _settings->value("MAX_VELOCITY_M_SECS", DEFAULT_MAX_VELOCITY_M_SECS).toDouble();
        MAX_ANGLE_DEG = _settings->value("MAX_ANGLE_DEG", DEFAULT_MAX_ANGLE_DEG).toDouble();
        COEF_STROBE_HOLD = _settings->value("COEF_STROBE_HOLD", DEFAULT_COEF_STROBE_HOLD).toDouble();
        _settings->endGroup();

        /* MANEUVER PARAMETERS */
        _settings->beginGroup("MANEUVER_PARAMETERS");
        MANEUVER_VELOCITY_M_SECS = _settings->value("MANEUVER_VELOCITY_M_SECS", DEFAULT_MANEUVER_VELOCITY_M_SECS).toDouble();
        MANEUVER_ANGLE_DEG = _settings->value("MANEUVER_ANGLE_DEG", DEFAULT_MANEUVER_ANGLE_DEG).toDouble();
        _settings->endGroup();
    }
}

void SettingsTracker::saveParameters() {
    /* ENCRYPTION PARAMETERS */
    _settings->beginGroup("ENCRYPTION_PARAMETERS");
    _settings->setValue("KEY", KEY);
    _settings->setValue("INITIALIZING_VECTOR", INITIALIZING_VECTOR);
    _settings->endGroup();

    /* ANTENNA SYSTEM PARAMETERS*/
    _settings->beginGroup("ANTENNA_SYSTEM_PARAMETERS");
    _settings->setValue("SCAN_MSECS", SCAN_MSECS);
    _settings->setValue("WAIT_INFO_MSECS", WAIT_INFO_MSECS);
    _settings->endGroup();

    /* LOCK PARAMETERS */
    _settings->beginGroup("LOCK_PARAMETERS");
    _settings->setValue("NUMBER_OF_PLOTS_TO_LOCK", NUMBER_OF_PLOTS_TO_LOCK);
    _settings->setValue("NUMBER_OF_SCANS_TO_LOCK", NUMBER_OF_SCANS_TO_LOCK);
    _settings->endGroup();

    /* HOLD PARAMETERS */
    _settings->beginGroup("HOLD_PARAMETERS");
    _settings->setValue("NUMBER_OF_MISSING_PLOTS", NUMBER_OF_MISSING_PLOTS);
    _settings->setValue("MIN_VELOCITY_M_SECS", MIN_VELOCITY_M_SECS);
    _settings->setValue("MAX_VELOCITY_M_SECS", MAX_VELOCITY_M_SECS);
    _settings->setValue("MAX_ANGLE_DEG", MAX_ANGLE_DEG);
    _settings->setValue("COEF_STROBE_HOLD", COEF_STROBE_HOLD);
    _settings->endGroup();

    /* MANEUVER PARAMETERS */
    _settings->beginGroup("MANEUVER_PARAMETERS");
    _settings->setValue("MANEUVER_VELOCITY_M_SECS", MANEUVER_VELOCITY_M_SECS);
    _settings->setValue("MANEUVER_ANGLE_DEG", MANEUVER_ANGLE_DEG);
    _settings->endGroup();
}
