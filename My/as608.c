#include<REG52.H>
#include"as608.h"
#include"newUart.h"

 //发送包头
void AS608_Cmd_Send_Pack_Head(void)
{
	int i;
    
	for(i=0;i<6;i++) //包头
	{
		StartTXD(AS608_Pack_Head[i]);   
	}		
}

//发送指令
void AS608_Cmd_Check(void)
{
	int i=0;
	AS608_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<10;i++)
	{		
		StartTXD(AS608_Get_Device[i]);
	}
}

//接收反馈数据缓冲
void AS608_Receive_Data(unsigned char ucLength)
{
	unsigned char i;	
    StartRXD(ucLength);			 
	// for (i=0;i<ucLength;i++){
        
	// 	AS608_RECEICE_BUFFER[i] = Uart_Receive_Byte();
    // }
}

//FINGERPRINT_获得指纹图像命令
void AS608_Cmd_Get_Img(void)
{
    unsigned char i;
    AS608_Cmd_Send_Pack_Head(); //发送通信协议包头
    for(i=0;i<6;i++) //发送命令 0x1d
	{
       StartTXD(AS608_Get_Img[i]);
	}
}

//将图像转换成特征码存放在Buffer1中
void FINGERPRINT_Cmd_Img_To_Buffer1(void)
{
 	unsigned char i;
	AS608_Cmd_Send_Pack_Head(); //发送通信协议包头      
   	for(i=0;i<7;i++)   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
	{
		StartTXD(AS608_Img_To_Buffer1[i]);
	}
}
//将图像转换成特征码存放在Buffer2中
void FINGERPRINT_Cmd_Img_To_Buffer2(void)
{
	unsigned char i;
	AS608_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<7;i++)   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
	{
		StartTXD(AS608_Img_To_Buffer2[i]);
	}
}

//搜索全部用户999枚
void AS608_Cmd_Search_Finger(void)
{
	unsigned char i;	   	    
	AS608_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<11;i++)
	{
		StartTXD(AS608_Search[i]);   
	}
}

//转换成特征码
void AS608_Cmd_Reg_Model(void)
{
	unsigned char i;	   		    
	AS608_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<6;i++)
	{
		StartTXD(AS608_Reg_Model[i]);   
	}
}

//删除指纹模块里的所有指纹模版
void FINGERPRINT_Cmd_Delete_All_Model(void)
{
	unsigned char i;    
    AS608_Cmd_Send_Pack_Head(); //发送通信协议包头   
    for(i=0;i<6;i++) //命令删除指纹模版
	{
      	StartTXD(AS608_Delete_All_Model[i]);   
	}	
}

//保存指纹
void AS608_Cmd_Save_Finger( unsigned int storeID )
{
	unsigned long temp = 0;
	unsigned char i;
	AS608_Save_Finger[5] =(storeID&0xFF00)>>8;
	AS608_Save_Finger[6] = (storeID&0x00FF);
	for(i=0;i<7;i++)   //计算校验和
		temp = temp + AS608_Save_Finger[i]; 
	AS608_Save_Finger[7]=(temp & 0x00FF00) >> 8; //存放校验数据
	AS608_Save_Finger[8]= temp & 0x0000FF;		   
	AS608_Cmd_Send_Pack_Head(); //发送通信协议包头	
	for(i=0;i<9;i++)  
		StartTXD(AS608_Save_Finger[i]);      //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
}

//查看当前指纹库中指纹模板数
int AS608_number_of_fingers()
{
 	int num=1;//默认模板库中有一个模板
	uchar i=0;
	AS608_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<6;i++)
	{
	  	StartTXD(AS608_Get_Templete_Count[i]);
	}
	AS608_RECEICE_BUFFER[9]=1;//方便后续判断是否接收成功
	StartRXD(14);//接收数据
	if(AS608_RECEICE_BUFFER[9]==0) //接收成功
	{
	 	num=AS608_RECEICE_BUFFER[10]*256+AS608_RECEICE_BUFFER[11];//拼接模板总个数			
	}
	return num;
}
