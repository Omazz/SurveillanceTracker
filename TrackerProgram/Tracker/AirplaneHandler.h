#ifndef AIRPLANEHANDLER_H
#define AIRPLANEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QtCore>
#include <QByteArray>
#include "../../DataEncryption/Сipher/mycrypto.hpp"
#include "../../DataEncryption/Сipher/Kuznyechik.hpp"
#include "../../DataSettings/SettingsTracker.h"

class AirplaneHandler : public QObject
{
    Q_OBJECT

public:
    AirplaneHandler();

public slots:
    void readDatagram();

private:
    ByteBlock _key;
    ByteBlock _iv;

    QUdpSocket* _socket;
};

#endif // AIRPLANEHANDLER_H
