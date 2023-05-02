#ifndef MATRIX_H
#define MATRIX_H

#include <QtCore>

class Matrix
{
public:
    const static quint8 DEFAULT_NUMBER_ROWS = 5;
    const static quint8 DEFAULT_NUMBER_COLUMNS = 5;

    Matrix();

    Matrix(const quint8& rows, const quint8& columns);

    Matrix(const quint8& rows, const quint8& columns, const qreal& value);

    Matrix sum(const Matrix& otherMatrix) const;

    Matrix difference(const Matrix& otherMatrix) const;

    Matrix multiply(const Matrix& otherMatrix) const;

    Matrix multiply(const qreal& coefficient) const;

    qreal get(const quint8& row, const quint8& column) const;

    void set(const quint8& row,const quint8& column, const qreal& value);

    quint8 rows() const;

    quint8 columns() const;

    Matrix transpose() const;

    qreal calculateDeterminant() const;

    Matrix calculateInverseMatrix() const;

    qreal calculateAlgebraicAdjunct(const quint8& row, const quint8& column) const;

    void clearMatrix();

private:
    std::pair<Matrix, Matrix> calculateLU() const;


    quint8 m_rows;
    quint8 m_columns;
    QVector<QVector<qreal>> m_matrix;
};

#endif // MATRIX_H
