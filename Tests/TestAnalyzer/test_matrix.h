#ifndef TEST_MATRIX_H
#define TEST_MATRIX_H

#include <QObject>
#include <QTest>
#include "../../PriFiles/FilterCheckingFiles/Filters/Matrix.h"

class Test_Matrix : public QObject
{
    Q_OBJECT
public:
    explicit Test_Matrix(QObject *parent = nullptr);

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

#endif // TEST_MATRIX_H
