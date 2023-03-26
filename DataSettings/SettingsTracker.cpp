#include "SettingsTracker.h"

QSettings* SettingsTracker::_settings;

/* ENCRYPTION PARAMETERS */
QString SettingsTracker::KEY;
QString SettingsTracker::INITIALIZING_VECTOR;

SettingsTracker::SettingsTracker()
{

}

void SettingsTracker::initialization() {
    _settings = new QSettings("PetProjects", "OverviewTracker");

    if(_settings->childGroups().isEmpty()) {
        KEY = DEFAULT_KEY;
        INITIALIZING_VECTOR = DEFAULT_INITIALIZING_VECTOR;

        _settings->beginGroup("ENCRYPTION");
        _settings->setValue("KEY", KEY);
        _settings->setValue("INITIALIZING_VECTOR", INITIALIZING_VECTOR);
        _settings->endGroup();

    } else {
        _settings->beginGroup("ENCRYPTION");
        KEY = _settings->value("KEY").toString();
        INITIALIZING_VECTOR = _settings->value("INITIALIZING_VECTOR").toString();
        _settings->endGroup();
    }
}

void SettingsTracker::saveParameters() {
    _settings->beginGroup("ENCRYPTION");
    _settings->setValue("KEY", KEY);
    _settings->setValue("INITIALIZING_VECTOR", INITIALIZING_VECTOR);
    _settings->endGroup();
}
