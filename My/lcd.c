#include<REG52.H>
#include"lcd.h"
#include"date.h"

/* lec.h */
void LcdWaitReady(){   // 等待液晶准备好
    uchar sta;
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do {
        LCD1602_E = 1;
        sta = LCD1602_DB; //读取状态字
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7等于1表示液晶正忙，重复检测直到其等于0为止
}

void LcdWriteCmd(uchar cmd) // 向LCD1602液晶写入一字节命令，cmd-待写入命令值
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
}

void LcdWriteDat(uchar dat) // 向LCD1602液晶写入一字节数据，dat-待写入数据值
{
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
}

void LcdSetCursor(uchar x, uchar y)  // 设置显示RAM起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标
{
    uchar addr;
    
    if (y == 0)  //由输入的屏幕坐标计算显示RAM的地址
        addr = 0x00 + x;  //第一行字符地址从0x00起始
    else
        addr = 0x40 + x;  //第二行字符地址从0x40起始
    LcdWriteCmd(addr | 0x80);  //设置RAM地址
}

void LcdShowStr(uchar x, uchar y, uchar *str) // 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针
{
    LcdSetCursor(x, y);   //设置起始地址
    while (*str != '\0')  //连续写入字符串数据，直到检测到结束符
    {
        LcdWriteDat(*str++);
    }
}

void LcdAreaClear(uchar x, uchar y, uchar len) // 区域清除，清除从(x,y)坐标起始的len个字符位
{
    LcdSetCursor(x, y);   //设置起始地址
    while (len--)         //连续写入空格
    {
        LcdWriteDat(' ');
    }
}

void InitLcd1602() // 初始化1602液晶
{
    LcdWriteCmd(0x38);  //16*2显示，5*7点阵，8位数据接口
    LcdWriteCmd(0x0C);  //显示器开，光标关闭
    LcdWriteCmd(0x06);  //文字不动，地址自动+1
    LcdWriteCmd(0x01);  //清屏
}

void LcdFullClear(){
    LcdWriteCmd(0x01);
}




