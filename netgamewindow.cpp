#include "netgamewindow.h"
#include "ui_netgamewindow.h"

NetGameWindow::NetGameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetGameWindow)
{
    udpSocket = new QUdpSocket;
    IsSetPort = false;
    IsChooseRole = false;
    PlayerRole = NEMPTY;
    WINROLE = NEMPTY;
    ChessCount = 0;
    memset(chessboard, NEMPTY, sizeof(chessboard));//初始化棋盘
    ui->setupUi(this);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
    centralWidget()->setMouseTracking(true);//启用鼠标支持，QMainWindow需要多设置这一步
    setMouseTracking(true);//启用鼠标支持
    update();
}

NetGameWindow::~NetGameWindow()
{
    delete ui;
    delete udpSocket;
}

void NetGameWindow::paintEvent(QPaintEvent *)
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
    if (PlayerRole == NBLACK)
        brush.setColor(Qt::black);
    else if (PlayerRole == NWHITE)
        brush.setColor(Qt::white);
    painter.setBrush(brush);
    if (clickPosX >= 0 && clickPosX <= 14 && clickPosY >= 0 && clickPosY <= 14)
        painter.drawEllipse(BORDER + WIDTH * clickPosX - 15, BORDER + WIDTH * clickPosY - 15, 30, 30);

    //绘制棋子
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            if (chessboard[i][j] == NBLACK)
            {
                painter.setBrush(QBrush(Qt::black));
                painter.drawEllipse(BORDER + WIDTH * i - 15, BORDER + WIDTH * j - 15, 30, 30);
            }
            else if (chessboard[i][j] == NWHITE)
            {
                painter.setBrush(QBrush(Qt::white));
                painter.drawEllipse(BORDER + WIDTH * i - 15, BORDER + WIDTH * j - 15, 30, 30);
            }
        }
    }
}

void NetGameWindow::mouseMoveEvent(QMouseEvent *event)//重写鼠标移动事件
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

void NetGameWindow::mouseReleaseEvent(QMouseEvent *)
{
    if (IsSetPort == false)
    {
        QMessageBox::information(this, "Info", "Please Set Ports");
        return;
    }
    else if (IsChooseRole == false)
    {
        QMessageBox::information(this, "Info", "Please Choose Role!");
        return;
    }
    if (clickPosX >= 0 && clickPosX <= 14 && clickPosY >= 0 && clickPosY <= 14)//判断坐标是否越界
    {
        if (chessboard[clickPosX][clickPosY] != NEMPTY)
            return;
        chessboard[clickPosX][clickPosY] = PlayerRole;
        QString SocketMsg = QString::number(clickPosX, 10).append(" ").append(QString::number(clickPosY, 10)).append(" ").append(QString::number(PlayerRole, 10));
        udpSocket->writeDatagram(SocketMsg.toLatin1().data(), SocketMsg.size(), QHostAddress("127.0.0.1"), GuestPort);
        ++ChessCount;
        _DealWin(clickPosX, clickPosY);
    }
    update();
}

void NetGameWindow::ChooseRole()
{
    QMessageBox::StandardButton result = QMessageBox::question(this, "Choose", "Choose Role(Yes/Black, No/White)", QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes)
        PlayerRole = NBLACK;
    else if (result == QMessageBox::No)
        PlayerRole = NWHITE;
    ui->pushButton_4->setDisabled(true);
    IsChooseRole = true;
}

void NetGameWindow::ipPortSetting()
{
    bool ErrFlag = false;
    LocalPort = ui->lineEdit->text().toInt();
    GuestPort = ui->lineEdit_2->text().toInt();
    bool result = udpSocket->bind(LocalPort);
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox::information(this, "Error", "Please Input Port!");
        ErrFlag = true;
    }
    else if (LocalPort == GuestPort || LocalPort < 1 || LocalPort > 65535 || GuestPort < 1 || GuestPort > 65535)
    {
        QMessageBox::critical(this, "Error", "Invalid Port!");
        ErrFlag = true;
    }
    else if (result == false)
    {
        QMessageBox::critical(this, "Error", "Connected Failed!");
        ErrFlag = true;
    }
    if (ErrFlag == true)
    {
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        delete udpSocket;
        udpSocket = new QUdpSocket;
        return;
    }
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(RecvMsg()));
    IsSetPort = true;
    ui->lineEdit->setDisabled(true);
    ui->lineEdit_2->setDisabled(true);
    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(false);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(false);
}

void NetGameWindow::CancelConnect()
{
    IsSetPort = false;
    IsChooseRole = false;
    delete udpSocket;
    udpSocket = new QUdpSocket;
    ui->lineEdit->setDisabled(false);
    ui->lineEdit_2->setDisabled(false);
    ui->pushButton->setDisabled(false);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(false);
    ui->pushButton_4->setDisabled(true);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    memset(chessboard, NEMPTY, sizeof(chessboard));
    update();
}

void NetGameWindow::RecvMsg()
{
    if(udpSocket->hasPendingDatagrams())//拥有等待的数据报
    {
        QByteArray datagram; //拥于存放接收的数据报
        datagram.resize(udpSocket->pendingDatagramSize());
        if (udpSocket->readDatagram(datagram.data(),datagram.size()) != -1)
        {
            QString msg = datagram.data();
            QStringList list = msg.split(" ");
            chessboard[list[0].toInt()][list[1].toInt()] = NCHESS(list[2].toInt());
            ++ChessCount;
            _DealWin(list[0].toInt(), list[1].toInt());
            update();
        }
    }
}

void NetGameWindow::GameExit()
{
    this->close();
    MainWindow *main = new MainWindow;
    main->show();
}

void NetGameWindow::CheckWin(int x, int y)
{
    int i, j, k, dx, dy, count;
    NCHESS flag;
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

void NetGameWindow::_DealWin(int x, int y)
{
    CheckWin(x, y);
    WinWindow *win = new WinWindow;
    if (WINROLE == NBLACK)
    {
        this->close();
        win->SetWinPlayer("Black Win!");
        win->show();
        delete udpSocket;
    }
    else if (WINROLE == NWHITE)
    {
        this->close();
        win->SetWinPlayer("White Win!");
        win->show();
        delete udpSocket;

    }
    else if (ChessCount == 225 && WINROLE == NEMPTY)
    {
        this->close();
        win->SetWinPlayer("Draw!");
        win->show();
        delete udpSocket;
    }
}
