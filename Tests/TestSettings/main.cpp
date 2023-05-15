#include <QCoreApplication>
#include <QtTest>

#include "TestSettingsTracker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int code1 = QTest::qExec(new TestSettingsTracker);

    exit(code1);
}
