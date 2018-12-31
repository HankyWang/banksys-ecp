#ifndef BIND_H
#define BIND_H

#include <QDialog>
#include "prompt.h"
#include "ecpsys.h"
#include <QtNetwork>
#include <QTimer>

namespace Ui {
class Bind;
}

class Bind : public QDialog
{
    Q_OBJECT

public:
    explicit Bind(QWidget *parent = 0);
    ~Bind();

private slots:
    void parse();
    void timeout();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Bind *ui;
    Prompt *prompt;
    QTcpSocket *tcpSocket;
    QTimer *tcpTimer;
    QString acc_id,password;
};

#endif // BIND_H
