#include <reg52.h>  
#define uint8 unsigned char  
#define uint16 unsigned short int  
  
//数码管段码显示：0~f,不亮  
uint8 code LED_Disp[] = {0xC0,0xF9,0xA4,0xB0,    //这一组编码当中不包含小数点  
                        0x99,0x92,0x82,0xF8,  
                        0x80,0x90};         //十个分别显示 0 1 2 3 4 5 6 7 8 9  
uint8 code LED_Disp1[] = {0x40,0x79,0x24,0x30,    //这一个数组的编码中包含小数点  
                        0x19,0x12,0x02,0x78,  
                        0x00,0x10};         //十个分别显示 0 1 2 3 4 5 6 7 8 9  
  
unsigned int counter = 0;  
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
  
    int a[4];  
    int j, k;  
    j = counter%60;  
    k = counter/60;  
    a[3] = j%10;  
    a[2] = j/10;  
    a[1] = k%10;  
    a[0] = k/10;  
    P2 = 0xef;  
    P0 = LED_Disp[a[0]];  
    mDelay(5);  
    P2 = 0xdf;  
    P0 = LED_Disp1[a[1]];  
    mDelay(5);  
    P2 = 0xbf;  
    P0 = LED_Disp[a[2]];  
    mDelay(5);  
    P2 = 0x7f;  
    P0 = LED_Disp[a[3]];  
    mDelay(5);  
  
  
  
}  
void timeDelay(unsigned int  time)  
{  
    while(time--)  
    {  
            LED_Disp_Seg7();  
    }  
}  
  
  
  
void main()  
{  
    while(1)  
    {  
        timeDelay(100);  
        counter++;  
    }  
}  