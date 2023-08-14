#include <gtest/gtest.h>
#include <QCoreApplication>
#include "Matrix.h"

TEST(TestMatrix, calculateSumMatrices) {
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
    ASSERT_TRUE((result.get(0, 0) == 5) &&
                (result.get(1, 0) == 5) &&
                (result.get(1, 1) == 5) &&
                (result.get(0, 1) == 5)
                );

}

TEST(TestMatrix, calculateDifferenceMatrices) {
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
    ASSERT_TRUE((result.get(0, 0) == -3) &&
                (result.get(1, 0) == -1) &&
                (result.get(1, 1) == 1)  &&
                (result.get(0, 1) == 3)
                );
}

TEST(TestMatrix, calculateMultiplyMatrices) {
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
    ASSERT_TRUE((result.get(0, 0) == 16) &&
                (result.get(1, 0) == 17) &&
                (result.get(1, 1) == 8)  &&
                (result.get(0, 1) == 9)
                );
}

TEST(TestMatrix, calculateTransposeMatrix) {
    Matrix a(2, 2);
    a.set(0, 0, 1);
    a.set(1, 0, 2);
    a.set(1, 1, 3);
    a.set(0, 1, 4);

    Matrix result = a.transpose();
    ASSERT_TRUE((result.get(0, 0) == 1) &&
                (result.get(1, 0) == 4) &&
                (result.get(1, 1) == 3) &&
                (result.get(0, 1) == 2)
                );
}

TEST(TestMatrix, calculateDeterminantMatrix) {
    Matrix a(2, 2);
    a.set(0, 0, 1);
    a.set(1, 0, 2);
    a.set(1, 1, 3);
    a.set(0, 1, 4);

    qreal determinant = a.calculateDeterminant();
    ASSERT_TRUE(qFuzzyCompare(determinant, -5));
}

TEST(TestMatrix, calculateInverseMatrix) {
    Matrix a(2, 2);
    a.set(0, 0, 1);
    a.set(1, 0, 2);
    a.set(1, 1, 3);
    a.set(0, 1, 4);

    Matrix result = a.calculateInverseMatrix();
    ASSERT_TRUE(qFuzzyCompare(result.get(0, 0), -0.6) &&
                qFuzzyCompare(result.get(1, 0), 0.8)  &&
                qFuzzyCompare(result.get(1, 1), -0.2) &&
                qFuzzyCompare(result.get(0, 1), 0.4)
                );
}

TEST(TestMatrix, calculateAlgebraicAdjunctMatrix) {
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
    ASSERT_TRUE(qFuzzyCompare(algebraicAdjunct, -3));
}


int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
