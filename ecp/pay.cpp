#include "pay.h"
#include "ui_pay.h"

extern ECPSys * sys;

Pay::Pay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pay)
{
    ui->setupUi(this);
    prompt = new Prompt(this);
    prompt->setVisible(false);
    this->ui->price1->setVisible(false);
    this->ui->price2->setVisible(false);
    this->ui->price3->setVisible(false);
    this->ui->book->setVisible(false);
    this->ui->gar->setVisible(false);
    tcpSocket = new QTcpSocket();
    tcpTimer = new QTimer();
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress::LocalHost, 6666);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(parse()));
    connect(tcpTimer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void Pay::setPrice(double b, double g, double off, double price){
    if (b==0&&g==0){
        this->ui->price->setText(QString("¥ ")+QString::number(price,'f',2));
        this->ui->line->setVisible(false);
    }
    else if (b==0&&g!=0){
        this->ui->price->setText(QString("¥ ")+QString::number(off,'f',2));
        this->ui->gar->setVisible(true);
        this->ui->price2->setText(QString("¥ ")+QString::number(price,'f',2));
        this->ui->price3->setText(QString("-¥ ")+QString::number(g,'f',2));
        this->ui->price2->setVisible(true);
        this->ui->price3->setVisible(true);
    }
    else if (b!=0&&g==0){
        this->ui->price->setText(QString("¥ ")+QString::number(off,'f',2));
        this->ui->book->setVisible(true);
        this->ui->price2->setText(QString("¥ ")+QString::number(price,'f',2));
        this->ui->price3->setText(QString("-¥ ")+QString::number(b,'f',2));
        this->ui->price2->setVisible(true);
        this->ui->price3->setVisible(true);
    }
    else {
        this->ui->price->setText(QString("¥ ")+QString::number(off,'f',2));
        this->ui->gar->setVisible(true);
        this->ui->book->setVisible(true);
        this->ui->price1->setText(QString("¥ ")+QString::number(price,'f',2));
        this->ui->price2->setText(QString("-¥ ")+QString::number(b,'f',2));
        this->ui->price3->setText(QString("-¥ ")+QString::number(g,'f',2));
        this->ui->price1->setVisible(true);
        this->ui->price2->setVisible(true);
        this->ui->price3->setVisible(true);
    }
    this->ui->comboBox->addItems(sys->active_user->getCards());
}

Pay::~Pay()
{
    delete ui;
}

void Pay::parse(){
    tcpTimer->stop();
    QString str = tcpSocket->readAll();
    qDebug()<<"Data Recieved:" << str;
    QStringList res = str.split(';');
    if (res.size()==1){
        prompt->setText(QString("支付成功"));
        prompt->setVisible(true);
        this->setVisible(false);
        emit success();
    }
    else{
        prompt->setText(res[1]);
        prompt->setVisible(true);
    }
}

void Pay::timeout(){
    tcpTimer->stop();
    tcpSocket->abort();
    prompt->setText(QString("请求超时"));
    prompt->setVisible(true);
}

void Pay::on_pushButton_clicked()
{
    QString acc_id=this->ui->comboBox->currentText().left(10);
    QString password= this->ui->password->text();
    QString price=this->ui->price->text().mid(2);
    QString data=QString(acc_id+";"+password+";"+price);
    qDebug()<<acc_id<<password<<price;
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress::LocalHost, 9898);
    tcpSocket->write(QByteArray(data.toUtf8()));
    tcpTimer->setInterval(3000);
    tcpTimer->start();
}

void Pay::on_pushButton_2_clicked()
{
    this->setVisible(false);
}
