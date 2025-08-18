/* 主要为图表更新设置 */
/* 含数据显示及保存过程 */
/* 此外，点击保存数据这里，确实有bug！ */

#include "frontpage.h"
#include "ui_frontpage.h"
#include "data.h"
#include "sqliteuser.h"
#include "warningwindow.h"
#include "settingpage.h"

#define UPDATA_UI_T 100 //图表更新周期，单位毫秒
int FrontPage::lastSequence[2]={-1,-1};   // 上个数据，判断重复使用

bool FrontPage::isSingle=false;  // 是否单模显示
bool FrontPage::isSave = false;  // 是否保存

FrontPage::FrontPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontPage)
{
    ui->setupUi(this);

    variableInit(); /*类成员变量初始化*/
    viewInit();     /*界面样式初始化*/
    connectInit();  /*槽函数绑定初始化*/
}

FrontPage::~FrontPage()
{
    delete ui;
}

// 类成员变量更新
void FrontPage:: variableInit()
{
    btnPush_Turn = ui->btnPush_Turn;
    btnPush_Save = ui->btnPush_SaveData;

    for(int i=0;i<2;i++)
        chartPanel[i] = new ChartPanel();

    timerDataDeal = new QTimer();//开启定时更新
    timerDataDeal->start(UPDATA_UI_T);

    editWindow = new EditWindow();

}

// 更新界面
void FrontPage:: viewInit()
{
    QGridLayout *mainLayout = new QGridLayout(ui->wid_Charts);
    mainLayout->addWidget(chartPanel[0]);
    mainLayout->addWidget(chartPanel[1]);
}

void FrontPage:: connectInit()
{
    // 点击模式按钮，进行模式切换
    connect(ui->btnPush_Mode,&QPushButton::clicked,this,&FrontPage:: modeSwitch);
    // 点击保存按钮，进行数据保存
    connect(ui->btnPush_SaveData,&QPushButton::clicked,this,&FrontPage::saveClicked);

    // 绑定定时刷新
    // 定时器超时，更新界面UI
    connect(timerDataDeal,&QTimer::timeout,this,&FrontPage::upDataUI);
    // 输入保存文件名后，保存文件
    connect(editWindow,&EditWindow::getFileName,this,&FrontPage::setSaveFile);
    // 图表0 / 1 点击，图表最大化
    connect(chartPanel[0]->user_ClickLabMax,&ClickableLabel::myClicked,this,&FrontPage:: maxChartPanel);
    connect(chartPanel[1]->user_ClickLabMax,&ClickableLabel::myClicked,this,&FrontPage:: maxChartPanel);
}

/* ******************************************************************************
 * @name     modeSwitch
 * @date     2022-03-26
 * @brief    模式切换函数，用于切换显示模式
 *              单模显示    双发射板
 *              双模显示    双发射板
 * @author   Elon Joker
 * @param    none
 * @return   none
 * *****************************************************************************/
// 单模显示
void FrontPage:: modeSwitch()
{
    if("单模显示"==ui->btnPush_Mode->text())
    {
        isSingle = true;
        chartPanel[0]->setVisible(true);
        chartPanel[1]->setVisible(false);

        ui->btnPush_Mode->setText("双模显示");
    }
    else
    {
        isSingle = false;
        chartPanel[0]->setVisible(true);
        chartPanel[1]->setVisible(true);

        ui->btnPush_Mode->setText("单模显示");
    }

}

// 最大化界面
void FrontPage::maxChartPanel()
{
    if(isSingle)//单例模式下，放大作用失效
        return;

    ClickableLabel *pClick = qobject_cast<ClickableLabel *>(sender());
    int index = 0;
    if(pClick == chartPanel[0]->user_ClickLabMax)
    {
        index =1;
    }
    else
    {
        index =0;
    }

    if(chartPanel[index]->user_ClickLabMax->isVisible())
    {
        chartPanel[index]->setVisible(false);
    }
    else
    {
        chartPanel[index]->setVisible(true);
    }

}

// 文件保存
void FrontPage::saveClicked()
{
    // 如果未开启通信时点击保存数据，显示警报提示
    if("开启通信"==ui->btnPush_Turn->text())
    {
        WarningWindow *warningWindow = new  WarningWindow();
        warningWindow->setTitleStyle("提示",":/warning-on.png");
        warningWindow->showWarningMsg("未开启通信!");
        warningWindow->setWindowModality(Qt::ApplicationModal);//屏蔽其他窗口
        warningWindow->show();
        QTimer::singleShot(3000, [warningWindow](){
            if(warningWindow->isVisible())
                warningWindow->close();
        });
        return;
    }
    // 如果开启保存数据
    if(isSave)
    {
        ui->btnPush_SaveData->setText("保存数据");
        isSave =false;
    }
    // 如果未开启保存数据
    else
    {
        // ui->btnPush_SaveData->setText("停止保存");    // 移动至下方 isSave =true 处
        editWindow->setWindowModality(Qt::ApplicationModal);//屏蔽其他窗口
        editWindow->show();
    }

}

/******************
* 更新UI界面  *
* 需要参照 data 文件 *
*******************/
void FrontPage::upDataUI()
{
    if("开启通信"==ui->btnPush_Turn->text())
        return;//未开启通信时候，无需处理

    // 第一个，数据原始缓存 == 数据时间缓存，恒成立，因为只要收到数据，就会保存响应时间
    // 第二个，只要收到数据，原始缓存就会大于0，
    while((Data::buffer_origin.size()==Data::buffer_time.size())&&Data::buffer_origin.size()>0)//每一个周期内处理完所有已经收到的数据
    {
        QByteArray temp = Data::buffer_origin.dequeue();  // 获得收到的数据
        QString time = Data::buffer_time.dequeue();   // 获得保存数据的时间
        DataFrame *dataFrame = new DataFrame();
        int index =0;

        if(!Data::dataDealProcess(temp,dataFrame))//CRC校验不通过则直接返回
            continue ;

        if(dataFrame->ID==Data::ID[0])//ID不属于已经保存的范围内的，直接跳出
            index = 0;
        else if(dataFrame->ID==Data::ID[1])
            index = 1;
        else
            continue ;

        //判断重复帧
        if(lastSequence[index]!=-1)
        {
            if(lastSequence[index] == dataFrame->sequence)
                continue;
            else
                lastSequence[index] = dataFrame->sequence;
        }
        else
            lastSequence[index] = dataFrame->sequence;

        //更新图表
        chartPanel[index]->updataChart(dataFrame);
        chartPanel[index]->setChartTitle(SQLiteUser::data,index*9);//标题

        MsgStruct msg;
        Data::RestoreData(temp,&msg);   // 还原后数据保存在msg中
        QByteArray msg_ByteArray;
        msg_ByteArray = Data::MsgToByteArray(&msg);     // msg 转化为 msg_ByteArray

        //将所有保存的数据进行拼接
        QString m,debug;
        //故障预警 TODO
        m.append(time);//时间
        m.append(tr("%1,").arg(dataFrame->ID&0xffffffff,0,16).toUpper());//ID
        m.append(tr("%1,").arg(dataFrame->sequence));//发射帧序列号
        for (int j=0;j<8;j++)//八通道温度数据
            m.append(tr("%1,").arg(static_cast<int>( (dataFrame->data[j]*10) +0.5) /  10.0));//保留一位小数
        m.append(tr("%1,").arg(static_cast<double>(dataFrame->Temperature)));//冷端温度
        m.append(tr("%1,").arg(dataFrame->Voltage));//电池电压
        m.append(tr("%1,").arg(dataFrame->count));//发射板时刻
        m.append(tr("%1,").arg(dataFrame->channel));//通道
        m.append(tr("%1,").arg(dataFrame->CRC));//CRC
        m.append(tr("%1,\n").arg(dataFrame->CRC_));//CRC
        //调试信息
        debug.append(temp.toHex());
        debug.append(",\n");
        debug.append(msg_ByteArray.toHex());
        debug.append(",\n");
        debug.append(m);

        QString sublistPath,debuglistPath;
        sublistPath.append(SettingPage::dataPath+"/"+SQLiteUser::data[index*9]);
        debuglistPath.append(SettingPage::dataPath+"/"+SQLiteUser::data[index*9]+"/debug");

        //保存数据——用户手动保存
        if(isSave)
        {
            Data::saveData(m,sublistPath,fileNameUser+".csv");
            QDateTime current_date_time = QDateTime::currentDateTime();
            //保存数据——自动保存，保存所有数据
            Data::saveData(debug,debuglistPath,fileNameUser+".csv");
        }
        delete dataFrame;
    }
}

// 设置文件保存
void FrontPage::setSaveFile(QString name)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString time = current_date_time.toString("[yyyy-MM-dd-hh_mm_ss]");
    fileName.clear();
    fileNameUser.clear();//用户主动数据保存，以用户输入为准，默认时间

    if(name!="")
        fileNameUser.append(name);
    else
        fileNameUser.append(time);

    fileName.append(time);//debug数据保存文件名，以时间为基准

//    qDebug()<<fileNameUser;
    isSave =true;
    ui->btnPush_SaveData->setText("停止保存");    // 移动至下方 isSave =true 处
}
