#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QString>
#include <QJsonDocument>
#include <QFloat16>
#include <QVector>
#include <QDate>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class Operation
{
public:
    Operation();
    ~Operation();
    Operation(QDate &time, qfloat16 amount,QString op_type);//Withdraw ,Top up, Transfer, ch_pwd
private:
    QString op_type;
    QDate time;
    double amount;
};

class Account
{
public:
    Account();
    Account(QString pwd, QString acc_id, QString bn, QString id, QString n, double b);
    int ch_pwd(QString old_pwd, QString new_pwd, QString new_pwd_r);
    int withdraw(double amount);
    void top_up(double amount);
    int verify(QString pwd);
    QString get(QString attr);
    QJsonObject toJson();
    double getbalance(void);
private:
    QString account_id,id,password,bank_name,name;
    double balance;
    //QVector<Operation> ops;
};



#endif // ACCOUNT_H
