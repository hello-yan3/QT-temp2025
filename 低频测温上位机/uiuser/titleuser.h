#ifndef TITLEUSER_H
#define TITLEUSER_H

#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include "ui_titleuser.h"

#if _MSC_VER >= 1600
#pragma execution_character_set( "utf-8")
#endif

namespace Ui {
class TitleUser;
}

class TitleUser : public QWidget
{
    Q_OBJECT

public:
    explicit TitleUser(QWidget *parent = nullptr);
    ~TitleUser() override;

    QWidget *wid_Back;
    QPushButton *btnPush_Minimize;
    QPushButton *btnPush_Maximize;
    QPushButton *btnPush_Close;
    QLabel *lab_Title0;
    QLabel *lab_Title;
    QLabel *lab_Interval;
    // 其实这些都可以不定义，直接使用
    // × 后面的继承文件需要
    void setHight(int hight);

protected:

    // 双击标题栏进行界面的最大化/还原
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    // 进行鼠界面的拖动
    virtual void mousePressEvent(QMouseEvent *event) override;

private slots:

    // 进行最小化、最大化/还原、关闭操作
    void onClicked();

private:
    Ui::TitleUser *ui;

    int titleHight=40;
    QString title="标题";
};

#endif // TITLEUSER_H
