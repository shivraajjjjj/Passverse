#ifndef CRYPTOHELPER_H
#define CRYPTOHELPER_H
#include<QString>
#include<QByteArray>
class CryptoHelper
{
public:
    static QByteArray generateRandomBytes(int size);
    static QByteArray deriveKey(const QString &password, QByteArray salt);
    static QByteArray encryptAES(const QString &plaintext, const QByteArray &key, QByteArray &iv);
    static QString decryptAES(const QByteArray &ciphertext, const QByteArray &key, const QByteArray &iv);
    static bool validateDecryption(const QByteArray &cipher, const QByteArray &key, const QByteArray &iv);
};

#endif // CRYPTOHELPER_H
