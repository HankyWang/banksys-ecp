#include "prompt.h"
#include "ui_prompt.h"

Prompt::Prompt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prompt)
{
    ui->setupUi(this);
}

Prompt::~Prompt()
{
    delete ui;
}

void Prompt::setText(QString t){
    this->ui->label->setText(t);
}

void Prompt::on_pushButton_clicked()
{
    this->setVisible(false);
}

