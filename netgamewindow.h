#ifndef NETGAMEWINDOW_H
#define NETGAMEWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <cstdlib>
#include <QtNetwork>
#include <QString>
#include <stack>
#include <QMessageBox>
#include "mainwindow.h"
#include "winwindow.h"

namespace Ui {
class NetGameWindow;
}

enum NCHESS
{
    NEMPTY,
    NBLACK,
    NWHITE
};

class NetGameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetGameWindow(QWidget *parent = nullptr);
    ~NetGameWindow();

private slots:
    void ipPortSetting();
    void CancelConnect();
    void RecvMsg();
    void GameExit();
    void ChooseRole();

private:
    Ui::NetGameWindow *ui;
    NCHESS chessboard[15][15];//棋盘
    NCHESS PlayerRole;//角色
    NCHESS WINROLE;
    const int BORDER = 50;
    const int WIDTH = 40;
    int clickPosX, clickPosY;//坐标
    int LocalPort, GuestPort;//本机端口号，对方端口号
    bool IsSetPort;//是否设置端口号
    bool IsChooseRole;//是否选择角色
    QUdpSocket *udpSocket;
    int ChessCount;//记录下棋数量
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void CheckWin(int, int);
    void _DealWin(int, int);
};

#endif // NETGAMEWINDOW_H
