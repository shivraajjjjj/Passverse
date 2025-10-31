#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include <QDialog>
namespace Ui {
class DeleteDialog;
}

class DeleteDialog : public QDialog {
    Q_OBJECT

public:
    explicit DeleteDialog(QWidget *parent = nullptr);
    ~DeleteDialog();

    bool confirmDelete() const;
private slots:
    void on_btnYes_clicked();
    void on_btnNo_clicked();

private:
    Ui::DeleteDialog *ui;
    bool confirmed;
};
#endif // DELETEDIALOG_H
