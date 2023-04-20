#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include "../../DataEncryption/Сipher/mycrypto.hpp"
#include "../../DataEncryption/Сipher/Kuznyechik.hpp"
#include "../../DataEncryption/Сipher/gost341112.h"
#include "../../DataSettings/SettingsTracker.h"
#include "../TrackerFiles/asterixreader.h"


static const quint8 PRIMARY_SURVEILLIANCE = 0x20;
static const quint8 SECONDARY_SURVEILLIANCE = 0x40;

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    MessageHandler(QObject* parent = nullptr);

public slots:
    void readDatagram();

signals:


private:
    ByteBlock _key;
    ByteBlock _iv;
    Streebog streebog;
    QUdpSocket* _socket;
};

#endif // MESSAGEHANDLER_H
