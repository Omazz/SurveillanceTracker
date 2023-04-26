#include "MessageHandler.h"

MessageHandler::MessageHandler(QObject* parent) : QObject(parent)
{
    mSocket = new QUdpSocket(this);
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
        ByteBlock blocksToSend((BYTE*)datagram.data(), datagram.size());
        ByteBlock encryptionBlocks;

        CFB_Mode<Kuznyechik> encryptor(Kuznyechik(mKey), mIV);
        encryptor.encrypt(blocksToSend, encryptionBlocks);

        mIV = ByteBlock((encryptionBlocks.byte_ptr() + encryptionBlocks.size() - 16), 16);

        QByteArray byteArray;
        BYTE* encryption_pointer = encryptionBlocks.byte_ptr();

        for(int i = 0; i < encryptionBlocks.size(); i++) {
            byteArray.push_back(encryption_pointer[i]);
        }

        /* Формируем имитовставку по зашифрованному сообщению и добавляем её в конец */
        unsigned char* mac = mStreebog.Hash(encryption_pointer, encryptionBlocks.size());
        for(int i = 0; i < 32; ++i) {
            byteArray.push_back(mac[i]);
        }

        const char* packet = (const char*) datagram.data();

        if(packet[0] == CAT34) {
            Asterix34 record34 = AsterixReader::parseAsterix34((const uint8_t*)(packet));

            mSocket->writeDatagram(byteArray.data(), byteArray.size(), QHostAddress::LocalHost, 12223);

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

            mSocket->writeDatagram(byteArray.data(), byteArray.size(), QHostAddress::LocalHost, 12223);

            qDebug() << Qt::hex
                     << "\nRecord Asterix48"
                     << "\nFSPEC:" << record48.FSPEC
                     << "\nDataSourceIdentifier:" << record48.DataSourceIdentifier
                     << "\nTimeOfDay:" << record48.TimeOfDay
                     << "\nMeasuredPositionInSlantPolarCoordinates:" << record48.MeasuredPositionInSlantPolarCoordinates << "\n";
        }
    }
}

