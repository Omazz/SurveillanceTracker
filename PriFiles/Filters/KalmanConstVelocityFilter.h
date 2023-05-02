#ifndef KALMANCONSTVELOCITYFILTER_H
#define KALMANCONSTVELOCITYFILTER_H

#include "AbstractFilter.h"
#include "Matrix.h"

class KalmanConstVelocityFilter : public AbstractFilter {
public:
    KalmanConstVelocityFilter(qreal coordinateMSE, qreal velocityMSE);

    void initialization(QVector<Target> array) override;

    Target filterMeasuredValue(Target measurement) override;

    Target updateExtrapolation(qreal time) override;

    Target extrapolateOnTime(qreal time) const override;

    QPointF getVelocity() const override;

private:
    Target m_filteredTarget;
    QPointF m_velocity;

    Matrix m_I; ///< I -- единичная матрица 4х4
    Matrix m_H; ///< H -- взвешенная матрица
    Matrix m_R; ///< R -- взвешенная матрица шума
    Matrix m_A; ///< A -- матрица перехода
    Matrix m_G; ///< G -- вспомогательная матрица для вычисления Q
    Matrix m_Q; ///< Q -- ковариационная матрица шума
    Matrix m_x; ///< x -- вектор состояния
    Matrix m_P; ///< P -- ковариационная матрица

    qreal m_coordinateMSE;
    qreal m_velocityMSE;
};

#endif // KALMANCONSTVELOCITYFILTER_H