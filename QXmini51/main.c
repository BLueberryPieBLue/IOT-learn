#include <reg52.h> 
#include <string.h>	
#define uchar unsigned char
#define uint unsigned int
uchar *dat,p[10];
int i;
sbit led0=P1^0;
sbit led1=P1^1;
sbit led2=P1^2;
sbit led3=P1^3;
void main()
{	
	TMOD=0x20;		   //用定时器设置串口波特率
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	REN=1;          //串口初始化
	SM0=0;
	SM1=1;
	EA=1;           //开启总中断
	ES=1;			//开启串口中断
	while(1){
    
	}
}	

void ser() interrupt 4
{
/*	*dat=SBUF;//接收数据SBUF，即将计算机的数据接收。
    RI = 0;//清除接收中断标志位
	    if(*dat=='0'){led0=0;led1=0;led2=0;led3=0;}
		if(*dat=='9'){led0=1;led1=1;led2=1;led3=1;} 		
    SBUF=*dat;//将接收到的数据放入到发送寄存器
    while(!TI);	//等待发送数据完成
    TI=0;//清除发送完成标志位*/
	uchar i=0;
	if(RI){
	p[i]=SBUF;
	}
	i++;
	if(i>4){
	i=0;
	}
	if(TI){
	TI=0;
	}	
 }	 

  
/*
串口通信 向电脑发送字符串（任意） 、数字。
电脑向单片机发送数字，字符。
扩展：
加入网页，通过网页发送接收数据（后期有用，暂不做要求）
审核内容：
1、单片机每两秒（定时器完成）向pc发送一个字符串，每次发送内容包括一行数字（当前秒数），一行字符串任意。
2、pc向单片机发送“123” 则单片机向pc发送当前秒数，如果发送“321”发送字符串“get 321！” 。
3、pc向单片机发送“0”，单片机开启所有小灯，发送”9”关闭所有小灯。

*/