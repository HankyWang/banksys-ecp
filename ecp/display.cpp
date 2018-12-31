#include "display.h"
#include "ui_display.h"

extern ECPSys *sys;

Display::Display(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);
}

void Display::setTarget(Item *item){
    QImage img;
    img.load(sys->cfg.path()+item->getStr(QString("img_path")));
    qDebug()<<sys->cfg.path()+item->getStr(QString("img_path"));
    this->ui->img->setPixmap((QPixmap::fromImage(img).scaled(ui->img->size())));
    this->ui->desp->setText(item->getStr(QString("desp")));
    this->ui->name->setText(item->getStr(QString("name")));
    this->ui->amount->setText(QString::number(item->getInt(QString("amount"))));
    this->ui->categ->setText(item->getStr(QString("categ")));
    if (item->is_off){
        this->ui->price->setText(QString("¥ ")+QString::number(item->getDouble(QString("off_price")),'f',2));
        this->ui->former->setText(QString("¥ ")+QString::number(item->getDouble(QString("price")),'f',2));
        QFont *font=new QFont(this->ui->former->font());
        font->setStrikeOut(true);
        this->ui->former->setFont(*font);
    }
    else {
        this->ui->price->setText(QString("¥ ")+QString::number(item->getDouble(QString("price")),'f',2));
        this->ui->former->setVisible(false);
    }
    this->ui->num->setMaximum(item->getInt(QString("amount")));
}



Display::~Display()
{
    delete ui;
}

void Display::on_pushButton_2_clicked()
{
    this->setVisible(false);
}

void Display::on_pushButton_clicked()
{
    int num=this->ui->num->value();
    sys->shopItem.append(this->ui->name->text());
    sys->shopNum.append(num);
    sys->shopCateg.append(this->ui->categ->text());
    this->setVisible(false);
}
