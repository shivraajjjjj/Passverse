#include "sessionkey.h"

QByteArray sessionkey::encryptionKey;

void sessionkey::setKey(const QByteArray &key) {
    encryptionKey = key;
}

QByteArray sessionkey::getKey() {
    return encryptionKey;
}
