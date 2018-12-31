#ifndef BANKSYS_H
#define BANKSYS_H
#include "account.h"
#include <QVector>
#include <QString>
#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QCryptographicHash>
#include <QDebug>
#include <random>
#include <QTextStream>
#include <QJsonDocument>



class BankSys
{
public:
    BankSys();
    QString add_new_acc(QString bn,QString id, QString pwd, QString first_n, QString last_n);
    Account *active_user;
    Account *find_by_id(QString id);
    int log_in(QString acc_id, QString pwd);
    int log_out();
    void banks_from_file(QString filename);
    void accounts_from_file(QString filename);
    QDir cfg;
    void save_file();
private:
    QVector<Account> accounts;
    QStringList bank_names;
    QStringList bank_ids;
};


#endif // BANKSYS_H
