#ifndef __WIFI_H_
#define __WIFI_H_

#define uint unsigned int
#define uchar unsigned char

/*  - 定义针脚 - */
sbit light = P0^0;

/*  - 关键词 - */
uchar Recive_table[15];
uint i;

/*  - 方法  - */
void delay_ms(uint ms); // 延时 xxx ms

void delay_us(uchar us); // 延时 xxx us

void Usart_Init(); // 初始话串口

void SENT_At(uchar *At_Comd); // 串口发送信息

void WIFI_Init(); // 初始化WIFI端口










