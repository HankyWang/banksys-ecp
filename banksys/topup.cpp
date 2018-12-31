#include "topup.h"
#include "ui_topup.h"

extern BankSys *mybank;

Topup::Topup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Topup)
{
    ui->setupUi(this);
    prompt = new Prompt(this);
    prompt->setVisible(false);
}

Topup::~Topup()
{
    delete ui;
}

void Topup::setValue(double balance){
//    this->ui->balance->setText(QString(int(mybank->active_user->getbalance()*100)/100));
    this->ui->balance->setText(QString::number(balance,'f',2));
}

void Topup::on_pushButton_clicked()
{
    this->setVisible(false);
}

void Topup::on_pushButton_2_clicked()
{
    prompt->setText(QString("存款成功"));
    int amount=ui->amount->value();
    mybank->active_user->top_up(double(amount));
    this->setValue(mybank->active_user->getbalance());
    mybank->save_file();
    emit success();
    prompt->setVisible(true);
}
