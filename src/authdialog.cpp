#include "authdialog.h"
#include "ui_authdialog.h"
#include "sessionkey.h"
#include "cryptohelper.h"
#include<QMessageBox>
#include<QFile>
#include<QTextStream>
#include<QByteArray>
#include<QCryptographicHash>
#include<QJsonObject>
#include<QJsonDocument>
authDialog::authDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::authDialog),
    attemptCount(0)
{
    ui->setupUi(this);
}

authDialog::~authDialog()
{
    delete ui;
}
static const QString MASTER_FILE = "master.pass";

bool authDialog::masterExists() {
    QFile file(MASTER_FILE);
    return file.exists();
}
void authDialog::on_submitButton_clicked(){
    QString enteredPass = ui->mpassEdit->text();

    if (enteredPass.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your master password!");
        return;
    }
    if (authDialog::verifyMasterPassword(enteredPass)) {
        QByteArray salt = authDialog::getSalt();
        QByteArray key = CryptoHelper::deriveKey(enteredPass, salt);
        sessionkey::setKey(key);  // ✅ Now set correct session key
        accept();
    } else {
        attemptCount++;
        if (attemptCount >= 3) {
            QMessageBox::critical(this, "Access Denied", "Too many failed attempts! Application will exit.");
            qApp->exit();
        } else {
            int remaining = 3 - attemptCount;
            QMessageBox::warning(this, "Incorrect Password",
                                 QString("Incorrect password! You have %1 attempt(s) left.").arg(remaining));
            ui->mpassEdit->clear();
        }
    }
}
bool authDialog::verifyMasterPassword(const QString &password) {
    QFile file(MASTER_FILE);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "master.pass not found!";
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return false;

    QJsonObject obj = doc.object();
    QByteArray salt = QByteArray::fromBase64(obj["salt"].toString().toUtf8());
    QByteArray storedHash = QByteArray::fromBase64(obj["hash"].toString().toUtf8());
    int iterations = obj["iterations"].toInt(100000);

    QByteArray derived = CryptoHelper::deriveKey(password, salt);
    return derived == storedHash;
}

QByteArray authDialog::getSalt() {
    QFile file(MASTER_FILE);
    if (!file.open(QIODevice::ReadOnly)) return QByteArray();

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    return QByteArray::fromBase64(obj["salt"].toString().toUtf8());
}

