#ifndef __NEWUART_H_
#define __NEWUART_H_

/*  - 定义针脚 - */
sbit PIN_TXD  = P1^3;
sbit PIN_RXD  = P1^2;

/*  - 关键词 - */
bit RxdOrTxd = 0;  //指示当前状态为接收还是发送
bit RxdEnd = 0;    //接收结束标志
bit TxdEnd = 0;    //发送结束标志
unsigned char RxdBuf = 0;  //接收缓冲器
unsigned char TxdBuf = 0;  //发送缓冲器
static unsigned char cnt = 0; // 定义位数
/*  - 方法  - */
void ConfigUART(unsigned int baud);
void StartTXD(unsigned char dat);
void StartRXD(unsigned char len);


