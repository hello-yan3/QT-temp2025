#include "data.h"
#include "string.h"
#include <math.h>
#include "crc.h"
#include <QTime>
#include <QFile>
#include "warningwindow.h"
#include <QDir>
#include <QTimer>
#include <QDebug>

uint32_t Data::ID[2]={0xFFFF,0xFFFF};
QQueue<QByteArray> Data::buffer_origin;
QQueue<QString> Data::buffer_time;
//QQueue<DataFrame> Data::buffer_ID[2];


Data::Data()
{

}

/**
 * @brief 将通道压缩后的数据还原，其数据具体格式位如下：
 * @param  指向FIFO指针。
 * @retval 无
 */
void Data::RestoreData(QByteArray data, MsgStruct *msg)
{
    // 定义FIFO，将data数据复制进入FIFO中
    uint8_t FIFO[DATA_LENGTH];
    memcpy(FIFO,data,sizeof(FIFO));

    uint8_t i=0;

    /********** 其他调试数据 **********/
    msg->Debug[0] = FIFO[ DATA_LENGTH-1 -2 ];      // 保存接收板time计数，表示两次接收数据之间间隔多少ms
    msg->Debug[1] = FIFO[ DATA_LENGTH-1 -1 ];
    msg->Debug[2] = FIFO[ DATA_LENGTH-1 -0 ];      // 接收通道记录

    /********** CRC 校验 **********/
    msg->_CRC_[1] = CRC::crc8_chk_value(FIFO,DATA_LENGTH-1 -3);     // 接收板对射频接收数据的CRC校验
    msg->_CRC_[0] = FIFO[ DATA_LENGTH-1 -3 ];     //  发射板对射频数据的CRC校验

    /******* 冷端温度/电池电压还原 *******/
    if(ColdTemp_Vol_Size==2)
    {
        if(USE_FXTH_TEMP)    // 发送8位FXTH87温度数据及电池电压数据
        {
            msg->ColdTemp_Voltage[2] = FIFO[ DATA_LENGTH-3 -3 ];  // FXTH87冷端温度
            msg->ColdTemp_Voltage[3] = FIFO[ DATA_LENGTH-2 -3 ];  // 电池电压
        }
        /*
                    0444 3333 2222 0000
         0000 0000 4443 3332
                             0000 0000 222 xxxx x

                              0444 3333
                   (2000 0000) | (0222 0000)
        * */
        else     // 将TMP117温度数据、电池电压数据 压缩后发送
        {
            msg->ColdTemp_Voltage[0] = FIFO[ DATA_LENGTH-3 -3 ] >> 1;  // 冷端温度
            msg->ColdTemp_Voltage[1] = ( FIFO[ DATA_LENGTH-3 -3 ] << 7 ) |
                                       ( (FIFO[DATA_LENGTH-2 -3]&0xE0)>>1 );  // 0xE0=1110 0000
            msg->ColdTemp_Voltage[3] = ( FIFO[ DATA_LENGTH-2 -3 ] &0x1F ) *8;     // 电池电压
        }
    }

    else if(ColdTemp_Vol_Size==3)
    {
        if(USE_FXTH_TEMP)    // 将TMP117温度数据、电池电压数据压缩后和FXTH87冷端温度数据一同发送
        {
            msg->ColdTemp_Voltage[0] = FIFO[ DATA_LENGTH-4 -3 ] >> 1;  // TMP117冷端温度
            msg->ColdTemp_Voltage[1] = ( FIFO[ DATA_LENGTH-4 -3 ] << 7 )
                                     | ( (FIFO[DATA_LENGTH-3 -3]&0xE0)>>1 );  // 0xE0=1110 0000
      msg->ColdTemp_Voltage[2] = FIFO[ DATA_LENGTH-2 -3 ];       // FXTH87冷端温度
            msg->ColdTemp_Voltage[3] = ( FIFO[ DATA_LENGTH-3 -3 ] &0x1F ) *8;     // 电池电压
        }

        else     // TMP117温度数据和电池电压数据，不压缩直接发送
        {
            msg->ColdTemp_Voltage[0] = FIFO[DATA_LENGTH-4 -3];   // 冷端温度(高8位)
            msg->ColdTemp_Voltage[1] = FIFO[DATA_LENGTH-3 -3];   // 冷端温度(低8位)
            msg->ColdTemp_Voltage[3] = FIFO[DATA_LENGTH-2 -3];   // 电池电压
        }
    }

    else if(ColdTemp_Vol_Size==4)   // 同时发送TMP117温度数据、FXTH87温度数据及电池电压数据，且均不压缩
    {
        msg->ColdTemp_Voltage[0] = FIFO[DATA_LENGTH-5 -3];   // 冷端温度(高8位)
        msg->ColdTemp_Voltage[1] = FIFO[DATA_LENGTH-4 -3];   // 冷端温度(低8位)
        msg->ColdTemp_Voltage[2] = FIFO[DATA_LENGTH-3 -3];   // FXTH87冷端温度
        msg->ColdTemp_Voltage[3] = FIFO[DATA_LENGTH-2 -3];   // 电池电压
    }


    /******* 8通道温度数据还原 *******/
    if(Temp_DataSize*2==16)   // 不压缩数据
    {
    //		for(i=0; i<16; i++)
    //		{
    //			msg->Channel_Temp_Data[16-1-i] = FIFO[ID_Sequ_Size+Temp_DataSize*2-1-i];   // 将FIFO温度(17→2)赋值给Channel_Temp(15→0)
    //		} // (15→0)                       // ID号(2)+16-1-i   (18-1-0)→(18-1-15)  17→2
        memcpy(msg->Channel_Temp_Data, FIFO+2 , 16);     // 将FIFO温度(2→17)赋值给Channel_Temp(0→15)
    }
    /*
    TempData: 0000 3333 2222 1111

    3333 2222   →   0000 3333
                     2222 1111
    1111 3333   →   0000 3333

    2222 1111   →   2222 1111
    * */
    else if(Temp_DataSize*2==12)
    {
        for(i=0; i<4; i++)
        {
            msg->Channel_Temp_Data[16-1-4*i] =  FIFO[ID_Sequ_Size+Temp_DataSize*2-1-3*i];         // 将FIFO温度位赋值给msg->Channel_Temp_Data
            msg->Channel_Temp_Data[16-2-4*i] =  FIFO[ID_Sequ_Size+Temp_DataSize*2-2-3*i]&0x0F;    // 将FIFO温度位赋值给msg->Channel_Temp_Data
            msg->Channel_Temp_Data[16-3-4*i] = (FIFO[ID_Sequ_Size+Temp_DataSize*2-2-3*i]>>4) |
                                               (FIFO[ID_Sequ_Size+Temp_DataSize*2-3-3*i]<<4);     // 将FIFO温度位赋值给msg->Channel_Temp_Data
            msg->Channel_Temp_Data[16-4-4*i] =  FIFO[ID_Sequ_Size+Temp_DataSize*2-3-3*i]>>4;      // 将FIFO温度位赋值给msg->Channel_Temp_Data
            // 以4为一轮 (15→0)                 // ID号(2)+12-1/2/3-3*i   (14-1-0)→(14-3-9)   13→2
        }
    }
    else if(Temp_DataSize*2==10)
    {
        msg->Channel_Temp_Data[15] = FIFO[ID_Sequ_Size+9];                            // FIFO[11] 8~0位 赋给FIFO[17] 8~0位
        msg->Channel_Temp_Data[14] = (FIFO[ID_Sequ_Size+8]&0x03)>>0;                  // FIFO[10] 2~0位 赋给FIFO[16] 2~0位
        msg->Channel_Temp_Data[13] = (FIFO[ID_Sequ_Size+8]>>2) | (FIFO[ID_Sequ_Size+7]<<6);        // FIFO[10] 8~2位和FIFO[9] 2~0位 赋给FIFO[15] 6~0位和8~6位
        msg->Channel_Temp_Data[12] = (FIFO[ID_Sequ_Size+7]&0x0C)>>2;                   // FIFO[9] 4~2位 赋给FIFO[14] 2~0位
        msg->Channel_Temp_Data[11] = (FIFO[ID_Sequ_Size+7]>>4) | (FIFO[ID_Sequ_Size+6]<<4);         // FIFO[9] 8~4位和FIFO[8] 4~0位 赋给FIFO[13] 4~0位和8~4位
        msg->Channel_Temp_Data[10] = (FIFO[ID_Sequ_Size+6]&0x30)>>4;                   // FIFO[8] 6~4位 赋给FIFO[12] 2~0位
        msg->Channel_Temp_Data[9] = (FIFO[ID_Sequ_Size+6]>>6) | (FIFO[ID_Sequ_Size+5]<<2);          // FIFO[8] 8~6位和FIFO[7] 6~0位 赋给FIFO[11] 2~0位和8~2位
        msg->Channel_Temp_Data[8] = (FIFO[ID_Sequ_Size+5]&0xC0)>>6;		                // FIFO[7] 8~6位 赋给FIFO[10] 2~0位
        msg->Channel_Temp_Data[7] = FIFO[ID_Sequ_Size+4];                              // FIFO[6] 8~0位 赋给FIFO[9] 8~0位
        msg->Channel_Temp_Data[6] = (FIFO[ID_Sequ_Size+3]&0x03)>>0;                    // FIFO[5] 2~0位 赋给FIFO[8] 2~0位
        msg->Channel_Temp_Data[5] = (FIFO[ID_Sequ_Size+3]>>2) | (FIFO[ID_Sequ_Size+2]<<6);          // FIFO[5] 8~2位和FIFO[4] 2~0位 赋给FIFO[7] 6~0位和8~6位
        msg->Channel_Temp_Data[4] = (FIFO[ID_Sequ_Size+2]&0x0C)>>2;                    // FIFO[4] 4~2位 赋给FIFO[6] 2~0位
        msg->Channel_Temp_Data[3] = (FIFO[ID_Sequ_Size+2]>>4) | (FIFO[ID_Sequ_Size+1]<<4);          // FIFO[4] 8~4位和FIFO[3] 4~0位 赋给FIFO[5] 4~0位和8~4位
        msg->Channel_Temp_Data[2] = (FIFO[ID_Sequ_Size+1]&0x30)>>4;                    // FIFO[3] 6~4位 赋给FIFO[4] 2~0位
        msg->Channel_Temp_Data[1] = (FIFO[ID_Sequ_Size+1]>>6) | (FIFO[ID_Sequ_Size]<<2);          // FIFO[3] 8~6位和FIFO[2] 6~0位 赋给FIFO[3] 2~0位和8~2位
        msg->Channel_Temp_Data[0] = (FIFO[ID_Sequ_Size]&0xC0)>>6;                    // FIFO[2] 8~6位 赋给FIFO[2] 2~0位
    }


    /******* 序列号&ID号还原 *******/
    /*
     x    xxx xxxx
     ↑       ↑
   ID    序列号
    * */
    if(ID_Sequ_Size==1)        // 压缩数据
    {
        msg->ID_sequence[0] = FIFO[0]>>7;  // ID号
        msg->ID_sequence[1] = FIFO[0]&0x7F;  // 序列号
    }
    else if(ID_Sequ_Size==2)   // 不压缩数据
    {
        msg->ID_sequence[0] = FIFO[1];  // ID号
        msg->ID_sequence[1] = FIFO[0];  // 序列号
    }
}


/* ******************************************************************************
 * @name     dataAnalysis
 * @date     2022-03-26
 * @brief    数据解析，用于将一个数据帧的数据按照协议进行解析
 * @author   Elon Joker
 * @param    none
 * @return   返回解析完成的一帧数据
 * *****************************************************************************/
DataFrame Data::dataAnalysis(QByteArray data)
{
    MsgStruct msg;
    RestoreData(data, &msg);                 // 将压缩后的热电偶通道数据还原至msg中

    DataFrame dataFrame;
    //协议解析
    //解析出数据存储到对应位置
//    dataFrame.head = static_cast<uint16_t>( (data[0]&0xff)<<8 | (data[1]&0xff) );
//    dataFrame.sequence=static_cast<uint16_t>( (data[2]&0xff)<<8 | (data[3]&0xff) );
//    dataFrame.ID=static_cast<uint32_t>( (data[4]&0xff)<<24|(data[5]&0xff)<<16|(data[6]&0xff)<<8|(data[7]&0xff) );
    dataFrame.ID=static_cast<uint8_t>(msg.ID_sequence[0]);
    dataFrame.sequence=static_cast<uint8_t>(msg.ID_sequence[1]);

    if(USE_FXTH_TEMP == 0)   // 如果使用FXTH87作为冷端温度
        dataFrame.Temperature=static_cast<int>((10*static_cast<double>((msg.ColdTemp_Voltage[0]&0xff)<<8
                                                 |(msg.ColdTemp_Voltage[1]&0xff))*TMP_K) +0.5) /10.0;
    else                                         // TMP117冷端温度计算,转换为1位小数,+0.5实现四舍五入
        dataFrame.Temperature=static_cast<double>(msg.ColdTemp_Voltage[2])-55.0;       // FXTH87冷端温度计算

    dataFrame.Voltage=static_cast<double>(msg.ColdTemp_Voltage[3]&0xff) *0.010+1.22;   // 电池电压计算

    dataFrame.CRC=static_cast<uint8_t>(msg._CRC_[0]&0xff);
    dataFrame.CRC_=static_cast<uint8_t>(msg._CRC_[1]&0xff);

    dataFrame.count = static_cast<int>((msg.Debug[0]&0xff)<<8 | (msg.Debug[1]&0xFF));
    dataFrame.channel = static_cast<uint8_t>(msg.Debug[2]);

    for(int i=0;i<8;i++)
    { // 将电压转换为温度
        int16_t data_temp = static_cast<int16_t>((msg.Channel_Temp_Data[0+i*2]&0xff)<<8
                                                |(msg.Channel_Temp_Data[1+i*2]&0xff));
/* 负值温度转化为正值 */
//        if(data_temp>0x360){
//            data_temp = data_temp-0x3ff;
//        }
        dataFrame.data[i]=k_thermocoupleCalculate(
                    data_temp*AD_K*1000,
                    dataFrame.Temperature);
    }

    return dataFrame;
}


/**** 数据解析 + CRC校验 ****/
/**** 数据解析 + CRC校验 ****/
bool Data::dataDealProcess(QByteArray data, DataFrame *dataFrame)
{
    // data是原始数据，dataAnalysis是数据解析函数
    // 将解析后数据放至 DataFrame 结构体中
    // C++结构体可以直接赋值
    DataFrame dataFrameTemp = dataAnalysis(data);

    // uint8_t data_u8[20];
    // memcpy(data_u8,data,sizeof (data_u8));
    // if(dataFrameTemp.CRC != CRC::crc8_chk_value(data_u8,20))//根据协议要求，前20位为实际数据，第21位为CRC

    memcpy(dataFrame,&dataFrameTemp,sizeof (dataFrameTemp));//通过内存copy将数据带出

    if(dataFrameTemp.CRC!=dataFrameTemp.CRC_)
        return false;//CRC校验不通过，抛弃

    //判断是否已经接收到两个ID的数据
    //qDebug()<<dataFrame.ID;
    //最先接收到的两个ID的数据为有效
    if(ID[0]==0xFFFF)
        ID[0] = dataFrameTemp.ID;
    else if(ID[1]==0xFFFF && dataFrameTemp.ID!=ID[0])
        ID[1] = dataFrameTemp.ID;

    return true;
}

/* 文件保存 */
/* 文件保存 */
bool Data::saveData(QString data,QString path,QString fileName)
{
    //创建数据文件夹
    QDir dir;
    if (!dir.exists(path))
    {
      dir.mkpath(path);
    }

    QFile file(path+"/"+fileName);
    if(!file.open(QIODevice::Append|QIODevice::Text))
    {
       WarningWindow *warningWindow = new WarningWindow();
       warningWindow->setTitleStyle("错误",":/warning-on.png");
       warningWindow->showWarningMsg("无法创建文件！");
       warningWindow->setWindowModality(Qt::ApplicationModal);//屏蔽其他窗口
       warningWindow->show();
       QTimer::singleShot(500, [warningWindow](){
           warningWindow->close();
       });
       return false;
    }

    QString dataSaving;
    if(file.size()==0)
    {
       dataSaving.append("Times,ID,Sequence,"
                         "Channel1 temperatures,Channel2 temperatures,"
                         "Channel3 temperatures,Channel4 temperatures,"
                         "Channel5 temperatures,Channel6 temperatures,"
                         "Channel7 temperatures,Channel8 temperatures,"
                         "Cold-junction temperatures,Battery voltages,"
                         "Receive time,Receive channels,"
                         "CRC origin,CRC calculate,CRC correct,"
                         "origin buffer,restore buffer\n");
    }
    dataSaving.append(data);
    std::string str = dataSaving.toStdString();
    const char* ch = str.c_str();

    file.write(ch);//将ch存进test.txt文件夹里面
    file.flush();
    file.close();

    return true;
}

// msg转化为QByteArray
QByteArray Data::MsgToByteArray(MsgStruct *msg)
{
    uint8_t msg_array[27];      // msg数据长度占用27bytes
    QByteArray msg_ByteArray;
    memcpy(msg_array,msg,27);   // 结构体数据拷入数组
    msg_ByteArray.append((char*)msg_array, 27);   // 数组数据保存至QByteArray
    return msg_ByteArray;
}


/*
* param: ktype_vol: k-type sample Voltage (mV)   k型热电偶读到的电压
* param:     Tcj    : cold conjunction temp / (degree/C)  冷端芯片读到的温度
* return: return converted temperature (degree/C)  测点的真实温度
*/

double Data::k_thermocoupleCalculate(double ktype_vol, double Tcj)
{
    double cold = Tcj;

    double e = ktype_vol; // [mV] cold junction equivalent thermocouple voltage

    if (cold < 0.0) {
        // E = sum(i=0 to n) c_i t^i.
        const static double a[] = { // -270 .. 0 degC
            0.000000000000E+00, 0.394501280250E-01, 0.236223735980E-04, -0.328589067840E-06,
            -0.499048287770E-08, -0.675090591730E-10, -0.574103274280E-12, -0.310888728940E-14,
            -0.104516093650E-16, -0.198892668780E-19, -0.163226974860E-22,
        };

        for (int i = 0; i < ARRAY_SIZE(a); i++)
            e += a[i] * pow(cold, i);

    } else {
        // E = sum(i=0 to n) c_i t^i + a0 exp(a1 (t - a2)^2).
        const static double a[] = { // 0 .. 1372 degC
            -0.176004136860E-01, 0.389212049750E-01, 0.185587700320E-04, -0.994575928740E-07,
            0.318409457190E-09, -0.560728448890E-12, 0.560750590590E-15, -0.320207200030E-18,
            0.971511471520E-22, -0.121047212750E-25,
        };

        const static double c[] = {
            0.118597600000E+00,
            -0.118343200000E-03,
            0.126968600000E+03,
        };

        for (int i = 0; i < ARRAY_SIZE(a); i++)
            e += a[i] * pow(cold, i);

        e += c[0] * exp(c[1] * (cold - c[2]) * (cold - c[2]));
    }

//    if (e < -5.891 || e > 54.886) {//冷端温度超过某个范围
//        //fprintf(stderr, "invalid cold junction equivalent thermocouple voltage %lf\n", e);
//        e = static_cast<double>(NAN) ;
////        TRACE1("invalid cold junction");
//    }

    /* t90 = C0 + C1xE + C2xE^2 + ... */
    static const double inverse_coeff[3][10] = {
        {
            0.0000000E+00, 2.5173462E+01, -1.1662878E+00, -1.0833638E+00, -8.9773540E-01,
            -3.7342377E-01, -8.6632643E-02, -1.0450598E-02, -5.1920577E-04, 0.0000000E+00,
        }, {
            0.000000E+00, 2.508355E+01, 7.860106E-02, -2.503131E-01, 8.315270E-02,
            -1.228034E-02, 9.804036E-04, -4.413030E-05, 1.057734E-06, -1.052755E-08,
        }, {
            -1.318058E+02, 4.830222E+01, -1.646031E+00, 5.464731E-02, -9.650715E-04,
            8.802193E-06, -3.110810E-08, 0.000000E+00, 0.000000E+00, 0.000000E+00,
        }
    };

    const double *b;
    if (e < 0.0) {
        b = inverse_coeff[0];
    } else if (e < 20.644) {
        b = inverse_coeff[1];
    } else {
        b = inverse_coeff[2];
    }

    double temp = b[0];
    for (int i = 1; i < ARRAY_SIZE(inverse_coeff[0]); i++)
        temp += b[i] * pow(e, i);

    return temp;
}
