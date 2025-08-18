/* 主窗体界面 */

#include "chartpanel.h"
#include "ui_chartpanel.h"

#define ROW 8   //通道数
#define COLUMN 30 //最多显示数据点数

ChartPanel::ChartPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartPanel)
{
    ui->setupUi(this);
    variableInit(); /*类成员变量初始化*/
    viewInit();     /*界面样式初始化*/
    connectInit();  /*槽函数绑定初始化*/
}

ChartPanel::~ChartPanel()
{
    delete ui;
}

/*类成员变量初始化*/
void ChartPanel::variableInit()
{
    //初始化图表控件
    for(int i=0;i<8;i++)
    {
        charts[i] = new QChart();
        series[i] = new QSplineSeries();
        seriesLab[i] = new QScatterSeries();
        xAxis[i]=new QValueAxis();
        yAxis[i]=new QValueAxis();
    }
    user_ClickLabMax = ui->lab_Max;

}

/*界面样式初始化*/
void ChartPanel::viewInit()
{
    //设置UI界面
    addChartView(ui->wid_Aisle1,charts[0],series[0],seriesLab[0],xAxis[0],yAxis[0]);
    addChartView(ui->wid_Aisle2,charts[1],series[1],seriesLab[1],xAxis[1],yAxis[1]);
    addChartView(ui->wid_Aisle3,charts[2],series[2],seriesLab[2],xAxis[2],yAxis[2]);
    addChartView(ui->wid_Aisle4,charts[3],series[3],seriesLab[3],xAxis[3],yAxis[3]);
    addChartView(ui->wid_Aisle5,charts[4],series[4],seriesLab[4],xAxis[4],yAxis[4]);
    addChartView(ui->wid_Aisle6,charts[5],series[5],seriesLab[5],xAxis[5],yAxis[5]);
    addChartView(ui->wid_Aisle7,charts[6],series[6],seriesLab[6],xAxis[6],yAxis[6]);
    addChartView(ui->wid_Aisle8,charts[7],series[7],seriesLab[7],xAxis[7],yAxis[7]);

    ui->lab_icon->setPixmap(QPixmap(":/chip.png"));

    QPixmap *pixmap = new QPixmap(":/Maximize.png");
//    pixmap->scaled(ui->lab_Max->size(), Qt::KeepAspectRatio);
    ui->lab_Max->setScaledContents(true);
    ui->lab_Max->setPixmap(*pixmap);

    ui->wid_Chart->setStyleSheet("background-color:white;"
                                 "border-bottom-left-radius:10px;"
                                 "border-bottom-right-radius:10px;");
    ui->wid_Title->setStyleSheet(
                                 "background-color:rgb(85, 255, 255);"
                                 "border-top-left-radius:10px;"
                                 "border-top-right-radius:10px;");
}

void ChartPanel::connectInit()
{

}

/* 设置图表组件 */
void ChartPanel::addChartView(QWidget *widget,QChart *chart,
                              QSplineSeries* series,QScatterSeries* seriesLab,
                              QValueAxis* xAxis,QValueAxis* yAxis)
{
    QGridLayout *mainLayout = new QGridLayout(widget);
    QChartView *chartview = new QChartView();
    mainLayout->addWidget(chartview);
    mainLayout->setMargin(0);
    //创建QChart，进行简单设置
    chart->legend()->hide();
    chart->setTitle("测点位置");
    chart->setBackgroundBrush(QColor(238, 241, 243));
    chartview->setChart(chart);
    //利用散点图实现最新点数据展示
    seriesLab->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    seriesLab->setMarkerSize(10);
    seriesLab->setPointLabelsFormat("@yPoint");
    seriesLab->setPointLabelsClipping(false);
    seriesLab->setPointLabelsVisible(true);
    //初始化图表显示
    for(int i=0;i<COLUMN;i++)
        series->append(i,10*sin(2*(i+1)*3.14/COLUMN));
    seriesLab->append(29,0.01);
    //把曲线添加到图表
    chart->addSeries(series);
    chart->addSeries(seriesLab);
    //设置坐标轴间隔
    yAxis->setRange(-15, 15);
    yAxis->setTickCount(3);       //设置多少格
    yAxis->setMinorTickCount(1);  //设置每格小刻度线的数目，
    yAxis->setLabelFormat("%d");  //设置刻度的格式，整数形式
    chart->addAxis ( yAxis, Qt::AlignLeft );
    seriesLab->attachAxis(yAxis); //将坐标轴附加在图表上
    series->attachAxis(yAxis);

    xAxis->setRange(0, 31);
    chart->addAxis ( xAxis, Qt::AlignBottom );
    seriesLab->attachAxis(xAxis);
    series->attachAxis(xAxis);
    xAxis->setVisible(false);

//    chart->createDefaultAxes();
//    chart->axes(Qt::Horizontal).back()->setVisible(false);
//    chart->axes(Qt::Vertical).back()->setRange(0,20);
}

/* ******************************************************************************
 * @name     updataChart
 * @date     2022-03-25
 * @brief    供给外部用于更新数据
 * @author   Elon Joker
 * @param    DataFrame 数据帧结构体
 * @return   none
 * *****************************************************************************/
void ChartPanel::updataChart(const DataFrame* dataFrame)
{
    double min[8],max[8];
    for(int i =0;i<8;i++)
    {
        min[i]=dataFrame->data[0];
        max[i]=dataFrame->data[0];
    }
    //更新曲线数据
    for(int i=0;i<8;i++)
    {
        pointY[i].append(static_cast<int>( dataFrame->data[i]*10) / 10.0);//保留一位小数
        if(pointY[i].length()>COLUMN)//限制展示的数据点为COLUMN个
            pointY[i].removeFirst();

        QList<QPointF> points;   // pointf，坐标点数据
        points.clear();
        int len =pointY[i].length();
        for(int j=0;j<len;j++)//生成数据点
        {
            points.append( QPointF(j,pointY[i].at(j)) );
        }

        if(!isVisible())//未显示的表盘就不需要显示，直接返回
            return ;

        /* 设置图表曲线 */
        QSplineSeries *series =new QSplineSeries();
        series->append(points);
        QScatterSeries *seriesLab =new QScatterSeries();
        seriesLab->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        seriesLab->setMarkerSize(10);
        seriesLab->setPointLabelsFormat("@yPoint");
        seriesLab->setPointLabelsVisible(true);
        seriesLab->append(len-1,pointY[i].at(len-1));

        charts[i]->removeAllSeries();
        charts[i]->addSeries(series);
        charts[i]->addSeries(seriesLab);

        //找到最大最小值，设置y轴范围
        auto max_temp = std::max_element(pointY[i].begin(),pointY[i].end());
        //最小值表示：
        auto min_temp = std::min_element(pointY[i].begin(),pointY[i].end());
        max[i] = *max_temp;
        min[i] = *min_temp;


        if( (max[i]-min[i]) >10)//最大值与最小值相差10度以上时，直接取整设置范围
        {
            yAxis[i]->setRange(floor(min[i]),ceil(max[i]));
        }
        else//否则，取平均值的上下五度
        {
            //求平均值
            double avg =0;
            for(int k=0;k<pointY[i].length();k++)//求各自通道的平均值
                avg+=pointY[i].at(k);
            yAxis[i]->setRange(avg/pointY[i].length()-2.5,avg/pointY[i].length()+2.5);
        }
        // x轴y轴设置
        series->attachAxis(yAxis[i]);
        seriesLab->attachAxis(yAxis[i]);
        series->attachAxis(xAxis[i]);
        seriesLab->attachAxis(xAxis[i]);

        //等比例缩放整个视图
        qreal rVal = 2; // 设置比例
//        double newRangeMax =(rVal+1)/2*max[i] - (rVal-1)/2*min[i];//计算新的范围，推一下公式
//        double newRangeMin = (rVal+1)/2*min[i] - (rVal-1)/2*max[i];
        // 1. 读取视图基本信息
        QRectF oPlotAreaRect = charts[i]->plotArea();
        QPointF oCenterPoint = oPlotAreaRect.center();
        // 3. 竖直调整
        oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
        // 4.1 计算视点，视点不变，围绕中心缩放
        QPointF oNewCenterPoint(oCenterPoint);
        // 5. 设置视点
        oPlotAreaRect.moveCenter(oNewCenterPoint);
        // 6. 提交缩放调整
        charts[i]->zoomIn(oPlotAreaRect);
    }

    //更新基本信息

    /******************
    * ID & 冷端温度 & 电池电压
    *******************/

    ui->lab_ID->setText(tr("0X%1\t").arg(dataFrame->ID&0xffffffff,0,16).toUpper());
    ui->lab_Temp->setText(tr("%1℃").arg(dataFrame->Temperature));
    ui->lab_Battery->setText(tr("%1V").arg(dataFrame->Voltage));
}

// 设置图表标题：测点位置x
void ChartPanel::setChartTitle(QList<QString> title,int startIndex)
{
    ui->lab_title->setText(title.at(startIndex+0));
    for(int i=0;i<8;i++)
    {
        charts[i]->setTitle(title[startIndex+i+1]);
    }
}
