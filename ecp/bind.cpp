#include "bind.h"
#include "ui_bind.h"

extern ECPSys *sys;

Bind::Bind(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Bind)
{
    ui->setupUi(this);
    prompt = new Prompt(this);
    prompt->setVisible(false);
    tcpSocket = new QTcpSocket();
    tcpTimer = new QTimer();
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress::LocalHost, 6666);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(parse()));
    connect(tcpTimer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void Bind::parse(){
    tcpTimer->stop();
    QString str = tcpSocket->readAll();
    qDebug()<<"Data Recieved:" << str;
    QStringList res = str.split(';');
    if (res.size()==1){
        sys->active_user->addCard(this->acc_id);
        sys->save_accounts();
        prompt->setText(QString("绑定成功"));
        prompt->setVisible(true);
        this->setVisible(false);
    }
    else{
        prompt->setText(res[1]);
        prompt->setVisible(true);
    }
}

void Bind::timeout(){
    tcpTimer->stop();
    tcpSocket->abort();
    prompt->setText(QString("请求超时"));
    prompt->setVisible(true);
}

Bind::~Bind()
{
    delete ui;
}

void Bind::on_pushButton_clicked()
{
    this->acc_id=this->ui->acc_id->text();
    this->password=this->ui->password->text();
    QString data=QString(this->acc_id+";"+this->password);
    qDebug()<<acc_id<<password;
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress::LocalHost, 9898);
    tcpSocket->write(QByteArray(data.toUtf8()));
    tcpTimer->setInterval(3000);
    tcpTimer->start();
}

void Bind::on_pushButton_2_clicked()
{
    this->setVisible(false);
}
