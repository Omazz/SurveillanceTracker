#ifndef TRACKHANDLER_H
#define TRACKHANDLER_H

#include <QtCore>
#include "AlphaBetaFilter.h"
#include "AlphaBetaLeastSquaresFilter.h"
#include "KalmanConstVelocityFilter.h"
#include "KalmanConstAccelerationFilter.h"
#include "AdaptiveKalmanConstVelocityFilter.h"
#include "AlphaBetaWLeastSquaresFilter.h"

const quint16 NUMBER_MODULATIONS = 1000;

struct AlphaBetaParams {
    bool isExist = false;
    quint16 k_max;
};

struct AlphaBetaWLeastSquaresParams {
    bool isExist = false;
    quint16 k_max;
    quint16 WLS_max;
};

struct KalmanConstVelocityParams {
    bool isExist = false;
    quint16 k_max;
    qreal rhoMSE;
    qreal thetaMSE;
    qreal velocityMSE;
};

struct KalmanConstAccelerationParams {
    bool isExist = false;
    quint16 k_max;
    qreal rhoMSE;
    qreal thetaMSE;
    qreal velocityMSE;
    qreal accelerationMSE;
};

struct AdaptiveKalmanConstVelocityParams {
    bool isExist = false;
    quint16 numberRecalcsP;
    quint16 numberRecalcsR;
    qreal rhoMSE;
    qreal thetaMSE;
    qreal velocityMSE;
};

struct FiltrationParams {
    AlphaBetaParams alphaBeta;
    AlphaBetaWLeastSquaresParams alphaBetaWLS;
    KalmanConstVelocityParams kalmanCV;
    KalmanConstAccelerationParams kalmanCA;
    AdaptiveKalmanConstVelocityParams adaptiveKalmanCV;
};

class TrackHandler : public QObject
{
public:
    TrackHandler();

    void calculateLineTrack(const QPointF& start, const QPointF& end,
                            const qreal& velocity, const qreal& updateTime);

    void calculateTurnTrack(const qreal& radius,
                            const qreal& velocity, const qreal& updateTime);

    void calculateCircleTrack(const qreal& radius,
                              const qreal& velocity, const qreal& updateTime);

    QVector<qreal> calculateStandardDeviation(const QVector<QVector<QPointF>>& array);

    QVector<QPointF> addNoiseToMeasurements(const QVector<QPointF>& measurements,
                                            const qreal& deviationRho, const qreal& deviationAngle);

    void calculateTracks(const FiltrationParams& params, const qreal& updateTime,
                         const qreal& deviationRho, const qreal& deviationAngle);

    QVector<QPointF> filterMeasurements(AbstractFilter* const filter, QVector<Target> measurements);

    QPointF polarToCart(qreal rho, qreal theta);

    void clearTracks();

    const QVector<QPointF> &originalTrack() const;

    const QVector<QVector<QPointF> > &noiseTrack() const;

    const QVector<QVector<QPointF> > &alphaBetaTrack() const;

    const QVector<QVector<QPointF> > &alphaBetaWlsmTrack() const;

    const QVector<QVector<QPointF> > &kalmanConstVelocityTrack() const;

    const QVector<QVector<QPointF> > &kalmanConstAccelerationTrack() const;

    const QVector<QVector<QPointF> > &adaptiveKalmanConstVelocityTrack() const;

private:
    QVector<QPointF> m_originalTrack;
    QVector<QVector<QPointF>> m_noiseTrack;
    QVector<QVector<QPointF>> m_alphaBetaTrack;
    QVector<QVector<QPointF>> m_alphaBetaWlsmTrack;
    QVector<QVector<QPointF>> m_kalmanConstVelocityTrack;
    QVector<QVector<QPointF>> m_kalmanConstAccelerationTrack;
    QVector<QVector<QPointF>> m_adaptiveKalmanConstVelocityTrack;

};

#endif // TRACKHANDLER_H
