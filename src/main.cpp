#include "mainwindow.h"
#include "authdialog.h"
#include "creatempassdialog.h"
#include"databasemanager.h"
#include <QApplication>
#include<QFile>
#include<QDebug>
#include<QIcon>
#include<QSslSocket>
int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("C:/Qt/6.9.2/mingw_64/plugins");
    QApplication a(argc, argv);
    QFile styleFile(":/style.qss");
    if (!styleFile.open(QFile::ReadOnly)) {
        qDebug() << "Failed to load QSS file";
    } else {
        QString style = QLatin1String(styleFile.readAll());
        a.setStyleSheet(style);
    }

    if (!DatabaseManager::initDatabase()) {
        qCritical() << "Database initialization failed.";
        return -1;
    }
    QFile file("master.pass");

    if (!file.exists()) {
        // First-time user — create a master password
        createMpassDialog createDialog;
        createDialog.exec();
    }

    // After creation, check again
    if (!file.exists()) {
        // User closed without creating password
        return 0;
    }
    // Authenticate existing user
    authDialog auth;
    if (auth.exec() == QDialog::Accepted) {
        MainWindow w;
        w.resize(1200,800);
        w.show();
        return a.exec();
    }
    return 0;
}
