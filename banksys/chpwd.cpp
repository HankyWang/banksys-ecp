#include "chpwd.h"
#include "ui_chpwd.h"

extern BankSys *mybank;

Chpwd::Chpwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chpwd)
{
    ui->setupUi(this);
    prompt=new Prompt(this);
    prompt->setVisible(false);
}

Chpwd::~Chpwd()
{
    delete ui;
}

void Chpwd::on_pushButton_2_clicked()
{
    int flag;
    if (mybank->active_user) {
        flag=mybank->active_user->ch_pwd(this->ui->o_pwd->text(),ui->n_pwd->text(),ui->n_pwd_r->text());
        if (flag==1) {
            prompt->setText(QString("成功更改密码"));
            mybank->save_file();
            this->setVisible(false);
        }
        else if (flag==2) prompt->setText(QString("密码错误！"));
        else if (flag==3) prompt->setText(QString("两次密码不一致"));
        else if (flag==4) prompt->setText(QString("不能与旧密码相同"));
        else prompt->setText("不合法的密码");
        prompt->setVisible(true);
    }
    else{
        prompt->setText(QString("请先登录"));
        prompt->setVisible(true);
    }
    this->ui->o_pwd->setText(QString());
    this->ui->n_pwd->setText(QString());
    this->ui->n_pwd_r->setText(QString());
}
