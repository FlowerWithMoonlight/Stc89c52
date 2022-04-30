#ifndef __ledBuzzer_H_
#define __ledBUzzer_H_


/*  - 定义针脚 - */
sbit buzzer = P1^6;
sbit ENLED = P1^4

/*  - 关键词 - */
bit LedFlag = 1;
unsigned char led=0x0f;

/*  - 方法 -*/
void Beep_Times(unsigned char times); //蜂鸣器函数

void floweLed();   // 左右流水灯

void shutLed();