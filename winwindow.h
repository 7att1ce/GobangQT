#ifndef WINWINDOW_H
#define WINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "mainwindow.h"

namespace Ui {
class WinWindow;
}

class WinWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WinWindow(QWidget *parent = nullptr);
    ~WinWindow();
    void SetWinPlayer(QString);

private slots:
    void HomeMenu();

private:
    Ui::WinWindow *ui;
};

#endif // WINWINDOW_H
