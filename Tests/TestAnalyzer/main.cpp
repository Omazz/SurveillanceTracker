#include <QCoreApplication>
#include <QTest>
#include "test_matrix.h"
#include "test_alphabetafilter.h"
#include "test_kalmanfilter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int code1 = QTest::qExec(new Test_Matrix);

    int code2 = QTest::qExec(new Test_AlphaBetaFilter);

    int code3 = QTest::qExec(new Test_KalmanFilter);


    exit(code1 + code2 + code3);
}
