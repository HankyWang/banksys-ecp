#ifndef CHPWD_H
#define CHPWD_H

#include <QDialog>
#include "banksys.h"
#include "prompt.h"

namespace Ui {
class Chpwd;
}

class Chpwd : public QDialog
{
    Q_OBJECT

public:
    explicit Chpwd(QWidget *parent = 0);
    ~Chpwd();


private slots:
    void on_pushButton_2_clicked();

private:
    Ui::Chpwd *ui;
    Prompt *prompt;
};

#endif // CHPWD_H
