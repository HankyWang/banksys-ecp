#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include "prompt.h"
#include <QRegularExpression>
#include "ecpsys.h"

namespace Ui {
class Signup;
}

class Signup : public QDialog
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = 0);
    ~Signup();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Signup *ui;
    Prompt *prompt;
};

#endif // SIGNUP_H
