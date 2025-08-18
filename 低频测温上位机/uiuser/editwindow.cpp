/* 单击 保存文件按钮 */
/* 点击 确定，保存路径 */
/* 点击 取消，取消保存 */

#include "editwindow.h"
#include "ui_editwindow.h"

EditWindow::EditWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());//隐藏系统标题栏

    connect(ui->btnPush_Cancel,&QPushButton::clicked,this,&EditWindow::buttonClicked);
    connect(ui->btnPush_OK,&QPushButton::clicked,this,&EditWindow::buttonClicked);
}

EditWindow::~EditWindow()
{
    delete ui;
}

void EditWindow::buttonClicked()
{
    QPushButton *btn = static_cast<QPushButton*>(sender());

    if(btn==ui->btnPush_OK)//确定按钮
    {
        QString fileName = ui->lineEdit_fileName->text();
        emit getFileName(fileName);
        this->close();
    }

    if(btn==ui->btnPush_Cancel)//取消按钮
    {
        this->close();
    }
}
