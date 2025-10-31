#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class DatabaseManager {
public:
    static bool initDatabase();
    static QSqlDatabase getDB();
private:
    static QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
