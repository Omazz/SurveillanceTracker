#ifndef AIRPLANEHANDLER_H
#define AIRPLANEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QtCore>
#include <QByteArray>

#include "MessageHandler.h"

class AirplaneHandler : public QObject
{
    Q_OBJECT

public:
    AirplaneHandler();

private:
    MessageHandler* _messageHandler;
};

#endif // AIRPLANEHANDLER_H
