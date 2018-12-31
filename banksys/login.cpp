#include "login.h"
#include "ui_login.h"

extern BankSys *mybank;

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    prompt = new Prompt(this);
    prompt->setVisible(false);

}

Login::~Login()
{
    delete ui;
}


void Login::on_pushButton_clicked()
{   if (remain_time>0){
        int flag = mybank->log_in(ui->account->text(),ui->pwd->text());
        if (flag==-1){
            prompt->setText(QString("用户不存在"));
            prompt->setVisible(true);
        }
        if (flag==0) {
            this->remain_time-=1;
            prompt->setText(QString("错误的用户id或密码\n剩余尝试次数：")+QString::number(this->remain_time+1));
            prompt->setVisible(true);
        }
        if (flag==1){
            prompt->setText(QString("成功登陆"));
            prompt->setVisible(true);
            this->setVisible(false);
            emit login_s();
//            qDebug()<<mybank->active_user->toJson();
        }
//        this->ui->account->setText(QString(""));
        this->ui->pwd->setText(QString(""));
    } else {
        this->prompt->setText(QString("错误用户id或密码.\n 请稍后再试"));
        prompt->setVisible(true);
        this->setVisible(false);
    }

}

void Login::on_pushButton_2_clicked()
{
    this->ui->account->setText(QString(""));
    this->ui->pwd->setText(QString(""));
    this->setVisible(false);
}
