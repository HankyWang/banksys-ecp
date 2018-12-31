#include "mainwindow.h"
#include "ui_mainwindow.h"

ECPSys *sys;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->mysys = new ECPSys();
    sys=mysys;
    this->mysys->accounts_fromfile(QString("accounts.json"));
    this->mysys->items_fromfile(QString("items.json"));
    ui->setupUi(this);
    tablelist.clear();
    this->ui->pad->setVisible(false);
    QImage frontpage;
    frontpage.load("frontpage.jpg");
    this->ui->frontpage->setPixmap((QPixmap::fromImage(frontpage).scaled(ui->frontpage->size())));
    this->ui->frontpage->setVisible(true);
    this->setItems();
    this->search_inds.clear();
    prompt = new Prompt(this);
    prompt->setVisible(false);
    for (int i=0;i<this->tablelist.size();i++)
        connect(this->tablelist[i],SIGNAL(cellDoubleClicked(int,int)),this,SLOT(itemDoubleClicked(int,int)));
    connect(this->ui->result,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(searchresultDoubleClicked(int,int)));
    display = new Display(this);
    display->setVisible(false);
    shopcart = new Shopcart(this);
    shopcart->setVisible(false);
    connect(shopcart,SIGNAL(success()),this,SLOT(on_pushButton_3_clicked()));
    bind = new Bind(this);
    bind->setVisible(false);
    this->ui->result->setSelectionBehavior ( QAbstractItemView::SelectRows);
    if (sys->active_user) {
        this->ui->pushButton->setVisible(false);
        this->ui->pushButton_2->setVisible(false);
        this->ui->label->setVisible(false);
        this->ui->label_2->setVisible(false);
        this->ui->password->setVisible(false);
        this->ui->account->setVisible(false);

        this->ui->gar->setVisible(true);
        this->ui->book->setVisible(true);
        this->ui->welcome->setVisible(true);
        this->ui->shopcart->setVisible(true);
        this->ui->logout->setVisible(true);
        this->ui->bind->setVisible(true);
        this->ui->welcome->setText(QString("欢迎您，")+sys->active_user->getString(QString("acc_id")));
    }
    else{
        this->ui->welcome->setVisible(false);
        this->ui->shopcart->setVisible(false);
        this->ui->logout->setVisible(false);
        this->ui->bind->setVisible(false);
        this->ui->gar->setVisible(false);
        this->ui->book->setVisible(false);

        this->ui->pushButton->setVisible(true);
        this->ui->pushButton_2->setVisible(true);
        this->ui->label->setVisible(true);
        this->ui->label_2->setVisible(true);
        this->ui->password->setVisible(true);
        this->ui->account->setVisible(true);
    }
    signup = new Signup(this);
    signup->setVisible(false);
    this->ui->result->setColumnWidth(0,112);
    this->ui->result->setColumnWidth(4,90);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::itemDoubleClicked(int row, int column){
    QString categ_name=sys->categs.at(this->ui->display->currentIndex());
    QVector<Item*> categ;
    for (int i=0;i<sys->items.size();i++)
        if (sys->items[i].getStr((QString("categ")))==categ_name)
                categ.append(&(sys->items[i]));
    if (column<7){
        display->setTarget(categ[row]);
        display->setVisible(true);
    }
}

void MainWindow::searchresultDoubleClicked(int row, int column){
    QVector<int> inds=this->search_inds;
    if (column<6){
        display->setTarget(&sys->items[inds[row]]);
        display->setVisible(true);
    }
}

void MainWindow::setItems(){
    int tabcount=this->ui->display->count();
    int tabsize=mysys->categs.size();
    QTableWidget *tableitem=this->ui->table;
    QWidget *tabitem;
    QTableWidgetItem *item;
    int w=tableitem->width(),h=tableitem->height();
    QVector<Item> categ;
    tablelist.append(this->ui->table);
    for (int i=0;i<tabcount;i++){
        this->ui->display->setTabText(i,sys->categs.at(i));
        categ.clear();
        for (int j=0;j<sys->items.size();j++)
            if (sys->items[j].getStr(QString("categ"))==sys->categs.at(i))
                categ.append(sys->items[j]);
        this->ui->table->setColumnWidth(1,270);
        this->ui->table->setColumnWidth(2,128);
        this->ui->table->setSelectionBehavior ( QAbstractItemView::SelectRows);
        this->ui->table->setRowCount(categ.size());
        for (int i=0;i<categ.size();i++){
            item=new QTableWidgetItem(categ[i].getStr(QString("name")));
            this->ui->table->setItem(i,0,item);
            item=new QTableWidgetItem(categ[i].getStr(QString("desp")));
            this->ui->table->setItem(i,1,item);
            QString pricetag;
            if (categ[i].is_off)
                pricetag="¥"+QString::number(categ[i].getDouble(QString("off_price")),'f',2)+"/¥"+QString::number(categ[i].getDouble(QString("price")),'f',2);
            else pricetag="¥"+QString::number(categ[i].getDouble(QString("price")));
            item=new QTableWidgetItem(pricetag);
            this->ui->table->setItem(i,2,item);
        }
    }
    for (int i=tabcount;i<tabsize;i++){
        tabitem = new QWidget(this->ui->display->currentWidget());
        tableitem = new QTableWidget(tabitem);
        tableitem->resize(w,h);
        tableitem->setColumnCount(3);
        QStringList headers;
        headers.append(QString("商品名称"));
        headers.append(QString("商品描述"));
        headers.append(QString("商品价格"));
        tableitem->setHorizontalHeaderLabels(headers);
        tableitem->setRowCount(2);
        this->ui->display->addTab(tabitem,sys->categs.at(i));
        tablelist.append(tableitem);
        categ.clear();
        for (int j=0;j<sys->items.size();j++)
            if (sys->items[j].getStr(QString("categ"))==sys->categs.at(i))
                categ.append(sys->items[j]);
        tableitem->setColumnWidth(1,270);
        tableitem->setColumnWidth(2,128);
        tableitem->setSelectionBehavior ( QAbstractItemView::SelectRows);
        tableitem->setRowCount(categ.size());
        tableitem->setEditTriggers(QAbstractItemView::NoEditTriggers);
        for (int i=0;i<categ.size();i++){
            item=new QTableWidgetItem(categ[i].getStr(QString("name")));
            tableitem->setItem(i,0,item);
            item=new QTableWidgetItem(categ[i].getStr(QString("desp")));
            tableitem->setItem(i,1,item);
            QString pricetag;
            if (categ[i].is_off)
                pricetag="¥"+QString::number(categ[i].getDouble(QString("off_price")),'f',2)+"/¥"+QString::number(categ[i].getDouble(QString("price")),'f',2);
            else pricetag="¥"+QString::number(categ[i].getDouble(QString("price")));
            item=new QTableWidgetItem(pricetag);
            tableitem->setItem(i,2,item);
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    int flag=sys->log_in(this->ui->account->text(),this->ui->password->text());
    if (flag==1) prompt->setText(QString("登陆成功"));
    else if (flag==0) prompt->setText(QString("错误的用户名或密码"));
    else if (flag==-1) prompt->setText(QString("找不到用户，请先注册"));
    prompt->setVisible(true);
    if (sys->active_user) {
        this->ui->pushButton->setVisible(false);
        this->ui->pushButton_2->setVisible(false);
        this->ui->label->setVisible(false);
        this->ui->label_2->setVisible(false);
        this->ui->password->setVisible(false);
        this->ui->account->setVisible(false);

        this->ui->gar->setVisible(true);
        this->ui->book->setVisible(true);
        this->ui->welcome->setVisible(true);
        this->ui->shopcart->setVisible(true);
        this->ui->logout->setVisible(true);
        this->ui->bind->setVisible(true);
        this->ui->welcome->setText(QString("欢迎您，")+sys->active_user->getString(QString("acc_id")));
    }
    else{
        this->ui->welcome->setVisible(false);
        this->ui->shopcart->setVisible(false);
        this->ui->logout->setVisible(false);
        this->ui->bind->setVisible(false);
        this->ui->gar->setVisible(false);
        this->ui->book->setVisible(false);

        this->ui->pushButton->setVisible(true);
        this->ui->pushButton_2->setVisible(true);
        this->ui->label->setVisible(true);
        this->ui->label_2->setVisible(true);
        this->ui->password->setVisible(true);
        this->ui->account->setVisible(true);
    }
}

void MainWindow::on_pushButton_clicked()
{
    signup->setVisible(true);
}
void MainWindow::on_logout_clicked()
{
    sys->active_user=NULL;
    this->ui->account->setText(QString());
    this->ui->password->setText(QString());
    if (sys->active_user) {
        this->ui->pushButton->setVisible(false);
        this->ui->pushButton_2->setVisible(false);
        this->ui->label->setVisible(false);
        this->ui->label_2->setVisible(false);
        this->ui->password->setVisible(false);
        this->ui->account->setVisible(false);

        this->ui->gar->setVisible(true);
        this->ui->book->setVisible(true);
        this->ui->welcome->setVisible(true);
        this->ui->shopcart->setVisible(true);
        this->ui->logout->setVisible(true);
        this->ui->bind->setVisible(true);
        this->ui->welcome->setText(QString("欢迎您，")+sys->active_user->getString(QString("acc_id")));
    }
    else{
        this->ui->welcome->setVisible(false);
        this->ui->shopcart->setVisible(false);
        this->ui->logout->setVisible(false);
        this->ui->bind->setVisible(false);
        this->ui->gar->setVisible(false);
        this->ui->book->setVisible(false);

        this->ui->pushButton->setVisible(true);
        this->ui->pushButton_2->setVisible(true);
        this->ui->label->setVisible(true);
        this->ui->label_2->setVisible(true);
        this->ui->password->setVisible(true);
        this->ui->account->setVisible(true);
    }
}

void MainWindow::on_shopcart_clicked()
{
    shopcart->ready();
    this->shopcart->setVisible(true);
}

void MainWindow::on_pushButton_3_clicked()
{
    this->ui->pad->setVisible(true);
    this->ui->frontpage->setVisible(false);
    this->ui->table->setVisible(false);
    this->ui->result->setVisible(true);
    QString kw=this->ui->searchbar->text();
    QVector<int> inds=sys->search(kw);
    this->search_inds=inds;
    QTableWidgetItem *unit;
    if (inds.size()==0){
        this->ui->result->setRowCount(1);
        unit=new QTableWidgetItem(QString("无匹配结果"));
        this->ui->result->setItem(0,0,unit);
        return;
    }
    this->ui->result->setRowCount(inds.size());
    for (int i=0;i<inds.size();i++){
        unit=new QTableWidgetItem(sys->items[inds[i]].getStr(QString("name")));
        this->ui->result->setItem(i,0,unit);
        unit=new QTableWidgetItem(sys->items[inds[i]].getStr(QString("categ")));
        this->ui->result->setItem(i,1,unit);
        unit=new QTableWidgetItem(QString::number(sys->items[inds[i]].getInt(QString("amount"))));
        this->ui->result->setItem(i,2,unit);
        if (sys->items[inds[i]].is_off){
            unit=new QTableWidgetItem("¥"+QString::number(sys->items[inds[i]].getDouble(QString("price")),'f',2));
            this->ui->result->setItem(i,3,unit);
            unit=new QTableWidgetItem("¥"+QString::number(sys->items[inds[i]].getDouble(QString("off_price")),'f',2));
            this->ui->result->setItem(i,4,unit);
        }else{
            unit=new QTableWidgetItem(QString("-"));
            this->ui->result->setItem(i,3,unit);
            unit=new QTableWidgetItem(QString::number(sys->items[inds[i]].getDouble(QString("price")),'f',2));
            this->ui->result->setItem(i,4,unit);
        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    this->ui->pad->setVisible(true);
    this->ui->frontpage->setVisible(false);
    this->ui->table->setVisible(true);
    this->ui->result->setVisible(false);
}

void MainWindow::on_bind_clicked()
{
    this->bind->setVisible(true);
}

void MainWindow::on_actionlogin_triggered()
{
    int flag=sys->log_in(this->ui->account->text(),this->ui->password->text());
    if (flag==1) prompt->setText(QString("登陆成功"));
    else if (flag==0) prompt->setText(QString("错误的用户名或密码"));
    else if (flag==-1) prompt->setText(QString("找不到用户，请先注册"));
    prompt->setVisible(true);
    if (sys->active_user) {
        this->ui->pushButton->setVisible(false);
        this->ui->pushButton_2->setVisible(false);
        this->ui->label->setVisible(false);
        this->ui->label_2->setVisible(false);
        this->ui->password->setVisible(false);
        this->ui->account->setVisible(false);

        this->ui->gar->setVisible(true);
        this->ui->book->setVisible(true);
        this->ui->welcome->setVisible(true);
        this->ui->shopcart->setVisible(true);
        this->ui->logout->setVisible(true);
        this->ui->bind->setVisible(true);
        this->ui->welcome->setText(QString("欢迎您，")+sys->active_user->getString(QString("acc_id")));
    }
    else{
        this->ui->welcome->setVisible(false);
        this->ui->shopcart->setVisible(false);
        this->ui->logout->setVisible(false);
        this->ui->bind->setVisible(false);
        this->ui->gar->setVisible(false);
        this->ui->book->setVisible(false);

        this->ui->pushButton->setVisible(true);
        this->ui->pushButton_2->setVisible(true);
        this->ui->label->setVisible(true);
        this->ui->label_2->setVisible(true);
        this->ui->password->setVisible(true);
        this->ui->account->setVisible(true);
    }
}

void MainWindow::on_actionlogout_triggered()
{
    sys->active_user=NULL;
    this->ui->account->setText(QString());
    this->ui->password->setText(QString());
    if (sys->active_user) {
        this->ui->pushButton->setVisible(false);
        this->ui->pushButton_2->setVisible(false);
        this->ui->label->setVisible(false);
        this->ui->label_2->setVisible(false);
        this->ui->password->setVisible(false);
        this->ui->account->setVisible(false);

        this->ui->gar->setVisible(true);
        this->ui->book->setVisible(true);
        this->ui->welcome->setVisible(true);
        this->ui->shopcart->setVisible(true);
        this->ui->logout->setVisible(true);
        this->ui->bind->setVisible(true);
        this->ui->welcome->setText(QString("欢迎您，")+sys->active_user->getString(QString("acc_id")));
    }
    else{
        this->ui->welcome->setVisible(false);
        this->ui->shopcart->setVisible(false);
        this->ui->logout->setVisible(false);
        this->ui->bind->setVisible(false);
        this->ui->gar->setVisible(false);
        this->ui->book->setVisible(false);

        this->ui->pushButton->setVisible(true);
        this->ui->pushButton_2->setVisible(true);
        this->ui->label->setVisible(true);
        this->ui->label_2->setVisible(true);
        this->ui->password->setVisible(true);
        this->ui->account->setVisible(true);
    }
}

void MainWindow::on_actionsignup_triggered()
{
    signup->setVisible(true);
}

void MainWindow::on_actionpurchase_triggered()
{
    this->ui->pad->setVisible(true);
    this->ui->frontpage->setVisible(false);
    this->ui->table->setVisible(true);
    this->ui->result->setVisible(false);
}
