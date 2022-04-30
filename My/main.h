#ifndef __MAIN_H_
#define __MAIN_H_



/*  - 定义针脚 - */


/*  - 关键词 - */
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

void ConfigTimer0(uint ms);
void Init();
void showTime();

#endif