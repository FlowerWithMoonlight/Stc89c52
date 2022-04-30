#ifndef __LCD_H_
#define __LCD_H_


/*  - 定义针脚 - */
sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E  = P1^5;

/* 
sbit LCD1602_RS = P1^2;
sbit LCD1602_RW = P1^3;
sbit LCD1602_E  = P1^5;
*/

/*  - 关键词 - */
#define LCD1602_DB P0
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

#ifndef ulong 
#define ulong unsigned long
#endif



/*  - 方法  - */
void LcdWaitReady();   // 等待液晶初始话ok
void LcdWriteCmd(uchar cmd); //向LCD1602液晶写入一字节命令，cmd-待写入命令值
void LcdWriteDat(uchar dat); // 向LCD1602液晶写入一字节数据，dat-待写入数据值
void LcdSetCursor(uchar x, uchar y); //设置显示RAM起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标
void LcdShowStr(uchar x, uchar y, uchar *str); //在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针
void LcdAreaClear(uchar x, uchar y, uchar len); //区域清除，清除从(x,y)坐标起始的len个字符位
void InitLcd1602(); // 初始化1602液晶 
void LcdFullClear();

#endif
