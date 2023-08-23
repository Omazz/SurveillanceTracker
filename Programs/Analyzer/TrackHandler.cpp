#include "TrackHandler.h"

TrackHandler::TrackHandler()
{

}

void TrackHandler::calculateLineTrack(const QPointF& start, const QPointF& end,
                                      const qreal& velocity, const qreal& updateTime) {

    qreal numberSteps = qSqrt(qPow(start.x() - end.x(), 2) + qPow(start.y() - end.y(), 2)) / (velocity * updateTime);
    QPointF step = (end - start) / numberSteps;

    /* Создание исходной траектории */
    m_originalTrack.append(start);

    for(int currentStep = 0; currentStep < numberSteps; currentStep++) {
        m_originalTrack.append(start + (step * currentStep));
    }
}

void TrackHandler::calculateTurnTrack(const qreal& radius,
                                      const qreal& velocity, const qreal& updateTime) {

    qreal step = velocity * updateTime;
    qreal stepAngle = step / radius;
    qreal currAngle = 0;

    do {
        currAngle += stepAngle;
        m_originalTrack.append(QPointF((radius * qCos(currAngle)) - radius, (radius * qSin(currAngle)) - radius));
    } while(currAngle <  M_PI_2);
}

void TrackHandler::calculateCircleTrack(const qreal& radius,
                                        const qreal& velocity, const qreal& updateTime) {

    qreal step = velocity * updateTime;
    qreal stepAngle = step / radius;
    qreal currAngle = 0;

    m_originalTrack.append(QPointF(radius, 0));
    do {
        currAngle += stepAngle;
        m_originalTrack.append(QPointF(radius * qCos(currAngle), radius * qSin(currAngle)));
    } while(currAngle < 1.75 * M_PI);
}

QVector<qreal> TrackHandler::calculateStandardDeviation(const QVector<QVector<QPointF>>& track) {
    QVector<qreal> standartDeviation;

    for(int i = 0; i < m_originalTrack.size(); ++i) {
        QPointF currDeviation(0, 0);

        for(int indexModulation = 0; indexModulation < NUMBER_MODULATIONS; ++indexModulation) {
            QPointF diffNoise = (track[indexModulation][i] - m_originalTrack[i]);
            diffNoise.setX(diffNoise.x() * diffNoise.x());
            diffNoise.setY(diffNoise.y() * diffNoise.y());
            currDeviation += diffNoise;
        }

        standartDeviation.append(qSqrt((currDeviation.x() / NUMBER_MODULATIONS) +
                                       (currDeviation.y() / NUMBER_MODULATIONS)));
    }

    return standartDeviation;
}

QVector<QPointF> TrackHandler::addNoiseToMeasurements(const QVector<QPointF>& measurements,
                                                      const qreal& deviationRho, const qreal& deviationAngle) {
    std::random_device randomDevice{};
    std::mt19937 generator{randomDevice()};
    std::normal_distribution<> normalDistributionRho{0, deviationRho};
    std::normal_distribution<> normalDistributionTheta{0, deviationAngle / 60.0};

    QVector<QPointF> measurementsWithNoise;
    for(int i = 0; i < measurements.size(); ++i) {
        qreal rho = sqrt(qPow(measurements[i].x(), 2) + qPow(measurements[i].y(), 2));
        qreal theta = qAtan2(measurements[i].y(), measurements[i].x());

        if(qIsNull(measurements[i].y())) {
            theta = 0;
        }

        theta = M_PI_2 - theta;
        if(theta < 0) {
            theta += (2.0 * M_PI);
        }

        qreal rhoWithNoise = rho + normalDistributionRho(generator);
        qreal thetaWithNoise = theta + qDegreesToRadians(normalDistributionTheta(generator));

        measurementsWithNoise.append(QPointF(rhoWithNoise * qSin(thetaWithNoise),
                                             rhoWithNoise * qCos(thetaWithNoise)));
    }

    return measurementsWithNoise;
}

void TrackHandler::calculateTracks(const FiltrationParams& params, const qreal& updateTime,
                                   const qreal& deviationRho, const qreal& deviationAngle) {

    for(int i = 0; i < NUMBER_MODULATIONS; ++i) {
        /* Создание зашумленной траектории */
        m_noiseTrack.append(addNoiseToMeasurements(m_originalTrack, deviationRho, deviationAngle));

        QVector<Target> targets;
        for(int j = 0; j < m_originalTrack.size(); ++j) {
            targets.append(Target(m_noiseTrack.last()[j], j * updateTime));
        }

        /* Альфа-бета фильтр */
        if(params.alphaBeta.isExist) {
            AlphaBetaFilter filter(params.alphaBeta.k_max);
            m_alphaBetaTrack.append(filterMeasurements(&filter, targets));
        }

        /* Альфа-бета фильтр с взвешенным методом наименьших квадратов */
        if(params.alphaBetaWLS.isExist) {
            AlphaBetaWLeastSquaresFilter filter;
            m_alphaBetaWlsmTrack.append(filterMeasurements(&filter, targets));
        }

        /* Линейный фильтр Калмана */
        if(params.kalmanCV.isExist) {
            KalmanConstVelocityFilter filter(
                                                params.kalmanCV.k_max,
                                                params.kalmanCV.rhoMSE,
                                                qDegreesToRadians(params.kalmanCV.thetaMSE / 60.0),
                                                params.kalmanCV.velocityMSE
                                            );
            m_kalmanConstVelocityTrack.append(filterMeasurements(&filter, targets));
        }

        /* Квадартичный фильтр Калмана */
        if(params.kalmanCA.isExist) {
            KalmanConstAccelerationFilter filter(
                                                    params.kalmanCA.k_max,
                                                    params.kalmanCA.rhoMSE,
                                                    qDegreesToRadians(params.kalmanCA.thetaMSE / 60.0),
                                                    params.kalmanCA.velocityMSE,
                                                    params.kalmanCA.accelerationMSE
                                                 );
            m_kalmanConstAccelerationTrack.append(filterMeasurements(&filter, targets));
        }

        /* Адаптивный фильтр Калмана */
        if(params.adaptiveKalmanCV.isExist) {
            AdaptiveKalmanConstVelocityFilter filter(
                                                        params.adaptiveKalmanCV.numberRecalcsP,
                                                        params.adaptiveKalmanCV.numberRecalcsR,
                                                        params.adaptiveKalmanCV.rhoMSE,
                                                        qDegreesToRadians(params.adaptiveKalmanCV.thetaMSE / 60.0),
                                                        params.adaptiveKalmanCV.velocityMSE
                                                     );
            m_adaptiveKalmanConstVelocityTrack.append(filterMeasurements(&filter, targets));
        }
    }
}



QVector<QPointF> TrackHandler::filterMeasurements(AbstractFilter* const filter, QVector<Target> measurements) {
    QVector<QPointF> filteredMeasurements = {
                                                measurements[0].coordinate,
                                                measurements[1].coordinate,
                                                measurements[2].coordinate
                                            };
    filter->initialization({
                               measurements[0],
                               measurements[1],
                               measurements[2]
                           });
    for(int i = 3; i < measurements.size(); ++i) {
        filteredMeasurements.append(filter->filterMeasuredValue(measurements[i]).coordinate);
    }

    return filteredMeasurements;
}

void TrackHandler::clearTracks() {
    m_originalTrack.clear();
    m_noiseTrack.clear();
    m_alphaBetaTrack.clear();
    m_alphaBetaWlsmTrack.clear();
    m_kalmanConstVelocityTrack.clear();
    m_kalmanConstAccelerationTrack.clear();
    m_adaptiveKalmanConstVelocityTrack.clear();
}

const QVector<QPointF> &TrackHandler::originalTrack() const
{
    return m_originalTrack;
}

const QVector<QVector<QPointF> > &TrackHandler::noiseTrack() const
{
    return m_noiseTrack;
}

const QVector<QVector<QPointF> > &TrackHandler::alphaBetaTrack() const
{
    return m_alphaBetaTrack;
}

const QVector<QVector<QPointF> > &TrackHandler::alphaBetaWlsmTrack() const
{
    return m_alphaBetaWlsmTrack;
}

const QVector<QVector<QPointF> > &TrackHandler::kalmanConstVelocityTrack() const
{
    return m_kalmanConstVelocityTrack;
}

const QVector<QVector<QPointF> > &TrackHandler::kalmanConstAccelerationTrack() const
{
    return m_kalmanConstAccelerationTrack;
}

const QVector<QVector<QPointF> > &TrackHandler::adaptiveKalmanConstVelocityTrack() const
{
    return m_adaptiveKalmanConstVelocityTrack;
}

QPointF TrackHandler::polarToCart(qreal rho, qreal theta) {
    return  {
                rho * qCos(theta - M_PI_2),
                rho * qSin(theta - M_PI_2)
            };
}
