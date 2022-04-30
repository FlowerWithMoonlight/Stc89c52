#include<REG52.H>
#include"motor.h"
signed long beats = 0;  //电机转动节拍总数

void StartMotor(signed long angle)
{
    //在计算前关闭中断，完成后再打开，以避免中断打断计算过程而造成错误
    EA = 0;
    beats = (angle * 4076) / 360; //实测为4076拍转动一圈
    EA = 1;
    TurnMotor();
}

void StopMotor()
{
    EA = 0;
    beats = 0;
    EA = 1;
}
void TurnMotor()
{
    unsigned char tmp;  
    static unsigned char Motorindex = 0;  
    unsigned char code BeatCode[4] = {  
         0xC,0x9,0x3, 0x6
    };
    
    if (beats != 0)  
    {
        if (beats > 0)  
        {
            Motorindex++;               
            Motorindex = Motorindex & 0x07;  
            beats--;               
        }
        else            
        {
            Motorindex--;               
            Motorindex = Motorindex & 0x07;  
            beats++;               
        }
        tmp = MotorPin;                    
        tmp = tmp & 0xF0;            
        tmp = tmp | BeatCode[Motorindex]; 
        MotorPin  = tmp;                   
    }
    else  
    {
        MotorPin = MotorPin | 0x0F;
    }
}
