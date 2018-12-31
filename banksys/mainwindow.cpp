#include "mainwindow.h"
#include "ui_mainwindow.h"

BankSys *mybank;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bank = new BankSys();
    mybank=bank;
    bank->accounts_from_file("accounts.json");
    signup = new Signup(this);
    signup->setVisible(false);
    login = new Login(this);
    login->setVisible(false);
    prompt = new Prompt(this);
    prompt->setVisible(false);
    if (mybank->active_user){
        this->ui->loginbtn->setVisible(false);
        this->ui->signupbtn->setVisible(false);
        this->ui->actionLog_Out->setDisabled(false);
        this->ui->actionAccount_Setting->setDisabled(false);

        this->ui->label->setVisible(true);
        this->ui->logoutbtn->setVisible(true);
        this->ui->withdrawbtn->setVisible(true);
        this->ui->topupbtn->setVisible(true);
        this->ui->chpwdbtn->setVisible(true);
        this->ui->welcome->setVisible(true);
        this->ui->balance->setVisible(true);
        this->ui->welcome->setText(QString("您好，"+mybank->active_user->get(QString("name"))));
        this->ui->balance->setText(QString("¥ ")+QString::number(mybank->active_user->getbalance(),'f',2));
        this->ui->balance->setReadOnly(true);
    }
    else {
        this->ui->loginbtn->setVisible(true);
        this->ui->signupbtn->setVisible(true);
        this->ui->actionLog_Out->setDisabled(true);
        this->ui->actionAccount_Setting->setDisabled(true);

        this->ui->label->setVisible(false);
        this->ui->logoutbtn->setVisible(false);
        this->ui->withdrawbtn->setVisible(false);
        this->ui->topupbtn->setVisible(false);
        this->ui->chpwdbtn->setVisible(false);
        this->ui->welcome->setVisible(false);
        this->ui->balance->setVisible(false);
    }
    connect(login,SIGNAL(login_s()),this,SLOT(chglog()));
    connect(signup,SIGNAL(success()),this,SLOT(chglog()));
    topup = new Topup(this);
    topup->setVisible(false);
    withdraw = new Withdraw(this);
    withdraw->setVisible(false);
    chpwd = new Chpwd(this);
    chpwd->setVisible(false);
    connect(topup,SIGNAL(success()),this,SLOT(ch_display()));
    connect(withdraw,SIGNAL(success()),this,SLOT(ch_display()));
    QImage bkg;
    bkg.load("bkg.jpg");
    this->ui->bkg->setPixmap((QPixmap::fromImage(bkg).scaled(ui->bkg->size())));
    tcpServer = new QTcpServer(this);
    tcpSocket = Q_NULLPTR;
    if(!tcpServer->listen(QHostAddress::LocalHost, 9898)){
        qDebug()<<tcpServer->errorString();
        close();
    }
    else connect(tcpServer, SIGNAL(newConnection()), this, SLOT(connected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ch_display(){
    if (mybank->active_user) this->ui->balance->setText(QString::number(mybank->active_user->getbalance(),'f',2));
}

void MainWindow::on_actionLog_In_triggered()
{
    login->setVisible(true);
}

void MainWindow::on_actionSign_Up_triggered()
{
    signup->setVisible(true);
}

void MainWindow::on_actionLog_Out_triggered()
{
    int flag=mybank->log_out();
    if (flag==1) {
        prompt->setText(QString("成功登出"));
        prompt->setVisible(true);
    }
    if (mybank->active_user){
        this->ui->loginbtn->setVisible(false);
        this->ui->signupbtn->setVisible(false);
        this->ui->actionLog_Out->setDisabled(false);
        this->ui->actionAccount_Setting->setDisabled(false);

        this->ui->label->setVisible(true);
        this->ui->logoutbtn->setVisible(true);
        this->ui->withdrawbtn->setVisible(true);
        this->ui->topupbtn->setVisible(true);
        this->ui->chpwdbtn->setVisible(true);
        this->ui->welcome->setVisible(true);
        this->ui->balance->setVisible(true);
        this->ui->welcome->setText(QString("您好，"+mybank->active_user->get(QString("name"))+" 您的账户余额为"));
        this->ui->balance->setText(QString("¥ ")+QString::number(mybank->active_user->getbalance(),'f',2));
        this->ui->balance->setReadOnly(true);
    }
    else {
        this->ui->loginbtn->setVisible(true);
        this->ui->signupbtn->setVisible(true);
        this->ui->actionLog_Out->setDisabled(true);
        this->ui->actionAccount_Setting->setDisabled(true);

        this->ui->label->setVisible(false);
        this->ui->logoutbtn->setVisible(false);
        this->ui->withdrawbtn->setVisible(false);
        this->ui->topupbtn->setVisible(false);
        this->ui->chpwdbtn->setVisible(false);
        this->ui->welcome->setVisible(false);
        this->ui->balance->setVisible(false);
    }
}

void MainWindow::chglog(){
    if (mybank->active_user){
        this->ui->loginbtn->setVisible(false);
        this->ui->signupbtn->setVisible(false);
        this->ui->actionLog_Out->setDisabled(false);
        this->ui->actionAccount_Setting->setDisabled(false);

        this->ui->label->setVisible(true);
        this->ui->logoutbtn->setVisible(true);
        this->ui->withdrawbtn->setVisible(true);
        this->ui->topupbtn->setVisible(true);
        this->ui->chpwdbtn->setVisible(true);
        this->ui->welcome->setVisible(true);
        this->ui->balance->setVisible(true);
        this->ui->welcome->setText(QString("您好，"+mybank->active_user->get(QString("name"))));
        this->ui->balance->setText(QString("¥ ")+QString::number(mybank->active_user->getbalance(),'f',2));
        this->ui->balance->setReadOnly(true);
    }
    else {
        this->ui->loginbtn->setVisible(true);
        this->ui->signupbtn->setVisible(true);
        this->ui->actionLog_Out->setDisabled(true);
        this->ui->actionAccount_Setting->setDisabled(true);

        this->ui->label->setVisible(false);
        this->ui->logoutbtn->setVisible(false);
        this->ui->withdrawbtn->setVisible(false);
        this->ui->topupbtn->setVisible(false);
        this->ui->chpwdbtn->setVisible(false);
        this->ui->welcome->setVisible(false);
        this->ui->balance->setVisible(false);
    }
}

void MainWindow::on_actionTop_up_triggered()
{
    if (mybank->active_user){
        topup->setValue(mybank->active_user->getbalance());
//        qDebug()<<mybank->active_user->getbalance();
        topup->setVisible(true);
    }
    else {
        prompt->setText(QString("请先登陆"));
        prompt->setVisible(true);
    }

}

void MainWindow::on_actionAccount_Setting_triggered()
{
    if (mybank->active_user){
        chpwd->setVisible(true);
    }
    else {
        prompt->setText(QString("请先登陆"));
        prompt->setVisible(true);
    }
}

void MainWindow::on_actionWithdraw_triggered()
{
    if (mybank->active_user){
        withdraw->setValue(mybank->active_user->getbalance());
//        qDebug()<<mybank->active_user->getbalance();
        withdraw->setVisible(true);
    }
    else {
        prompt->setText(QString("请先登陆"));
        prompt->setVisible(true);
    }
}

void MainWindow::on_chpwdbtn_clicked()
{
    this->on_actionAccount_Setting_triggered();
}

void MainWindow::on_loginbtn_clicked()
{
    this->on_actionLog_In_triggered();
}

void MainWindow::on_logoutbtn_clicked()
{
    this->on_actionLog_Out_triggered();
}

void MainWindow::on_signupbtn_clicked()
{
    this->on_actionSign_Up_triggered();
}

void MainWindow::on_topupbtn_clicked()
{
    this->on_actionTop_up_triggered();
}

void MainWindow::on_withdrawbtn_clicked()
{
    this->on_actionWithdraw_triggered();
}

void MainWindow::parseData(){
    QString data = tcpSocket->readAll();
    qDebug() << data;
    QString yes = "yes";
    QString no = "no;";
    QStringList res = data.split(';');
    if (res.size()==3){
        Account *acc=mybank->find_by_id(res[0]);
        int flag = acc->verify(res[1]);
        if (flag==1) {
            int wflag=acc->withdraw(res[2].toDouble());
            mybank->save_file();
            if (mybank->active_user) this->ui->balance->setText(QString::number(mybank->active_user->getbalance(),'f',2));
            if (wflag==1) tcpSocket->write(yes.toUtf8());
            else tcpSocket->write((no+QString("余额不足")).toUtf8());
        }
        else tcpSocket->write((no+QString("密码错误")).toUtf8());
    }
    else if (res.size()==2){
        Account *acc=mybank->find_by_id(res[0]);
        if (acc){
            int flag=acc->verify(res[1]);
            if (flag==1) tcpSocket->write(yes.toUtf8());
            else tcpSocket->write((no+QString("绑定失败，检查账户密码")).toUtf8());
        }else tcpSocket->write((no+QString("绑定失败，检查账户密码")).toUtf8());
    }
}

void MainWindow::connected()
{
    tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(parseData()));
    qDebug()<<QString("Tcp连接已建立!");
}
