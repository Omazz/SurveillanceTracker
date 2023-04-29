#ifndef AIRPLANE_H
#define AIRPLANE_H

#include <QObject>
#include <QtCore>
#include "Plot.h"
#include "../../PriFiles/DataSettings/SettingsTracker.h"

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


    qreal mOldAngle;
    qreal mOldVelocity;

    Plot mMeasuredPlot;
    Plot mFilteredPlot;

    quint16 mTrackNumber;

    QPointF mVelocity; /// < м/с
    qreal mDirectionAngle; ///< Радианы

    qreal mCounterExtrapolations = 0;

    qreal mAlpha;
    qreal mBeta;
    quint8 mNumberSteps;
    quint8 mNumberStepsMax;

    /* Для экстраполяции */
    QQueue<Plot> coordsMNK;
};

#endif // AIRPLANE_H
