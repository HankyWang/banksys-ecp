#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "banksys.h"
#include "prompt.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

signals:
        void login_s();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Login *ui;
    Prompt *prompt;
    int remain_time=2;

};

#endif // LOGIN_H
