#include "MessageHandler.h"

MessageHandler::MessageHandler(QObject* parent) : QObject(parent)
{
    mSocket = new QUdpSocket(this);
    mSocketCheckMAC = new QUdpSocket(this);
    mSocket->bind(QHostAddress::LocalHost, 12222);
    mKey = hex_to_bytes(SettingsTracker::KEY.toStdString());
    mIV = hex_to_bytes(SettingsTracker::INITIALIZING_VECTOR.toStdString());
    mStreebog.SetMode(256);
    connect(mSocket, &QUdpSocket::readyRead, this, &MessageHandler::readDatagram);
}

void MessageHandler::readDatagram() {
    QHostAddress sender;
    quint16 senderPort;
    while (mSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(mSocket->pendingDatagramSize());
        mSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        CFB_Mode<Kuznyechik> decryptor(Kuznyechik(mKey), mIV);

        unsigned char* mac = mStreebog.Hash((unsigned char*)datagram.data(), datagram.size() - 32);
        unsigned char* mac_message = (unsigned char*)(datagram.data() + datagram.size() - 32);
        QString mac1, mac2;

        for(int i = 0; i < 32; i++) {
            mac1.append(mac[i]);
            mac2.append(mac_message[i]);
        }

        if(QString::compare(mac1, mac2) != 0) {
            QByteArray byteArray;
            byteArray.append(ERROR);
            mSocket->writeDatagram(byteArray, byteArray.size(), QHostAddress::LocalHost, 12224);
            return;
        }

        mIV = ByteBlock((BYTE*)(datagram.data() + datagram.size() - 16 - 32), 16);
        ByteBlock encryptionMessage((BYTE*)datagram.constData(), datagram.size() - 32);
        ByteBlock decryptionMessage;
        decryptor.decrypt(encryptionMessage, decryptionMessage);
        const char* packet = (const char*) decryptionMessage.byte_ptr();

        if(packet[0] == CAT34) {
            Asterix34 record34 = AsterixReader::parseAsterix34((const uint8_t*)(packet));

            mSocket->writeDatagram(packet, decryptionMessage.size(), QHostAddress::LocalHost, 12223);

            QByteArray byteArray;
            byteArray.append(OK);
            mSocket->writeDatagram(byteArray, byteArray.size(), QHostAddress::LocalHost, 12224);


            qDebug() << Qt::hex
                      << "\nRecord Asterix34"
                      << "\nFSPEC:" << record34.FSPEC
                      << "\nDataSourceIdentifier:" << record34.DataSourceIdentifier
                      << "\nTimeOfDay:" << record34.TimeOfDay
                      << "\nMessageType:" << record34.MessageType << "\n";

        } else if(packet[0] == CAT48) {
            Asterix48 record48 = AsterixReader::parseAsterix48((const uint8_t*)(packet));


            if(record48.TargetReportDescriptor.size() == 0 ||
               record48.TargetReportDescriptor.front() != PRIMARY_SURVEILLIANCE ) {
               return;
            }

            mSocket->writeDatagram(packet, decryptionMessage.size(), QHostAddress::LocalHost, 12223);

            QByteArray byteArray;
            byteArray.append(OK);
            mSocket->writeDatagram(byteArray, byteArray.size(), QHostAddress::LocalHost, 12224);

            qDebug() << Qt::hex
                     << "\nRecord Asterix48"
                     << "\nFSPEC:" << record48.FSPEC
                     << "\nDataSourceIdentifier:" << record48.DataSourceIdentifier
                     << "\nTimeOfDay:" << record48.TimeOfDay
                     << "\nMeasuredPositionInSlantPolarCoordinates:" << record48.MeasuredPositionInSlantPolarCoordinates << "\n";
        }
    }
}

