#ifndef TESTSETTINGSTRACKER_H
#define TESTSETTINGSTRACKER_H


#include <QObject>
#include <QTest>
#include "../../PriFiles/Settings/SettingsTracker.h"

class TestSettingsTracker : public QObject
{
    Q_OBJECT
public:
    explicit TestSettingsTracker(QObject *parent = nullptr);

signals:

private slots:

    void initialization_checkField_SCAN_MSECS();

    void initialization_checkField_WAIT_INFO_MSECS();

    void initialization_checkField_NUMBER_OF_PLOTS_TO_LOCK();

    void initialization_checkField_NUMBER_OF_SCANS_TO_LOCK();

    void initialization_checkField_NUMBER_OF_MISSING_PLOTS();

    void initialization_checkField_MIN_VELOCITY_M_SECS();

    void initialization_checkField_MAX_VELOCITY_M_SECS();

    void initialization_checkField_MEAN_DEVIATION_RHO_M();

    void initialization_checkField_MEAN_DEVIATION_ANGLE_ARCMIN();

    void initialization_checkField_COEF_STROBE_HOLD();

    void initialization_checkField_MANEUVER_VELOCITY_M_SECS();

    void initialization_checkField_MANEUVER_ANGLE_DEG();
};
#endif // TESTSETTINGSTRACKER_H
