#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#pragma once
#include <QDialog>
#include <memory>
#include "Database.h"
#include "ExtraDB.h"

namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartScreen(std::shared_ptr<Database> dbPtr = nullptr,
                         std::shared_ptr<ExtraDB> m_extraDBPtr = nullptr,
                         QWidget *parent = nullptr);
    ~StartScreen();
    void setLoginForm();
    void setRegistrationForm();

    int userId() const;
    QString userName() const;
    std::shared_ptr<Database> getDatabase() const;
    std::shared_ptr<ExtraDB> getExtraDB() const;

public slots:
    void onLoggedIn(uint userId, QString userName);
    void onRejectRequested();

private:
    Ui::StartScreen *ui;
    int m_userId;
    QString m_userName;
    std::shared_ptr<Database> m_dbPtr;
    std::shared_ptr<ExtraDB> m_extraDBPtr;
};

#endif // STARTSCREEN_H
