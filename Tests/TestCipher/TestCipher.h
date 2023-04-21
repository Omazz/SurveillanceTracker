#ifndef TESTCIPHER_H
#define TESTCIPHER_H

#include <QObject>
#include <QTest>
#include "../../PriFiles/Сipher/Kuznyechik.hpp"
#include "../../PriFiles/Сipher/mycrypto.hpp"
#include "../../PriFiles/Сipher/gost341112.h"

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

    void hash_exampleFromStandart256();

    void hash_exampleFromStandart512();

    void hash_exampleWorkCipherWithMAC_attackerHasBeenDetected();
};

#endif // TESTCIPHER_H
