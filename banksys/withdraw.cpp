#include "withdraw.h"
#include "ui_withdraw.h"

extern BankSys *mybank;

Withdraw::Withdraw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Withdraw)
{
    ui->setupUi(this);
    prompt = new Prompt(this);
    prompt->setVisible(false);
}

Withdraw::~Withdraw()
{
    delete ui;
}

void Withdraw::setValue(double balance){
//    this->ui->balance->setText(QString(int(mybank->active_user->getbalance()*100)/100));
    this->ui->balance->setText(QString::number(balance,'f',2));
}

void Withdraw::on_pushButton_clicked()
{
    this->setVisible(false);
}


void Withdraw::on_pushButton_2_clicked()
{
    int amount=ui->amount->value();
    QString pwd=ui->pw->text();
    if (mybank->active_user->verify(pwd)){
        int wflag=mybank->active_user->withdraw(double(amount));
        if (wflag==1) {
            prompt->setText(QString("取款成功！"));
            this->setValue(mybank->active_user->getbalance());
            mybank->save_file();
            emit success();
        } else prompt->setText(QString("余额不足"));
    }
    else prompt->setText(QString("密码错误"));
    prompt->setVisible(true);
    this->ui->pw->setText(QString());
}
