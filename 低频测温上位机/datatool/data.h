#ifndef DATA_H
#define DATA_H

#include <stdint.h>
#include <QQueue>
#include <QByteArray>
#include "protocol.h"


/****** 确定数据压缩还原过程 ******/
// ID号和序列号压缩方式
#define ID_Sequ_Size  1u     // 1表示将ID号、序列号压缩为1byte， 2表示不压缩
// 定义温度数据占用字节数(2*DataSize)，
// 5表示16bytes数据压缩至10bytes，6表示16bytes数据压缩至12bytes，8表示16bytes字节数据不压缩;
#define Temp_DataSize   6u
// 表示冷端温度数据如何发送
// TMP117温度数据16位，FXTH87温度数据8位，二者均可作为冷端温度，电池电压数据8位；
/*  ColdTemp_Vol_Size     USE_FXTH_TEMP
 *        2                   0             将TMP117温度数据、电池电压数据 压缩后发送
 *        2                   1             发送8位FXTH87温度数据及电池电压数据
 *        3                   0             TMP117温度数据和电池电压数据，不压缩直接发送
 *        3                   1             将TMP117温度数据、电池电压数据压缩后和FXTH87冷端温度数据一同发送
 *        4                   1             同时发送TMP117温度数据、FXTH87温度数据及电池电压数据，且均不压缩
 * */
#define ColdTemp_Vol_Size  2u  // 表示冷端温度&电池电压数据大小
#define USE_FXTH_TEMP  0u      // 表示使用FXTH87温度数据

#define DATA_LENGTH  (ID_Sequ_Size+2*Temp_DataSize+ColdTemp_Vol_Size+1+3)   // 一帧数据的长度，+1表示CRC，+3表示通道号+时间

//#define  P (1*0.256/32768.0) //温度数据转换公式，根据硬件配置更换
#define  AD_K   (1*0.256/32768.0)     // ADS1115分辨率
#define  TMP_K  (0.0078125*1)         // tmp117分辨率

#define ARRAY_SIZE(a) static_cast<int>((sizeof (a)/sizeof (a[0])))


class Data
{
public:
    Data();

    static uint32_t ID[2];
    static QQueue<QByteArray> buffer_origin;   // 数据原始缓存（所有接收到数据都在这里）
    static QQueue<QString> buffer_time;    // 接收到一个数据，就会有相应的时间缓存
//    static QQueue<DataFrame> buffer_ID[2];

    static bool dataDealProcess(QByteArray data,DataFrame *dataFrame);
    static bool saveData(QString data,QString path,QString fileName);
    static double k_thermocoupleCalculate(double ktype_vol, double Tcj);
    static DataFrame dataAnalysis(QByteArray data);
    static void RestoreData(QByteArray data, MsgStruct *msg);
    static QByteArray MsgToByteArray(MsgStruct *msg);

};


#endif // DATA_H
