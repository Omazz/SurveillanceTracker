#include <QCoreApplication>
#include <QTest>
#include "TestMatrix.h"
#include "TestAlphaBetaFilter.h"
#include "TestAlphaBetaLeastSquaresFilter.h"
#include "TestKalmanConstVelocityFilter.h"
#include "TestKalmanConstAccelerationFilter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int code1 = QTest::qExec(new TestMatrix);

    int code2 = QTest::qExec(new TestAlphaBetaFilter);

    int code3 = QTest::qExec(new TestAlphaBetaLeastSquaresFilter);

    int code4 = QTest::qExec(new TestKalmanConstVelocityFilter);

    int code5 = QTest::qExec(new TestKalmanConstAccelerationFilter);

    exit(code1 + code2 + code3 + code4 + code5);
}
