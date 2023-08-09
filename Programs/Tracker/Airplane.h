#ifndef AIRPLANE_H
#define AIRPLANE_H

#include <QObject>
#include <QtCore>
#include "Plot.h"
#include "../../PriFiles/Settings/SettingsTracker.h"

class Airplane : public QObject
{
    Q_OBJECT
public:
    explicit Airplane(QObject *parent = nullptr);

    Airplane(QVector<Plot> initPlots, quint16 trackNumber, QObject* parent = nullptr);

    void setTrack(Plot measuredPlot);

    bool isManeuverAngle() const;

    bool isManeuverVelocity() const;

    void initializationMNK(QVector<Plot> initPlots);

    void replaceQueue(Plot plot);

    std::pair<Plot, QPointF> doExtrapolation(qreal currentTimeSecs);

    void updateExtrapolationPlot();

    const Plot &measuredPlot() const;

    const Plot &filteredPlot() const;

    qreal timeToNextPlot() const;

    qreal velocity() const;

    qreal velocityX() const;

    qreal velocityY() const;

    qreal directionAngle() const;

    quint16 trackNumber() const;

    qreal counterExtrapolations() const;

    QByteArray getExtrapolationPlot() const;

    QByteArray getFilteredPlot() const;

signals:

private:
    void addTrackData(qreal x, qreal y, quint16 trackNumber, qreal velocity, qreal angleVelocity);

    void changeDistanceAngleTimeData(qreal distance, qreal angle, qreal time);

    void removePolarCoordinates();

    qreal m_oldAngle;
    qreal m_oldVelocity;

    Plot m_measuredPlot;
    Plot m_filteredPlot;

    quint16 m_trackNumber;

    QPointF m_velocity; /// < м/с
    qreal m_directionAngle; ///< Радианы

    qreal m_counterExtrapolations = 0;

    qreal m_alpha;
    qreal m_beta;
    quint8 m_numberSteps;
    quint8 m_numberStepsMax;

    /* Для экстраполяции */
    QQueue<Plot> m_coordsMNK;
};

#endif // AIRPLANE_H
