#ifndef TESTCIPHER_H
#define TESTCIPHER_H

#include <QObject>
#include <QTest>
#include "../../DataEncryption/Сipher/Kuznyechik.hpp"
#include "../../DataEncryption/Сipher/mycrypto.hpp"

class TestCipher : public QObject
{
    Q_OBJECT
public:
    explicit TestCipher(QObject *parent = nullptr);

signals:

private slots:

    void encrypt_encryptingSimpleMessage();

    void decrypt_decryptingSimpleMessage();

    void encrypt_streamingEncryption();
};

#endif // TESTCIPHER_H
