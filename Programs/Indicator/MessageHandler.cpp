#include "MessageHandler.h"

MessageHandler::MessageHandler(QObject* parent)
    : QObject(parent), m_socket(new QUdpSocket())
{
    m_socket->bind(QHostAddress::LocalHost, 12223);
    connect(m_socket.data(), &QUdpSocket::readyRead, this, &MessageHandler::readDatagram);
}

void MessageHandler::readDatagram() {
    QHostAddress sender;
    quint16 senderPort;
    while (m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        const uint8_t* packet = reinterpret_cast<const uint8_t*>(datagram.data());

        if(packet[0] == CAT34) {
            Asterix34 record34 = AsterixReader::parseAsterix34(packet);
            qreal angle_deg = record34.SectorNumber * 360.0 / qPow(2, 8);
            emit changeSector(angle_deg);
        } else if(packet[0] == CAT48) {
            Asterix48 record48 = AsterixReader::parseAsterix48(packet);

            if(record48.TrackNumber == 0) {
                qreal rho_km = ((record48.MeasuredPositionInSlantPolarCoordinates >> 16) & 0x0000FFFF) / 256.0 * 1.852;
                qreal theta_rad = qDegreesToRadians(
                            static_cast<qreal>(360.0 * (record48.MeasuredPositionInSlantPolarCoordinates & 0x0000FFFF) / (0x1 << 16)));

                emit newPlot(rho_km, theta_rad);
             } else {
                qint16 x = (record48.CalculatedPositionInCartesianCoordiantes >> 16) & 0x0000FFFF;
                qint16 y = (record48.CalculatedPositionInCartesianCoordiantes & 0x0000FFFF);
                qreal x_km = x / 128.0 * 1.852;
                qreal y_km = y / 128.0 * 1.852;

                qreal directionAngle_rad = qDegreesToRadians(static_cast<qreal>
                                (360.0 * (record48.CalculatedTrackVelocityInPolarRepresentation & 0x0000FFFF) / (0x1 << 16)));

                if(record48.header48.MeasuredPosition_isExist == true) {
                    emit newTrack(x_km, y_km, directionAngle_rad);
                } else {
                    emit newExtrapolatedTrack(x_km, y_km, directionAngle_rad);
                }

             }
        }
    }
}

