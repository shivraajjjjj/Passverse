#include "adddialog.h"
#include "ui_adddialog.h"
#include"cryptohelper.h"
#include"databasemanager.h"
#include"sessionkey.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include<QFile>
#include<QTextStream>
#include<QMessageBox>
#include<QByteArray>
#include<QDebug>
addDialog::addDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDialog)
{
    ui->setupUi(this);
}

addDialog::~addDialog()
{
    delete ui;
}

void addDialog::on_submitButton_clicked()
{
    QString site = ui->siteEdit->text().trimmed();
    QString username = ui->userEdit->text().trimmed();
    QString password = ui->passEdit->text().trimmed();

    if (site.isEmpty() || username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill all the fields");
        return;
    }

    QByteArray ivSite = CryptoHelper::generateRandomBytes(16);
    QByteArray ivUser = CryptoHelper::generateRandomBytes(16);
    QByteArray ivPass = CryptoHelper::generateRandomBytes(16);

    QByteArray encSite = CryptoHelper::encryptAES(site, sessionkey::getKey(), ivSite);
    QByteArray encUser = CryptoHelper::encryptAES(username, sessionkey::getKey(), ivUser);
    QByteArray encPass = CryptoHelper::encryptAES(password, sessionkey::getKey(), ivPass);
    QSqlDatabase db = DatabaseManager::getDB();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB Error", "Database not open!");
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO credentials (site, ivsite, username, ivuser, password, ivpass) "
                  "VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(encSite.toBase64());
    query.addBindValue(ivSite.toBase64());
    query.addBindValue(encUser.toBase64());
    query.addBindValue(ivUser.toBase64());
    query.addBindValue(encPass.toBase64());
    query.addBindValue(ivPass.toBase64());

    if (!query.exec()) {
        qDebug() << "Insert failed:" << query.lastError().text();
    }
    emit entryAdded();  // tell main window to refresh table
    QMessageBox::information(this, "Success", "Entry added successfully!");
    this->accept();
}

