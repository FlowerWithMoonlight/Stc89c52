#include<REG52.H>
#include"key.h"

void KeyDriver() // 按键驱动函数，检测按键动作，调度相应动作函数，需在主循环中调用
{
    unsigned char i, j;
    for (i=0; i<4; i++)  //循环检测4*4的矩阵按键
    {
        for (j=0; j<4; j++)
        {
            if (backup[i][j] != KeySta[i][j])    //检测按键动作
            {
                if (backup[i][j] != 0)           //按键按下时执行动作
                {
                    KeyAction(KeyCodeMap[i][j]); //调用按键动作函数
                }
                backup[i][j] = KeySta[i][j];     //刷新前一次的备份值
            }
        }
    }
}

void KeyScan() // 按键扫描函数，需在定时中断中调用，调用间隔1ms
{   unsigned char i;
    //将一行的4个按键值移入缓冲区
    keybuf[keyout][0] = (keybuf[keyout][0] << 1) | Key_IN_1;
    keybuf[keyout][1] = (keybuf[keyout][1] << 1) | Key_IN_2;
    keybuf[keyout][2] = (keybuf[keyout][2] << 1) | Key_IN_3;
    keybuf[keyout][3] = (keybuf[keyout][3] << 1) | Key_IN_4;
    //消抖后更新按键状态
    for (i=0; i<4; i++)  //每行4个按键，所以循环4次
    {
        if ((keybuf[keyout][i] & 0x0F) == 0x00)
        {   //连续4次扫描值为0，即4*4ms内都是按下状态时，可认为按键已稳定的按下
            KeySta[keyout][i] = 0;
        }
        else if ((keybuf[keyout][i] & 0x0F) == 0x0F)
        {   //连续4次扫描值为1，即4*4ms内都是弹起状态时，可认为按键已稳定的弹起
            KeySta[keyout][i] = 1;
        }
    }
    //执行下一次的扫描输出
    keyout++;                //输出索引递增
    keyout = keyout & 0x03;  //索引值加到4即归零
    switch (keyout)          //根据索引，释放当前输出引脚，拉低下次的输出引脚
    {
        case 0: Key_OUT_4 = 1; Key_OUT_1 = 0; break;
        case 1: Key_OUT_1 = 1; Key_OUT_2 = 0; break;
        case 2: Key_OUT_2 = 1; Key_OUT_3 = 0; break;
        case 3: Key_OUT_3 = 1; Key_OUT_4 = 0; break;
        default: break;
    }
}



