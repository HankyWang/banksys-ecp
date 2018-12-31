#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "banksys.h"
#include "signup.h"
#include "login.h"
#include "prompt.h"
#include "topup.h"
#include "withdraw.h"
#include "chpwd.h"
#include <QtNetwork>
#include <QImage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLog_In_triggered();

    void on_actionSign_Up_triggered();

    void on_actionLog_Out_triggered();

    void chglog();

    void connected();

    void parseData();

    void on_actionTop_up_triggered();

    void on_actionAccount_Setting_triggered();

    void on_actionWithdraw_triggered();

    void on_chpwdbtn_clicked();

    void on_loginbtn_clicked();

    void on_logoutbtn_clicked();

    void on_signupbtn_clicked();

    void on_topupbtn_clicked();

    void on_withdrawbtn_clicked();

    void ch_display();

private:
    Ui::MainWindow *ui;
    BankSys *bank;
    Signup * signup;
    Login * login;
    Prompt * prompt;
    Topup *topup;
    Chpwd *chpwd;
    Withdraw *withdraw;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
};

#endif // MAINWINDOW_H
