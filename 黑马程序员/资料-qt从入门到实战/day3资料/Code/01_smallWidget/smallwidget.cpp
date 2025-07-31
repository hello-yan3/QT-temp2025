#include "smallwidget.h"
#include "ui_smallwidget.h"


// 添加新文件 -  Qt – 设计师界面类  (.h  .cpp  .ui) 
// 用于自定义封装界面 
// idget中使用自定义控件：拖拽一个Widget，右键，点击提升为，输入类名称，全局包含，点击添加，点击提升
// 注意 基类 类型必须匹配 
SmallWidget::SmallWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SmallWidget)
{
    ui->setupUi(this);

    // QSpinBox移动 QSlider跟着移动
    void (QSpinBox::*spSignal)(int) = &QSpinBox::valueChanged;   // 这里是为了解决valueChanged函数重载问题
    connect(ui->spinBox, spSignal, ui->horizontalSlider, &QSlider::setValue);

    // QSlider滑动  QSpinBox数字跟着改变
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);
}

// 设置对外接口
// 设置数字
void SmallWidget::setNum(int num)
{
    ui->spinBox->setValue(num);
}
// 获取数字
int SmallWidget::getNum()
{
    return ui->spinBox->value();
}

SmallWidget::~SmallWidget()
{
    delete ui;
}
