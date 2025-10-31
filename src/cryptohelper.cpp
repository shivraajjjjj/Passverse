#include "cryptohelper.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QByteArray>
#include <QDebug>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include "cryptohelper.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <QDebug>

QByteArray CryptoHelper::generateRandomBytes(int size) {
    QByteArray data(size, 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(data.data()), size);
    return data;
}

QByteArray CryptoHelper::deriveKey(const QString &password, QByteArray salt) {
    if (salt.isEmpty()) {
        salt = generateRandomBytes(16); // 128-bit salt
    }

    QByteArray key(32, 0); // AES-256 => 32-byte key
    PKCS5_PBKDF2_HMAC(
        password.toUtf8().constData(),
        password.size(),
        reinterpret_cast<const unsigned char*>(salt.constData()),
        salt.size(),
        100000, // iterations
        EVP_sha256(),
        key.size(),
        reinterpret_cast<unsigned char*>(key.data())
        );
    return key;
}

QByteArray CryptoHelper::encryptAES(const QString &plaintext, const QByteArray &key, QByteArray &iv) {
    iv = generateRandomBytes(16); // 128-bit IV
    QByteArray ciphertext(plaintext.toUtf8().size() + EVP_MAX_BLOCK_LENGTH, 0);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0, ciphertext_len = 0;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.constData()),
                       reinterpret_cast<const unsigned char*>(iv.constData()));

    EVP_EncryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(ciphertext.data()), &len,
                      reinterpret_cast<const unsigned char*>(plaintext.toUtf8().constData()), plaintext.toUtf8().size());
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(ciphertext.data()) + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(ciphertext_len);
    return ciphertext; // Store as text
}

QString CryptoHelper::decryptAES(const QByteArray &ciphertext, const QByteArray &key, const QByteArray &iv) {
    QByteArray plaintext(ciphertext.size(), 0);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0, plaintext_len = 0;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.constData()),
                       reinterpret_cast<const unsigned char*>(iv.constData()));

    EVP_DecryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(plaintext.data()), &len,
                      reinterpret_cast<const unsigned char*>(ciphertext.constData()), ciphertext.size());
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(plaintext.data()) + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len);
    return QString::fromUtf8(plaintext);
}
bool CryptoHelper::validateDecryption(const QByteArray &cipher, const QByteArray &key, const QByteArray &iv) {
    QString result = decryptAES(cipher, key, iv);
    return !result.isEmpty();
}
