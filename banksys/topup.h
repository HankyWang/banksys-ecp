#ifndef TOPUP_H
#define TOPUP_H

#include <QDialog>
#include "banksys.h"
#include "prompt.h"

namespace Ui {
class Topup;
}

class Topup : public QDialog
{
    Q_OBJECT

public:
    explicit Topup(QWidget *parent = 0);
    void setValue(double balance);
    ~Topup();
signals:
    void success();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Topup *ui;
    Prompt *prompt;
};

#endif // TOPUP_H
