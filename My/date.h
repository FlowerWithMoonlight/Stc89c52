#ifndef __DATE_H_
#define __DATE_H_


/*  - 定义针脚 - */
sbit DS1302_CE = P1^7;
sbit DS1302_CK = P3^5;
sbit DS1302_IO = P3^4;

/*  - 关键词 - */
struct sTime
{
    unsigned int year; // 年
    unsigned char month; // 月
    unsigned char day; // 天
    unsigned char hour; // 小时
    unsigned char min; // 分钟
    unsigned char sec; // 秒
    unsigned char week;  // 周
};


/*  - 方法  - */
void InitDS1302();

void SetDS1302(unsigned int year,unsigned char mon,unsigned char day,unsigned char hour,unsigned char min,unsigned char sec,unsigned char week);

void DS1302SingleWrite(unsigned char reg, unsigned char dat);

void DS1302ByteWrite(unsigned char dat);

void DS1302BurstWrite(unsigned char *dat);

unsigned char DS1302ByteRead();

void DS1302BurstRead(unsigned char *dat);

unsigned char DS1302SingleRead(unsigned char reg);

void SetRealTime(struct sTime *time);

void GetRealTime(struct sTime *time);

unsigned char Dec2Bcd(unsigned char dec);

unsigned char Bcd2Dec(unsigned char bcd);

unsigned int Dec2Bcd2(unsigned int dec);

#endif