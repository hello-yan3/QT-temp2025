#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ui->actionNew->setIcon(QIcon("E:/Image/Luffy.png"));

    // 使用添加Qt资源 ": + 前缀名 + 文件名 "
    ui->actionNew->setIcon(QIcon(":/Image/Luffy.png"));
    ui->actionOpen->setIcon(QIcon(":/Image/LuffyQ.png"));

    /*
    2.1	将图片文件 拷贝到项目位置下
    2.2	右键项目->添加新文件 –>  Qt -> Qt recourse File -> 给资源文件起名 res
    2.3	res 生成 res.qrc  
    2.4	open in editor  编辑资源
    2.5	添加前缀  添加文件
    2.6	使用  “ : + 前缀名 + / + 文件名 ”
    */


}

MainWindow::~MainWindow()
{
    delete ui;
}
