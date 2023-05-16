#include <QCoreApplication>
#include <QtTest>

#include "TestAirplaneHandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SettingsTracker::initialization();
    int code1 = QTest::qExec(new TestAirplaneHandler);

    exit(code1);
}
