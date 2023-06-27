#ifndef SETTINGSTRACKER_H
#define SETTINGSTRACKER_H

#include <QtCore>

/* ENCRYPTION PARAMETERS */
static QString DEFAULT_KEY = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
static QString DEFAULT_INITIALIZING_VECTOR = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

/* ANTENNA SYSTEM PARAMETERS*/
static qreal DEFAULT_SCAN_MSECS = 5000.0;
static qreal DEFAULT_WAIT_INFO_MSECS = 50.0;

/* LOCK PARAMETERS */
static qreal DEFAULT_MIN_VELOCITY_M_SECS = 30.0;
static qreal DEFAULT_MAX_VELOCITY_M_SECS = 330.0;
static quint8 DEFAULT_NUMBER_OF_PLOTS_TO_LOCK = 3;
static quint8 DEFAULT_NUMBER_OF_SCANS_TO_LOCK = 3;

/* HOLD PARAMETERS */
static qreal DEFAULT_MEAN_DEVIATION_RHO_M = 50;
static qreal DEFAULT_MEAN_DEVIATION_ANGLE_ARCMIN = 16;
static quint8 DEFAULT_NUMBER_OF_MISSING_PLOTS = 4;
static qreal DEFAULT_COEF_STROBE_HOLD = 1.15;

/* MANEUVER PARAMETERS */
static qreal DEFAULT_MANEUVER_VELOCITY_M_SECS = 5.0;
static qreal DEFAULT_MANEUVER_ANGLE_DEG = 3.0;

class SettingsTracker
{
private:
    static QSettings* _settings;

    SettingsTracker();

public:
    static void initialization();

    static void saveParameters();

    /* ENCRYPTION PARAMETERS */
    static QString KEY;
    static QString INITIALIZING_VECTOR;

    /* ANTENNA SYSTEM PARAMETERS*/
    static qreal SCAN_MSECS;
    static qreal WAIT_INFO_MSECS;

    /* LOCK PARAMETERS */
    static qreal MIN_VELOCITY_M_SECS;
    static qreal MAX_VELOCITY_M_SECS;
    static quint8 NUMBER_OF_PLOTS_TO_LOCK;
    static quint8 NUMBER_OF_SCANS_TO_LOCK;

    /* HOLD PARAMETERS */
    static qreal MEAN_DEVIATION_RHO_M;
    static qreal MEAN_DEVIATION_ANGLE_RAD;
    static quint8 NUMBER_OF_MISSING_PLOTS;
    static qreal COEF_STROBE_HOLD;

    /* MANEUVER PARAMETERS */
    static qreal MANEUVER_VELOCITY_M_SECS;
    static qreal MANEUVER_ANGLE_DEG;
};

#endif // SETTINGSTRACKER_H
