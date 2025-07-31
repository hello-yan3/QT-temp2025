#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
#include <QDebug>

// 第一个按钮显示 open  ，点击第一个按钮，打开一个新窗口
// 第二个按钮显示 close ，点击第二个按钮，关闭这个新窗口
// 第三个按钮，初始为 open ，单击后打开新窗口，并显示close；再次单击关闭新窗口，显示open

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    QWidget *newWidget = new QWidget;
    QPushButton *brt1 = new QPushButton(this);
    QPushButton *brt2 = new QPushButton(this);
    QPushButton *brt3 = new QPushButton(this);

    this->resize(800, 300);
    brt1->move(100, 150);
    brt2->move(300, 150);
    brt3->move(500, 150);

    brt1->setText("open");
    brt2->setText("close");
    brt3->setText("open");

    this->setWindowTitle("作业");

    connect(
        brt1, &QPushButton::clicked, newWidget, [=]() {
            newWidget->show();
        });

    connect(
        brt2, &QPushButton::clicked, newWidget, [=]() {
            newWidget->close();
        });

    connect(
        brt3, &QPushButton::clicked, newWidget, [=]() {
            if (brt3->text() == "close")
            {
                newWidget->close();
                brt3->setText("open");
            }
            else
            {
                newWidget->show();
                brt3->setText("close");
            }
        });
}

Widget::~Widget()
{
    delete ui;
}
