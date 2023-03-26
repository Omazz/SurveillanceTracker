#include "LocatorWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SettingsTracker::initialization();

    LocatorWindow w;
    w.setWindowIcon(QIcon("../Locator/icons/LocatorIcon.ico"));
    w.show();

    return a.exec();
}