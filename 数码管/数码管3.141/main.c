#include <reg52.h>
#define uint8 unsigned char
#define uint16 unsigned short int


//数码管段码显示：0~f,不亮
uint8 code LED_Disp[] = {0xC0,0xF9,0xA4,0x30,
                        0x99,0x92,0x82,0xF8,
						0x80,0x90,0x88,0x83,
						0xC6,0xA1,0x86,0x8E};			//前十个分别显示 0 1 2 3. 4 5 6 7 8 9 A B C D E F

//sfr LedPort = 0x80;	 //P0
//sfr LedCtrl = 0xa0;	 //P2
 
uint8 DisBuff[4];

uint8 Txd_data = 41;
uint8 Rxd_data = 31;
 
 

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
 
   DisBuff[0]   = Txd_data%10;//取个位数							
   DisBuff[1]   = Txd_data%100/10; //取十位数
   DisBuff[2]   = Rxd_data%10;	   //百位数
   DisBuff[3]   = Rxd_data%100/10; //千位数		

  

   P2 = 0x7f; //LedCtrl = 0x7f;
   P0 = LED_Disp[DisBuff[0]]; //LedPort = LED_Disp[DisBuff[0]];
   mDelay(5);
 
   P2 = 0xbf; //LedCtrl = 0xbf;
   P0 = LED_Disp[DisBuff[1]]; //LedPort = LED_Disp[DisBuff[1]];
   mDelay(5);
 
   P2 = 0xdf; //LedCtrl = 0xdf;
   P0 = LED_Disp[DisBuff[2]]; //LedPort = LED_Disp[DisBuff[2]];
   mDelay(5);
 
   P2 = 0xef; //LedCtrl = 0xef;
   P0 = LED_Disp[DisBuff[3]]; //LedPort = LED_Disp[DisBuff[3]];
   mDelay(5);
}
   			   
void main()
{
	while(1)
	{
		 LED_Disp_Seg7();
	}	
}
 