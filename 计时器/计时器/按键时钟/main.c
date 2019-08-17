
/*
 *	  ����ʱ�� �������ͨ��������ʼ����ֹͣ
 */
#include<reg52.h>
#define uchar unsigned char

sbit key =P3 ^ 3; //����

uchar counter=0,tmp,second=0,minute=0, change = 1;
int led[]= {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90}; //����0-9
int _led[]= {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

void clockrun();

void main() {
    //����TMOD�Ĵ���
    TMOD=0X01;							//����TMOD�Ĵ���
    TH0=(65536-5000)/256;			   //װ��ֵ
    TL0=(65536-5000)%256;
    EA=1;		//�� �ж�
    ET0=1;
    TR0=1;
    if(key==0) {//��������
        while(1) {
            clockrun();
        }
    }

}
void zhongduan()interrupt 1 {
    TH0=(65536-5000)/256;			   //װ��ֵ
    TL0=(65536-5000)%256;
    TF0=0;
    TR0=1;
    counter++;
    if(counter==200) {
        counter=0;
        second++;
        if(second==60) {
            second=0;
            minute++;
        }
    }
    change = 1;
}
void clockrun() {
    tmp=counter%4;
    switch(tmp) {
    case 0:
        P2 = 0x7f;
        P0 = led[second%10];
        break;
    case 1:
        P2 = 0xbf;
        P0 = led[second/10];
        break;
    case 2:
        P2 = 0xdf;
        P0 = _led[minute%10];
        break;
    case 3:
        P2 = 0xef;
        P0 = led[minute/10];
        break;
    }
}
