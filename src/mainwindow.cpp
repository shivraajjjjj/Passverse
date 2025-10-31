//“Built a cross-platform password manager using C++ and Qt. Implemented AES-256 encryption with PBKDF2 key derivation,
//master password authentication, and a responsive GUI.”
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sessionkey.h"
#include "databasemanager.h"
#include "deletedialog.h"
#include<QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->searchEdit->hide();
    // Set icons (from resource file)
    ui->addBtn->setIcon(QIcon(":/icons/add.png"));
    ui->searchBtn->setIcon(QIcon(":/icons/search.png"));

    // Set icon sizes (adjust to your UI)
    QSize iconSize(32, 32);
    ui->addBtn->setIconSize(iconSize);
    //ui->searchBtn->setIconSize(iconSize);

    //Optional: make them flat and borderless for a modern look
    ui->addBtn->setFlat(true);
    ui->searchBtn->setFlat(true);
    loadCredentials();

    connect(ui->addBtn, &QPushButton::clicked, this, [this]() {
        addDialog dialog(this);
        connect(&dialog, &addDialog::entryAdded, this, &MainWindow::loadCredentials);
        dialog.exec();
    });

    connect(ui->searchEdit, &QLineEdit::textChanged, this, &MainWindow::filterCredentials);

    ui->tableCredentials->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableCredentials, &QTableWidget::customContextMenuRequested,
            this, &MainWindow::showContextMenu);

    idleTimer = new QTimer(this);
    idleTimer->setInterval(2*60*1000); // 2 minutes
    connect(idleTimer, &QTimer::timeout, this, &MainWindow::lockApp);
    idleTimer->start();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::loadCredentials()
{
    ui->tableCredentials->setRowCount(0);
    // ensure table has 4 columns: Site, Username, Password, (hidden) id
    ui->tableCredentials->setColumnCount(4);
    ui->tableCredentials->setHorizontalHeaderLabels({"Site", "Username", "Password", "ID"});
    ui->tableCredentials->setColumnHidden(3, true);

    QSqlQuery query(DatabaseManager::getDB());
    query.prepare("SELECT id, site, ivsite, username, ivuser, password, ivpass FROM credentials");
    if (!query.exec()) {
        qDebug() << "Load query failed:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        int id = query.value("id").toInt();

        QByteArray encSite = QByteArray::fromBase64(query.value("site").toByteArray());
        QByteArray ivSite = QByteArray::fromBase64(query.value("ivsite").toByteArray());
        QString site = CryptoHelper::decryptAES(encSite, sessionkey::getKey(), ivSite);

        QByteArray encUser = QByteArray::fromBase64(query.value("username").toByteArray());
        QByteArray ivUser = QByteArray::fromBase64(query.value("ivuser").toByteArray());
        QString user = CryptoHelper::decryptAES(encUser, sessionkey::getKey(), ivUser);

         QByteArray encPass = QByteArray::fromBase64(query.value("password").toByteArray());
         QByteArray ivPass = QByteArray::fromBase64(query.value("ivpass").toByteArray());
        // QString pass = CryptoHelper::decryptAES(encPass, sessionkey::getKey(), ivPass);

        ui->tableCredentials->insertRow(row);
        QTableWidgetItem *passItem = new QTableWidgetItem("********");
        passItem->setData(Qt::UserRole, QString(encPass.toBase64()));      // store encrypted pass
        passItem->setData(Qt::UserRole + 1, QString(ivPass.toBase64()));
        ui->tableCredentials->setItem(row, 0, new QTableWidgetItem(site));
        ui->tableCredentials->setItem(row, 1, new QTableWidgetItem(user));
        ui->tableCredentials->setItem(row, 2, new QTableWidgetItem(*passItem));
        ui->tableCredentials->setItem(row, 3, new QTableWidgetItem(QString::number(id))); // hidden id

        row++;
    }

    ui->tableCredentials->resizeColumnsToContents();


    // Optional: make the table look better
    ui->tableCredentials->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableCredentials->setAlternatingRowColors(true);
    ui->tableCredentials->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableCredentials->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableCredentials->setShowGrid(true);
}
void MainWindow::on_searchBtn_clicked(){
    // Toggle visibility
    bool visible = ui->searchEdit->isVisible();
    ui->searchEdit->setVisible(!visible);

    if (!visible) {
        ui->searchEdit->setFocus();
    } else {
        // Hide and reset table if user clicks again
        ui->searchEdit->clear();
        loadCredentials();
    }
}
void MainWindow::filterCredentials(const QString &text)
{
    ui->tableCredentials->setRowCount(0);
    QSqlQuery query;
    query.prepare("SELECT id, site, ivsite, username, ivuser, password, ivpass FROM credentials");
    if (!query.exec()) {
        qDebug() << "Load query failed:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        int id = query.value("id").toInt();

        QByteArray encSite = QByteArray::fromBase64(query.value("site").toByteArray());
        QByteArray ivSite = QByteArray::fromBase64(query.value("ivsite").toByteArray());
        QString site = CryptoHelper::decryptAES(encSite, sessionkey::getKey(), ivSite);

        QByteArray encUser = QByteArray::fromBase64(query.value("username").toByteArray());
        QByteArray ivUser = QByteArray::fromBase64(query.value("ivuser").toByteArray());
        QString user = CryptoHelper::decryptAES(encUser, sessionkey::getKey(), ivUser);

        QByteArray encPass = QByteArray::fromBase64(query.value("password").toByteArray());
        QByteArray ivPass = QByteArray::fromBase64(query.value("ivpass").toByteArray());
        QString pass = CryptoHelper::decryptAES(encPass, sessionkey::getKey(), ivPass);

        if (text.isEmpty() ||site.contains(text, Qt::CaseInsensitive) ||user.contains(text, Qt::CaseInsensitive))
        {
            int row = ui->tableCredentials->rowCount();
            ui->tableCredentials->insertRow(row);
            ui->tableCredentials->setItem(row, 0, new QTableWidgetItem(site));
            ui->tableCredentials->setItem(row, 1, new QTableWidgetItem(user));
            ui->tableCredentials->setItem(row, 2, new QTableWidgetItem(pass));
            ui->tableCredentials->setItem(row, 3, new QTableWidgetItem(QString::number(id)));
        }
        row++;
    }
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (ui->searchEdit->isVisible()) {
        QWidget *clicked = childAt(event->pos());
        if (clicked != ui->searchEdit && clicked != ui->searchBtn) {
            ui->searchEdit->clear();
            ui->searchEdit->setVisible(false);
            loadCredentials();
        }
    }
    QMainWindow::mousePressEvent(event);
}
// showContextMenu: build menu and call delete/update using the row's DB id
void MainWindow::showContextMenu(const QPoint &pos) {
    QModelIndex index = ui->tableCredentials->indexAt(pos);
    if (!index.isValid()) return;

    QMenu contextMenu(this);
    QAction *deleteAction = contextMenu.addAction("Delete");
    QAction *updateAction = contextMenu.addAction("Update");
    QAction *copyAction = contextMenu.addAction("Copy Password");

    QAction *selectedAction = contextMenu.exec(ui->tableCredentials->viewport()->mapToGlobal(pos));
    if (!selectedAction) return;

    int row = index.row();

    if (selectedAction == deleteAction) {
        // Confirm & delete by id
        QTableWidgetItem *idItem = ui->tableCredentials->item(row, 3); // hidden id column
        if (!idItem) {
            QMessageBox::critical(this, "Error", "Internal error: id missing.");
            return;
        }
        int id = idItem->text().toInt();

        DeleteDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted && dialog.confirmDelete()) {
            if (deleteCredentialById(id)) {
                ui->tableCredentials->removeRow(row); // remove from UI only after DB deletion success
            }
        }
    } else if (selectedAction == updateAction) {
        // Launch inline update using the selected row
        on_updateAction_triggered_forRow(row);
    }  else if (selectedAction == copyAction) {
        copyPassword(index.row());
    }
}
// Delete helper: delete by DB id. returns true on success.
bool MainWindow::deleteCredentialById(int id) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Error", "Database not open!");
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM credentials WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Error", "Failed to delete entry: " + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::information(this, "Not Found", "No matching entry found to delete.");
        return false;
    }

    QMessageBox::information(this, "Deleted", "Entry deleted successfully!");
    return true;
}
// Entry point when user chooses Update from context menu for a specific row
void MainWindow::on_updateAction_triggered_forRow(int row)
{
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a credential to update.");
        return;
    }

    // retrieve id from hidden column (3)
    QTableWidgetItem *idItem = ui->tableCredentials->item(row, 3);
    if (!idItem) {
        QMessageBox::critical(this, "Error", "Internal error: id missing.");
        return;
    }
    int id = idItem->text().toInt();

    QString site = ui->tableCredentials->item(row, 0)->text();
    QString username = ui->tableCredentials->item(row, 1)->text();

    // build small inline dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Update Password");
    dialog.setFixedSize(360, 140);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLabel *label = new QLabel("Enter new password for " + site + " (" + username + "):");
    QLineEdit *passEdit = new QLineEdit();
    passEdit->setEchoMode(QLineEdit::Password);
    QPushButton *updateBtn = new QPushButton("Update");

    layout->addWidget(label);
    layout->addWidget(passEdit);
    layout->addWidget(updateBtn);

    // handle update click
    connect(updateBtn, &QPushButton::clicked, [&]() {
        QString newPass = passEdit->text().trimmed();
        if (newPass.isEmpty()) {
            QMessageBox::warning(&dialog, "Error", "Password cannot be empty!");
            return;
        }

        // Encrypt new password and store Base64
        QByteArray ivPass = CryptoHelper::generateRandomBytes(16);
        QByteArray encPass = CryptoHelper::encryptAES(newPass, sessionkey::getKey(), ivPass);

        QSqlDatabase db = QSqlDatabase::database();
        if (!db.isOpen()) {
            QMessageBox::critical(&dialog, "Error", "Database not open!");
            return;
        }

        QSqlQuery query(db);
        query.prepare("UPDATE credentials SET password = ?, ivpass = ? WHERE id = ?");
        query.addBindValue(encPass.toBase64());
        query.addBindValue(ivPass.toBase64());
        query.addBindValue(id);

        if (!query.exec()) {
            QMessageBox::critical(&dialog, "Error", "Update failed: " + query.lastError().text());
            return;
        }

        if (query.numRowsAffected() == 0) {
            QMessageBox::warning(&dialog, "Not Found", "No matching entry found to update.");
            return;
        }

        QMessageBox::information(&dialog, "Success", "Password updated successfully!");
        dialog.accept();
    });

    if (dialog.exec() == QDialog::Accepted) {
        loadCredentials(); // refresh UI after update
    }
}
void MainWindow::copyPassword(int row) {
    if (row < 0) return;

    // Extract the encrypted password and IV from table (hidden)
    QString encryptedPass = ui->tableCredentials->item(row, 2)->data(Qt::UserRole).toString(); // you may store it differently
    QString ivPass = ui->tableCredentials->item(row, 2)->data(Qt::UserRole + 1).toString();

    QByteArray enc = QByteArray::fromBase64(encryptedPass.toUtf8());
    QByteArray iv = QByteArray::fromBase64(ivPass.toUtf8());

    QString decrypted = CryptoHelper::decryptAES(enc, sessionkey::getKey(), iv);

    // Copy to clipboard
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(decrypted);

    QMessageBox::information(this, "Copied", "Password copied to clipboard for 30 seconds.");

    // Auto clear after 15 sec
    QTimer::singleShot(30000, [clipboard]() {
        if (clipboard->text().length() > 0)
            clipboard->clear();
    });
}
bool MainWindow::event(QEvent *event) {
    if (event->type() == QEvent::MouseMove ||
        event->type() == QEvent::KeyPress ||
        event->type() == QEvent::MouseButtonPress) {
        idleTimer->start(); // reset timer
    }
    return QMainWindow::event(event);
}
void MainWindow::lockApp() {
    idleTimer->stop();
    this->hide();

    authDialog auth;
    if (auth.exec() == QDialog::Accepted) {
        this->show();
        idleTimer->start();
    } else {
        QApplication::quit();
    }
}
