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
    if(SettingsTracker::NUMBER_PLOTS_TO_LOCK == 2 && SettingsTracker::NUMBER_SCANS_TO_LOCK == 2) {
        ui->RB_2_2->setChecked(true);
    } else if(SettingsTracker::NUMBER_PLOTS_TO_LOCK == 2 && SettingsTracker::NUMBER_SCANS_TO_LOCK == 3) {
        ui->RB_2_3->setChecked(true);
    } else if(SettingsTracker::NUMBER_PLOTS_TO_LOCK == 2 && SettingsTracker::NUMBER_SCANS_TO_LOCK == 4) {
        ui->RB_2_4->setChecked(true);
    } else if(SettingsTracker::NUMBER_PLOTS_TO_LOCK == 3 && SettingsTracker::NUMBER_SCANS_TO_LOCK == 3) {
        ui->RB_3_3->setChecked(true);
    } else if(SettingsTracker::NUMBER_PLOTS_TO_LOCK == 3 && SettingsTracker::NUMBER_SCANS_TO_LOCK == 4) {
        ui->RB_3_4->setChecked(true);
    } else if(SettingsTracker::NUMBER_PLOTS_TO_LOCK == 4 && SettingsTracker::NUMBER_SCANS_TO_LOCK == 4) {
        ui->RB_4_4->setChecked(true);
    }

    /* HOLD PARAMETERS */
    ui->SB_numberExtrapolations->setValue(SettingsTracker::NUMBER_MISSING_PLOTS);
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
        SettingsTracker::NUMBER_PLOTS_TO_LOCK = 2;
        SettingsTracker::NUMBER_SCANS_TO_LOCK = 2;
    } else if(ui->RB_2_3->isChecked()) {
        SettingsTracker::NUMBER_PLOTS_TO_LOCK = 2;
        SettingsTracker::NUMBER_SCANS_TO_LOCK = 3;
    } else if(ui->RB_2_4->isChecked()) {
        SettingsTracker::NUMBER_PLOTS_TO_LOCK = 2;
        SettingsTracker::NUMBER_SCANS_TO_LOCK = 4;
    } else if(ui->RB_3_3->isChecked()) {
        SettingsTracker::NUMBER_PLOTS_TO_LOCK = 3;
        SettingsTracker::NUMBER_SCANS_TO_LOCK = 3;
    } else if(ui->RB_3_4->isChecked()) {
        SettingsTracker::NUMBER_PLOTS_TO_LOCK = 3;
        SettingsTracker::NUMBER_SCANS_TO_LOCK = 4;
    } else if(ui->RB_4_4->isChecked()) {
        SettingsTracker::NUMBER_PLOTS_TO_LOCK = 4;
        SettingsTracker::NUMBER_SCANS_TO_LOCK = 4;
    }

    /* HOLD PARAMETERS */
    SettingsTracker::NUMBER_MISSING_PLOTS = ui->SB_numberExtrapolations->value();
    SettingsTracker::MIN_VELOCITY_M_SECS = ui->DSB_velocityMin->value();
    SettingsTracker::MAX_VELOCITY_M_SECS = ui->DSB_velocityMax->value();
    SettingsTracker::MAX_ANGLE_DEG = ui->DSB_difAngle->value();
    SettingsTracker::COEF_STROBE_HOLD = ui->DSB_coefStrobeHold->value();

    /* MANEUVER PARAMETERS */
    SettingsTracker::MANEUVER_VELOCITY_M_SECS = ui->DSB_velocity->value();
    SettingsTracker::MANEUVER_ANGLE_DEG = ui->DSB_angle->value();

    SettingsTracker::saveParameters();
}
