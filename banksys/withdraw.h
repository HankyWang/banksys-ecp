#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <QDialog>
#include "prompt.h"
#include "banksys.h"

namespace Ui {
class Withdraw;
}

class Withdraw : public QDialog
{
    Q_OBJECT

public:
    explicit Withdraw(QWidget *parent = 0);
    void setValue(double balance);
    ~Withdraw();
signals:
    void success();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Withdraw *ui;
    Prompt * prompt;
};

#endif // WITHDRAW_H
