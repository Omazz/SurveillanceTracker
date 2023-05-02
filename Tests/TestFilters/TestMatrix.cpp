#include "TestMatrix.h"

TestMatrix::TestMatrix(QObject *parent)
    : QObject{parent}
{

}


void TestMatrix::sum_calculateSumMatrices() {
    Matrix a(2, 2);
    a.set(0, 0, 1);
    a.set(1, 0, 2);
    a.set(1, 1, 3);
    a.set(0, 1, 4);

    Matrix b(2, 2);
    b.set(0, 0, 4);
    b.set(1, 0, 3);
    b.set(1, 1, 2);
    b.set(0, 1, 1);

    Matrix result = a.sum(b);
    QCOMPARE((result.get(0, 0) == 5) && (result.get(1, 0) == 5) && (result.get(1, 1) == 5) && (result.get(0, 1) == 5),
             true);

}

void TestMatrix::difference_calculateDifferenceMatrices() {
    Matrix a(2, 2);
    a.set(0, 0, 1);
    a.set(1, 0, 2);
    a.set(1, 1, 3);
    a.set(0, 1, 4);

    Matrix b(2, 2);
    b.set(0, 0, 4);
    b.set(1, 0, 3);
    b.set(1, 1, 2);
    b.set(0, 1, 1);

    Matrix result = a.difference(b);
    QCOMPARE((result.get(0, 0) == -3) && (result.get(1, 0) == -1) && (result.get(1, 1) == 1) && (result.get(0, 1) == 3),
             true);
}

void TestMatrix::multiply_calculateMultiplyMatrices() {
    Matrix a(2, 2);
    a.set(0, 0, 1);
    a.set(1, 0, 2);
    a.set(1, 1, 3);
    a.set(0, 1, 4);

    Matrix b(2, 2);
    b.set(0, 0, 4);
    b.set(1, 0, 3);
    b.set(1, 1, 2);
    b.set(0, 1, 1);

    Matrix result = a.multiply(b);
    QCOMPARE((result.get(0, 0) == 16) && (result.get(1, 0) == 17) && (result.get(1, 1) == 8) && (result.get(0, 1) == 9),
             true);
}

void TestMatrix::transpose_calculateTransposeMatrix() {
    Matrix a(2, 2);
    a.set(0, 0, 1);
    a.set(1, 0, 2);
    a.set(1, 1, 3);
    a.set(0, 1, 4);

    Matrix result = a.transpose();
    QCOMPARE((result.get(0, 0) == 1) && (result.get(1, 0) == 4) && (result.get(1, 1) == 3) && (result.get(0, 1) == 2),
             true);
}

void TestMatrix::calculateDeterminant_calculateDeterminantMatrix() {
    Matrix a(2, 2);
    a.set(0, 0, 1);
    a.set(1, 0, 2);
    a.set(1, 1, 3);
    a.set(0, 1, 4);

    qreal determinant = a.calculateDeterminant();
    QCOMPARE(qFuzzyCompare(determinant, -5),
             true);
}

void TestMatrix::calculateInverseMatrix_calculateInverseMatrix() {
    Matrix a(2, 2);
    a.set(0, 0, 1);
    a.set(1, 0, 2);
    a.set(1, 1, 3);
    a.set(0, 1, 4);

    Matrix result = a.calculateInverseMatrix();
    QCOMPARE(qFuzzyCompare(result.get(0, 0), -0.6) && qFuzzyCompare(result.get(1, 0), 0.8) &&
             qFuzzyCompare(result.get(1, 1), -0.2) && qFuzzyCompare(result.get(0, 1), 0.4),
             true);
}

void TestMatrix::calculateAlgebraicAdjunct_calculateAlgebraicAdjunctMatrix() {
    Matrix a(3, 3);
    a.set(0, 0, 1);
    a.set(0, 1, 2);
    a.set(0, 2, 3);
    a.set(1, 0, 4);
    a.set(1, 1, 5);
    a.set(1, 2, 6);
    a.set(2, 0, 7);
    a.set(2, 1, 8);
    a.set(2, 2, 9);

    qreal algebraicAdjunct = a.calculateAlgebraicAdjunct(0, 0);
    QCOMPARE(qFuzzyCompare(algebraicAdjunct, -3),
             true);
}
