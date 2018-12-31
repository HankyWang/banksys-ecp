#include "account.h"

Account::Account()
{
    this->account_id=QString();
    this->id=QString();
    this->password=QString();
    this->bank_name=QString();
    this->balance=0.0;
    this->name=QString();
    //this->ops.clear();
}

Account::Account(QString pwd, QString acc_id, QString bn, QString _id, QString n,double b=0.0){
    this->password=pwd;
    this->account_id=acc_id;
    this->bank_name=bn;
    this->id=_id;
    this->name=n;
    this->balance=b;
}

int Account::ch_pwd(QString old_pwd, QString new_pwd, QString new_pwd_r){ //1success 2oldnomatch 3newdiff
    if (this->password==old_pwd)
        if (new_pwd!=new_pwd_r) return 3;   //  两次密码不一样
        else if (new_pwd==old_pwd) return 4; //密码相同
        else {
            QRegularExpression pwd_pattern("(^\\d{6}$)");
            QRegularExpressionMatch match=pwd_pattern.match(new_pwd);
            if (match.hasMatch()==false)
                return 5;//密码不合法
            else{
                this->password=new_pwd;
                return 1;
            }//成功
        }
    else return 2;                             //密码错误
}

int Account::verify(QString pwd){
    if (this->password==pwd) return 1;
    else return 0;
}

QJsonObject Account::toJson(){
    QJsonObject node;
    node.insert("password",this->password);
    node.insert("account_id",this->account_id);
    node.insert("name",this->name);
    node.insert("balance",float(this->balance));
    node.insert("id",this->id);
    node.insert("bank_name",this->bank_name);
    return node;
}

int Account::withdraw(double amount){//1success 0nobalance
    if (this->balance<amount) return 0;
    this->balance-=amount;
    return 1;
}

void Account::top_up(double amount){
    this->balance+=amount;
}

QString Account::get(QString attr){
    if (attr=="acc_id") return this->account_id;
    else if (attr=="id") return this->id;
    else if (attr=="bank") return this->bank_name;
    else if (attr=="name") return this->name;
    return QString("");
}

double Account::getbalance(){
    return this->balance;
}

