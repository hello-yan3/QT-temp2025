#ifndef WARNINGWINDOW_H
#define WARNINGWINDOW_H

#include <QWidget>
#include <QString>
#include "titleusersimple.h"

namespace Ui {
class WarningWindow;
}

class WarningWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WarningWindow(QWidget *parent = nullptr);
    ~WarningWindow() override;

    void showWarningMsg(QString message);
    void setTitleStyle(QString title,QString iconPath);
private:
    Ui::WarningWindow *ui;

    TitleUserSimple *user_TitleBar;
};

#endif // WARNINGWINDOW_H
