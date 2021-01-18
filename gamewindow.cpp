#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    PlayerRole = BLACK;
    WINROLE = EMPTY;
    memset(chessboard, EMPTY, sizeof(chessboard));//初始化棋盘
    ui->setupUi(this);
    centralWidget()->setMouseTracking(true);//启用鼠标支持，QMainWindow需要多设置这一步
    setMouseTracking(true);//启用鼠标支持
    update();
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::GameExit()//临时退出游戏
{
    this->close();
    MainWindow *main = new MainWindow();
    main->show();
}

void GameWindow::paintEvent(QPaintEvent *)//绘制游戏页面
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);//抗锯齿

    //绘制棋盘
    for (int i = 0; i < 15; ++i)
    {
        painter.drawLine(BORDER, BORDER + WIDTH * i, BORDER + WIDTH * 14, BORDER + WIDTH * i);
        painter.drawLine(BORDER + WIDTH * i, BORDER, BORDER + WIDTH * i, BORDER + WIDTH * 14);
    }

    //绘制五个实心点
    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(BORDER + WIDTH * 7 - 5, BORDER + WIDTH * 7 - 5, 10, 10);
    painter.drawEllipse(BORDER + WIDTH * 3 - 5, BORDER + WIDTH * 3 - 5, 10, 10);
    painter.drawEllipse(BORDER + WIDTH * 11 - 5, BORDER + WIDTH * 11 - 5, 10, 10);
    painter.drawEllipse(BORDER + WIDTH * 3 - 5, BORDER + WIDTH * 11 - 5, 10, 10);
    painter.drawEllipse(BORDER + WIDTH * 11 - 5, BORDER + WIDTH * 3 - 5, 10, 10);

    //绘制鼠标落点
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if (PlayerRole == BLACK)
        brush.setColor(Qt::black);
    else if (PlayerRole == WHITE)
        brush.setColor(Qt::white);
    painter.setBrush(brush);
    if (clickPosX >= 0 && clickPosX <= 14 && clickPosY >= 0 && clickPosY <= 14)
        painter.drawEllipse(BORDER + WIDTH * clickPosX - 15, BORDER + WIDTH * clickPosY - 15, 30, 30);

    //绘制棋子
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            if (chessboard[i][j] == BLACK)
            {
                painter.setBrush(QBrush(Qt::black));
                painter.drawEllipse(BORDER + WIDTH * i - 15, BORDER + WIDTH * j - 15, 30, 30);
            }
            else if (chessboard[i][j] == WHITE)
            {
                painter.setBrush(QBrush(Qt::white));
                painter.drawEllipse(BORDER + WIDTH * i - 15, BORDER + WIDTH * j - 15, 30, 30);
            }
        }
    }
}

void GameWindow::mouseMoveEvent(QMouseEvent *event)//重写鼠标移动事件
{
    //获得模糊坐标
    int PosX = (event->x() - BORDER + WIDTH / 2) / WIDTH;
    int PosY = (event->y() - BORDER + WIDTH / 2) / WIDTH;

    //如果位置与上一次相同则不操作
    if(clickPosX == PosX && clickPosY == PosY)
        return;

    clickPosX = PosX;
    clickPosY = PosY;
    update();
}

void GameWindow::mouseReleaseEvent(QMouseEvent *)//重写鼠标单击事件
{  
    if (clickPosX >= 0 && clickPosX <= 14 && clickPosY >= 0 && clickPosY <= 14)//判断坐标是否越界
    {
        if (chessboard[clickPosX][clickPosY] != EMPTY)
            return;
        chessboard[clickPosX][clickPosY] = PlayerRole;
        GameReplay.push(ChessPos(clickPosX, clickPosY, chessboard[clickPosX][clickPosY]));
        DealWin(clickPosX, clickPosY);
        if (PlayerRole == BLACK)
            PlayerRole = WHITE;
        else if (PlayerRole == WHITE)
            PlayerRole = BLACK;
    }
    update();
}

void GameWindow::RegretChess()//悔棋操作
{
    if (!GameReplay.empty())
    {
        ChessPos _tmp = GameReplay.top();
        GameReplay.pop();
        chessboard[_tmp._x][_tmp._y] = EMPTY;
        if (PlayerRole == BLACK)
            PlayerRole = WHITE;
        else if (PlayerRole == WHITE)
            PlayerRole = BLACK;
    }
    update();
}

void GameWindow::CheckWin(int x, int y)
{
    int i, j, k, dx, dy, count;
    CHESS flag;
    flag = chessboard[x][y];  //获得方块属性

    for(k = 1; k <= 4; k++)
    {
        switch(k)
        {
        case 1: dx = 1;  dy = 0; break;//横向
        case 2: dx = 0;  dy = 1; break;//纵向
        case 3: dx = 1;  dy = 1; break;//右下斜
        case 4: dx = -1; dy = 1; break;//左下斜
        }
        count = 0;
        for(i = x - 4 * dx, j = y - 4 * dy;
            j <= y + 4 * dy && (dx == -1 ? i >= x + 4 * dx : i <= x + 4 * dx);
            i += dx, j += dy)
        {
            if(i < 0 || i >= 15 || j < 0 || j >= 15)
                continue;
            if(chessboard[i][j] == flag)
            {
                ++count;
                if(count == 5)
                {
                    WINROLE = flag;
                    return;
                }
            }
            else
            {
                count = 0;
            }
        }
    }
    return;
}

void GameWindow::DealWin(int x, int y)
{
    CheckWin(x, y);
    WinWindow *win = new WinWindow;
    if (WINROLE == BLACK)
    {
        this->close();
        win->SetWinPlayer("Black Win!");
        win->show();
    }
    else if (WINROLE == WHITE)
    {
        this->close();
        win->SetWinPlayer("White Win!");
        win->show();
    }
    else if (GameReplay.size() == 225 && WINROLE == EMPTY)
    {
        this->close();
        win->SetWinPlayer("Draw!");
        win->show();
    }
}
