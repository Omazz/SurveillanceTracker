#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QMessageBox>
#include "../../PriFiles/TrackerFiles/asterixreader.h"

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    MessageHandler(QObject* parent = nullptr);

public slots:
    void readDatagram();

    void readDatagramMAC();
signals:
    void changeSector(qreal angle_deg);

    void newPlot(qreal rho_km, qreal angle_rad);

    void newTrack(qreal x_km, qreal y_km);

private:
    QUdpSocket* _socket;
    QUdpSocket* _socketCheckMAC;
};

#endif // MESSAGEHANDLER_H
