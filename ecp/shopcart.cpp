#include "shopcart.h"
#include "ui_shopcart.h"

extern ECPSys *sys;

Shopcart::Shopcart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Shopcart)
{
    ui->setupUi(this);
    this->ui->table->setColumnWidth(0,30);
    this->ui->table->setColumnWidth(1,140);
    this->ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    prompt = new Prompt(this);
    prompt->setVisible(false);
    pay = new Pay(this);
    pay->setVisible(false);
    clicked.clear();
    connect(this->pay,SIGNAL(success()),this,SLOT(modItem()));
}

Shopcart::~Shopcart()
{
    delete ui;
}

void Shopcart::modItem(){
    QList<int> inds = this->clicked;
    for (int i=0;i<inds.size();i++){
        for (int j=0;j<sys->items.size();j++)
            if (sys->items[j].getStr(QString("name"))==sys->shopItem[inds[i]]){
                sys->items[j].purchase(sys->shopNum[inds[i]]);
                break;
            }
    }
    for (int i=0;i<inds.size();i++){
        sys->shopCateg.removeAt(inds[i]);
        sys->shopItem.removeAt(inds[i]);
        sys->shopNum.removeAt(inds[i]);
    }
    emit success();
    sys->save_items();
    this->ready();
}

void Shopcart::ready(){
    QTableWidgetItem *unit;
    this->ui->table->setRowCount(sys->shopItem.size());
    for (int i=0;i<sys->shopItem.size();i++){
        Item *item=nullptr;
      for (int j=0;j<sys->items.size();j++)
            if (sys->items[j].getStr(QString("name"))==sys->shopItem[i]){
                item=&(sys->items[j]);
                break;
               }
        QString name=item->getStr(QString("name"));
        int amount=sys->shopNum[i];
        double price;
        if (item->is_off) price=item->getDouble(QString("off_price"));
        else price=item->getDouble(QString("price"));
        double total_price=price*amount;
        unit = new QTableWidgetItem(name);
        this->ui->table->setItem(i,1,unit);
        unit = new QTableWidgetItem(QString::number(amount));
        this->ui->table->setItem(i,2,unit);
        unit = new QTableWidgetItem(QString::number(price,'f',2));
        this->ui->table->setItem(i,3,unit);
        unit = new QTableWidgetItem(QString::number(total_price,'f',2));
        this->ui->table->setItem(i,4,unit);
        unit = new QTableWidgetItem(QString(" "));
        unit->setCheckState(Qt::Unchecked);
        this->ui->table->setItem(i,0,unit);
    }
}



void Shopcart::on_pushButton_clicked()
{
    if (sys->active_user->getCards().size()==0){
        prompt->setText(QString("无可用银行卡，请先绑定。"));
        prompt->setVisible(true);
    } else if (sys->shopItem.size()==0){
        prompt->setText(QString("购物车为空，不如去逛逛"));
        prompt->setVisible(true);
    }
    else{
        double total=0;
        double off_total=0;
        double book_off=0;
        double gar_off=0;
        QTableWidgetItem *unit;
        QStringList categset;
        QVector<int> amount;
        QVector<double> cost;
        for (int i=0;i<sys->shopItem.size();i++){
            unit = this->ui->table->item(i,0);
            if (unit->checkState()==2){
                clicked.append(i);
                if (categset.contains(sys->shopCateg[i])){
                    int ind=categset.indexOf(sys->shopCateg[i]);
                    amount[ind]+=this->ui->table->item(i,2)->text().toInt();
                    cost[ind]+=this->ui->table->item(i,4)->text().toDouble();
                }
                else {
                    categset.append(sys->shopCateg[i]);
                    amount.append(this->ui->table->item(i,2)->text().toInt());
                    cost.append(this->ui->table->item(i,4)->text().toDouble());
                }
            }
        }
        if (clicked.size()==0){
            prompt->setText(QString("请选择商品"));
            prompt->setVisible(true);
            return;
        }
        for (int i=0;i<categset.size();i++)
            if (categset[i]==QString("book")&&amount[i]>=3){
                total+=cost[i];
                off_total+=cost[i]*0.7;
                book_off=cost[i]*0.3;
            }
            else if (categset[i]==QString("garment")){
                total+=cost[i];
                gar_off=(int(cost[i])/200)*20;
                off_total+=cost[i]-gar_off;
            }
            else {
                off_total+=cost[i];
                total+=cost[i];
            }
        pay->setPrice(book_off,gar_off,off_total,total);
        pay->setVisible(true);
    }
}

void Shopcart::on_cancel_clicked()
{
    this->setVisible(false);
}
