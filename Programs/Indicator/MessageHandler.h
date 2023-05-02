#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QMessageBox>
#include "../../PriFiles/EncryptionTools/mycrypto.hpp"
#include "../../PriFiles/EncryptionTools/Kuznyechik.hpp"
#include "../../PriFiles/EncryptionTools/gost341112.h"
#include "../../PriFiles/Settings/SettingsTracker.h"
#include "../../PriFiles/AsterixHandler/asterixreader.h"

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
    ByteBlock mKey;
    ByteBlock mIV;
    Streebog mStreebog;
    QUdpSocket* _socket;
};

#endif // MESSAGEHANDLER_H
