#include<REG52.H>
#include"delay.h"
#include"main.h"
#include"key.h"
#include"lcd.h"
#include"date.h"
#include"motor.h"
#include"as608.h"
#include"newUart.h"
#include"ledBuzzer.h"


/*  全局变量  */
uchar T0RH = 0;  //T0重载值的高字节
uchar T0RL = 0;  //T0重载值的低字节
uchar Slen = 3;  //密码长度
uchar tmr200ms = 0; // 定时器中的记录定时200ms的标志变量
bit flag200ms = 1; // 200ms定时标志
struct sTime bufTime; // 日期时间缓冲
uchar weekShow[7][4] = {
    "Mon","Tue","Wen","Thu","Fri","Sat","Sun"
};
uchar i = 0; // wifi模块

uint finger_id[2] = {0}; // 指纹id
uint password[8] = {0}; // 密码
uint inPassword[8] = {0}; // 输入密码
uint wordLen = 0; // 密码长度


uchar Admin = 0; // 是否进入 管理员界面
uchar Add = 0; // 是否进入 添加指纹界面
uchar Del = 0; // 是否进入 删除指纹界面
uchar Upd = 0; // 是否进入 更新密码界面
uchar Srt = 0; // 是否进入 写入密码界面

uchar next = 0; // 是否 回车
uchar Esc = 0; // 是否摁下 esc
uchar Left = 0; // 是否摁下 左键
uchar right = 0; // 是否摁下 右键
uchar up = 0; // 是否摁下上键
uchar down = 0;// 是否摁下 下键

/* 实现方法  */

/* main.c */
void ConfigTimer0(uint ms){
    ulong temp;
    temp = 11059200/12; // 计数频率
    temp = (temp * ms) / 1000; // 计算所需的计数值
    temp = 65536 - temp; // 计算重载值
    temp = temp + 28; // 补偿中断延时误差
    T0RH = (unsigned char)(temp>>8);  //定时器重载值拆分为高低字节
    T0RL = (unsigned char)temp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}
void Init(){// 初始化
    EA = 1;
    uchar psec = 0xAA; // 初始化时间
    ConfigTimer0(1);
    InitLcd1602();
    delay(100);
    InitDS1302();
    delay(100);
    Usart_Init();
    delay(100);
	WIFI_Init();
    delay(100);
    ConfigUART(9600);
    delay(100);
}
void showTime(){ // 显示时间
    LcdShowStr(3,0,"20  -  -  ");
    LcdShowStr(4,1,"  :  :  ");
        if(flag200ms){
            flag200ms = 0;
             GetRealTime(&bufTime);
            if(psec != bufTime.sec){
                RefreshTimeShow();
                psec = bufTime.sec;
            }
        }    
}
bit password_check(uchar *inPassword){ // 密码是否正确
    uchar x;
    for(x=0;x<8;x++){
        if(password[x]!=inPassword[x]) return 0;
    }
    return 1;
}

/*  lcd.c */
void LcdWaitReady();   // 等待液晶初始话ok
void LcdWriteCmd(uchar cmd); //向LCD1602液晶写入一字节命令，cmd-待写入命令值
void LcdWriteDat(uchar dat); // 向LCD1602液晶写入一字节数据，dat-待写入数据值
void LcdSetCursor(uchar x, uchar y); //设置显示RAM起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标
void LcdShowStr(uchar x, uchar y, uchar *str); //在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针
void LcdAreaClear(uchar x, uchar y, uchar len); //区域清除，清除从(x,y)坐标起始的len个字符位
void InitLcd1602(); // 初始化1602液晶 

/* 将一个BCD码字节显示到屏幕上，(x,y)-屏幕起始坐标，bcd-待显示BCD码 */
void ShowBcdByte(unsigned char x, unsigned char y, unsigned char bcd);
void RefreshTimeShow();

void loading(){
    unsigned char i =0;
    for(;i<10;i++){
        LcdShowStr(2+i,1,"〓");
        delay(200);
    }
}

/* key.c */
void KeyDriver();// 按键驱动函数，检测按键动作，调度相应动作函数，需在主循环中调用
void KeyAction(uchar keycode)  // 按键动作函数，根据键码执行相应的操作，keycode-按键键码
{
    if ((keycode>=0x30) && (keycode<=0x39))  //输入0-9的数字 密码
    {
        
        if(Slen == 3) LcdFullClear();
        LcdShowStr(0,0,"Please Input :");
        inPassword[wordLen] = keycode-0x30;
        wordLen++; Slen++;
        if(Slen-3>8){  // 设置 密码最大长度为 8
            LcdFullClear();
            LcdShowStr(0,1," password wrong!");
            delay(100);
            LcdFullClear();
            Slen = 3;
            Admin = 0;
            wordLen = 0;
        }else if(Slen-3 == 8){
            if(Upd){
                E2Write(inPassword,0x8E,sizeof(password));
                LcdFullClear();
				LcdShowStr(0,0," update success ");
            }
            bit ans = 0;
            ans = password_check(inPassword);
            if(ans){
                LcdFullClear();
				LcdShowStr(0,0,"password success ");
                LcdShowStr(0,1,"Welcome to 622!");
				StartMotor(90);		
				Beep_Times(1);
            }else{
            LcdFullClear();
            LcdShowStr(0,1," password wrong! ");
            delay(100);
            LcdFullClear();  
            Slen = 3;
            Admin = 0;
            wordLen = 0;
            }
        }else{
        LcdShowStr(Slen,1,"*");            
        }
        
    }
    else if (keycode == 0x26)  //上建 增加指纹 
    {
        if(Admin == 0){
            LcdFullClear();
            LcdShowStr(0,0,"Please in admin");
            LcdShowStr(0,1,"Up to got in");
        }
        // 设置一个锁 普通模式无论怎么样都进不入
        else{
        Add = 1;
        Slen = 3;
        LcdFullClear();
        AS608_Add_Fingerprint();
        }
        /* 指纹录入模块 */
        //LcdShowStr(15,1,"up");
    }
    else if (keycode == 0x28)  //下键 进入删除界面
    {
        // 设置一个锁 普通模式无论怎么样都进不入
        if(Admin == 0){
            LcdFullClear();
            LcdShowStr(0,0,"Please in admin");
            LcdShowStr(0,1,"Up to got in");
        }else{
        Slen = 3;
        LcdFullClear();
        LcdShowStr(0,0,"Del  Fingerprint");
        LcdShowStr(0,1,"Input The Id:");
        }
        /* 指纹删除模块 */  // 设置一个全局变量 表示是否是进入删除状态

        // LcdShowStr(15,1,"down");
    }
    else if (keycode == 0x0D)  //回车键，进入管理员界面
    {
        Admin = 1;
        Slen = 3;
        LcdFullClear();
        LcdShowStr(0,0,"  Administrator");
        LcdShowStr(0,1,"Input password");
        /* 第二个全局变量 */ // 用不同的标识符来表示当回车提交做的事情
        
        /* 模块功能 */
        // up - > 添加指纹
        // down - > 删除指纹
        // left  - > 修改密码



        // LcdShowStr(15,1,"next");
    }
    else if(keycode == 0x25){// 向左 1.修改密码 2.递减id号
        if(Add == 1){
            Left = 1;
            continue;
        }
        Slen = 3;
        // 设置一个锁 普通模式无论怎么样都进不入
        if(Admin==0){
            LcdFullClear();
            LcdShowStr(0,0,"Please in admin");
            LcdShowStr(0,1,"Up to got in");
        }else{
        LcdFullClear();
        LcdShowStr(0,0,"Update password");
        LcdShowStr(0,1,"Input adpassword");
        Upd = 1;
        }
    }
    else if(keycode == 0x27) // 向右 清除/恢复原状态
    {
        LcdFullClear();
        Slen = 3;
        if()
    }
    else if(keycode == 0x1B){ // esc
        Esc = 1;
    }
}
void KeyScan();

/* date.c */
void InitDS1302();
void SetDS1302(unsigned int year,unsigned char mon,unsigned char day,unsigned char hour,unsigned char min,unsigned char sec,unsigned char week);
void DS1302SingleWrite(uchar reg, uchar dat);
void DS1302ByteWrite(uchar dat);
void DS1302BurstWrite(uchar *dat);
unsigned char DS1302ByteRead();
void DS1302BurstRead(uchar *dat);
unsigned char DS1302SingleRead(uchar reg);
void SetRealTime(struct sTime *time);
void GetRealTime(struct sTime *time);

/* date.h */
void ShowBcdByte(uchar x, uchar y, uchar bcd)//将一个BCD码字节显示到屏幕上，(x,y)-屏幕起始坐标，bcd-待显示BCD码
{
    unsigned char str[4];
    str[0] = (bcd >> 4) + '0';
    str[1] = (bcd&0x0F) + '0';
    str[2] = '\0';

    // unsigned char str[3];
    // str[0] = bcd ;
    // str[1] = '\0';
    
    // unsigned char str[2];
    // str[0] = bcd;
    LcdShowStr(x, y,str);
}
void RefreshTimeShow() /* 刷新日期时间的显示 */
{
    ShowBcdByte(5,  0, bufTime.year);
    ShowBcdByte(8,  0, bufTime.month);
    ShowBcdByte(11, 0, bufTime.day);
    ShowBcdByte(4,  1, bufTime.hour);
    ShowBcdByte(7,  1, bufTime.min);
    ShowBcdByte(10, 1, bufTime.sec);
    LcdShowStr(13, 1, weekShow[Bcd2Dec(bufTime.week)]);

    // ShowBcdByte(5,  0, InitTime.year);
    // ShowBcdByte(8,  0, InitTime.month);
    // ShowBcdByte(11, 0, InitTime.day);
    // ShowBcdByte(4,  1, InitTime.hour);
    // ShowBcdByte(7,  1, InitTime.min);
    // ShowBcdByte(10, 1, InitTime.sec);
}

/* motor.c */
void StartMotor(signed long angle); //步进电机启动函数，angle-需转过的角度
void StopMotor(); // 步进电机停止函数
void TurnMotor(); // 电机转动控制函数

/* wifi.c */
void delay_ms(uint ms); // 延时 xxx ms
void delay_us(uchar us); // 延时 xxx us
void Usart_Init(); // 初始话串口
void SENT_At(uchar *At_Comd); // 串口发送信息
void WIFI_Init(); // 初始化WIFI端口

/* as608.c */
void Device_Check(void){
    	unsigned char i=0;
		AS608_RECEICE_BUFFER[9]=1;				           //串口数组第九位可判断是否通信正常
        LcdShowStr(0,0,"Init AS ing..");
		LcdShowStr(4,1,"Loading");	           //设备加载中界面							   
		for(i=0;i<5;i++)						           //进度条式更新，看起来美观
		{
			LcdShowStr(12+i,1,"*");	                       //42对应ASIC码的 *
			Delay_ms(20);						           //控制进度条速度
		}		
        LcdFullClear();					
        LcdShowStr(0,0,"Init AS ing..");
		LcdShowStr(0,1,"Init As fail!");      //液晶先显示对接失败，如果指纹模块插对的话会将其覆盖	
		AS608_Cmd_Check();								//单片机向指纹模块发送校对命令
		AS608_Receive_Data(12);							//将串口接收到的数据转存
 		if(AS608_RECEICE_BUFFER[9] == 0)					//判断数据低第9位是否接收到0
		{
		LcdFullClear();					
        LcdShowStr(0,0,"Init AS ing..");
		LcdShowStr(0,1,"Init As succes!");	//符合成功条件则显示对接成功
		}

}

void AS608_Add_Fingerprint(){ //添加指纹
	unsigned char id_show[]={0,0,0};
	unsigned char i = 7;
    LcdFullClear();
    while (1)
    {
    LcdShowStr(0,0," ADD  Finger ");
    LcdShowStr(0,0,"  ID  is  ");    
    if(Esc == 1){
	LcdFullClear();
    LcdShowStr(0,0,"Exiting...");
    loading();
    Add = 0;
    Esc = 0;
    break;
    }
    if(Left == 1){
        if(finger_id == 99) finger_id = 1;
        else finger_id++;
        Left = 0;
    }
    LcdShowStr(5,1,finger_id/10);
    LcdShowStr(6,1,finger_id%10);
    if(next == 1){
        LcdFullClear();
        LcdShowStr(0,0,"Please in finger");
        if(Esc == 1){
	    LcdFullClear();
        LcdShowStr(0,0,"Exiting...");
        loading();
        Add = 0;
        Esc = 0;
        next = 0;
        break;
        }
		AS608_Cmd_Get_Img(); //获得指纹图像
		AS608_Receive_Data(12);
		//判断接收到的确认码,等于0指纹获取成功
		if(AS608_RECEICE_BUFFER[9]==0)
		{
			Delay_ms(100);
			FINGERPRINT_Cmd_Img_To_Buffer1();
			AS608_Receive_Data(12);
			LcdFullClear();
            LcdShowStr(0,0,"Successful entry");
			Beep_Times(10);
		    delay(1000);
            break;
        }else{
			LcdFullClear();
            LcdShowStr(0,0,"Fail entry");
            LcdShowStr(0,1,"Please try again");
            delay(100);
        }
        Esc = 0; next = 0;
        }
    }
    Add = 0; Admin =0; Esc = 0; next = 0;
}

void AS608_Find_Fingerprint(){ // 搜索指纹
    unsigned char id;
    unsigned char cnt = 0;
    unsigned char zxc = 0;
    do
	{
        if(cnt == 3){
            LcdFullClear();
            LcdShowStr(0,0,"Error thrid !!!");
            LcdShowStr(0,1,"  Warning!!");
            Beep_Times(50);
            for(;zxc<100;zxc++) shutLed();
            break;
        }
        cnt++;
        LcdFullClear();
		LcdShowStr(0,0," Please  finger ");
		AS608_Cmd_Get_Img(); //获得指纹图像
		AS608_Receive_Data(12);		
		//判断接收到的确认码,等于0指纹获取成功
		if(AS608_RECEICE_BUFFER[9]==0)
		{			
			delay(100);
			FINGERPRINT_Cmd_Img_To_Buffer1();
			AS608_Receive_Data(12);		
			AS608_Cmd_Search_Finger();
			AS608_Receive_Data(16);			
			if(AS608_RECEICE_BUFFER[9] == 0) //搜索到  
			{
				//解锁成功//
                LcdFullClear();
				LcdShowStr(0,0," Search success ");
                LcdShowStr(0,1,"    ID is       ");
				Beep_Times(1);					
				//拼接指纹ID数
				id = AS608_RECEICE_BUFFER[10]*256 + AS608_RECEICE_BUFFER[11];					
				 //指纹iD值显示处理 
                LcdShowStr(16,1,id);
				//配置IO口，执行开锁操作
				StartMotor(90);		
                break;					
			}
			else //没有找到
			{
                LcdFullClear();
				LcdShowStr(0,0," Search  failed ");
                LcdShowStr(0,1," Please in it ");
				Beep_Times(20);
			}
		}		
		}while(!Esc);
    Esc = 0; Admin =0;
}

void AS608_Delete_All_Fingerprint(){ // 删除所有指纹
		unsigned char i=0;
		LcdShowStr(0,0,"   empty all    ");
		LcdShowStr(0,1"   Yes or no ?  ");
		do
		 {
           if(next == 1){
               LcdFullClear();
               LcdShowStr(0,0,"   emptying     ");
               loading();
			   FINGERPRINT_Cmd_Delete_All_Model();
			   AS608_Receive_Data(12); 
               delay(100);
               LcdFullClear();
               LcdShowStr(0,0,"   All empty    ");
               Beep_Times(10);
               next = 0;
               break;
           }

		 }while(!Esc);
        Esc = 0; Admin =0;
}

/* eeprom.c */
void I2CStart(); /* 产生总线起始信号 */

/* 产生总线停止信号 */
void I2CStop();

/* I2C总线写操作，dat-待写入字节，返回值-从机应答位的值 */
bit I2CWrite(unsigned char dat);

/* I2C总线读操作，并发送非应答信号，返回值-读到的字节 */
unsigned char I2CReadNAK();

/* I2C总线读操作，并发送应答信号，返回值-读到的字节 */
unsigned char I2CReadACK();

/* 将一段内存数据转换为十六进制格式的字符串，
   str-字符串指针，src-源数据地址，len-数据长度 */
void MemToStr(unsigned char *str, unsigned char *src, unsigned char len)
{
    unsigned char tmp;

    while (len--)
    {
        tmp = *src >> 4;           //先取高4位
        if (tmp <= 9)              //转换为0-9或A-F
            *str++ = tmp + '0';
        else
            *str++ = tmp - 10 + 'A';
        tmp = *src & 0x0F;         //再取低4位
        if (tmp <= 9)              //转换为0-9或A-F
            *str++ = tmp + '0';
        else
            *str++ = tmp - 10 + 'A';
        *str++ = ' ';              //转换完一个字节添加一个空格
        src++;
    }
	*str = '\0';                   //添加字符串结束符
}

/* E2读取函数，buf-数据接收指针，addr-E2中的起始地址，len-读取长度 */
void E2Read(unsigned char *buf, unsigned char addr, unsigned char len)
{
    do {                       //用寻址操作查询当前是否可进行读写操作
        I2CStart();
        if (I2CWrite(0x50<<1)) //应答则跳出循环，非应答则进行下一次查询
        {
            break;
        }
        I2CStop();
    } while(1);
    I2CWrite(addr);            //写入起始地址
    I2CStart();                //发送重复启动信号
    I2CWrite((0x50<<1)|0x01);  //寻址器件，后续为读操作
    while (len > 1)            //连续读取len-1个字节
    {
        *buf++ = I2CReadACK(); //最后字节之前为读取操作+应答
        len--;
    }
    *buf = I2CReadNAK();       //最后一个字节为读取操作+非应答
    I2CStop();
}

/* E2写入函数，buf-源数据指针，addr-E2中的起始地址，len-写入长度 */
void E2Write(unsigned char *buf, unsigned char addr, unsigned char len)
{
    while (len--)
    {
        do {                       //用寻址操作查询当前是否可进行读写操作
            I2CStart();
            if (I2CWrite(0x50<<1)) //应答则跳出循环，非应答则进行下一次查询
            {
                break;
            }
            I2CStop();
        } while(1);
        I2CWrite(addr++);  //写入起始地址
        I2CWrite(*buf++);  //写入一个字节数据
        I2CStop();         //结束写操作，以等待写入完成
    }
}

void InitE2(){
    E2Read(password,0x8E,sizeof(password));
    E2Read(finger_id,0x9E,sizeof(finger_id));
}

/* 主函数 */
void main()
{   
    Init(); // 初始化
    showTime();
    while (1)
    {
        KeyDriver();
    }
    
}


/* 中断 */
/* T0中断服务函数，完成数码管、按键扫描与秒定时 */
void InterruptTimer0() interrupt 1
{
    TH0 = T0RH;   //重新加载重载值
    TL0 = T0RL;
    KeyScan();   //按键扫描
    tmr200ms++;
    if(tmr200ms>=200){
        tmr200ms = 0;
        flag200ms = 1;
    }
}

void Uart() interrupt 4 {
	if(RI==1)
	{
		RI = 0;
		Recive_table[i] = SBUF; 
		if(Recive_table[0]=='+')
		  i++;
		else
			i = 0;
		if(i>=10)
		{
			if((Recive_table[0]=='+')&&(Recive_table[1]=='I')&&(Recive_table[2]=='P')&&(Recive_table[3]=='D'))
			{
				if(Recive_table[9]=='1')  // 灯开
					light = 0;
				else if(Recive_table[9]=='0') // 灯关
					light = 1;
                else if(Recive_table[9]=='2') // 门开
                    StartMotor(360);
                else if(Recive_table[9]=='3') // 门关
                    StartMotor(-360);
			}
			i = 0;
		}
	}
	else
   TI = 0;		
}

void InterruptTimer1() interrupt 3
{
    if(RxdOrTxd){
        cnt++;
        if (cnt <= 8)  //低位在先依次发送8bit数据位
        {
            PIN_TXD = TxdBuf & 0x01;
            TxdBuf >>= 1;
        }
        else if (cnt == 9)  //发送停止位
        {
            PIN_TXD = 1;
        }
        else  //发送结束
        {
            cnt = 0;    //复位bit计数器
            TR0 = 0;    //关闭T0
            TxdEnd = 1; //置发送结束标志
        }
    }else{
                if (cnt == 0)     //处理起始位
        {
            if (!PIN_RXD) //起始位为0时，清零接收缓冲器，准备接收数据位
            {
                unsigned char i;
                for(i=0;i<len;i++)
                AS608_RECEICE_BUFFER[i] = 0;
                cnt++;
            }
            else          //起始位不为0时，中止接收
            {
                TR1 = 0;  //关闭T0
            }
        }
        else if (cnt < len)   //处理8位数据位
        {
                            //低位在先，所以将之前接收的位向右移
            if (PIN_RXD)     //接收脚为1时，缓冲器最高位置1，
            {                //而为0时不处理即仍保持移位后的0
                AS608_RECEICE_BUFFER[cnt] |= 0x80;
            }
            cnt++;
        }
        else  //停止位处理
        {
            cnt = 0;         //复位bit计数器
            TR0 = 0;         //关闭T0
            if (PIN_RXD)     //停止位为1时，方能认为数据有效
            {
                RxdEnd = 1;  //置接收结束标志
            }
        }
    }
}

