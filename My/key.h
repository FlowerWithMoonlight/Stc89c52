#ifndef __KEY_H_
#define __KEY_H_


/*  - 定义针脚 - */
sbit Key_IN_4 = P2^7;
sbit Key_IN_3 = P2^6;
sbit Key_IN_2 = P2^5;
sbit Key_IN_1 = P2^4;
sbit Key_OUT_1 = P2^3;
sbit Key_OUT_2 = P2^2;
sbit Key_OUT_3 = P2^1;
sbit Key_OUT_4 = P2^0;

/*  - 关键词 - */
//矩阵按键编号到标准键盘键码的映射表
static unsigned char code KeyCodeMap[4][4] =  {
    { 0x31, 0x32, 0x33, 0x26 }, //数字键1、数字键2、数字键3、向上键
    { 0x34, 0x35, 0x36, 0x25 }, //数字键4、数字键5、数字键6、向左键
    { 0x37, 0x38, 0x39, 0x28 }, //数字键7、数字键8、数字键9、向下键
    { 0x30, 0x1B, 0x0D, 0x27 }  //数字键0、ESC键、  回车键、 向右键
};

static unsigned char KeySta[4][4] = {  //全部矩阵按键的当前状态
    {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
};

static unsigned char backup[4][4] = {  //按键值备份，保存前一次的值
        {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
};

static unsigned char keyout = 0;   //矩阵按键扫描输出索引

static unsigned char keybuf[4][4] = {  //矩阵按键扫描缓冲区
    {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF}
};

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
void Key_Scan();  // 扫描键盘
void KeyDriver(); // 按键驱动函数，检测按键动作，调度相应动作函数，需在主循环中调用
void KeyAction(uchar keycode); // 按键动作函数，根据键码执行相应的操作，keycode-按键键码
#endif