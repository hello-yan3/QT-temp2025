#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent) : QWidget(parent),  ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 设置单选按钮 男默认选中
    ui->rBtnMan->setChecked(true);

    // 选中女后 打印信息
    connect(ui->rBtnWoman, &QRadioButton::clicked, [=]()
            { qDebug() << "选中了女了！"; });

    // 多选按钮  2是选中  0是未选中 1是半选
    connect(ui->cBox, &QCheckBox::stateChanged, [=](int state)
            { qDebug() << state; });

    /*
    // 上例主要是看：stateChanged信号表达式值
        connect(ui->cBox,&QCheckBox::clicked,[=](){
        qDebug() << "选中老板娘！";
    });
    // 这样写也可
    */

    // 利用listWidget写诗
    // QListWidgetItem * item = new QListWidgetItem("锄禾日当午");
    // 将一行诗放入到listWidget控件中
    // ui->listWidget->addItem(item);
    // 重复四次打印全

    // 设置居中
    // item->setTextAlignment(Qt::AlignHCenter);

    // QStringList类; List<String> (List 容器中放的是 String)   QList<QString>
    // 在QT中，这两种写法等价的
    QStringList list;        // 声明 List 容器
    // QList<QString> list;     // 这样写也可以
    list << "锄禾日当午" << "旱地和下土" << "谁知盘中餐" << "粒粒皆辛苦";   // 重载的 << 运算符
    ui->listWidget->addItems(list);
    // 该方法不能做居中
}

Widget::~Widget()
{
    delete ui;
}
