#ifndef TESTENCRYPTIONTOOLS_H
#define TESTENCRYPTIONTOOLS_H

#include <QObject>
#include <QTest>
#include "../../PriFiles/EncryptionTools/Kuznyechik.hpp"
#include "../../PriFiles/EncryptionTools/mycrypto.hpp"
#include "../../PriFiles/EncryptionTools/gost341112.h"

class TestEncryptionTools : public QObject
{
    Q_OBJECT
public:
    explicit TestEncryptionTools(QObject *parent = nullptr);

signals:

private slots:

    void encrypt_encryptingSimpleMessage();

    void decrypt_decryptingSimpleMessage();

    void encrypt_streamingEncryption();

    void hash_exampleFromStandart256();

    void hash_exampleFromStandart512();

    void hash_exampleWorkCipherWithMAC_attackerHasBeenDetected();
};

#endif // TESTENCRYPTIONTOOLS_H
