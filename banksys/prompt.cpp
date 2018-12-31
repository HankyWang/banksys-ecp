#include "prompt.h"
#include "ui_prompt.h"

Prompt::Prompt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prompt)
{
    ui->setupUi(this);
    this->ui->lineEdit->setVisible(false);
}

Prompt::~Prompt()
{
    delete ui;
}

void Prompt::setText(QString t){
    this->ui->label->setText(t);
}
void Prompt::setLine(QString t){
    this->ui->lineEdit->setText(t);
    this->ui->lineEdit->setReadOnly(true);
    this->ui->lineEdit->setVisible(true);
}

void Prompt::on_pushButton_clicked()
{
    this->setVisible(false);
    this->ui->lineEdit->setVisible(false);
}
