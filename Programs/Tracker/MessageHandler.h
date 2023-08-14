#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include "SettingsTracker.h"
#include "AsterixReader.h"
#include "Plot.h"


static const quint8 PRIMARY_SURVEILLIANCE = 0x20;
static const quint8 SECONDARY_SURVEILLIANCE = 0x40;
static const quint8 TEST_SURVEILLIANCE = 0xA0;

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    MessageHandler(QObject* parent = nullptr);

    void sendDatagram(QByteArray dataToSend);

    static Plot convertFromDatagramToPlot(Asterix48 datagram);

public slots:
    void readDatagram();

signals:
    void newPlot(Plot plot);

private:
    QUdpSocket* m_socket;
};

#endif // MESSAGEHANDLER_H
