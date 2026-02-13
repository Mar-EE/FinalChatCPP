#ifndef LOGINFORM_H
#define LOGINFORM_H
#pragma once
#include <QWidget>
#include <memory>
#include "Database.h"
#include "ExtraDB.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();
    void setDatabase(std::shared_ptr<Database> dbPtr);
    void setExtraDB(std::shared_ptr<ExtraDB> m_extraDBPtr);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_registrationPushButton_clicked();

signals:
    void registerRequested();
    void accepted(int userId, QString userName);
    void rejected();

private:
    Ui::LoginForm *ui;
    std::shared_ptr<Database> m_dbPtr;
    std::shared_ptr<ExtraDB> m_extraDBPtr;
};

#endif // LOGINFORM_H
