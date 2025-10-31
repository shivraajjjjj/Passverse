#include "deletedialog.h"
#include "ui_deletedialog.h"
#include<QMessageBox>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
DeleteDialog::DeleteDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::DeleteDialog), confirmed(false)
{
    ui->setupUi(this);
    setWindowTitle("Confirm Delete");
}

DeleteDialog::~DeleteDialog() {
    delete ui;
}

bool DeleteDialog::confirmDelete() const {
    return confirmed;
}

void DeleteDialog::on_btnYes_clicked() {
    confirmed = true;
    accept();
}

void DeleteDialog::on_btnNo_clicked() {
    confirmed = false;
    reject();
}
