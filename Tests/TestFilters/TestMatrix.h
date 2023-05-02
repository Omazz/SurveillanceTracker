#ifndef TESTMATRIX_H
#define TESTMATRIX_H

#include <QObject>
#include <QTest>
#include "../../PriFiles/Filters/Matrix.h"

class TestMatrix : public QObject
{
    Q_OBJECT
public:
    explicit TestMatrix(QObject *parent = nullptr);

signals:

private slots:

    void sum_calculateSumMatrices();

    void difference_calculateDifferenceMatrices();

    void multiply_calculateMultiplyMatrices();

    void transpose_calculateTransposeMatrix();

    void calculateDeterminant_calculateDeterminantMatrix();

    void calculateInverseMatrix_calculateInverseMatrix();

    void calculateAlgebraicAdjunct_calculateAlgebraicAdjunctMatrix();
};

#endif // TESTMATRIX_H
