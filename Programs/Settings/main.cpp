#include "SettingsWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsTracker::initialization();

    SettingsWindow w;
    w.setWindowIcon(QIcon("../../ApplicationIcons/SettingsIcon.ico"));
    w.show();
    return a.exec();
}
