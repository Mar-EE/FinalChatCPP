#include "registrationform.h"
#include "ui_registrationform.h"
#include <QMessageBox>

RegistrationForm::RegistrationForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationForm)
{
    ui->setupUi(this);
}

RegistrationForm::~RegistrationForm()
{
    delete ui;
}

void RegistrationForm::setDatabase(std::shared_ptr<Database> dbPtr)
{
    m_dbPtr = dbPtr;
}

void RegistrationForm::on_loginButton_clicked()
{
    emit loginRequested();
}


void RegistrationForm::on_buttonBox_accepted()
{
    if (ui->passwordEdit->text() != ui->confirmPasswordEdit->text()){
        QMessageBox::critical(this, "Ошибка", "Пароли не совпадают");
        return;
    }
    auto userId = m_dbPtr->addUser(ui->loginEdit->text().toStdString(), ui->passwordEdit->text().toStdString());
    switch (userId) {
    case -1:
        QMessageBox::critical(this, "Ошибка", "Неверный логин");
        return;
    case -2:
        QMessageBox::critical(this, "Ошибка", "Пользователь уже существует");
        return;
    default:
        emit accepted(userId, ui->loginEdit->text());
    }
}


void RegistrationForm::on_buttonBox_rejected()
{
    emit rejected();
}

