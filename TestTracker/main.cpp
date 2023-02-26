#include <QCoreApplication>
#include <QtTest>

#include "test_asterixreader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTest::qExec(new Test_AsterixReader);

    return a.exec();
}
