#include "MessageHandler.h"

MessageHandler::MessageHandler(QObject* parent) : QObject(parent)
{
    m_socket = new QUdpSocket(this);
    m_socket->bind(QHostAddress::LocalHost, 12222);
    connect(m_socket, &QUdpSocket::readyRead, this, &MessageHandler::readDatagram);
}

void MessageHandler::sendDatagram(QByteArray dataToSend) {
    if(dataToSend.isEmpty()) {
        return;
    }

    m_socket->writeDatagram(dataToSend.data(), dataToSend.size(), QHostAddress::LocalHost, 12223);
}

void MessageHandler::readDatagram() {
    QHostAddress sender;
    quint16 senderPort;
    while (m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        const char* packet = (const char*) datagram.data();

        if(packet[0] == CAT34) {
            Asterix34 record34 = AsterixReader::parseAsterix34((const uint8_t*)(packet));

            sendDatagram(datagram);

//            qDebug() << Qt::hex
//                      << "\nRecord Asterix34"
//                      << "\nFSPEC:" << record34.FSPEC
//                      << "\nDataSourceIdentifier:" << record34.DataSourceIdentifier
//                      << "\nTimeOfDay:" << record34.TimeOfDay
//                      << "\nMessageType:" << record34.MessageType << "\n";

        } else if(packet[0] == CAT48) {
            Asterix48 record48 = AsterixReader::parseAsterix48((const uint8_t*)(packet));


            if(record48.TargetReportDescriptor.size() == 0 ||
               (record48.TargetReportDescriptor.front() == 0xA0)) {
               return;
            }

            emit newPlot(convertFromDatagramToPlot(record48));

//            qDebug() << Qt::hex
//                     << "\nRecord Asterix48"
//                     << "\nFSPEC:" << record48.FSPEC
//                     << "\nDataSourceIdentifier:" << record48.DataSourceIdentifier
//                     << "\nTimeOfDay:" << record48.TimeOfDay
//                     << "\nMeasuredPositionInSlantPolarCoordinates:" << record48.MeasuredPositionInSlantPolarCoordinates << "\n";
        }
    }
}


Plot MessageHandler::convertFromDatagramToPlot(Asterix48 datagram) {
    quint16 temp = (datagram.MeasuredPositionInSlantPolarCoordinates >> 16) & 0xFFFF;
    qreal range = static_cast<qreal>((qreal)temp * 1852.0 / 256.0);
    qreal angle = static_cast<qreal>(360.0 * (datagram.MeasuredPositionInSlantPolarCoordinates & 0x0000FFFF) / (0x1 << 16));
    qreal frequencyDoppler = 0;
    if(datagram.RadialDopplerSpeed.RawDopplerSpeed.size() != 0) {
       frequencyDoppler = datagram.RadialDopplerSpeed.RawDopplerSpeed.front() & 0xFF;
    }
    qreal amplitude = datagram.RadarPlotCharacteristics.front().PAM;
    qreal time = QDateTime::currentMSecsSinceEpoch() / 1000.0;

    return Plot(range, angle, frequencyDoppler, amplitude, time, datagram);
}


