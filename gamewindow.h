#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <cstdlib>
#include <stack>
#include "winwindow.h"
#include "mainwindow.h"

namespace Ui {
class GameWindow;
}

enum CHESS
{
    EMPTY,
    BLACK,
    WHITE
};

struct ChessPos
{
    int _x, _y;
    CHESS _chess;
    ChessPos(int _xx, int _yy, CHESS _cchess) : _x(_xx), _y(_yy), _chess(_cchess) {}
};

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void GameExit();
    void RegretChess();
    void CheckWin(int, int);
    void DealWin(int, int);

private:
    Ui::GameWindow *ui;
    CHESS chessboard[15][15];//棋盘
    CHESS PlayerRole;//角色
    CHESS WINROLE;//获胜角色
    std::stack<ChessPos> GameReplay;//游戏记录，以便悔棋
    const int BORDER = 50;
    const int WIDTH = 40;
    int clickPosX, clickPosY;//坐标
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // GAMEWINDOW_H
