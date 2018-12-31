#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include "banksys.h"
#include <QRegularExpression>
#include "prompt.h"

namespace Ui {
class Signup;
}

class Signup : public QDialog
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = 0);
    ~Signup();

signals:
    void success();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Signup *ui;
    Prompt *prompt;
};

#endif // SIGNUP_H
