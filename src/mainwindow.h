#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "adddialog.h"
#include"deletedialog.h"
#include"updatedialog.h"
#include"authdialog.h"
#include"cryptohelper.h"
#include <QClipboard>
#include <QGuiApplication>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include<QMouseEvent>
#include<QIcon>
#include<QMessageBox>
#include<QStringList>
#include<QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include<QLayout>
#include <QMainWindow>
#include<QByteArray>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QByteArray encryptionKey;
private slots:
    void on_searchBtn_clicked();

private:
    Ui::MainWindow *ui;
    void loadCredentials();
    void filterCredentials(const QString &text);
    void mousePressEvent(QMouseEvent *event);
    void restoreFullTable();
    void showContextMenu(const QPoint &pos);
    bool deleteCredentialById(int id);
    void on_updateAction_triggered_forRow(int row);
    void copyPassword(int row);
    bool event(QEvent *event);
    QTimer *idleTimer;  // auto-lock timer
    void lockApp();
    QStringList allLines;
};
#endif // MAINWINDOW_H
