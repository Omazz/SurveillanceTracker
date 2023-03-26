#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

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
    ui->LE_key->setText(SettingsTracker::KEY);
    ui->LE_iv->setText(SettingsTracker::INITIALIZING_VECTOR);
}

void SettingsWindow::saveSettings() {
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

    SettingsTracker::saveParameters();
}
