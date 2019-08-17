#include "REG52.h"

sbit led0=P2^0;
sbit led1=P2^1;
sbit led2=P2^2;
sbit led3=P2^3;




void Delay(unsigned int);	//延时函数

void main(void)
{
	while(1)
	{
			led0=0;
			Delay(60000);
		    led0=1;
		    Delay(60000);
			led1=0;
			Delay(60000);
		    led1=1;
		    Delay(60000);
			led2=0;
			Delay(60000);
		    led2=1;
		    Delay(60000);
			led3=0;
			Delay(60000);
		    led3=1;
		    Delay(60000);
	}
}


//延时函数
void Delay(unsigned int m)
{
	while(m--);
}

