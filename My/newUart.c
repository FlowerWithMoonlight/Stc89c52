#include <reg51.h>
#include "newUart.h"

/*  - 定义针脚 - */


/*  - 关键词 - */




/*  - 方法  - */
/* 串口配置函数，baud-通信波特率 */
void ConfigUART(unsigned int baud) 
{
    TMOD &= 0x0F;   //清零T1的控制位
    TMOD |= 0x20;   //配置T1为模式2
    TH1 = 256 - (11059200/12)/baud;  //计算T0重载值
}

/* 启动串行接收 */
void StartRXD(unsigned char len)
{
    unsigned char cnt = 0; //位接收或发送计数
    TL1 = 256 - ((256-TH1)>>1);  //接收启动时的T0定时为半个波特率周期
    ET1 = 1;        //使能T0中断
    TR1 = 1;        //启动T0
    RxdEnd = 0;     //清零接收结束标志
    RxdOrTxd = 0;   //设置当前状态为接收
}

/* 启动串行发送，dat-待发送字节数据 */
void StartTXD(unsigned char dat)
{
    unsigned char cnt = 0; //位接收或发送计数
    TxdBuf = dat;   //待发送数据保存到发送缓冲器
    TL1 = TH1;      //T0计数初值为重载值
    ET1 = 1;        //使能T0中断
    TR1 = 1;        //启动T0
    PIN_TXD = 0;    //发送起始位
    TxdEnd = 0;     //清零发送结束标志
    RxdOrTxd = 1;   //设置当前状态为发送
}