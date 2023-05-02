#include <QCoreApplication>
#include <QtTest>

#include "TestEncryptionTools.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int code1 = QTest::qExec(new TestEncryptionTools);

    exit(code1);
}
