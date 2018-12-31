#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QWidget>

namespace Ui {
class secondWindow;
}

class secondWindow : public QWidget
{
    Q_OBJECT

public:
    explicit secondWindow(QWidget *parent = 0);
    ~secondWindow();

private:
    Ui::secondWindow *ui;
};

#endif // SECONDWINDOW_H
