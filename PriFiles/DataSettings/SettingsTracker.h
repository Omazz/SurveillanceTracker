#ifndef SETTINGSTRACKER_H
#define SETTINGSTRACKER_H

#include <QtCore>

/* ENCRYPTION PARAMETERS */
static QString DEFAULT_KEY = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
static QString DEFAULT_INITIALIZING_VECTOR = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";


class SettingsTracker
{
private:
    static QSettings* _settings;

    SettingsTracker();

public:
    static void initialization();

    static void saveParameters();

    static QString KEY;
    static QString INITIALIZING_VECTOR;
};

#endif // SETTINGSTRACKER_H
