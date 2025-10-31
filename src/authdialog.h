#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
namespace Ui {
class authDialog;
}

class authDialog : public QDialog
{
    Q_OBJECT

public:
    explicit authDialog(QWidget *parent = nullptr);
    ~authDialog();
private slots:
    void on_submitButton_clicked();
private:
   bool masterExists();
    bool authenticate()const;
    QByteArray getSalt();
    bool verifyMasterPassword(const QString &password);
    Ui::authDialog *ui;
    int attemptCount;
};

#endif // AUTHDIALOG_H
