#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "frontpage.h"
#include "settingpage.h"

#if _MSC_VER >= 1600
#pragma execution_character_set( "utf-8")
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    FrontPage *frontPage;
    SettingPage *settingPage;

    void variableInit();
    void viewInit();
    void connectInit();

    void switchPage(int index);
private slots:
    void onTabClicked();
    void openClicked();
};
#endif // MAINWINDOW_H
