#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include <QMainWindow>
#include <memory>
#include "Database.h"
#include "ExtraDB.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int userId,
                        QString userName,
                        std::shared_ptr<Database> dbPtr = nullptr,
                        std::shared_ptr<ExtraDB> extraDBPtr = nullptr,
                        QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow* createClient(std::shared_ptr<Database> dbPtr = nullptr,
                                    std::shared_ptr<ExtraDB> extraDBPtr = nullptr);
    static int kInstanceCount;
    void updateUsersList();

private slots:

    void on_actionChangeClient_triggered();
    void on_actionExit_triggered();
    void updateChats();
    void on_commonMessageSendButton_clicked();
    void on_commonMessageLineEdit_returnPressed();
    void on_privateMessageLineEdit_returnPressed();
    void on_privateMessageSendButton_clicked();
    void on_actionShowUsersList_triggered();
    void on_actionShowChat_triggered();
    void on_deleteUserButton_clicked();
    void on_banUserButton_clicked();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Database> m_dbPtr;
    std::shared_ptr<ExtraDB> m_extraDBPtr;
    int m_userId;
    QString m_userName;
};

#endif // MAINWINDOW_H
