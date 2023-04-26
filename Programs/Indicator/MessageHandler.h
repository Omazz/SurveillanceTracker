#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QMessageBox>
#include "../../PriFiles/Сipher/mycrypto.hpp"
#include "../../PriFiles/Сipher/Kuznyechik.hpp"
#include "../../PriFiles/Сipher/gost341112.h"
#include "../../PriFiles/DataSettings/SettingsTracker.h"
#include "../../PriFiles/TrackerFiles/asterixreader.h"

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

    void newTrack(qreal x_km, qreal y_km);

private:
    ByteBlock mKey;
    ByteBlock mIV;
    Streebog mStreebog;
    QUdpSocket* _socket;
};

#endif // MESSAGEHANDLER_H
