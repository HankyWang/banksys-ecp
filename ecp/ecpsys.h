#ifndef ECPSYS_H
#define ECPSYS_H

#include <QString>
#include <QVector>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QDebug>
#include <QRegularExpression>

class Item{
private:
    QString img_path,name,desp,categ;
    int total_amount,amount;
    double price,off_price;
public:
    Item();
    Item(QString img_path,QString name,QString desp,QString categ,int is_off,double price,double off_price,int total_amount,int amount);
    QJsonObject toJson();
    QString getStr(QString attr);
    double getDouble(QString attr);
    int getInt(QString attr);
    int is_off;
    int purchase(int a);

};

class Account{
public:
    Account();
    Account(QString name,QString acc_id,QString password,QString phonenumebr,QStringList bankcards);
    QString getString(QString attr);
    QStringList getCards();
    void addCard(QString card);
    QJsonObject toJson();
private:
    QString name,acc_id,password,phonenumber;
    QStringList bank_cards;
};

class ECPSys
{
public:
    ECPSys();
    void items_fromfile(QString filename);
    void accounts_fromfile(QString filename);
    void add_account(QString acc_id,QString name,QString password,QString phonenumber);
    void add_item();
    QDir cfg;
    QVector<int> search(QString kw);
    int log_in(QString acc_id, QString pwd);
    int log_out();
    void save_accounts();
    void save_items();
    Account *active_user;
    QVector<Item> items;
    QVector<Account> accounts;
    QStringList categs;
    QStringList shopItem;
    QStringList shopCateg;
    QVector<int> shopNum;
};

#endif // ECPSYS_H
