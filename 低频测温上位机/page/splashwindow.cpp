/* 进入时的页面刷新进度条 */
#include "splashwindow.h"
#include "ui_splashwindow.h"
#include <QTimer>
#include "mainwindow.h"

SplashWindow::SplashWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplashWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());//隐藏系统标题栏

    progressBar = ui->progressBar;


    ui->progressBar->setStyleSheet("QProgressBar{"
                                   "font:9pt;"
                                   "background-color: transparent;"
                                   "border-color: rgb(180, 180, 180);}"
                                   "QProgressBar:chunk{"
                                   "background-color:rgba(69, 104, 220, 200);}");
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    ui->progressBar->setTextVisible(false);
}

SplashWindow::~SplashWindow()
{
    delete ui;
}

void SplashWindow::myShow()
{
    this->show();
    while(ui->progressBar->value()<100)
    {
        ui->progressBar->setValue(ui->progressBar->value()+20);
        Sleep(500);
    }
    this->close();
}
