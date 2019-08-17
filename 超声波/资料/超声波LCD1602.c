#include<reg52.h>  
#include <intrins.h>  
   
void ConfigUART(unsigned int baud);
void UART_Send_Str(char *s);
void UART_Send_Byte(unsigned char mydata);

 
typedef unsigned char uchar;  
typedef unsigned int uint;  
 
sbit Echo=P3^2;//������ģ��Ļ����ź������P3^2Ҳ��51��Ƭ���ⲿ�ж�0������ˣ������½����������ж�  
sbit Trig=P1^4;//�����źŽ�  
  
  
sbit lcdrs=P1^5;//��������ѡ��ˣ�Ϊ0ʱдָ�Ϊ1ʱд����  
sbit lcdrw=P1^6;//����д����ѡ��ˣ�Ϊ0ʱд���ݣ�Ϊ1ʱ�����ݣ���״̬ʱ��1������ʱ�䶼��0��  
sbit lcden=P1^7;//ʹ�ܶ�  
 
sbit STA7=P0^7;//D0~D7���ݿڶ�ӦP0^0~P0^7������Һ��������ʱ��STA7��ӦP0^7��STA7Ϊ1ʱ��ʾҺ��æ���޷���������  
  
float dis;//����Ļ���  
uchar flag;//�жϱ�־λ  
char code table[]="distance:";//�ڹ涨λ����ʾ�ַ�  
char code table1[]="cm";  
  
void delayms(uchar x)  
{  
    uint i,j;  
    for(i=0;i<x;i++)  
    {  
        for(j=0;j<110;j++);  
    }  
}  
void nops()//��ʱ10.9us��ÿ����������Լ1.09us���ھ���Ϊ11.0592MHzʱ  
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
  
void distance()//�������ĺ���  
{  
    Trig=0;//���Ƚ���������  
    nops();  
    Trig=1;//������10us�ĸߵ�ƽ������ģ��  
    nops();  
    nops();  
    Trig=0;//��ʱ�Ѿ�������ģ�飬���������̿�����ʱ������  
    TR0=1;//�򿪶�ʱ��0  
    EX0=1;//���ⲿ�жϣ��ⲿ�ж�����ΪP3^2���½�����Ч�������ж�  
    delayms(1);//�ȴ�һ�£�һ����������ԼΪ1.09us��������ȴ������ܾʹ������dis��������ѭ������Զ�޷��ó�ֵ  
    if(flag==1)//�����־λ��1����ʾEcho����½��أ������ս���  
    {  
  
        flag=0;  
    }  
  
  
}  
  
void wait()//�ȴ�Һ������  
{  
    P0=0xff;  
    do  
    {  
        lcdrs=0;//ѭ�������Щ����Ƕ�״̬ʱ�Ĳ���ʱ��  
        lcdrw=1;  
        lcden=0;  
        lcden=1;  
    }while(STA7==1);//��Һ�����е�ʱ���ٳ�ѭ��  
    lcden=0;//ʹʹ��Ϊ0  
}  
  
void write_com(uchar com)//д�������  
{  
    wait();  
    lcdrs=0;//ʹRSΪ0�����������ݻᱻҺ�����������������ʵ���ݣ���Ҳ����д���ݳ����Ψһ��ͬ  
    P0=com;   //����comָ��
    lcdrw=0;//����д�ģ���Ϊ0  
    delayms(5);//ʹ����ƽ�ȣ���Һ����ʱ���ȡ  
    lcden=1;//���ݲ���ʱ��ʹ��Ҫ��������壬���������Ǿ乲ͬ����һ��������  
    delayms(5);  
    lcden=0;  
}  
  
void write_date(uchar date)//д���ݳ���  
{  
    wait();  
    lcdrs=1;//ע������д����  
    P0=date;  
    lcdrw=0;  
    delayms(5);  
    lcden=1;  
    delayms(5);  
    lcden=0;  
}  
  
/*��ʼ������*/  
void init()  
{  
    wait();  
   //dula=0;//û��  
    //wela=0;  
    lcden=0;  
    write_com(0x38);    //����16*2��ʾ��5*7����8λ���ݿڡ� 
    write_com(0x0c);    //��ʾ���������ر�
    write_com(0x06);    //���ֲ�������ַ�Զ�+1
	write_com(0x01);    //����
    TMOD=0x01;//16λ�����������ﲻ�ÿ�����ʱ���жϣ���Ϊ��ʱ���ж����¿�����ֻҪ��ʱ�����ֵ���У�������轫TF0��1  
    TH0=0;//ȫ����Ϊ0  
    TL0=0;  
    IT0=1;//�����ⲿ�ж��½�����Ч  
    EA=1;//�����ж�  
}  
/*��ʾ����*/  
void display(float dis)  
{  
    char a[5];
    uint bai,shi,ge,p1,p2;//�ֱ��ǰ�λһ�����£���С�������λ  
	dis /= 2;
    bai=dis/100;  
    shi=(dis-bai*100)/10;  
    ge=dis-bai*100-shi*10;  
    p1=(dis*10)-bai*1000-shi*100-ge*10;  
    p2=(dis*100)-bai*10000-shi*1000-ge*100-p1*10;  
    write_date(0x30+bai);//������ת��Ϊ�ַ�������+0x30�����⣬1602Һ��ֻ�ܽ����ַ��͵�����  
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
  


void main()//������  
{  
    uchar i=0;  

    init();  
    write_com(0x80);//���õ�ַ���ڵ�һ�е�һ����ʾdistance����0x80�ǵ�һ�е���ʼ��ַ  
    	ConfigUART(9600); //���ò�����Ϊ 9600
	while(table[i]!='\0')  
    {  
        write_date(table[i]);  
        i++;  
        delayms(5);  
    }  
    i=0;  
    write_com(0x80+0x40+6);//���õ�ַ���ڵڶ��е�7λ��ʾcm��0x80+0x40�ǵڶ��е���ʼ��ַ  
    while(table1[i]!='\0')  
    {  
        write_date(table1[i]);  
        i++;  
        delayms(5);  
    }  
    while(1)  
    {  
        distance();  
        write_com(0x80+0x40);//�������ݵ���ʾ��ַ��Ϊ�ڶ��е���ʼλ��֮���ڽ���ѭ��ʱ����ˢ��������ط���ʾ  
        display(dis);  
        delayms(60);  
    }  
  	 EA = 1;   //ʹ�����ж�

     while (1);
  
} 
void ex() interrupt 0//�ⲿ�жϵ��жϺ���  
{  
    TR0=0;//һ���ܵ��½��أ�����ֹͣ��ʱ������  
    dis=(TH0*256+TL0)*1.09/58;//��ȡ����ʱ�����ʱ��ֵ��֮���ٽ���ʱ����0  
    flag=1;//����־λ��0  
    TH0=0;  
    TL0=0;  
} 
void ConfigUART(unsigned int baud)
{
SCON = 0x50; //���ô���Ϊģʽ 1
TMOD &= 0x0F; //���� T1 �Ŀ���λ
TMOD |= 0x20; //���� T1 Ϊģʽ 2
TH1 = 256 - (11059200/12/32)/baud; //���� T1 ����ֵ
TL1 = TH1; //��ֵ��������ֵ
ET1 = 0; //��ֹ T1 �ж�
ES = 1; //ʹ�ܴ����ж�
TR1 = 1; //���� T1
}


/* UART �жϷ����� */
void InterruptUART() interrupt 4
{
if (RI) //���յ��ֽ�
{
RI = 0; //�ֶ���������жϱ�־λ
SBUF = SBUF +1;

}
if (TI) //�ֽڷ������
{
TI = 0; //�ֶ����㷢���жϱ�־λ
}
} 
void UART_Send_Str(char *s)
{
 while(*s!='\0') //���ַ�����תΪASCLL��
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
