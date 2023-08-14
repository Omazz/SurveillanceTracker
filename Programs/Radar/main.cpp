#include "RadarWindow.h"
#include "AppLoadScreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    RadarWindow w;
    w.setWindowIcon(QIcon("../../../ApplicationIcons/RadarIcon.ico"));
    AppLoadScreen loadScreen(QPixmap("../../../ApplicationIcons/loadscreen_Radar.png"));
    loadScreen.showLoadingScreen(2500, &w);
    w.show();
    w.activateWindow();

    return a.exec();
}
