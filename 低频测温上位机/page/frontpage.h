#ifndef FRONTPAGE_H
#define FRONTPAGE_H

#include <QWidget>
#include <QPushButton>

#include "chartpanel.h"
#include "editwindow.h"
#if _MSC_VER >= 1600
#pragma execution_character_set( "utf-8")
#endif

namespace Ui {
class FrontPage;
}

class FrontPage : public QWidget
{
    Q_OBJECT

public:
    explicit FrontPage(QWidget *parent = nullptr);
    ~FrontPage() override;

    QPushButton *btnPush_Turn; // 开启通信
    QPushButton *btnPush_Save; // 保存数据
    static bool isSingle;
    static bool isSave;
    ChartPanel* chartPanel[2];

    static int lastSequence[2];
private:
    Ui::FrontPage *ui;
    QTimer *timerDataDeal;
    EditWindow* editWindow;

    QString fileName;
    QString fileNameUser;

    void variableInit();
    void viewInit();
    void connectInit();
private slots:
    void modeSwitch();
    void saveClicked();
    void upDataUI();
    void maxChartPanel();
    void setSaveFile(QString fileName);
};

#endif // FRONTPAGE_H
