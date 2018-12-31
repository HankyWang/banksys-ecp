#include "signup.h"
#include "ui_signup.h"

extern ECPSys *sys;

Signup::Signup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);
    prompt = new Prompt(this);
    prompt->setVisible(false);
}

Signup::~Signup()
{
    delete ui;
}

void Signup::on_pushButton_clicked()
{
    int err=0;
    QString acc_id=this->ui->acc_id->text();
    QString pwd=this->ui->pwd->text();
    QString phonenumber=this->ui->phone->text();
    QString pwd_r=this->ui->pwd_r->text();
    QString name=this->ui->name->text();
    QRegularExpression numalpha("^\\w*$");
    QRegularExpressionMatch match = numalpha.match(acc_id);
    if (match.hasMatch()==false){
        prompt->setText(QString("用户名不合法"));
        err=1;
    }
    match=numalpha.match(pwd);
    if (match.hasMatch()==false){
        prompt->setText(QString("密码中不能含有非法字符"));
        err=1;
    }
    if (pwd!=pwd_r){
        prompt->setText(QString("两次密码不一致"));
        err=1;
    }
    QRegularExpression phone("^1[3|4|5|7|8][0-9]{9}$");
    match = phone.match(phonenumber);
    if (match.hasMatch()==false){
        prompt->setText(QString("请输入正确的电话号码"));
        err=1;
    }
    int re=0;
    for (int i=0;i<sys->accounts.size();i++)
        if (sys->accounts[i].getString(QString("acc_id"))==acc_id)
            re=1;
    if (re==1){
        prompt->setText(QString("用户名已存在请重试"));
        err=1;
    }
    if (err==0){
        sys->add_account(acc_id,name,pwd,phonenumber);
        prompt->setText(QString("注册成功"));
        prompt->setVisible(true);
        this->setVisible(false);
    }
    else {
        prompt->setVisible(true);
        this->ui->pwd->setText(QString());
        this->ui->pwd_r->setText(QString());
    }
}



void Signup::on_pushButton_3_clicked()
{
    QString acc_id=this->ui->acc_id->text();
    int re=0;
    for (int i=0;i<sys->accounts.size();i++)
        if (sys->accounts[i].getString(QString("acc_id"))==acc_id)
            re=1;
    if (re==1)
        prompt->setText(QString("用户名已存在请重试"));
    else
        prompt->setText(QString("恭喜你，用户名可用"));
    prompt->setVisible(true);
}

void Signup::on_pushButton_2_clicked()
{
    this->setVisible(false);
}
