#ifndef DISPLAY_H
#define DISPLAY_H

#include <QDialog>
#include "ecpsys.h"

namespace Ui {
class Display;
}

class Display : public QDialog
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();
    void setTarget(Item *item);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Display *ui;
};

#endif // DISPLAY_H
