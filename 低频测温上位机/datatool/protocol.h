#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef struct frame
{
    uint8_t sequence;
    uint8_t ID;
    double data[8];
    double Temperature;
    double Voltage;
    uint8_t CRC;
    uint8_t CRC_;
    int count;
    uint8_t channel;
} DataFrame;

typedef struct Msg
{
    uint8_t  ID_sequence[2];         // 数据序列&ID号
    uint8_t  Channel_Temp_Data[16];  // 8通道温度数据
    uint8_t  ColdTemp_Voltage[4];    // 冷端温度&电池电压
    uint8_t  _CRC_[2];   // CRC校验数据
    uint8_t  Debug[3];   // 其他调试信息（程序运行时间，天线号）
} MsgStruct;

#endif // PROTOCOL_H
