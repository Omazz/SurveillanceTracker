#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include "../TrackerProgram/TrackerFiles/asterixreader.h"

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    MessageHandler(QObject* parent = nullptr);

public slots:
    void readDatagram();

signals:
    void changeSector(qreal angle_deg);

    void addPlot(qreal rho_m, qreal angle_deg);

    void addTrack(qreal x_m, qreal y_m, quint16 trackNumber);

private:
    QUdpSocket* _socket;
};

#endif // MESSAGEHANDLER_H
