#ifndef TITLEUSERSIMPLE_H
#define TITLEUSERSIMPLE_H

#include <QWidget>
#include "titleuser.h"

class TitleUserSimple : public TitleUser
{
    Q_OBJECT
public:
    explicit TitleUserSimple(QWidget *parent = nullptr);

protected:
    // 双击标题栏进行界面的最大化/还原
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:

};

#endif // TITLEUSERSIMPLE_H
