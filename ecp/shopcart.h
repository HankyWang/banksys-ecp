#ifndef SHOPCART_H
#define SHOPCART_H

#include <QDialog>
#include "ecpsys.h"
#include "prompt.h"
#include "pay.h"

namespace Ui {
class Shopcart;
}

class Shopcart : public QDialog
{
    Q_OBJECT

public:
    explicit Shopcart(QWidget *parent = 0);
    void ready();
    ~Shopcart();

private slots:
    void on_pushButton_clicked();
    void modItem();

    void on_cancel_clicked();

signals:
    void success();

private:
    Ui::Shopcart *ui;
    Prompt *prompt;
    Pay *pay;
    QList<int> clicked;
};

#endif // SHOPCART_H
