#ifndef __AS608_H_
#define __AS608_H_

/*  - 定义针脚 - */



/*  - 关键词 - */

//AS608指纹模块
volatile unsigned char AS608_RECEICE_BUFFER[32]; //volatile:系统总是重新从它所在的内存读取数据，即使它前面的指令刚刚从该处读取过数据
//volatile unsigned char AS608_RECEICE_BUFFER;
//FINGERPRINT通信协议定义
code unsigned char AS608_Get_Device[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};//口令验证
code unsigned char AS608_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  //协议包头
code unsigned char AS608_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    //获得指纹图像
code unsigned char AS608_Get_Templete_Count[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 }; //获得模版总数
code unsigned char AS608_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; //搜索指纹搜索范围0 - 999,使用BUFFER1中的特征码搜索
code unsigned char AS608_Search_0_9[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x00,0x13,0x00,0x21}; //搜索0-9号指纹
code unsigned char AS608_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08}; //将图像放入到BUFFER1
code unsigned char AS608_Img_To_Buffer2[7] = {0x01,0x00,0x04,0x02,0x02,0x00,0x09}; //将图像放入到BUFFER2
code unsigned char AS608_Reg_Model[6] = {0x01,0x00,0x03,0x05,0x00,0x09}; //将BUFFER1跟BUFFER2合成特征模版
code unsigned char AS608_Delete_All_Model[6] = {0x01,0x00,0x03,0x0d,0x00,0x11};//删除指纹模块里所有的模版
volatile unsigned char AS608_Save_Finger[9] = {0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};//将BUFFER1中的特征码存放到指定的位置



/*  - 方法  - */

 //发送包头
void AS608_Cmd_Send_Pack_Head(void);

//发送指令
void AS608_Cmd_Check(void);

//接收反馈数据缓冲
void AS608_Receive_Data(unsigned char ucLength);

//FINGERPRINT_获得指纹图像命令
void AS608_Cmd_Get_Img(void);

//将图像转换成特征码存放在Buffer1中
void FINGERPRINT_Cmd_Img_To_Buffer1(void);

//将图像转换成特征码存放在Buffer2中
void FINGERPRINT_Cmd_Img_To_Buffer2(void);

//搜索全部用户999枚
void AS608_Cmd_Search_Finger(void);

//转换成特征码
void AS608_Cmd_Reg_Model(void);

//删除指纹模块里的所有指纹模版
void FINGERPRINT_Cmd_Delete_All_Model(void);

//保存指纹
void AS608_Cmd_Save_Finger( unsigned int storeID );

//查看当前指纹库中指纹模板数
int AS608_number_of_fingers();

//判断初始化
void Device_Check(void);

//删除所有储存的指纹库
void AS608_Delete_All_Fingerprint();

//搜索指纹
void AS608_Find_Fingerprint();

//添加指纹
void AS608_Add_Fingerprint();

