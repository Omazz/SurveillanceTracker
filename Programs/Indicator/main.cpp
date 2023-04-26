#include "IndicatorWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsTracker::initialization();
    IndicatorWindow w;
    w.setWindowIcon(QIcon("../../ApplicationIcons/IndicatorIcon.ico"));
    w.show();
    return a.exec();
}
