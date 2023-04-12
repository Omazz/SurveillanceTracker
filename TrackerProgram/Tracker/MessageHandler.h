#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include "../../DataEncryption/Сipher/mycrypto.hpp"
#include "../../DataEncryption/Сipher/Kuznyechik.hpp"
#include "../../DataSettings/SettingsTracker.h"
#include "../TrackerProgram/TrackerFiles/asterixreader.h"

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

    QUdpSocket* _socket;
};

#endif // MESSAGEHANDLER_H
