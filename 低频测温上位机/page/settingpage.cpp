/* 串口读取设置 */
/* 含文件保存方法 */

#include "settingpage.h"
#include "ui_settingpage.h"
#include "warningwindow.h"
#include <QFile>
#include <QDebug>
#include "sqliteuser.h"
#include "data.h"
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <stdint.h>
#include <string.h>

#define UPDATA_PORT_T 500 //串口更新周期，单位毫秒

QString SettingPage::dataPath;

SettingPage::SettingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingPage)
{
    ui->setupUi(this);

    variableInit(); /*类成员变量初始化*/
    viewInit();     /*界面样式初始化*/
    connectInit();  /*槽函数绑定初始化*/
}

SettingPage::~SettingPage()
{
    delete ui;
}

void SettingPage::variableInit()
{
    serial = new QSerialPort();
    //配置定时器用于跟新串口
    timer = new QTimer();
    timer->start(UPDATA_PORT_T);   // 开启定时器
    //绑定控件
    lineEditsList[0] = ui->widget_2->findChildren<QLineEdit*>();
    lineEditsList[1] = ui->widget_3->findChildren<QLineEdit*>();
    //创建数据库
    SQLiteUser::dataBaseInit();  // 数据库初始化
    if(SQLiteUser::inquire())
    {
        for(int j=0;j<2;j++)
            for (int i=0;i<9 ;i++ )
            {
                lineEditsList[j].at(i)->setText(SQLiteUser::data.at(9*j+i));
            }
    }
    //获取文件夹地址
    dataPath = qApp->applicationDirPath();
    ui->lab_DataPath->setText(dataPath);
    //创建数据文件夹,保存文件时候启用


//    QDir dir;
//    dataPath=qApp->applicationDirPath()+"/data";
//    if (!dir.exists(dataPath))
//    {
//      dir.mkpath(dataPath);
//    }
}

/* ******************************************************************************
 * @name     viewInit
 * @date     2022-03-22
 * @brief    UI初始化
 * @author   Elon Joker
 * @param    none
 * @return   none
 * *****************************************************************************/
void SettingPage::viewInit()
{
    //标题栏样式设置
    QString qss;
    QFile qssFile(":/settingpageqss.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
      qss = QLatin1String(qssFile.readAll());
      this->setStyleSheet(qss);
      qssFile.close();
    }
}

/* ******************************************************************************
 * @name     wid_FrontPage
 * @date     2022-03-22
 * @brief    槽函数连接初始化
 * @author   Elon Joker
 * @param    none
 * @return   none
 * *****************************************************************************/
void SettingPage::connectInit()
{
    connect(ui->btnPush_Save,&QPushButton::clicked,this,&SettingPage::save); // 保存按钮 //保存响应绑定
    connect(ui->btnPush_ChooseFile,&QPushButton::clicked,this,&SettingPage::chooseFilePath);  // 选择路径
    connect(timer,&QTimer::timeout,this,&SettingPage::UpdatePort);//串口更新响应绑定
    // 定时器(500ms)超时，重新寻找系统的串口，并更新信息
}

// 槽函数1，点击保存按钮，保存界面数据
void SettingPage::save()
{
    //获取串口数据
    baudRateIndex = ui->comBox_Baud->currentIndex();
    portName = ui->comBox_Port->currentText();
    bitIndex = ui->comBox_Bit->currentIndex();
    ParityIndex = ui->comBox_Parity->currentIndex();
    stopIndex = ui->comBox_Stop->currentIndex();
    //获取传感器配置
    bool statu = saveSQLite();
    //保存数据存储路径
    dataPath = ui->lab_DataPath->text();
    //保存成功提示
    WarningWindow *warningWindow = new  WarningWindow();
    if(statu)
    {
        warningWindow->setTitleStyle("提示",":/hint.png");
        warningWindow->showWarningMsg("保存成功！");
    }
    else
    {
        warningWindow->setTitleStyle("错误",":/warning-on.png");
        warningWindow->showWarningMsg("保存失败！");
    }

    warningWindow->setWindowModality(Qt::ApplicationModal);//屏蔽其他窗口
    warningWindow->show();

    QTimer::singleShot(700, [warningWindow](){
        warningWindow->close();
    });
}

/* ******************************************************************************
 * @name     openSerial
 * @date     2022-03-23
 * @brief    开启串口，供给外部调用开启串口
 * @author   Elon Joker
 * @param    none
 * @return   bool 返回串口开启结果
 * *****************************************************************************/
bool SettingPage::openSerial()
{
    serial = new QSerialPort();
    //设置串口名
    serial->setPortName(portName);
    //打开串口
    if(!serial->open(QIODevice::ReadWrite))//串口打开失败提醒
    {
        //错误信息提示，串口打开失败，检查串口设备
        return false;
    }
    //设置波特率
    switch (baudRateIndex)
    {
    case 0:
        serial->setBaudRate(QSerialPort::Baud115200);
        break;
    case 1:
        serial->setBaudRate(QSerialPort::Baud9600);
      break;
    case 2:
        serial->setBaudRate(QSerialPort::Baud4800);
      break;
    default:
        break;
    }
    //设置数据位数
    switch (bitIndex)
    {
    case 0:
        serial->setDataBits(QSerialPort::Data8);//设置数据位8
        break;
    case 1:
        serial->setDataBits(QSerialPort::Data7);//设置数据位7
        break;
    case 2:
        serial->setDataBits(QSerialPort::Data6);//设置数据位6
        break;
    case 3:
        serial->setDataBits(QSerialPort::Data5);//设置数据位5
        break;
    default:
        break;
    }
    //设置校验位
    switch (ParityIndex)
    {
    case 0:
        serial->setParity(QSerialPort::NoParity);
        break;
    default:
        break;
    }
    //设置停止位
    switch (stopIndex)
    {
    case 0:
        serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
        break;
    case 1:
        serial->setStopBits(QSerialPort::TwoStop);
        break;
    default:
        break;
    }
    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制
    //串口接收响应
    connect(getSerial(),&QSerialPort::readyRead,this,&SettingPage::ReadData);
    /* 串口收到新数据时，开始读取数据 */
    return true;
}

QSerialPort *SettingPage::getSerial() const
{
    return serial;
}

const QString &SettingPage::getPortName() const
{
    return portName;
}

/* ******************************************************************************
 * @name     UpdatePort
 * @date     2022-03-23
 * @brief    定时更新串口信息
 * @author   Elon Joker
 * @param    none
 * @return   none
 * *****************************************************************************/
void SettingPage::UpdatePort()
{
    QStringList newPortStringList;
    const auto infos = QSerialPortInfo::availablePorts();//获取系统上的所有串口

    if(infos.length()<=0)//不存在串口
    {
        ui->comBox_Port->clear();
        ui->comBox_Port->addItem("NO");
        return;
    }

    //获取串口名称
    for (const QSerialPortInfo &info : infos)
    {
        newPortStringList += info.portName();
    }
    //如果串口没有增加，就直接返回
    if(oldPortStringList.size()== newPortStringList.size())
        return;

    //去掉不存在的,也就是被拔掉的
    for (const QString &name :oldPortStringList)
    {
        if(!newPortStringList.contains(name))//最新串口信息中不含旧串口，表明该串口已经被拔掉
        {
            portName="NO";
            oldPortStringList.removeOne(name);
        }
        else//最新串口信息中包含旧串口
        {
            newPortStringList.removeOne(name);
        }
    }
    oldPortStringList.append(newPortStringList);

    int index = ui->comBox_Port->currentIndex();

    ui->comBox_Port->clear();
    ui->comBox_Port->addItems(oldPortStringList);

    ui->comBox_Port->setCurrentIndex(index);//此举可以在已经选中的串口被热拔插的时候做出空提示
}

// 看不懂，貌似作用是：获取：传感器1、传感器8状态，数据放入数据库中
bool SettingPage::saveSQLite()
{
    QList<QString> data;
    data.append(SQLiteUser::data[0]);
    for (int i=1;i<10 ;i++ )
    {
        data.append(lineEditsList[0].at(i-1)->text());
    }

    bool statu1 = SQLiteUser::updata(data);

    data.clear();

    data.append(SQLiteUser::data[9]);
    for (int i=1;i<10 ;i++ )
    {
        data.append(lineEditsList[1].at(i-1)->text());
    }
    bool statu2 = SQLiteUser::updata(data);

    SQLiteUser::inquire();//查询数据，获取最新数据

    return statu1&&statu2;
}

/******************
* 数据保存方法实现 *
* 需要参照 data 文件 *
*******************/
void SettingPage::ReadData()
{
    static QByteArray buffer;
    // 接收数据
    delay(5);  //为防止数据未完全接收，增加延时等待接收完毕,此处需要配合接收板调整
    // 实测 2ms 数据可以传输完
    QByteArray buf_hex=getSerial()->readAll();//将串口里的可读信息全部读取
    buffer.append(buf_hex);
    // 使用append，是为了配合下面的长度控制，如果长度不足25，前面数据与后面连接累加
    // 此外，若使用前导码等等，用此函数也更加方便
    // 取消 buffer.size()长度控制
    // if(buffer.size()>25)
    // {
    // 保存时间信息
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString time = current_date_time.toString("[yyyy-MM-dd hh:mm::ss::zzz],");
    Data::buffer_time.enqueue(time);

    // DataFrame *temp=new DataFrame();
    // Data::dataDealProcess(buf_hex,temp); // 数据解析 + CRC校验
    // Data::buffer_origin.enqueue(buf_hex);// 将数据帧放入原始缓存中
    Data::buffer_origin.enqueue(buffer);// 将数据帧放入原始缓存中
    // 数据解析
    QString data;
    MsgStruct msg;
    DataFrame *dataFrame = new DataFrame();

    Data::dataDealProcess(buffer,dataFrame);  // 解析后数据保存在dataFrame中
    Data::RestoreData(buffer,&msg);   // 还原后数据保存在msg中
    QByteArray msg_ByteArray;
    msg_ByteArray = Data::MsgToByteArray(&msg);     // msg 转化为 msg_ByteArray

    /* 只要收到数据就进行保存 */
    data.append(time);  // 写入时间
    data.append(tr("%1,").arg(dataFrame->ID&0xffffffff,0,16).toUpper());//ID
    data.append(tr("%1,").arg(dataFrame->sequence));//发射帧序列号
    for (int j=0;j<8;j++)   //八通道温度数据
       data.append(tr("%1,").arg(static_cast<int>(dataFrame->data[j]*10) / 10.0));//保留一位小数
    data.append(tr("%1,").arg(static_cast<double>(dataFrame->Temperature)));//冷端温度
    data.append(tr("%1,").arg(dataFrame->Voltage));//电池电压
    data.append(tr("%1,").arg(dataFrame->count));//发射板时刻
    data.append(tr("%1,").arg(dataFrame->channel));//通道
    data.append(tr("%1,").arg(dataFrame->CRC));//CRC
    data.append(tr("%1,").arg(dataFrame->CRC_));//CRC
    if(dataFrame->CRC==dataFrame->CRC_)
        data.append("1,");//CRC
    else
        data.append("0,");//CRC

    data.append(buffer.toHex()+",");  // 写入原始十六进制数据
    data.append(msg_ByteArray.toHex()+",\n");  // 写入原始十六进制数据

    /* 设置文件保存路径 */
    QString dataFileName = current_date_time.toString("[yyyy-MM-dd-hh]");  // 文件名
    Data::saveData(data,qApp->applicationDirPath()+"/"+"recData"
                  ,dataFileName+".csv");  // 数据保存
    // 完成数据保存
    buffer.clear();  // 清空缓冲区数据
    delete dataFrame;
}

// 设置文件目录
void SettingPage::chooseFilePath()
{
    QString curDir=ui->lab_DataPath->text();
    QString aDir=QFileDialog::getExistingDirectory(this,"select a File",curDir
                                                   ,QFileDialog::ShowDirsOnly);
    if(aDir!="")
        ui->lab_DataPath->setText(aDir);
}

/* ******************************************************************************
 * @name     sleep
 * @date     2022-03-23
 * @brief    简单的延时函数，用于串口接收延时
 * @author   Elon Joker
 * @param    msec 时间，单位毫秒
 * @return   none
 * *****************************************************************************/
void SettingPage::delay(const int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        {
        QCoreApplication::processEvents(QEventLoop::AllEvents, msec);
        //这条语句能够使程序在while等待期间，去处理一下本线程的事件循环，处理事件循环最多msec ms必须返回本语句，
        //如果提前处理完毕，则立即返回这条语句。这也就导致了该Delay_MSec函数的定时误差可能高达msec ms。
    }

}
