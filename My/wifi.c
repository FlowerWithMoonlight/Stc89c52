#include<REG52.H>
#include"wifi.h"

void delay_ms(uint ms)
{
	uchar i,j;
	for(i=ms;i>0;i--)
	for(j=120;j>0;j--);
}

void delay_us(uchar us)
{
	while(us--);
}

void Usart_Init()
{
	SCON = 0x50;
	PCON = 0;
}

void SENT_At(uchar *At_Comd)
{
	ES = 0;
	while(*At_Comd!='\0')
	{
		SBUF = *At_Comd;
		while(!TI);
		TI = 0;
		delay_us(5);
		At_Comd++;
	}
}

void WIFI_Init()
{
	SENT_At("AT+CIPMUX=1\r\n");
	delay_ms(1000);
	SENT_At("AT+CIPSERVER=1,8080\r\n");
	delay_ms(1000);
	ES = 1;
}

