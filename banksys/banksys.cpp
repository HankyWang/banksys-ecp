#include "banksys.h"

BankSys::BankSys()
{
    accounts.clear();
    bank_names.clear();
    bank_ids.clear();
    active_user=nullptr;
    cfg=QDir::current();
    if (cfg.entryList().contains(QString("data"))) cfg.cd("data");
    else{
        cfg.mkdir("data");
        cfg.cd("data");
    }
    banks_from_file("banks.json");
}

QString BankSys::add_new_acc(QString bn, QString id, QString pwd, QString first_n, QString last_n){
    QString b_id,hash_part; // 3 3 4
    unsigned int encode;
    QCryptographicHash sha(QCryptographicHash::Sha1);
    QByteArray tmp = sha.hash(QString(first_n+last_n).toUtf8(),QCryptographicHash::Sha1);
        for (int i=0;i<tmp.size();i++)
            encode = (encode+(unsigned int)tmp[i]);
    encode%=10000;
    hash_part = QString::number(encode);
    for (int i=hash_part.length();i<4;i++)
        hash_part="0"+hash_part;
    b_id=bank_ids.at(bank_names.indexOf(bn));
    int flag=0;
    QString acc_id;
    while (flag==0){
        flag=1;
        int middle = rand()%100;
        QString middlestr=QString::number(middle);
        for (int i=middlestr.length();i<3;i++)
            middlestr="0"+middlestr;
        acc_id=b_id+hash_part+middlestr;
        for (int i=0;i<accounts.size();i++)
            if (accounts[i].get(QString(QString("acc_id")))==acc_id){
                flag=0;
                middle=rand()%1000;
                acc_id=QString(b_id)+QString::number(middle)+QString(hash_part);
                break;
            }
    }
    //qDebug()<<acc_id;
    this->accounts.append(Account(pwd,acc_id,bn,id,QString(last_n+first_n),0.0));
    save_file();
    return acc_id;
}

Account * BankSys::find_by_id(QString id){
    for (int i=0;i<this->accounts.size();i++)
        if (this->accounts[i].get(QString("acc_id"))==id)
            return &(this->accounts[i]);
    return nullptr;
}

void BankSys::banks_from_file(QString filename){
    QString filepath = QString(cfg.path()+"/"+filename);
    QFile readfile(filepath);
    readfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError err;
    QJsonDocument info=QJsonDocument::fromJson(readfile.readAll(),&err);
    readfile.close();
    //qDebug()<<(err.errorString());
    QJsonObject banks=info.object();
    this->bank_names=banks.keys();
    for (int i=0;i<bank_names.size();i++)
        this->bank_ids.append(banks[bank_names[i]].toString());
}

void BankSys::accounts_from_file(QString filename){
    QString filepath = QString(cfg.path()+"/"+filename);
    QFile readfile(filepath);
    readfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError err;
    QJsonDocument info=QJsonDocument::fromJson(readfile.readAll(),&err);
    readfile.close();
    //qDebug()<<(err.errorString());
    QJsonArray acc_info = info.object()["accounts"].toArray();
    for (int i=0;i<acc_info.size();i++){
        QJsonObject item_info=acc_info[i].toObject();
        QString pwd=item_info["password"].toString();
        QString acc_id=item_info["account_id"].toString();
        this->accounts.append(Account(item_info["password"].toString(),item_info["account_id"].toString(),item_info["bank_name"].toString(),item_info["id"].toString(),item_info["name"].toString(),item_info["balance"].toDouble()));
    }
}

int BankSys::log_in(QString acc_id, QString pwd){
    for (int i=0;i<accounts.size();i++)
        if (accounts[i].get(QString("acc_id"))==acc_id){
            if (accounts[i].verify(pwd)==1){
                active_user =&accounts[i];
                return 1;
            }
            return 0;//pwd no match
        }
    return -1;//no such account
}

int BankSys::log_out(){
    if (active_user==nullptr) return 0;
    active_user=nullptr;
    return 1;
}


void BankSys::save_file(){
    QString filepath = QString(cfg.path()+"/accounts.json");
    QFile writefile(filepath);
    writefile.open(QIODevice::WriteOnly|QIODevice::Text);
    QJsonArray acc_info;
    for (int i=0;i<this->accounts.size();i++)
        acc_info.insert(i,this->accounts[i].toJson());
    QJsonObject info;
    info.insert("accounts",acc_info);
    QJsonDocument doc;
    doc.setObject(info);
    QByteArray writeable = doc.toJson(QJsonDocument::Indented);
    writefile.write(writeable);
    writefile.close();
}
