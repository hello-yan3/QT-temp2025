/* CRC校验 */
#include "crc.h"

#include <QDebug>

CRC::CRC()
{

}

uint16_t CRC:: crc16ForModbus(const QString &hexText)
{
    QByteArray data  = QByteArray::fromHex(hexText.toLocal8Bit());

    uint8_t buf;
    uint16_t crc16 = 0xFFFF;

    for ( auto i = 0; i < data.size(); ++i )
    {
        buf = static_cast<uint8_t>((data.at(i)^crc16));
        crc16 >>= 8;
        crc16 ^= crc16Table[ buf ];
    }
//    qDebug()<<QString("%1").arg(crc16 , 4, 16, QLatin1Char('0'));
//   return  QString("%1").arg(crc16 , 4, 16, QLatin1Char('0'));   //拼凑成4个16进制字符，空位补0
    return crc16;
}

/* ******************************************************************************
 * @name     function name
 * @date     2022-03-24
 * @brief    CRC校验类型：CRC8/MAXIM
             多项式：X8+X5+X3+X2+X+1
             Poly：0010 1111  0x2f
             高位放到后面就变成 1111 0100 0xf4
 * @author   Elon Joker
 * @param    message 校验信息的地址
 *           len 需要校验的长度
 * @return   校验结果
 * *****************************************************************************/
uint8_t CRC::crc8_chk_value(uint8_t *message, uint8_t len)
{
    unsigned char i;
    unsigned char crc=0xAA;
    while(len--!=0)
    {
        for(i=0x80; i!=0; i/=2)
        {
            if((crc&0x80)!=0)
            {
                crc*=2;
                crc^=0X2F;
            } /* 余式CRC 乘以2 再求CRC */
            else
                crc*=2;
            if((*message&i)!=0)
                crc^=0X2F; /* 再加上本位的CRC */
        }
        message++;
    }
    return(crc);
}
