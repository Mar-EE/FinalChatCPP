#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QTimer>
#include "mainwindow.h"
#include "startscreen.h"
#include "ui_mainwindow.h"

int MainWindow::kInstanceCount = 0;

MainWindow::MainWindow(int userId,
                       QString userName,
                       std::shared_ptr<Database> dbPtr,
                       std::shared_ptr<ExtraDB> extraDBPtr,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_userId(userId),
    m_userName(userName)
{
    ui->setupUi(this);
    kInstanceCount++;
    if (dbPtr){
        m_dbPtr = dbPtr;
    }
    else{
        m_dbPtr = make_shared<Database>();
    }
    if (extraDBPtr){
        m_extraDBPtr = extraDBPtr;
    }
    else{
        m_extraDBPtr = make_shared<ExtraDB>();
    }
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateChats);
    timer->start(10);
    auto ql = new QLabel(this);
    ql->setAttribute(Qt::WA_DeleteOnClose);
    ql->setMargin(5);
    ql->setText("Текущий пользователь: " + m_userName);
    ui->statusbar->addWidget(ql);
}

MainWindow::~MainWindow()
{
    delete ui;
    kInstanceCount--;
    if(kInstanceCount<=0)
        qApp->exit(0);
}

MainWindow *MainWindow::createClient(std::shared_ptr<Database> dbPtr, std::shared_ptr<ExtraDB> extraDBPtr)
{
    StartScreen s(dbPtr, extraDBPtr);
    auto result = s.exec();
    if (result == QDialog::Rejected){
        return nullptr;
    }
    auto w = new MainWindow(s.userId(), s.userName(), s.getDatabase(), s.getExtraDB());
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->updateUsersList();
    return w;
}

void MainWindow::on_actionChangeClient_triggered()
{
    auto w = createClient(m_dbPtr, m_extraDBPtr);
    if(w)
        w->show();
    this->close();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::updateChats()
{
    auto chatMessages = m_dbPtr->getChatMessages();
    QString chat;
    for(const auto &msg : chatMessages){
        chat.append(QString::fromStdString(msg) + "\n");
    }
    if(ui->commonChatBrowser->toPlainText()!=chat)
        ui->commonChatBrowser->setText(chat);

    chat.clear();

    auto privateMessages = m_dbPtr->getPrivateMessage();
    for(const auto &msg : privateMessages){
        if(QString::fromStdString(msg.getSender()) != m_userName && msg.getDest() != m_userId)
            continue;
        QString prefix;
        if(m_userName == QString::fromStdString(msg.getSender()) && m_userId == msg.getDest()){
            prefix = "свое сообщение: ";
        }
        else if(m_userName == QString::fromStdString(msg.getSender())){
            prefix = "сообщение кому" + QString(" <%1> ").arg(QString::fromStdString(m_dbPtr->getUserName(msg.getDest())));
        }
        else{
            prefix = "<" + QString::fromStdString(msg.getSender()) + "> " + "писал тебе" + ": ";
        }
        chat.append(prefix + QString::fromStdString(msg.getText()) + "\n");
    }
    if(ui->privateChatBrowser->toPlainText()!=chat)
        ui->privateChatBrowser->setText(chat);
}

void MainWindow::on_commonMessageLineEdit_returnPressed()
{
    on_commonMessageSendButton_clicked();
}

void MainWindow::on_commonMessageSendButton_clicked()
{
    m_dbPtr->addChatMessage(m_userName.toStdString(), ui->commonMessageLineEdit->text().toStdString());
    ui->commonMessageLineEdit->setText("");
}

void MainWindow::on_privateMessageLineEdit_returnPressed()
{
    on_privateMessageSendButton_clicked();
}

void MainWindow::on_privateMessageSendButton_clicked()
{
    QDialog dial(this);
    dial.setModal(true);
    auto l = new QVBoxLayout();
    dial.setLayout(l);
    auto userListWgt = new QListWidget(&dial);
    l->addWidget(userListWgt);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dial);
    l->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dial, &QDialog::reject);

    auto userList = m_dbPtr->getUserList();
    for(auto user : userList){
        userListWgt->addItem(QString::fromStdString(user));
    }
    userListWgt->setCurrentRow(0);
    auto result = dial.exec();

    if(result == QDialog::Accepted && userListWgt->currentItem()){
        m_dbPtr->addPrivateMessage(m_userName.toStdString(),
                                   userListWgt->currentItem()->text().toStdString(),
                                   ui->privateMessageLineEdit->text().toStdString());
        ui->privateMessageLineEdit->setText("");
    }
}

void MainWindow::updateUsersList()
{
    ui->userListWidget->clear();
    ui->disabledUserListWidget->clear();
    ui->bannedUserListWidget->clear();

    auto userList = m_dbPtr->getUserList();
    auto aaa = m_extraDBPtr->getDisablesUsersList();
    for(auto user : userList){
        if(!(m_extraDBPtr->getDisablesUsersList()).contains(QString::fromStdString(user)) &&
            !(m_extraDBPtr->getBannedUsersList()).contains(QString::fromStdString(user))){
ui->userListWidget->addItem(QString::fromStdString(user));
}
        /*if(!_disabledUsers.contains(QString::fromStdString(user)) &&
            !_bannedUsers.contains(QString::fromStdString(user))){
            ui->userListWidget->addItem(QString::fromStdString(user));
        }*/
    }
    ui->userListWidget->setCurrentRow(0);

    for(auto disabledUser : m_extraDBPtr->getDisablesUsersList())
    {
        ui->disabledUserListWidget->addItem(disabledUser);
    }
    for(auto bannedUser : m_extraDBPtr->getBannedUsersList())
    {
        ui->bannedUserListWidget->addItem(bannedUser);
    }
}

void MainWindow::on_actionShowChat_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionShowUsersList_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_deleteUserButton_clicked()
{
    if(ui->userListWidget->currentItem()){
        m_extraDBPtr->addDisabledUser(ui->userListWidget->currentItem()->text());
        //_disabledUsers.push_back(ui->userListWidget->currentItem()->text());
        updateUsersList();
    }
}


void MainWindow::on_banUserButton_clicked()
{
    if(ui->userListWidget->currentItem()){
        m_extraDBPtr->addBannedUser(ui->userListWidget->currentItem()->text());
        //_bannedUsers.push_back(ui->userListWidget->currentItem()->text());
        updateUsersList();
    }
}
