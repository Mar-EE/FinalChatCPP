#include "loginform.h"
#include "ui_loginform.h"
#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::setDatabase(std::shared_ptr<Database> dbPtr)
{
    m_dbPtr = dbPtr;
}

void LoginForm::setExtraDB(std::shared_ptr<ExtraDB> extraDBPtr)
{
    m_extraDBPtr = extraDBPtr;
}


void LoginForm::on_buttonBox_accepted()
{
    if(m_extraDBPtr->getDisablesUsersList().contains(ui->loginEdit->text()) ||
        m_extraDBPtr->getBannedUsersList().contains(ui->loginEdit->text())) {
        QMessageBox::critical(this, "Ошибка", "Вход невозможен. Пользователь в черном списке");
        return;
    }
    auto userId = m_dbPtr->checkPassword(ui->loginEdit->text().toStdString(), ui->passwordEdit->text().toStdString());
    if (userId == -1){
        QMessageBox::critical(this, "Ошибка", "Неверный пользователь или пароль");
        return;
    }
    emit accepted(userId, ui->loginEdit->text());
}


void LoginForm::on_buttonBox_rejected()
{
    emit rejected();
}


void LoginForm::on_registrationPushButton_clicked()
{
    emit registerRequested();
}

