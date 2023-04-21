#include "TestCipher.h"

TestCipher::TestCipher(QObject *parent)
    : QObject{parent}
{

}

void TestCipher::encrypt_encryptingSimpleMessage() {
    ByteBlock key = hex_to_bytes("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ByteBlock iv = hex_to_bytes("abcdef12345600dacdef94756eeabefa");

    ByteBlock message = hex_to_bytes("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ByteBlock encryptionMessage;

    CFB_Mode<Kuznyechik> encryptor(Kuznyechik(key), iv);
    encryptor.encrypt(message, encryptionMessage);

    QCOMPARE((hex_representation(encryptionMessage) != hex_representation(message)), true);
}

void TestCipher::decrypt_decryptingSimpleMessage() {
    ByteBlock key = hex_to_bytes("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ByteBlock iv = hex_to_bytes("abcdef12345600dacdef94756eeabefa");

    ByteBlock message = hex_to_bytes("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ByteBlock encryptionMessage;
    ByteBlock decryptionMessage;

    CFB_Mode<Kuznyechik> encryptor(Kuznyechik(key), iv);
    encryptor.encrypt(message, encryptionMessage);
    encryptor.decrypt(encryptionMessage, decryptionMessage);

    QCOMPARE((hex_representation(decryptionMessage) == hex_representation(message)), true);
}

void TestCipher::encrypt_streamingEncryption() {
    ByteBlock key = hex_to_bytes("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ByteBlock iv = hex_to_bytes("abcdef12345600dacdef94756eeabefa");
    ByteBlock message1 = hex_to_bytes("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ByteBlock message2 = hex_to_bytes("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    ByteBlock message3 = hex_to_bytes("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc");
    ByteBlock encryptionMessage1;
    ByteBlock encryptionMessage2;
    ByteBlock encryptionMessage3;
    ByteBlock decryptionMessage1;
    ByteBlock decryptionMessage2;
    ByteBlock decryptionMessage3;

    CFB_Mode<Kuznyechik> encryptor1(Kuznyechik(key), iv);
    iv = encryptor1.encrypt(message1, encryptionMessage1);
    encryptor1.decrypt(encryptionMessage1, decryptionMessage1);

    CFB_Mode<Kuznyechik> encryptor2(Kuznyechik(key), iv);
    iv = encryptor2.encrypt(message2, encryptionMessage2);
    encryptor2.decrypt(encryptionMessage2, decryptionMessage2);

    CFB_Mode<Kuznyechik> encryptor3(Kuznyechik(key), iv);
    iv = encryptor3.encrypt(message3, encryptionMessage3);
    encryptor3.decrypt(encryptionMessage3, decryptionMessage3);
    QCOMPARE((hex_representation(decryptionMessage1) == hex_representation(message1)) &&
             (hex_representation(decryptionMessage2) == hex_representation(message2)) &&
             (hex_representation(decryptionMessage3) == hex_representation(message3)),
             true);
}

void TestCipher::hash_exampleFromStandart256() {
    unsigned char m[] = {
        0x32, 0x31, 0x30, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 0x39, 0x38, 0x37,
        0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31,
        0x30, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 0x39, 0x38, 0x37, 0x36, 0x35,
        0x34, 0x33, 0x32, 0x31, 0x30, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30
    };

    unsigned char ans256[] = {
        0x00, 0x55, 0x7b, 0xe5, 0xe5, 0x84, 0xfd, 0x52, 0xa4, 0x49, 0xb1, 0x6b, 0x02, 0x51, 0xd0, 0x5d,
        0x27, 0xf9, 0x4a, 0xb7, 0x6c, 0xba, 0xa6, 0xda, 0x89, 0x0b, 0x59, 0xd8, 0xef, 0x1e, 0x15, 0x9d
    };

    Streebog streebog(256);
    unsigned char *out = streebog.Hash(m, 63);
    QCOMPARE(memcmp(out, ans256, 32 * sizeof(unsigned char)), 0);
    delete[] out;
}

void TestCipher::hash_exampleFromStandart512() {
    unsigned char m[] = {
        0x32, 0x31, 0x30, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 0x39, 0x38, 0x37,
        0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31,
        0x30, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 0x39, 0x38, 0x37, 0x36, 0x35,
        0x34, 0x33, 0x32, 0x31, 0x30, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30
    };

    unsigned char ans512[] = {
        0x48, 0x6f, 0x64, 0xc1, 0x91, 0x78, 0x79, 0x41, 0x7f, 0xef, 0x08, 0x2b, 0x33, 0x81, 0xa4, 0xe2,
        0x11, 0xc3, 0x24, 0xf0, 0x74, 0x65, 0x4c, 0x38, 0x82, 0x3a, 0x7b, 0x76, 0xf8, 0x30, 0xad, 0x00,
        0xfa, 0x1f, 0xba, 0xe4, 0x2b, 0x12, 0x85, 0xc0, 0x35, 0x2f, 0x22, 0x75, 0x24, 0xbc, 0x9a, 0xb1,
        0x62, 0x54, 0x28, 0x8d, 0xd6, 0x86, 0x3d, 0xcc, 0xd5, 0xb9, 0xf5, 0x4a, 0x1a, 0xd0, 0x54, 0x1b
    };

    Streebog streebog(512);
    unsigned char *out = streebog.Hash(m, 63);
    QCOMPARE(memcmp(out, ans512, 64 * sizeof(unsigned char)), 0);
    delete[] out;
}

void TestCipher::hash_exampleWorkCipherWithMAC_attackerHasBeenDetected() {
    ByteBlock key = hex_to_bytes("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ByteBlock iv = hex_to_bytes("abcdef12345600dacdef94756eeabefa");

    ByteBlock message = hex_to_bytes("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ByteBlock encryptionMessage;
    ByteBlock decryptionMessage;

    CFB_Mode<Kuznyechik> encryptor(Kuznyechik(key), iv);
    encryptor.encrypt(message, encryptionMessage);

    Streebog streebog(256);
    unsigned char* mac1 = streebog.Hash(encryptionMessage.byte_ptr(), encryptionMessage.size());

    encryptionMessage[3]++;
    unsigned char* mac2 = streebog.Hash(encryptionMessage.byte_ptr(), encryptionMessage.size());

    QCOMPARE(memcmp(mac1, mac2, 64 * sizeof(unsigned char)) != 0, true);
}
