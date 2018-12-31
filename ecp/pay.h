#ifndef PAY_H
#define PAY_H

#include <QDialog>
#include "prompt.h"
#include "ecpsys.h"
#include <QtNetwork>
#include <QTimer>

namespace Ui {
class Pay;
}

class Pay : public QDialog
{
    Q_OBJECT

public:
    explicit Pay(QWidget *parent = 0);
    void setPrice(double b,double g,double off,double price);
    ~Pay();
signals:
    void success();
private slots:
    void on_pushButton_clicked();
    void parse();
    void timeout();


    void on_pushButton_2_clicked();

private:
    Ui::Pay *ui;
    Prompt *prompt;
    QTcpSocket *tcpSocket;
    QTimer *tcpTimer;
};

#endif // PAY_H
