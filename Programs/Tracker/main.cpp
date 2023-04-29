#include <QCoreApplication>
#include "AirplaneHandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SettingsTracker::initialization();

    AirplaneHandler ah;


    return a.exec();
}
