#include<REG52.H>
#include"delay.h"

//延时函数
void delayus(int i)
{
	while(--i);
}

void delay(int ms)
{
    unsigned int i,j;
	for(i=time;i>0;i--)
	for(j=0;j<921;j++);
}
