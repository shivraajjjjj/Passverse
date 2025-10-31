#ifndef CREATEMPASSDIALOG_H
#define CREATEMPASSDIALOG_H

#include <QDialog>

namespace Ui {
class createMpassDialog;
}

class createMpassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit createMpassDialog(QWidget *parent = nullptr);
    ~createMpassDialog();

private slots:
    void on_submitBtn_clicked();

private:
    Ui::createMpassDialog *ui;
    bool masterExists();
};

#endif // CREATEMPASSDIALOG_H
