﻿#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QWidget>

namespace Ui {
class EditWindow;
}

class EditWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = nullptr);
    ~EditWindow() override;

private:
    Ui::EditWindow *ui;

    void buttonClicked();
signals:
    void getFileName(QString fileName);
};

#endif // EDITWINDOW_H
