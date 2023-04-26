#include "RadarWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SettingsTracker::initialization();

    RadarWindow w;
    w.setWindowIcon(QIcon("../../ApplicationIcons/RadarIcon.ico"));
    w.show();

    return a.exec();
}
