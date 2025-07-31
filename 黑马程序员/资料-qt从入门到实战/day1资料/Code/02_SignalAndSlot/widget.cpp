#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
#include <QDebug>


// Teacher 类  老师类
// Student 类  学生类
// 下课后 ，老师会触发一个信号，饿了 ，学生响应信号，请客吃饭

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 创建一个老师对象
    this->zt = new Teacher(this);

    // 创建一个学生对象
    this->st = new Student(this);

    //    //老师饿了 学生请客的连接
    //    connect(zt,&Teacher::hungry,st,&Student::treat);

    //    //调用下课函数
    //    classIsOver();


    // 连接带参数的 信号和槽
    // 指针 -> 地址
    // 函数指针 -> 函数地址
    // 这里主要是解决重载问题。使用上面方法，无法确定，究竟使用哪一个成员函数
    // 注意使用 :: 域运算符限定指针作用域

    void (Teacher::*teacherSignal)(QString) = &Teacher::hungry;
    void (Student::*studentSlot)(QString) = &Student::treat;
    connect(zt, teacherSignal, st, studentSlot);
    // classIsOver();


    // 点击一个 下课的按钮 ，再触发下课
    QPushButton *btn = new QPushButton("下课", this);
    // 重置窗口大小
    this->resize(600, 400);

    // 点击按钮 触发下课
    // connect(btn,&QPushButton::clicked,this,&Widget::classIsOver);


    // 无参信号和槽连接
    void (Teacher::*teacherSignal2)(void) = &Teacher::hungry;
    void (Student::*studentSlot2)(void) = &Student::treat;
    // connect(zt,teacherSignal2,st,studentSlot2);

    // 信号连接信号
    connect(btn, &QPushButton::clicked, zt, teacherSignal2);

    // 断开信号
    // disconnect(zt,teacherSignal2,st,studentSlot2);

    // 拓展
    // 1、信号是可以连接信号
    // 2、一个信号可以连接多个槽函数
    // 3、多个信号 可以连接 同一个槽函数
    // 4、信号和槽函数的参数 必须类型一一对应
    // 5、信号和槽的参数个数 是不是要一致? 信号的参数个数 可以多余槽函数的参数个数；但是需要确保类型相对应。

    // Qt4版本以前的信号和槽连接方式
    // 利用Qt4信号槽 连接无参版本
    // Qt4版本 底层SIGNAL("hungry")  SLOT( "treat")
    // connect(zt, SIGNAL(hungry()), st, SLOT(treat()));
    // connect(zt,SIGNAL(hungry(QString)) ,st , SLOT(treat(QString)));
    // Qt4版本优点：参数直观，缺点 ：类型不做检测
    // Qt5以上  支持 Qt4的版本写法，反之不支持


    /**********  lambda函数讲解 **********/
    //    QPushButton * btn2 = new QPushButton;

    //    [btn](){
    //        btn->setText("aaaa");
    //        btn2->setText("bbb"); //btn2看不到
    //    }();

    // mutable关键字 用于修饰值传递的变量，修改的是拷贝，而不是本体

    //    QPushButton * myBtn = new QPushButton (this);
    //    QPushButton * myBtn2 = new QPushButton (this);
    //    myBtn2->move(100,100);
    //    int m = 10;

    //    connect(myBtn,&QPushButton::clicked,this,[m] ()mutable { m = 100 + 10; qDebug() << m; });

    //    connect(myBtn2,&QPushButton::clicked,this,[=] ()  { qDebug() << m; });

    //    qDebug() << m;

    //    int ret = []()->int{return 1000;}();
    //    qDebug() << "ret = " << ret ;
    /**********  lambda函数讲解 **********/

    // 利用lambda表达式 实现点击按钮 关闭窗口
    // 主要是为了解决上面，信号传递信号，参数必须一一对应问题
    // 并且通过lambda表达式，同时连接多个槽函数
    QPushButton *btn2 = new QPushButton(this);
    btn2->setText("关闭");
    btn2->move(100, 0);
    // btn2->setParent(this);
    connect(btn2, &QPushButton::clicked, this ,[=]()  // 注意这里必须使用 = 方法，此外，第三个 this 在这里可以省略
            {
               btn2->setText("aaaa");
               emit zt->hungry("宫保鸡丁");
               this->close();
           } );

    // lambda表达式 最常用  [=](){}
}

void Widget::classIsOver()
{
    // 下课函数，调用后 触发老师饿了的信号
    // emit zt->hungry();
    emit zt->hungry("宫保鸡丁");
}

Widget::~Widget()
{
    delete ui;
}
