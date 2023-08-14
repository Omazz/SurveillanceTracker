#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include "SettingsTracker.h"
#include "AsterixReader.h"

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    MessageHandler(QObject* parent = nullptr);

public slots:
    void readDatagram();

signals:
    void changeSector(qreal angle_deg);

    void newPlot(qreal rho_km, qreal angle_rad);

    void newTrack(qreal x_km, qreal y_km, qreal directionAngle_rad);

    void newExtrapolatedTrack(qreal x_km, qreal y_km, qreal directionAngle_rad);

private:
    QScopedPointer<QUdpSocket> m_socket;
};

#endif // MESSAGEHANDLER_H
