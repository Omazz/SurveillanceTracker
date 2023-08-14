#ifndef ADAPTIVEKALMANCONSTVELOCITYFILTER_H
#define ADAPTIVEKALMANCONSTVELOCITYFILTER_H

#include "AbstractFilter.h"
#include "Matrix.h"

class AdaptiveKalmanConstVelocityFilter : public AbstractFilter {
public:
    AdaptiveKalmanConstVelocityFilter(quint16 numberRecalcsP, quint16 numberRecalcsR,
                                      qreal rhoMSE, qreal thetaMSE, qreal velocityMSE);

    void initialization(QVector<Target> array) override;

    Target filterMeasuredValue(Target measurement) override;

    Target updateExtrapolation(qreal time) override;

    Target extrapolateOnTime(qreal time) const override;

    QPointF getVelocity() const override;

private:
    void recalculateMatrixR();

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

    qreal m_rhoMSE;
    qreal m_thetaMSE;
    qreal m_velocityMSE;

    quint16 m_counterSteps;
    quint16 m_numberRecalcsP;
    quint16 m_numberRecalcsR;
};
#endif // ADAPTIVEKALMANCONSTVELOCITYFILTER_H
