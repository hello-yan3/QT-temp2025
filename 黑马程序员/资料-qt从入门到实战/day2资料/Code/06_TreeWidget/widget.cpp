#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    // treeWidget树控件使用

    // 设置水平头
    ui->treeWidget->setHeaderLabels(QStringList() << "英雄" << "英雄介绍"); // QStringList()为匿名对象
    // 设置顶层节点
    QTreeWidgetItem *liItem = new QTreeWidgetItem(QStringList() << "力量");
    QTreeWidgetItem *minItem = new QTreeWidgetItem(QStringList() << "敏捷");
    QTreeWidgetItem *zhiItem = new QTreeWidgetItem(QStringList() << "智力");
    // 加载顶层的节点
    ui->treeWidget->addTopLevelItem(liItem);
    ui->treeWidget->addTopLevelItem(minItem);
    ui->treeWidget->addTopLevelItem(zhiItem);
    // 另一种同时设置3层节点方法：
    // ui->treeWidget->addTopLevelItems(QList<QTreeWidgetItem *>() << liItem << minItem << zhiItem);

    // 设置子节点：教程方法
    // QStringList heroL1;
    // QStringList heroL2;
    // heroL1 << "刚被猪"
    //        << "前排坦克，能在吸收伤害的同时造成可观的范围输出";
    // heroL2 << "船长"
    //        << "前排坦克，能肉能输出能控场的全能英雄";
    // QTreeWidgetItem *l1 = new QTreeWidgetItem(heroL1);
    // QTreeWidgetItem *l2 = new QTreeWidgetItem(heroL2);
    // 此时创建的对象为实例对象
    QTreeWidgetItem *l1 = new QTreeWidgetItem(QStringList() << "刚被猪"
                                                            << "前排坦克，能在吸收伤害的同时造成可观的范围输出");
    QTreeWidgetItem *l2 = new QTreeWidgetItem(QStringList() << "船长"
                                                            << "前排坦克，能肉能输出能控场的全能英雄");
    // 加载子节点
    liItem->addChild(l1);
    liItem->addChild(l2);

    // 另一种同时加载多层子节点方法
    // liItem->addChildren(QList<QTreeWidgetItem *>() << l1 << l2);
}

Widget::~Widget()
{
    delete ui;
}
