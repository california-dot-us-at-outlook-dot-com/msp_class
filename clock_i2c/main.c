#include <msp430.h> 
#include"Config.h"


/**
 * main.c
 */
//#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
//#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
//#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define SDA BIT0
#define SCL BIT1
#define portOut P5OUT
#define portIn P5IN
#define portDir P5DIR
#define portDin portDir &= ~(SDA+SCL)
#define portDout portDir |= SDA+SCL
#define portSdaOut portDir |= SDA
#define portSclIn portDir &= ~SCL
#define portSdaIn portDir &= ~SDA
#define portSclOut portDir |= SCL

//#define uchar unsigned char


void Sda(unsigned char a){
    if(a==0){
        portOut &= (~SDA);
    }else{
        portOut |= SDA;
    }
}
void Scl(unsigned char a){
    if(a==0){
        portOut &= (~SCL);
    }else{
        portOut |= SCL;
    }
}


void start(){
    portDout;
    Sda(1);
    delay_us(10);
    Scl(1);
    delay_us(10);
    Sda(0);
    delay_us(10);
    Scl(0);
    delay_us(10);
}

void stop(){
    portDout;
    Sda(0);
    delay_us(10);
    Scl(1);
    delay_us(10);
    Sda(1);
    delay_us(10);
}

unsigned char w(unsigned char d){
    unsigned char a=0,b=0;
    portDout;
    for(a=0;a<8;a++){
        Sda(d>>7);
        d=(d<<1);
        delay_us(10);
        Scl(1);
        delay_us(10);
        Scl(0);
        delay_us(10);
    }
    Sda(1);
    delay_us(10);
    Scl(1);
    portSdaIn;
    while(portIn&SDA){
        b++;
        if(b>200){
            Scl(0);
            delay_us(10);
            return 0;
        }
    }
    Scl(0);
    delay_us(10);
    return 1;
}

unsigned char r(){
    unsigned char a=0,d=0;
    portSdaOut;
    Sda(1);
    portSdaIn;
    delay_us(10);
    for(a=0;a<8;a++){
        Scl(1);
        delay_us(10);
        d <<= 1;
        d |= (portIn&SDA);
        delay_us(10);
        Scl(0);
        delay_us(10);
    }
    return d;
}

unsigned char RC(unsigned char a){
    unsigned char d;
    start();
    w(0xd0);
    w(a);
    stop();

    start();
    w(0xd1);
    d=r();
    stop();
    return d;
}

unsigned char WC(unsigned char a,unsigned char b){
    start();
    w(0xd0);
    w(a);
//    w(0xd0);
    w(b);
    stop();
    return 1;
}
/*
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}
*/
//1602


/**
 * main.c
 */

//wd == WriteData
//wc == WriteCommend

void wd(unsigned char data){
    RS_SET;
    RW_CLR;
    EN_SET;
    DataPort=data;
    delay_ms(5);
    EN_CLR;
}

void wc(unsigned char data){
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataPort=data;
    delay_ms(5);
    EN_CLR;
}

void init(void){
    wc(0x38);
    delay_ms(5);
    wc(0x08);
    delay_ms(5);
    wc(0x01);
    delay_ms(5);
    wc(0x06);
    delay_ms(5);
    wc(0x0c);
    delay_ms(5);
}

unsigned char BCD2HEX(unsigned char a){
    unsigned char b=(a>>4);
    unsigned char c=(a%0x10);
    return c+(b*10);
}

unsigned char HEX2BCD(unsigned char a){
    unsigned char b=(a/10);
    unsigned char c=(a%10);
    return b*16+c;
}


void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	unsigned char sec,min,hou,day,mon,yea;
	uchar bia=8;
	uchar aia=2;
	Clock_Init();
	P6DIR=0xff;
	P6OUT=0xff;
    P4DIR=0xff;
    P4OUT=0x00;
    P5SEL=0x00;
    P5DIR=0xff;
    init();
    /*
    WC(0x00,0x00);
    WC(0x01,0x45);
    WC(0x02,0x21);
    WC(0x04,0x29);
    WC(0x05,0x09);
    WC(0x06,0x18);
    */
	while(1){
	    sec=BCD2HEX(RC(0x00));
	    min=BCD2HEX(RC(0x01));
	    hou=BCD2HEX(RC(0x02));
	    day=BCD2HEX(RC(0x04));
	    mon=BCD2HEX(RC(0x05));
	    yea=BCD2HEX(RC(0x06));

	    wc(0x80);
	    wd('2');
	    wc(0x81);
	    wd('0');

	    wc(0x80+aia);
	    wd((yea)/10+'0');
	    wc(0x80+1+aia);
	    wd((yea%10)+'0');

	    wc(0x80+2+aia);
	    wd('/');

	    wc(0x80+3+aia);
	    wd((mon)/10+'0');
	    wc(0x80+4+aia);
	    wd((mon%10)+'0');

	    wc(0x80+5+aia);
	    wd('/');

	    wc(0x80+6+aia);
	    wd((day)/10+'0');
	    wc(0x80+7+aia);
	    wd((day%10)+'0');

	    wc(0xc0+bia);
	    wd((hou)/10+'0');
	    wc(0xc0+1+bia);
	    wd((hou%10)+'0');

	    wc(0xc0+2+bia);
	    wd(':');

	    wc(0xc0+3+bia);
	    wd((min)/10+'0');
	    wc(0xc0+4+bia);
	    wd((min%10)+'0');

	    wc(0xc0+6+bia);
	    wd((sec)/10+'0');
	    wc(0xc0+7+bia);
	    wd((sec%10)+'0');
	}

}
