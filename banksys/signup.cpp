#include "signup.h"
#include "ui_signup.h"


extern BankSys *mybank;

Signup::Signup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);
    prompt = new Prompt(this);
}

Signup::~Signup()
{
    delete ui;
}

void Signup::on_pushButton_clicked()
{
    int flag=1;
    QString first_n=ui->fn->text();
    QString last_n=ui->ln->text();
    QString id=ui->id->text();
    QString bank_name=ui->bank->currentText();
    QString pwd=ui->pwd->text();
    QString pwd_r=ui->pwd_r->text();
    QString acc_id;
    QRegularExpression id_pattern("(^[1-9]\\d{5}(18|19|([23]\\d))\\d{2}((0[1-9])|(10|11|12))(([0-2][1-9])|10|20|30|31)\\d{3}[0-9Xx]$)");
    QRegularExpressionMatch match = id_pattern.match(id);
    if (match.hasMatch()==false){
        prompt->setText(QString("身份ID不合法"));
        prompt->setVisible(true);
        flag=0;
    }
    if (pwd!=pwd_r){
        prompt->setText(QString("两次密码不一致"));
        prompt->setVisible(true);
        flag=0;
    }
    QRegularExpression pwd_pattern("(^\\d{6}$)");
    match=pwd_pattern.match(pwd);
    if (match.hasMatch()==false){
        prompt->setText(QString("不合法的密码"));
        prompt->setVisible(true);
        flag=0;
    }
    if (flag==1) {
        acc_id=mybank->add_new_acc(bank_name.mid(bank_name.indexOf('(')+1,bank_name.indexOf(')')-bank_name.indexOf('(')-1),id,pwd,first_n,last_n);
        prompt->setText(QString("注册成功,您的账号为"));
        prompt->setLine(acc_id);
        prompt->setVisible(true);
//        qDebug()<<acc_id;
        mybank->log_in(acc_id,pwd);
        emit success();
        this->setVisible(false);
    }
//    qDebug()<<first_n<<last_n<<id<<pwd<<pwd_r;
}

void Signup::on_pushButton_2_clicked()
{
    this->setVisible(false);
}
