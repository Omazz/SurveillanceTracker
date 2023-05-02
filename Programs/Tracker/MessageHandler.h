#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include "../../PriFiles/EncryptionTools/mycrypto.hpp"
#include "../../PriFiles/EncryptionTools/Kuznyechik.hpp"
#include "../../PriFiles/EncryptionTools/gost341112.h"
#include "../../PriFiles/Settings/SettingsTracker.h"
#include "../../PriFiles/AsterixHandler/asterixreader.h"
#include "Plot.h"


static const quint8 PRIMARY_SURVEILLIANCE = 0x20;
static const quint8 SECONDARY_SURVEILLIANCE = 0x40;

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
    ByteBlock mKey;
    ByteBlock mIV;
    Streebog mStreebog;
    QUdpSocket* mSocket;
};

#endif // MESSAGEHANDLER_H
