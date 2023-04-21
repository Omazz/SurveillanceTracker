#include "MessageHandler.h"

MessageHandler::MessageHandler(QObject* parent) : QObject(parent)
{
    _socket = new QUdpSocket(this);
    _socket->bind(QHostAddress::LocalHost, 12223);
    connect(_socket, &QUdpSocket::readyRead, this, &MessageHandler::readDatagram);
}

void MessageHandler::readDatagram() {
    QHostAddress sender;
    quint16 senderPort;
    while (_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_socket->pendingDatagramSize());
        _socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
       char* packet = datagram.data();

        if(packet[0] == CAT34) {
            Asterix34 record34 = AsterixReader::parseAsterix34((const uint8_t*)packet);
            qreal angle_deg = record34.SectorNumber * 360.0 / qPow(2, 8);
            emit changeSector(angle_deg);
        } else if(packet[0] == CAT48) {
            Asterix48 record48 = AsterixReader::parseAsterix48((const uint8_t*)packet);

            if(record48.TrackNumber == 0) {
                qreal rho_km = ((record48.MeasuredPositionInSlantPolarCoordinates & 0xFFFF0000) >> 16) / 256.0 * 1.852;
                qreal theta_rad = qDegreesToRadians(
                            static_cast<qreal>(360 * (record48.MeasuredPositionInSlantPolarCoordinates & 0x0000FFFF) >> 16));
                emit newPlot(rho_km, theta_rad);
            } else {
                qreal x_km = ((record48.MeasuredPositionInSlantPolarCoordinates & 0xFFFF0000) >> 16) / 128.0 * 1.852;
                qreal y_km = (record48.MeasuredPositionInSlantPolarCoordinates & 0x0000FFFF) / 128.0 * 1.852;
                emit newTrack(x_km, y_km);
            }

        } else {
            qDebug() << "Error!";
        }

    }
}

