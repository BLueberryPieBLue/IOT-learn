#include<reg52.h>  
#include <intrins.h>  
   
void ConfigUART(unsigned int baud);
void UART_Send_Str(char *s);
void UART_Send_Byte(unsigned char mydata);

 
typedef unsigned char uchar;  
typedef unsigned int uint;  
 
sbit Echo=P3^2;//超声波模块的回响信号输出，P3^2也是51单片机外部中断0的输入端，利用下降沿来触发中断  
sbit Trig=P1^4;//触发信号脚  
  
  
sbit lcdrs=P1^5;//数据命令选择端，为0时写指令，为1时写数据  
sbit lcdrw=P1^6;//读、写命令选择端，为0时写数据，为1时读数据，读状态时置1，其他时间都是0；  
sbit lcden=P1^7;//使能端  
 
sbit STA7=P0^7;//D0~D7数据口对应P0^0~P0^7，当读液晶的数据时，STA7对应P0^7，STA7为1时表示液晶忙，无法接收数据  
  
float dis;//距离的缓存  
uchar flag;//中断标志位  
char code table[]="distance:";//在规定位置显示字符  
char code table1[]="cm";  
  
void delayms(uchar x)  
{  
    uint i,j;  
    for(i=0;i<x;i++)  
    {  
        for(j=0;j<110;j++);  
    }  
}  
void nops()//延时10.9us，每个机器周期约1.09us，在晶振为11.0592MHz时  
{  
    _nop_();  
    _nop_();  
    _nop_();  
    _nop_();  
    _nop_();  
    _nop_();  
    _nop_();  
    _nop_();  
    _nop_();  
    _nop_();  
}  
  
void distance()//计算距离的函数  
{  
    Trig=0;//首先将触发拉低  
    nops();  
    Trig=1;//给至少10us的高电平，启动模块  
    nops();  
    nops();  
    Trig=0;//此时已经触发了模块，接下来立刻开启定时器计数  
    TR0=1;//打开定时器0  
    EX0=1;//打开外部中断，外部中断输入为P3^2，下降沿有效，触发中断  
    delayms(1);//等待一下，一个机器周期约为1.09us，如果不等待，可能就错过计算dis，陷入死循环，永远无法得出值  
    if(flag==1)//如果标志位置1，表示Echo输出下降沿，即接收结束  
    {  
  
        flag=0;  
    }  
  
  
}  
  
void wait()//等待液晶空闲  
{  
    P0=0xff;  
    do  
    {  
        lcdrs=0;//循环里的这些语句是读状态时的操作时序  
        lcdrw=1;  
        lcden=0;  
        lcden=1;  
    }while(STA7==1);//等液晶空闲的时候再出循环  
    lcden=0;//使使能为0  
}  
  
void write_com(uchar com)//写命令程序  
{  
    wait();  
    lcdrs=0;//使RS为0，发出的数据会被液晶当做命令而不是现实数据，这也是与写数据程序的唯一不同  
    P0=com;   //传送com指令
    lcdrw=0;//总是写的，故为0  
    delayms(5);//使数据平稳，让液晶有时间读取  
    lcden=1;//根据操作时序，使能要输出高脉冲，这句与后面那句共同构成一个高脉冲  
    delayms(5);  
    lcden=0;  
}  
  
void write_date(uchar date)//写数据程序  
{  
    wait();  
    lcdrs=1;//注意这是写数据  
    P0=date;  
    lcdrw=0;  
    delayms(5);  
    lcden=1;  
    delayms(5);  
    lcden=0;  
}  
  
/*初始化函数*/  
void init()  
{  
    wait();  
   //dula=0;//没用  
    //wela=0;  
    lcden=0;  
    write_com(0x38);    //设置16*2显示，5*7点阵，8位数据口。 
    write_com(0x0c);    //显示器开，光标关闭
    write_com(0x06);    //文字不动，地址自动+1
	write_com(0x01);    //清屏
    TMOD=0x01;//16位计数器，这里不用开启定时器中断，因为定时器中断无事可做，只要定时器里的值就行，因此无需将TF0置1  
    TH0=0;//全部设为0  
    TL0=0;  
    IT0=1;//设置外部中断下降沿有效  
    EA=1;//开总中断  
}  
/*显示函数*/  
void display(float dis)  
{  
    char a[5];
    uint bai,shi,ge,p1,p2;//分别是百位一次向下，到小数点后两位  
	dis /= 2;
    bai=dis/100;  
    shi=(dis-bai*100)/10;  
    ge=dis-bai*100-shi*10;  
    p1=(dis*10)-bai*1000-shi*100-ge*10;  
    p2=(dis*100)-bai*10000-shi*1000-ge*100-p1*10;  
    write_date(0x30+bai);//将数字转换为字符，必须+0x30，另外，1602液晶只能接收字符型的数据  
    write_date(0x30+shi);  		 
    write_date(0x30 + ge);
	write_date('.');
	write_date(0x30 + p1);
	write_date(0x30 + p2); 
        a[0]=bai + 0x30;
	a[1]=shi + 0x30;
	a[2]=ge + 0x30;
        a[3] = '.';
	a[4] = p1 + 0x30;
	a[5] = p2 + 0x30;
	UART_Send_Str(a);
}  
  


void main()//主函数  
{  
    uchar i=0;  

    init();  
    write_com(0x80);//设置地址，在第一行第一列显示distance：，0x80是第一行的起始地址  
    	ConfigUART(9600); //配置波特率为 9600
	while(table[i]!='\0')  
    {  
        write_date(table[i]);  
        i++;  
        delayms(5);  
    }  
    i=0;  
    write_com(0x80+0x40+6);//设置地址，在第二行第7位显示cm，0x80+0x40是第二行的起始地址  
    while(table1[i]!='\0')  
    {  
        write_date(table1[i]);  
        i++;  
        delayms(5);  
    }  
    while(1)  
    {  
        distance();  
        write_com(0x80+0x40);//设置数据的显示地址，为第二行的起始位，之后在进行循环时不断刷新在这个地方显示  
        display(dis);  
        delayms(60);  
    }  
  	 EA = 1;   //使能总中断

     while (1);
  
} 
void ex() interrupt 0//外部中断的中断函数  
{  
    TR0=0;//一旦受到下降沿，立马停止定时器计数  
    dis=(TH0*256+TL0)*1.09/58;//先取出定时器里的时间值，之后再将定时器置0  
    flag=1;//将标志位置0  
    TH0=0;  
    TL0=0;  
} 
void ConfigUART(unsigned int baud)
{
SCON = 0x50; //配置串口为模式 1
TMOD &= 0x0F; //清零 T1 的控制位
TMOD |= 0x20; //配置 T1 为模式 2
TH1 = 256 - (11059200/12/32)/baud; //计算 T1 重载值
TL1 = TH1; //初值等于重载值
ET1 = 0; //禁止 T1 中断
ES = 1; //使能串口中断
TR1 = 1; //启动 T1
}


/* UART 中断服务函数 */
void InterruptUART() interrupt 4
{
if (RI) //接收到字节
{
RI = 0; //手动清零接收中断标志位
SBUF = SBUF +1;

}
if (TI) //字节发送完毕
{
TI = 0; //手动清零发送中断标志位
}
} 
void UART_Send_Str(char *s)
{
 while(*s!='\0') //将字符数据转为ASCLL码
 {
	 if(*s=='\n') 
	 {	 
	    UART_Send_Byte(0x0d);
	    UART_Send_Byte(0x0a);
	 }
	 else UART_Send_Byte(*s);
	 *s++;
 }
}
void UART_Send_Byte(unsigned char mydata)	
{
 ES=0;
 TI=0;
 SBUF=mydata;
 while(!TI);
 TI=0;
 ES=1;
}  
