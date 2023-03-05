#include <QCoreApplication>
#include <QtTest>

#include "test_asterixreader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int code1 = QTest::qExec(new Test_AsterixReader);

    exit(code1);
}
