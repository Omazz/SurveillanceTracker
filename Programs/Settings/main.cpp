#include "SettingsWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsTracker::initialization();
    SettingsWindow w;
    w.setWindowIcon(QIcon("../../../ApplicationIcons/SettingsIcon.ico"));
    AppLoadScreen loadScreen(QPixmap("../../../ApplicationIcons/loadscreen_Settings.png"));
    loadScreen.showLoadingScreen(1500, &w);
    w.show();
    w.activateWindow();
    return a.exec();
}
