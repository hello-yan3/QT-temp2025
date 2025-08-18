#ifndef CHARTPANEL_H
#define CHARTPANEL_H

#include <QWidget>
#include "protocol.h"
#include "clickablelabel.h"
#include <QtCharts>
using namespace QtCharts;

#if _MSC_VER >= 1600
#pragma execution_character_set( "utf-8")
#endif

namespace Ui {
class ChartPanel;
}

class ChartPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ChartPanel(QWidget *parent = nullptr);
    ~ChartPanel() override;
    /* 更新图表，此函数需要进行修改 */
    void updataChart(const DataFrame* dataFrame);
    ClickableLabel *user_ClickLabMax;    // 自定义的 点击label
    void setChartTitle(QList<QString> title,int startIndex);  // 设置图表标题
private:
    Ui::ChartPanel *ui;
    QChart* charts[8];             // 8个图表
    QSplineSeries* series[8];      // 曲线图
    QScatterSeries* seriesLab[8];  // 散点图（实现数据实时显示）
    QValueAxis* xAxis[8];  // X轴
    QValueAxis* yAxis[8];  // Y轴

    QList<double> pointY[8];   // 温度数据，使用8个list存储

    void variableInit();
    void viewInit();
    void connectInit();

    void addChartView(QWidget *widget,QChart *chart,
                      QSplineSeries* series,QScatterSeries* seriesLab,
                      QValueAxis* xAxis,QValueAxis* yAxis);
};

#endif // CHARTPANEL_H
