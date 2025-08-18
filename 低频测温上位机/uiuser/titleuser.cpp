/* 活塞表面温度无线测量系统   标题栏 */

#include "titleuser.h"
#include <QFile>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

TitleUser::TitleUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleUser)
{
    ui->setupUi(this);
    this->setFixedHeight(this->titleHight);

    btnPush_Minimize = ui->btnPush_Minimize;
    btnPush_Maximize = ui->btnPush_Maximize;
    btnPush_Close = ui->btnPush_Close;
    wid_Back = ui->wid_Back;
    lab_Title0 = ui->lab_Title0;
    lab_Title = ui->lab_Title;
    lab_Interval = ui->lab_Interval;

    //设置Icon和标题
    ui->lab_Title0->setText("");
    ui->lab_Title->setText(title);
    //设置标题栏布局
    ui->wid_Back->layout()->setSpacing(0);
    ui->wid_Back->layout()->setMargin(0);
    ui->wid_Back->setFixedHeight(this->titleHight);
    //添加图标
    ui->btnPush_Minimize->setIconSize(QSize(32,32));
    ui->btnPush_Minimize->setIcon(QIcon(":/Minimize.png"));
    ui->btnPush_Minimize->setFixedSize(QSize(50,this->titleHight));
    ui->btnPush_Maximize->setIconSize(QSize(32,32));
    ui->btnPush_Maximize->setIcon(QIcon(":/Maximize.png"));
    ui->btnPush_Maximize->setFixedSize(QSize(50,this->titleHight));
    ui->btnPush_Close->setIconSize(QSize(32,32));
    ui->btnPush_Close->setIcon(QIcon(":/closePower.png"));
    ui->btnPush_Close->setFixedSize(QSize(50,this->titleHight));

    //title的QSS美化 加载失效，在mainwindow下进行美化
//    QString qss;
//    QFile qssFile(":/titleqss.qss");
//    qssFile.open(QFile::ReadOnly);
//    if(qssFile.isOpen())
//    {
//      qss = QLatin1String(qssFile.readAll());
//      this->setStyleSheet("QLabel{font-family:Microsoft Yahei UI;font-size:30px;color:white;font-weight:bold;}");
//      qDebug()<<qss;
//      qssFile.close();
//    }

    //绑定槽函数
    connect(ui->btnPush_Minimize,&QPushButton::clicked,this,&TitleUser::onClicked);
    connect(ui->btnPush_Maximize,&QPushButton::clicked,this,&TitleUser::onClicked);
    connect(ui->btnPush_Close,&QPushButton::clicked,this,&TitleUser::onClicked);
}

TitleUser::~TitleUser()
{
    delete ui;
}

// 设置高度
void TitleUser:: setHight(int hight)
{
    this->setFixedHeight(hight);
    ui->wid_Back->setFixedHeight(hight);
    ui->btnPush_Minimize->setFixedSize(QSize(50,hight));
    ui->btnPush_Maximize->setFixedSize(QSize(50,hight));
    ui->btnPush_Close->setFixedSize(QSize(50,hight));
}

// 鼠标双击事件
void TitleUser::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit ui->btnPush_Maximize->clicked();
}

// 鼠标按压移动事件
void TitleUser::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel())
        {
           SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
       event->ignore();
#endif
}

// 当被点击按压
void TitleUser::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == ui->btnPush_Minimize)
        {
            pWindow->showMinimized();
        }
        else if (pButton == ui->btnPush_Maximize)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == ui->btnPush_Close)
        {
            pWindow->close();
        }
    }
}
