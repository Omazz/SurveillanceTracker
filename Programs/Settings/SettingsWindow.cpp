#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    loadSettings();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}


void SettingsWindow::on_PB_apply_clicked()
{
    saveSettings();
}


void SettingsWindow::loadSettings() {

    /* ENCRYPTION_PARAMETERS */
    ui->LE_key->setText(SettingsTracker::KEY);
    ui->LE_iv->setText(SettingsTracker::INITIALIZING_VECTOR);

    /* ANTENNA SYSTEM PARAMETERS */
    ui->DSB_scanPeriod->setValue(qRound(SettingsTracker::SCAN_MSECS));
    ui->DSB_waitInfo->setValue(qRound(SettingsTracker::WAIT_INFO_MSECS));

    /* LOCK PARAMETERS */
    if(SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK == 2 &&
       SettingsTracker::NUMBER_OF_SCANS_TO_LOCK == 2) {

        ui->RB_2_2->setChecked(true);

    } else if(SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK == 2 &&
              SettingsTracker::NUMBER_OF_SCANS_TO_LOCK == 3) {

        ui->RB_2_3->setChecked(true);

    } else if(SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK == 2 &&
              SettingsTracker::NUMBER_OF_SCANS_TO_LOCK == 4) {

        ui->RB_2_4->setChecked(true);

    } else if(SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK == 3 &&
              SettingsTracker::NUMBER_OF_SCANS_TO_LOCK == 3) {

        ui->RB_3_3->setChecked(true);

    } else if(SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK == 3 &&
              SettingsTracker::NUMBER_OF_SCANS_TO_LOCK == 4) {

        ui->RB_3_4->setChecked(true);

    } else if(SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK == 4 &&
              SettingsTracker::NUMBER_OF_SCANS_TO_LOCK == 4) {

        ui->RB_4_4->setChecked(true);

    }


    /* HOLD PARAMETERS */
    ui->SB_numberExtrapolations->setValue(SettingsTracker::NUMBER_OF_MISSING_PLOTS);
    ui->DSB_velocityMin->setValue(SettingsTracker::MIN_VELOCITY_M_SECS);
    ui->DSB_velocityMax->setValue(SettingsTracker::MAX_VELOCITY_M_SECS);
    ui->DSB_difAngle->setValue(SettingsTracker::MAX_ANGLE_DEG);
    ui->DSB_coefStrobeHold->setValue(SettingsTracker::COEF_STROBE_HOLD);

    /* MANEUVER PARAMETERS */
    ui->DSB_velocity->setValue(qRound(SettingsTracker::MANEUVER_VELOCITY_M_SECS));
    ui->DSB_angle->setValue(qRound(SettingsTracker::MANEUVER_ANGLE_DEG));
}

void SettingsWindow::saveSettings() {
    /* ENCRYPTION_PARAMETERS */
    QString keyString = ui->LE_key->text();
    while(keyString.size() < 64) {
        keyString.push_front("0");
    }
    SettingsTracker::KEY = keyString;

    QString ivString = ui->LE_iv->text();
    while(ivString.size() < 32) {
        ivString.push_front("0");
    }
    SettingsTracker::INITIALIZING_VECTOR = ivString;

    /* ANTENNA SYSTEM PARAMETERS */
    SettingsTracker::SCAN_MSECS = ui->DSB_scanPeriod->value();
    SettingsTracker::WAIT_INFO_MSECS = ui->DSB_waitInfo->value();

    /* LOCK PARAMETERS */
    if(ui->RB_2_2->isChecked()) {
        SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK = 2;
        SettingsTracker::NUMBER_OF_SCANS_TO_LOCK = 2;
    } else if(ui->RB_2_3->isChecked()) {
        SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK = 2;
        SettingsTracker::NUMBER_OF_SCANS_TO_LOCK = 3;
    } else if(ui->RB_2_4->isChecked()) {
        SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK = 2;
        SettingsTracker::NUMBER_OF_SCANS_TO_LOCK = 4;
    } else if(ui->RB_3_3->isChecked()) {
        SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK = 3;
        SettingsTracker::NUMBER_OF_SCANS_TO_LOCK = 3;
    } else if(ui->RB_3_4->isChecked()) {
        SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK = 3;
        SettingsTracker::NUMBER_OF_SCANS_TO_LOCK = 4;
    } else if(ui->RB_4_4->isChecked()) {
        SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK = 4;
        SettingsTracker::NUMBER_OF_SCANS_TO_LOCK = 4;
    }

    /* HOLD PARAMETERS */
    SettingsTracker::NUMBER_OF_MISSING_PLOTS = ui->SB_numberExtrapolations->value();
    SettingsTracker::MIN_VELOCITY_M_SECS = ui->DSB_velocityMin->value();
    SettingsTracker::MAX_VELOCITY_M_SECS = ui->DSB_velocityMax->value();
    SettingsTracker::MAX_ANGLE_DEG = ui->DSB_difAngle->value();
    SettingsTracker::COEF_STROBE_HOLD = ui->DSB_coefStrobeHold->value();

    /* MANEUVER PARAMETERS */
    SettingsTracker::MANEUVER_VELOCITY_M_SECS = ui->DSB_velocity->value();
    SettingsTracker::MANEUVER_ANGLE_DEG = ui->DSB_angle->value();

    SettingsTracker::saveParameters();
}

void SettingsWindow::on_PB_load_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "", "SurveillanceTracker.conf", "*.conf");

    if(!fileName.isEmpty()) {
        QSettings settings(fileName, QSettings::IniFormat);

        if(settings.status() == QSettings::NoError) {
            /* ENCRYPTION_PARAMETERS */
            settings.beginGroup("ENCRYPTION_PARAMETERS");
            ui->LE_key->setText(settings.value("KEY").toString());
            ui->LE_iv->setText(settings.value("INITIALIZING_VECTOR").toString());
            settings.endGroup();

            /* ANTENNA SYSTEM PARAMETERS */
            settings.beginGroup("ANTENNA_SYSTEM_PARAMETERS");
            ui->DSB_scanPeriod->setValue(settings.value("SCAN_MSECS").toDouble());
            ui->DSB_waitInfo->setValue(settings.value("WAIT_INFO_MSECS").toDouble());
            settings.endGroup();

            /* LOCK PARAMETERS */
            settings.beginGroup("LOCK_PARAMETERS");
            if(settings.value("NUMBER_OF_PLOTS_TO_LOCK").toUInt() == 2 &&
                    settings.value("NUMBER_OF_SCANS_TO_LOCK").toUInt() == 2) {

                ui->RB_2_2->setChecked(true);

            } else if(settings.value("NUMBER_OF_PLOTS_TO_LOCK").toUInt() == 2 &&
                      settings.value("NUMBER_OF_SCANS_TO_LOCK").toUInt() == 3) {

                ui->RB_2_3->setChecked(true);

            } else if(settings.value("NUMBER_OF_PLOTS_TO_LOCK").toUInt() == 2 &&
                      settings.value("NUMBER_OF_SCANS_TO_LOCK").toUInt() == 4) {

                ui->RB_2_4->setChecked(true);

            } else if(settings.value("NUMBER_OF_PLOTS_TO_LOCK").toUInt() == 3 &&
                      settings.value("NUMBER_OF_SCANS_TO_LOCK").toUInt() == 3) {

                ui->RB_3_3->setChecked(true);

            } else if(settings.value("NUMBER_OF_PLOTS_TO_LOCK").toUInt() == 3 &&
                      settings.value("NUMBER_OF_SCANS_TO_LOCK").toUInt() == 4) {

                ui->RB_3_4->setChecked(true);

            } else if(settings.value("NUMBER_OF_PLOTS_TO_LOCK").toUInt() == 4 &&
                      settings.value("NUMBER_OF_SCANS_TO_LOCK").toUInt() == 4) {

                ui->RB_4_4->setChecked(true);

            }
            settings.endGroup();

            /* HOLD PARAMETERS */
            settings.beginGroup("HOLD_PARAMETERS");
            ui->SB_numberExtrapolations->setValue(settings.value("NUMBER_OF_MISSING_PLOTS").toUInt());
            ui->DSB_velocityMin->setValue(settings.value("MIN_VELOCITY_M_SECS").toDouble());
            ui->DSB_velocityMax->setValue(settings.value("MAX_VELOCITY_M_SECS").toDouble());
            ui->DSB_difAngle->setValue(settings.value("MAX_ANGLE_DEG").toDouble());
            ui->DSB_coefStrobeHold->setValue(settings.value("COEF_STROBE_HOLD").toDouble());
            settings.endGroup();

            /* MANEUVER PARAMETERS */
            settings.beginGroup("MANEUVER_PARAMETERS");
            ui->DSB_velocity->setValue(settings.value("MANEUVER_VELOCITY_M_SECS").toDouble());
            ui->DSB_angle->setValue(settings.value("MANEUVER_ANGLE_DEG").toDouble());
            settings.endGroup();
        }
    }
}


void SettingsWindow::on_PB_save_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "", "SurveillanceTracker.conf", "*.conf");

    if(!fileName.isEmpty()) {

        QSettings settings(fileName, QSettings::IniFormat);

        if(settings.status() == QSettings::NoError) {
            /* ENCRYPTION PARAMETERS */
            settings.beginGroup("ENCRYPTION_PARAMETERS");
            QString keyString = ui->LE_key->text();
            while(keyString.size() < 64) {
                keyString.push_front("0");
            }
            settings.setValue("KEY", keyString);
            QString ivString = ui->LE_iv->text();
            while(ivString.size() < 32) {
                ivString.push_front("0");
            }
            settings.setValue("INITIALIZING_VECTOR", ivString);
            settings.endGroup();

            /* ANTENNA SYSTEM PARAMETERS*/
            settings.beginGroup("ANTENNA_SYSTEM_PARAMETERS");
            settings.setValue("SCAN_MSECS", ui->DSB_scanPeriod->value());
            settings.setValue("WAIT_INFO_MSECS", ui->DSB_waitInfo->value());
            settings.endGroup();

            /* LOCK PARAMETERS */
            settings.beginGroup("LOCK_PARAMETERS");
            if(ui->RB_2_2->isChecked()) {
                settings.setValue("NUMBER_OF_PLOTS_TO_LOCK", 2);
                settings.setValue("NUMBER_OF_SCANS_TO_LOCK", 2);
            } else if(ui->RB_2_3->isChecked()) {
                settings.setValue("NUMBER_OF_PLOTS_TO_LOCK", 2);
                settings.setValue("NUMBER_OF_SCANS_TO_LOCK", 3);
            } else if(ui->RB_2_4->isChecked()) {
                settings.setValue("NUMBER_OF_PLOTS_TO_LOCK", 2);
                settings.setValue("NUMBER_OF_SCANS_TO_LOCK", 4);
            } else if(ui->RB_3_3->isChecked()) {
                settings.setValue("NUMBER_OF_PLOTS_TO_LOCK", 3);
                settings.setValue("NUMBER_OF_SCANS_TO_LOCK", 3);
            } else if(ui->RB_3_4->isChecked()) {
                settings.setValue("NUMBER_OF_PLOTS_TO_LOCK", 3);
                settings.setValue("NUMBER_OF_SCANS_TO_LOCK", 4);
            } else if(ui->RB_4_4->isChecked()) {
                settings.setValue("NUMBER_OF_PLOTS_TO_LOCK", 4);
                settings.setValue("NUMBER_OF_SCANS_TO_LOCK", 4);
            }
            settings.endGroup();

            /* HOLD PARAMETERS */
            settings.beginGroup("HOLD_PARAMETERS");
            settings.setValue("NUMBER_OF_MISSING_PLOTS", ui->SB_numberExtrapolations->value());
            settings.setValue("MIN_VELOCITY_M_SECS", ui->DSB_velocityMin->value());
            settings.setValue("MAX_VELOCITY_M_SECS", ui->DSB_velocityMax->value());
            settings.setValue("MAX_ANGLE_DEG", ui->DSB_difAngle->value());
            settings.setValue("COEF_STROBE_HOLD", ui->DSB_coefStrobeHold->value());
            settings.endGroup();

            /* MANEUVER PARAMETERS */
            settings.beginGroup("MANEUVER_PARAMETERS");
            settings.setValue("MANEUVER_VELOCITY_M_SECS", ui->DSB_velocity->value());
            settings.setValue("MANEUVER_ANGLE_DEG", ui->DSB_angle->value());
            settings.endGroup();
        }
    }
}

