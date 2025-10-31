#include <QCoreApplication>
#include <QSslSocket>
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    qDebug() << "Supports SSL:" << QSslSocket::supportsSsl();
    qDebug() << "Build Version:" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "Runtime Version:" << QSslSocket::sslLibraryVersionString();

    return 0;
}
