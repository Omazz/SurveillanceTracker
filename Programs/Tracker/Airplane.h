#ifndef AIRPLANE_H
#define AIRPLANE_H

#include <QObject>
#include <QtCore>
#include "Plot.h"


class Airplane : public QObject
{
    Q_OBJECT
public:
    explicit Airplane(QObject *parent = nullptr);

signals:

private:
    qreal mOldAngle;
    qreal mOldVelocity;

    Plot mMeasuredPlot;
    Plot mFilteredPlot;

    quint16 mTrackNumber;

    QPointF mVelocity;

    qreal mCounterExtrapolations = 0;

    qreal mAlpha;
    qreal mBeta;
    quint8 mNumberSteps;
    quint8 mNumberStepsMax;

    /* Для экстраполяции */
    QQueue<Plot> coordsMNK;
};

#endif // AIRPLANE_H
