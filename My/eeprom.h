#ifndef __EEPROM_H_
#define __EEPROM_H_



/*  - 定义针脚 - */
sbit I2C_SCL = P3^7;
sbit I2C_SDA = P3^6;

/*  - 关键词 - */
#define I2CDelay()  {_nop_();_nop_();_nop_();_nop_();}


/*  - 方法 -*/

/* 产生总线起始信号 */
void I2CStart();

/* 产生总线停止信号 */
void I2CStop();

/* I2C总线写操作，dat-待写入字节，返回值-从机应答位的值 */
bit I2CWrite(unsigned char dat);

/* I2C总线读操作，并发送非应答信号，返回值-读到的字节 */
unsigned char I2CReadNAK();

/* I2C总线读操作，并发送应答信号，返回值-读到的字节 */
unsigned char I2CReadACK();

/* 将一段内存数据转换为十六进制格式的字符串，
   str-字符串指针，src-源数据地址，len-数据长度 */
void MemToStr(unsigned char *str, unsigned char *src, unsigned char len);

/* E2读取函数，buf-数据接收指针，addr-E2中的起始地址，len-读取长度 */
void E2Read(unsigned char *buf, unsigned char addr, unsigned char len);

/* E2写入函数，buf-源数据指针，addr-E2中的起始地址，len-写入长度 */
void E2Write(unsigned char *buf, unsigned char addr, unsigned char len);
