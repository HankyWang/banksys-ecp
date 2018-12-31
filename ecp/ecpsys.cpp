#include "ecpsys.h"

Item::Item(){
    this->img_path=QString();
    this->name=QString();
    this->desp=QString();
    this->categ=QString();
    this->is_off=0;
    this->price=0;
    this->off_price=0;
    this->total_amount=0;
    this->amount=0;
}

Item::Item(QString img_path, QString name, QString desp, QString categ, int is_off, double price, double off_price, int total_amount, int amount){
    this->img_path=img_path;
    this->name=name;
    this->desp=desp;
    this->categ=categ;
    this->is_off=is_off;
    this->price=price;
    this->off_price=off_price;
    this->total_amount=total_amount;
    this->amount=amount;
}

double Item::getDouble(QString attr){
    if (attr==QString("price")) return this->price;
    else if (attr==QString("off_price")) return this->off_price;
    else return 0;
}

int Item::getInt(QString attr){
    if (attr==QString("total_amount")) return this->total_amount;
    else if (attr==QString("amount")) return this->amount;
    else return 0;
}

QString Item::getStr(QString attr){
    if (attr==QString("name")) return this->name;
    else if (attr==QString("desp")) return this->desp;
    else if (attr==QString("categ")) return this->categ;
    else if (attr==QString("img_path")) return this->img_path;
    else return 0;
}

QJsonObject Item::toJson(){
    QJsonObject node;
    node.insert("amount",this->amount);
    node.insert("name",this->name);
    node.insert("desp",this->desp);
    node.insert("price",this->price);
    node.insert("off_price",this->off_price);
    node.insert("total_amount",this->total_amount);
    node.insert("categ",this->categ);
    node.insert("is_off",this->is_off);
    return node;
}

int Item::purchase(int a){
    if (this->amount>=a) {
        this->amount-=a;
        return 1;
    }else return 0;

}

Account::Account(){
    this->acc_id=QString();
    this->bank_cards=QStringList();
    this->name=QString();
    this->password=QString();
    this->phonenumber=QString();
}

Account::Account(QString name, QString acc_id, QString password, QString phonenumebr, QStringList bankcards){
    this->name=name;
    this->acc_id=acc_id;
    this->password=password;
    this->phonenumber=phonenumebr;
    this->bank_cards=bankcards;
}

QString Account::getString(QString attr){
    if (attr=="name") return this->name;
    else if (attr=="acc_id") return this->acc_id;
    else if (attr=="password") return this->password;
    else if (attr=="phonenumber") return this->phonenumber;
    else return QString("");
}

QStringList Account::getCards(){
    return this->bank_cards;
}

void Account::addCard(QString card){
    QString str=card;
    if (card.left(3)=="100") str+="(ICBC)";
    else if (card.left(3)=="101") str+="(CCB)";
    else if (card.left(3)=="102") str+="(HSBC)";
    else if (card.left(3)=="103") str+="(ABC)";
    else if (card.left(3)=="104") str+="(BOC)";
    else if (card.left(3)=="105") str+="(CMBC)";
    else if (card.left(3)=="106") str+="(CIB)";
    int flag=0;
    for (int i=0;i<this->bank_cards.size();i++)
        if (str==bank_cards.at(i)) flag=1;
    if (flag==0) bank_cards.append(str);
}

QJsonObject Account::toJson(){
    QJsonObject node;
    node.insert("acc_id",this->acc_id);
    node.insert("name",this->name);
    node.insert("password",this->password);
    node.insert("phonenumber",this->phonenumber);
    QJsonArray cards;
    for (int i=0;i<bank_cards.size();i++)
        cards.append(bank_cards.at(i));
    node.insert("bank_cards",cards);
    return node;
}

ECPSys::ECPSys()
{
    accounts.clear();
    items.clear();
    categs.clear();
    accounts.clear();
    active_user=nullptr;
    cfg=QDir::current();
    qDebug()<<cfg.path();
    if (cfg.entryList().contains(QString("data"))) cfg.cd("data");
    else{
        cfg.mkdir("data");
        cfg.cd("data");
    }
    shopNum.clear();
    shopItem.clear();
    shopCateg.clear();
}

void ECPSys::accounts_fromfile(QString filename){
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
        QJsonArray cards=item_info["bank_cards"].toArray();
        QStringList bankcards;
        for (int i=0;i<cards.size();i++)
            bankcards.append(cards[i].toString());
        this->accounts.append(Account(item_info["name"].toString(),item_info["acc_id"].toString(),item_info["password"].toString(),item_info["phonenumber"].toString(),bankcards));
    }
}

void ECPSys::items_fromfile(QString filename){
    QString filepath = QString(cfg.path()+"/"+filename);
    QFile readfile(filepath);
    readfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError err;
    QJsonDocument info=QJsonDocument::fromJson(readfile.readAll(),&err);
    readfile.close();
    QJsonArray acc_info = info.object()["items"].toArray();
    for (int i=0;i<acc_info.size();i++){
        QJsonObject item_info=acc_info[i].toObject();
        int  flag=0;
        for (int i=0;i<this->categs.size();i++)
            if (item_info["categ"].toString()==categs.at(i)) flag=1;
        if (flag==0) categs.append(item_info["categ"].toString());
        this->items.append(Item(item_info["img_path"].toString(),item_info["name"].toString(),item_info["desp"].toString(),item_info["categ"].toString(),item_info["is_off"].toInt(),item_info["price"].toDouble(),item_info["off_price"].toDouble(),item_info["total_amount"].toInt(),item_info["amount"].toInt()));
    }
}

void ECPSys::save_accounts(){
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

void ECPSys::save_items(){
    QString filepath = QString(cfg.path()+"/items.json");
    QFile writefile(filepath);
    writefile.open(QIODevice::WriteOnly|QIODevice::Text);
    QJsonArray acc_info;
    for (int i=0;i<this->items.size();i++)
        acc_info.insert(i,this->items[i].toJson());
    QJsonObject info;
    info.insert("items",acc_info);
    QJsonDocument doc;
    doc.setObject(info);
    QByteArray writeable = doc.toJson(QJsonDocument::Indented);
    writefile.write(writeable);
    writefile.close();
}

void ECPSys::add_account(QString acc_id, QString name, QString password, QString phonenumber){
    this->accounts.append(Account(name,acc_id,password,phonenumber,QStringList()));
    save_accounts();
}

int ECPSys::log_in(QString acc_id, QString pwd){
    for (int i=0;i<accounts.size();i++)
        if (accounts[i].getString(QString("acc_id"))==acc_id){
            if (accounts[i].getString(QString("password"))==pwd){
                active_user=&accounts[i];
                return 1;
            }
            return 0;//pwd no match
        }
    return -1;//no such account
}

int ECPSys::log_out(){
    if (active_user==nullptr) return 0;
    active_user=nullptr;
    return 1;
}

QVector<int> ECPSys::search(QString kw){
    QVector<int> result;
    result.clear();
    QRegularExpression keyword(kw);
    keyword.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match;
    for (int i=0;i<this->items.size();i++){
        QString str=this->items[i].getStr(QString("name"))+this->items[i].getStr(QString("categ"))+this->items[i].getStr(QString("desp"));
        match=keyword.match(str);
        if (match.hasMatch()==true) result.append(i);
    }

    return result;
}

