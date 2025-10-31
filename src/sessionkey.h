#ifndef SESSIONKEY_H
#define SESSIONKEY_H
#include<QString>
#include <QByteArray>
class sessionkey{
private:
    static QByteArray encryptionKey;

public:
    static void setKey(const QByteArray &key);
    static QByteArray getKey();
};

#endif // SESSIONKEY_H
