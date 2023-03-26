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

