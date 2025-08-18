/* 点击开启通信等等，弹出的 工具栏 */
#include "warningwindow.h"
#include "ui_warningwindow.h"

#include <QFile>
#include <QString>

WarningWindow::WarningWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarningWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | windowFlags());//设置为子窗口|隐藏系统标题栏

    user_TitleBar = ui->user_TitleBar;

    //设置自定义标题栏
    ui->user_TitleBar->lab_Title->setText("错误");
    ui->user_TitleBar->setHight(35);
    ui->user_TitleBar->lab_Title0->setFixedSize(QSize(24,24));
    QPixmap pixLogo(":/warning-on.png");
    pixLogo = pixLogo.scaled(ui->user_TitleBar->lab_Title0->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->user_TitleBar->lab_Title0->setPixmap(pixLogo);
    //设置界面元素
    ui->lab_WaringMsg->setStyleSheet("background-color: rgb(255,255,255);"
                                     "font-family:Microsoft Yahei UI;"
                                     "font-size:18px;");
    ui->user_TitleBar->lab_Title->setStyleSheet("color:black;font-size:18px;");
    ui->wid_Warning->setStyleSheet("#wid_Warning{border:1px solid blue;}");
}

WarningWindow::~WarningWindow()
{
    delete ui;
}

void WarningWindow::showWarningMsg(QString message)
{
    ui->lab_WaringMsg->setText(message);
}

void WarningWindow::setTitleStyle(QString title,QString iconPath)
{
    user_TitleBar->lab_Title->setText(title);
    QPixmap pixLogo(iconPath);
    pixLogo = pixLogo.scaled(user_TitleBar->lab_Title0->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    user_TitleBar->lab_Title0->setPixmap(pixLogo);
}
