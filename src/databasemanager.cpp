#include "databasemanager.h"
#include <QSqlDatabase>
#include <QDir>
#include <QCoreApplication>
#include <QSqlError>
QSqlDatabase DatabaseManager::db = QSqlDatabase();

bool DatabaseManager::initDatabase() {
    QCoreApplication::addLibraryPath("C:/Qt/6.9.2/mingw_64/plugins");
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qWarning() << "SQLite driver not available!";
        return false;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QDir::currentPath() + "/passverse.db";
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qWarning() << "Failed to open DB:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS credentials ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "site BLOB, ivsite BLOB,"
               "username BLOB, ivuser BLOB,"
               "password BLOB, ivpass BLOB"
               ");");
    QSqlQuery q("SELECT COUNT(*) FROM credentials");
    return true;
}

QSqlDatabase DatabaseManager::getDB() {
    return db;
}
