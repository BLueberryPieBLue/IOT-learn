#include <reg52.h>
#define uint8 unsigned char
#define uint16 unsigned short int

//数码管段码显示：0~f,不亮
uint8 code LED_Disp[] = {0xC0,0xF9,0xA4,0xB0,    //这一组编码当中不包含小数点
                        0x99,0x92,0x82,0xF8,
						0x80,0x90,0x88,0x83,
						0xC6,0xA1,0x86,0x8E};			//前十个分别显示 0 1 2 3 4 5 6 7 8 9 A B C D E F 
uint8 code LED_Disp1[] = {0x40,0x79,0x24,0x30,    //这一个数组的编码中包含小数点
                        0x19,0x12,0x02,0x78,
						0x00,0x10,0x88,0x83,
						0xC6,0xA1,0x86,0x8E};			//前十个分别显示 0 1 2 3 4 5 6 7 8 9 A B C D E F

uint8 DisBuff[4];


void mDelay(uint16 mtime)
{
	for(; mtime > 0; mtime--)
	{
		uint8 j = 244;
		while(--j);
	}	
}

void LED_Disp_Seg7()
{
   //uint8 temp = pi*1000;
   //DisBuff[0]   = temp%10;//取个位数
   //DisBuff[1]   = temp/10%10; //取十位数
   //DisBuff[2]   = temp/100%10;	   //百位数
   //DisBuff[3]   = temp/1000; //千位数

   P2 = 0x7f;
   P0 = LED_Disp[1];
   mDelay(5);
 
   P2 = 0xbf;
   P0 = LED_Disp[4];
   mDelay(5);
 
   P2 = 0xdf;
   P0 = LED_Disp[1];
   mDelay(5);
 
   P2 = 0xef;
   P0 = LED_Disp1[3];     //调用含有小数点的数组
   mDelay(5);
}

void main()
{
	while(1)
	{
		LED_Disp_Seg7();
	}	
}
 