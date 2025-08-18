#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QWidget>
#include <QLabel>
#include <QObject>
#include <QEvent>
#include <QMouseEvent>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = nullptr);

signals:
   void myClicked();                    // 单击产生的信号

private:
   void mousePressEvent(QMouseEvent* event) override;                 // 鼠标单击事件
};

#endif // CLICKABLELABEL_H
