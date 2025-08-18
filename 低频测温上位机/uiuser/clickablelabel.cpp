#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent)
{

}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit myClicked();
    // 触发 鼠标单机信号
}
