#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LocalGame()//本地模式设置
{
    this->close();
    GameWindow *game = new GameWindow();
    game->show();
}

void MainWindow::OnlineGame()//在线模式设置
{
    this->close();
    NetGameWindow *game = new NetGameWindow();
    game->show();
}
