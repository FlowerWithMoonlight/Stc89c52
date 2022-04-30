#include<REG52.H>
#include"ledBuzzer.h"
#include"delay.h"

/*  - 定义针脚 - */


/*  - 关键词 - */



/*  - 方法 -*/
void Beep_Times(unsigned char times) //蜂鸣器函数
{
	unsigned char i=0;
	for(i=0;i<times;i++)
	{
		 beep=0;
		 delay(200);
		 beep=1;
		 delay(200);
	}
}

void floweLed(){   // 左右流水灯
    P0=~led;
    delay(1000);
    if(flag==0)
    {
    led=led>>1;
    if(led==0x03){
    LedFlag = 1;
    }
    }
    else 
    {
    led=led<<1;
    if(led==0x10){
    LedFlag = 0;
    }
    }
}

void shutLed(){
    P0 = 0x1F;
    delay(100);
    P0 = 0x00;
}

