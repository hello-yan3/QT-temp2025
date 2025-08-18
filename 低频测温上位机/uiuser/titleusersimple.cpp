/* 点击开启通信等等，弹出的 工具栏 的  标题栏设置 */

#include "titleusersimple.h"

TitleUserSimple::TitleUserSimple(QWidget *parent) : TitleUser(parent)
{
    this->btnPush_Maximize->setVisible(false);
    this->btnPush_Minimize->setVisible(false);

    this->btnPush_Close->setText("");
    QPixmap pixmap(":/close.png");
    QPixmap fitpixmap = pixmap.scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    this->btnPush_Close->setIcon(QIcon(fitpixmap));
    this->btnPush_Close->setIconSize(QSize(30, 30));
    this->btnPush_Close->setFlat(true);
    this->btnPush_Close->setStyleSheet("border: 0px"); //消除边框

}


void TitleUserSimple::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
//    emit btnPush_Maximize->clicked();
}
