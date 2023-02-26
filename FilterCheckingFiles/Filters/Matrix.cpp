#include "Matrix.h"

Matrix::Matrix() :
    m_rows(DEFAULT_NUMBER_ROWS),
    m_columns(DEFAULT_NUMBER_COLUMNS)
{
    m_matrix.resize(m_rows);
    for(quint8 i = 0; i < m_rows; ++i) {
        m_matrix[i] = QVector<qreal>(m_columns);
    }
}

Matrix::Matrix(const quint8& rows, const quint8& columns) {
    m_rows = rows;
    m_columns = columns;
    m_matrix.resize(m_rows);
    for(quint8 i = 0; i < m_rows; ++i) {
        m_matrix[i] = QVector<qreal>(m_columns);
    }
}

Matrix::Matrix(const quint8& rows, const quint8& columns, const qreal& value) {
    m_rows = rows;
    m_columns = columns;
    m_matrix.resize(m_rows);
    for(quint8 i = 0; i < m_rows; ++i) {
        m_matrix[i] = QVector<qreal>(m_columns);
        m_matrix[i][i] = value;
    }
}

quint8 Matrix::rows() const
{
    return m_rows;
}

quint8 Matrix::columns() const
{
    return m_columns;
}

Matrix Matrix::sum(const Matrix& otherMatrix) const{
    if(m_columns != otherMatrix.columns() || m_rows != otherMatrix.rows()) {
        throw QException();
    }

    Matrix result(m_rows, m_columns);

    for(quint8 i = 0; i < m_rows; ++i) {
        for(quint8 j = 0; j < m_columns; ++j) {
            result.set(i, j, m_matrix[i][j] + otherMatrix.get(i, j));
        }
    }

    return result;
}

Matrix Matrix::difference(const Matrix& otherMatrix) const{
    if(m_columns != otherMatrix.columns() || m_rows != otherMatrix.rows()) {
        throw QException();
    }

    Matrix result(m_rows, m_columns);

    for(quint8 i = 0; i < m_rows; ++i) {
        for(quint8 j = 0; j < m_columns; ++j) {
            result.set(i, j, m_matrix[i][j] - otherMatrix.get(i, j));
        }
    }

    return result;
}


Matrix Matrix::multiply(const Matrix& otherMatrix) const{
    if(m_columns != otherMatrix.rows()) {
        throw QException();
    }
    Matrix result(m_rows, otherMatrix.columns());

    for(quint8 i = 0; i < m_rows; ++i) {
        for(quint8 j = 0; j < otherMatrix.columns(); ++j) {
            for(quint8 k = 0; k < m_columns; ++k) {
                result.set(i, j, result.get(i, j) + (get(i, k) * otherMatrix.get(k, j)));
            }
        }
    }

    return result;
}

Matrix Matrix::multiply(const qreal& coefficient) const {
    Matrix result(this->rows(), this->columns());

    for(quint8 i = 0; i < m_rows; ++i) {
        for(quint8 j = 0; j < m_columns; ++j) {
            result.set(i, j, m_matrix[i][j] * coefficient);
        }
    }

    return result;
}

qreal Matrix::get(const quint8& row, const quint8& column) const {
    if(row >= m_rows || column >= m_columns) {
        throw QException();
    }
    return m_matrix[row][column];
}

void Matrix::set(const quint8& row, const quint8& column, const qreal& value) {
    if(row >= m_rows || column >= m_columns) {
        throw QException();
    }
    m_matrix[row][column] = value;
}

Matrix Matrix::transopse() const{
    Matrix result(m_columns, m_rows);

    for(quint8 i = 0; i < m_rows; ++i) {
        for(quint8 j = 0; j < m_columns; ++j) {
            result.set(j, i, m_matrix[i][j]);
        }
    }

    return result;
}

qreal Matrix::calculateDeterminant() const {
    if(m_rows != m_columns) {
        throw QException();
    }
    qreal determinant = 1;

    std::pair<Matrix, Matrix> LU = calculateLU();
    Matrix L = LU.first;
    Matrix U = LU.second;

    for(quint8 i = 0; i < m_rows; ++i) {
        determinant *= L.get(i, i) * U.get(i, i);
    }

    return determinant;
}

std::pair<Matrix, Matrix> Matrix::calculateLU() const {
    Matrix L(m_rows, m_columns, 1);
    Matrix U(m_rows, m_columns);

    for(quint8 i = 0; i < m_rows; ++i) {
        for(quint8 j = 0; j < m_columns; ++j) {
            if(i <= j) {
                qreal value = 0;
                for(quint8 k = 0; k < i; ++k) {
                    value += (L.get(i, k) * U.get(k, j));
                }
                U.set(i, j, (m_matrix[i][j] - value));
            } else {
                qreal value = 0;
                for(quint8 k = 0; k < j; ++k) {
                    value += (L.get(i, k) * U.get(k, j));
                }
                L.set(i, j, ((m_matrix[i][j] - value) / U.get(j, j)));
            }
        }
    }

    return std::pair<Matrix, Matrix>(L, U);
}

Matrix Matrix::calculateInverseMatrix() const{
    Matrix result(m_rows, m_columns);
    qreal determinant = calculateDeterminant();
    for(quint8 i = 0; i < m_rows; ++i) {
        for(quint8 j = 0; j < m_columns; ++j) {
            result.set(i, j, calculateAlgebraicAdjunct(i, j) / determinant);
        }
    }

    return result;
}


qreal Matrix::calculateAlgebraicAdjunct(const quint8& row, const quint8& column) const {
    QVector<QVector<qreal>> matrix = m_matrix;
    for(quint8 i = 0; i < m_rows; ++i) {
        matrix[i].remove(column);
    }
    matrix.remove(row);
    Matrix res(matrix.size(), matrix[0].size());
    res.m_matrix = matrix;
    return res.calculateDeterminant() * pow(-1, row + column);
}

void Matrix::clearMatrix() {
    for(quint8 i = 0; i < m_rows; ++i) {
        for(quint8 j = 0; j < m_columns; ++j) {
            m_matrix[i][j] = 0;
        }
    }
}
