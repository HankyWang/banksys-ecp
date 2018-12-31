#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "prompt.h"
#include "ecpsys.h"
#include "display.h"
#include "shopcart.h"
#include "signup.h"
#include "bind.h"
#include <QtNetwork>
#include <QTableWidget>

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
    void itemDoubleClicked(int row,int column);

    void searchresultDoubleClicked(int row,int column);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_logout_clicked();

    void on_shopcart_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_bind_clicked();


    void on_actionlogin_triggered();

    void on_actionlogout_triggered();

    void on_actionsignup_triggered();

    void on_actionpurchase_triggered();

private:
    Ui::MainWindow *ui;
    Prompt *prompt;
    ECPSys *mysys;
    Display *display;
    Signup *signup;
    void setItems();
    QVector<QTableWidget*> tablelist;
    Shopcart *shopcart;
    QVector<int> search_inds;
    Bind *bind;
};

#endif // MAINWINDOW_H
