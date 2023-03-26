#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include "../DataSettings/SettingsTracker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWindow; }
QT_END_NAMESPACE

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private slots:
    void on_PB_apply_clicked();

private:
    void loadSettings();

    void saveSettings();

    Ui::SettingsWindow *ui;
};
#endif // SETTINGSWINDOW_H
