#include <QCoreApplication>
#include <QtTest>

#include "TestCipher.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int code1 = QTest::qExec(new TestCipher);

    exit(code1);
}
