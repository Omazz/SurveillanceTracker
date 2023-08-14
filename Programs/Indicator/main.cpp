#include "IndicatorWindow.h"
#include "AppLoadScreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsTracker::initialization();
    IndicatorWindow w;
    w.setWindowIcon(QIcon("../../../ApplicationIcons/IndicatorIcon.ico"));
    AppLoadScreen loadScreen(QPixmap("../../../ApplicationIcons/loadscreen_Indicator.png"));
    loadScreen.showLoadingScreen(2500, &w);
    w.show();
    w.activateWindow();
    return a.exec();
}
