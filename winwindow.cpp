#include "winwindow.h"
#include "ui_winwindow.h"

WinWindow::WinWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WinWindow)
{
    ui->setupUi(this);
    ui->pushButton->setGeometry(255, 500, 150, 50);
    ui->pushButton->setFont(QFont("Consolas", 13));
}

WinWindow::~WinWindow()
{
    delete ui;
}

void WinWindow::SetWinPlayer(QString tmp)
{
    ui->label->setGeometry(0, 0, 660, 400);
    ui->label->setText(tmp);
    ui->label->setFont(QFont("Consolas", 40));
    ui->label->setAlignment(Qt::AlignCenter);
}

void WinWindow::HomeMenu()
{
    this->close();
    MainWindow *main = new MainWindow();
    main->show();
}
