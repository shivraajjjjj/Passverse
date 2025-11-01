#include "creatempassdialog.h"
#include "ui_creatempassdialog.h"
#include "cryptohelper.h"
#include<QByteArray>
#include<QFile>
#include<QTextStream>
#include<QMessageBox>
#include<QJsonObject>
#include<QJsonDocument>
createMpassDialog::createMpassDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::createMpassDialog)
{
    ui->setupUi(this);
}

createMpassDialog::~createMpassDialog()
{
    delete ui;
}
static const QString MASTER_FILE = "master.pass";

bool createMpassDialog::masterExists() {
    QFile file(MASTER_FILE);
    return file.exists();
}
void createMpassDialog::on_submitBtn_clicked()
{
        QString password = ui->masterpass->text();
        QString confirm = ui->masterpass2->text();
        if(password.isEmpty()||confirm.isEmpty()){
            QMessageBox::warning(this,"Input error","fill both fields");
            return;
        }
        if(password==confirm){
        QByteArray salt = CryptoHelper::generateRandomBytes(16);
        QByteArray key = CryptoHelper::deriveKey(password, salt); // PBKDF2 key (hash)

        QJsonObject obj;
        obj["salt"] = QString(salt.toBase64());
        obj["hash"] = QString(key.toBase64());
        obj["iterations"] = 100000;

        QJsonDocument doc(obj);
        QFile file(MASTER_FILE);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Failed to create master.pass";
            return ;
        }
        file.write(doc.toJson());
        file.close();
        this->accept();
        }else{
            QMessageBox::warning(this,"Input error","password does not matched");
            return;
        }
}

