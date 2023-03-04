#include <QCoreApplication>
#include <QTest>
#include "test_matrix.h"
#include "test_alphabetafilter.h"
#include "test_kalmanfilter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTest::qExec(new Test_Matrix);

    QTest::qExec(new Test_AlphaBetaFilter);

    QTest::qExec(new Test_KalmanFilter);

    exit(0);
}
