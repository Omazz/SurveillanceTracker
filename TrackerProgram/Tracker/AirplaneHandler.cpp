#include "AirplaneHandler.h"

AirplaneHandler::AirplaneHandler()
{
    _socket = new QUdpSocket(this);
    _socket->bind(QHostAddress::LocalHost, 12222);

    _key = hex_to_bytes(SettingsTracker::KEY.toStdString());
    _iv = hex_to_bytes(SettingsTracker::INITIALIZING_VECTOR.toStdString());
    //std::cout << hex_representation(_key) << "\n" << hex_representation(_iv) << std::endl;
    connect(_socket, &QUdpSocket::readyRead, this, &AirplaneHandler::readDatagram);
}

void AirplaneHandler::readDatagram() {
    QHostAddress sender;
    quint16 senderPort;
    while (_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_socket->pendingDatagramSize());
        _socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        CFB_Mode<Kuznyechik> decryptor(Kuznyechik(_key), _iv);
        _iv = ByteBlock((BYTE*)(datagram.data() + datagram.size() - 16), 16);
        ByteBlock encryptionMessage((BYTE*)datagram.constData(), datagram.size());
        //qDebug() << "\nEncryption message:" << QString::fromStdString(hex_representation(encryptionMessage));
        ByteBlock decryptionMessage;
        decryptor.decrypt(encryptionMessage, decryptionMessage);
        //qDebug() << "Decryption message:" << QString::fromStdString(hex_representation(decryptionMessage));
    }
}
