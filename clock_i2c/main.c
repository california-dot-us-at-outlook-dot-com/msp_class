#include <msp430.h> 
#include"Config.h"


/**
 * main.c
 */
//#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
//#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
//#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define SDA BIT0    //接ds3231
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


#define SDA1 BIT0   //接I2C1602
#define SCL1 BIT1
#define port1Out P4OUT
#define port1In P4IN
#define port1Dir P4DIR
#define port1Din port1Dir &= ~(SDA1+SCL1)
#define port1Dout port1Dir |= SDA1+SCL1
#define portSda1Out port1Dir |= SDA1
#define portScl1In port1Dir &= ~SCL1
#define portSda1In port1Dir &= ~SDA1
#define portScl1Out port1Dir |= SCL1


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
    /*
    while(portIn&SDA){
        b++;
        if(b>200){
            Scl(0);
            delay_us(10);
            return 0;
        }
    }
    */
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

//秒表
unsigned long time=0;
unsigned long tmp_TAR=0;
unsigned char pause=1;
unsigned long record_TAR=0;
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A(void){
    //P6OUT=~P6OUT;
    if(pause==0){
        time++;
    }
}

void inTime_init(){
    TACCTL0=CCIE;//+CM1+CM0;//+CAP;

    TACCR0=32768;
    //CCR0=32768;
    TACTL=TASSEL_1+MC_1;
    _EINT();
}

//
//I2C 1602
void Sda1(unsigned char a){
    if(a==0){
        port1Out &= (~SDA1);
    }else{
        port1Out |= SDA1;
    }
}
void Scl1(unsigned char a){
    if(a==0){
        port1Out &= (~SCL1);
    }else{
        port1Out |= SCL1;
    }
}


void start1(){
    port1Dout;
    Sda1(1);
    delay_us(10);
    Scl1(1);
    delay_us(10);
    Sda1(0);
    delay_us(10);
    Scl1(0);
    delay_us(10);
}

void stop1(){
    port1Dout;
    Sda1(0);
    delay_us(10);
    Scl1(1);
    delay_us(10);
    Sda1(1);
    delay_us(10);
}

unsigned char w1(unsigned char d){
    unsigned char a=0,b=0;
    port1Dout;
    for(a=0;a<8;a++){
        Sda1(d>>7);
        d=(d<<1);
        delay_us(10);
        Scl1(1);
        delay_us(10);
        Scl1(0);
        delay_us(10);
    }
    Sda1(1);
    delay_us(10);
    Scl1(1);
    portSda1In;
    /*
    while(port1In&SDA1){
        b++;
        if(b>200){
            Scl1(0);
            delay_us(10);
            return 0;
        }
    }
    */
    Scl1(0);
    delay_us(10);
    return 1;
}

unsigned char r1(){
    unsigned char a=0,d=0;
    portSda1Out;
    Sda1(1);
    portSda1In;
    delay_us(10);
    for(a=0;a<8;a++){
        Scl1(1);
        delay_us(10);
        d <<= 1;
        d |= (port1In&SDA1);
        delay_us(10);
        Scl1(0);
        delay_us(10);
    }
    return d;
}


void I2C1602_wd(unsigned char d){
    unsigned char D;
    start1();
    w1(0x4e);
    D=d&0xf0;
    D|=0x0d;
    w1(D);
    D&=0xF9;
    w1(D);
    D=(d&0x0f)<<4;
    D|=0x0d;
    w1(D);
    D&=0xf9;
    w1(D);
    stop1();
}

void I2C1602_wc(unsigned char d){
    unsigned char D=0x00;
    start1();
    w1(0x4e);
    D=d&0xf0;
    D|=0x0c;
    w1(D);
    D&=0xf8;
    w1(D);

    D=(d&0x0f)<<4;
    D|=0x0c;
    w1(D);
    D&=0xf8;
    w1(D);
    stop1();
}


void I2C1602_init(){
    delay_ms(10);
    I2C1602_wc(0x33);
    delay_ms(5);
    I2C1602_wc(0x32);
    delay_ms(5);
    I2C1602_wc(0x28);
    delay_ms(5);
    I2C1602_wc(0x0c);
    delay_ms(5);
    I2C1602_wc(0x06);
    delay_ms(5);
    I2C1602_wc(0x01);
    delay_ms(5);
    delay_ms(10);

    delay_ms(10);
    I2C1602_wc(0x33);
    delay_ms(5);
    I2C1602_wc(0x32);
    delay_ms(5);
    I2C1602_wc(0x28);
    delay_ms(5);
    I2C1602_wc(0x0c);
    delay_ms(5);
    I2C1602_wc(0x06);
    delay_ms(5);
    I2C1602_wc(0x01);
    delay_ms(5);
    delay_ms(10);


}
//end
//interrupt_external
unsigned char ci=0;
unsigned char ifci=0;
//unsigned int tmp_TAR=32768;

unsigned long tmp_Time=0;
//unsigned long tmp_mTime=0;

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void){

    if((P1IN & BIT0)==0){
        if(pause==0){
            record_TAR=TAR;
            tmp_Time=time;
        }else{
            record_TAR=tmp_TAR;
        }
        ifci=1;
    }else if((P1IN &BIT1)==0){
        if(pause==0){
            pause=1;
            tmp_TAR=TAR;
        }else{
            pause=0;
            TAR=tmp_TAR;
        }
    }else if((P1IN & BIT2)==0){
        time=0;
        pause=1;
        tmp_TAR=0;
    }
    //delay_ms(500);
    P1IFG &= 0;
}
//



void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	unsigned char sec,min,hou,day,mon,yea,wee;
	unsigned char Filcker=0;
	unsigned long filcker=0;

	//unsigned char
	uchar bia=8;
	uchar aia=2;
	uchar cia=1;
	Clock_Init();
	P6DIR=0xff;
	P6OUT=0xff;
    P4DIR=0xff;
    P4OUT=0x00;
    P5SEL=0x00;
    P5DIR=0xff;
    init();
    P6DIR=0xff;
    inTime_init();

//init of externalInterruption
    P1DIR &= BIT1+BIT0;
    P1IES |= BIT0+BIT1+BIT2; //0:上升沿；    1：下降沿
    P1IE |= BIT0+BIT1+BIT2;  //中断允许，1为允许
    P1IFG &= 0;  //中断标志，0为可接受中断


    _EINT();    //开启总中断


    /*
    WC(0x00,0x00);//sec
    WC(0x01,0x45);//min
    WC(0x02,0x21);//hou
    WC(0x04,0x29);//day
    WC(0x05,0x09);//mon
    WC(0x06,0x18);//yea
    WC(0x03,0x03);//wee
    */
    I2C1602_init();
    /*
    wc(0xc0+4+cia);
    wd('0');
    wc(0xc0+5+cia);
    wd('0');
    */
    char *weekday[7]={"Sun\0","Mon\0","Tue\0","Wed\0","Thu\0","Fri\0","Sat\0"};
	unsigned char fps=0;
    while(1){
	    if(time>999){
	        time=0;
	    }
	    sec=BCD2HEX(RC(0x00));
	    min=BCD2HEX(RC(0x01));
	    hou=BCD2HEX(RC(0x02));
	    day=BCD2HEX(RC(0x04));
	    mon=BCD2HEX(RC(0x05));
	    yea=BCD2HEX(RC(0x06));
	    wee=BCD2HEX(RC(0x03));
/*
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

	    wc(0xc0+2+cia);
	    wd(a%10+'0');
	    wc(0xc0+1+cia);
	    wd((a%100)/10+'0');
	    wc(0xc0+cia);
	    wd(a/100+'0');

	    wc(0xc0);
	    wd('<');
	    wc(0xc0+3+cia);
	    wd('>');
*/

	    wc(0xc0+10);
	    wd('F');
	    wd('P');
	    wd('S');
	    wd(':');
	    if(ifci==1){
	        ifci=0;
	        if(ci<2){
	            wc(0x80+(ci)*64+2);
	            wd(tmp_Time%10+'0');
	            wc(0x80+(ci)*64+1);
	            wd((tmp_Time/10)%10+'0');
	            wc(0x80+(ci)*64);
                wd(tmp_Time/100+'0');
                wc(0x80+(ci)*64+3);
                wd('.');
                wc(0x80+(ci)*64+4);
                wd(((record_TAR)*10)/32768+'0');
                wc(0x80+(ci)*64+5);
                wd((((record_TAR)*100)/32768)%10+'0');

	        }
	        /*
	        else if(ci<3){
	            wc(0xc0+(ci-1)*7-3);
                wd(time%10+'0');
	            wc(0xc0+(ci-1)*7-4);
                wd((time/10)%10+'0');
                wc(0xc0+(ci-1)*7-5);
	            wd(time/100+'0');
	            wc(0xc0+(ci-1)*7-2);
	            wd('.');
	            wc(0xc0+(ci-1)*7-1);
	            wd((record_TAR*10)/32768+'0');
	            wc(0xc0+(ci-1)*7-0);
	            wd((((record_TAR)*100)/32768)%10+'0');
	        }
	        */
	        ci++;
	        if(ci>2){
	            ci=0;
	            for(;ci<6;ci++){
	                wc(0x80+ci);
	                wd(' ');
	                wc(0xc0+ci);
	                wd(' ');
	            }
	            ci=0;
	        }
	        //delay_ms(1000);
	    }




	    //I2C
        I2C1602_wc(0x80);
        //2条命令是为了避免一个BUG
	    I2C1602_wc(0x80+12);
	    I2C1602_wd(weekday[wee][0]);
        I2C1602_wd(weekday[wee][1]);
        I2C1602_wd(weekday[wee][2]);


        I2C1602_wc(0x80);
        I2C1602_wd('2');
        I2C1602_wc(0x81);
        I2C1602_wd('0');

        I2C1602_wc(0x80+aia);
        I2C1602_wd((yea)/10+'0');
        I2C1602_wc(0x80+1+aia);
        I2C1602_wd((yea%10)+'0');

        I2C1602_wc(0x80+2+aia);
        I2C1602_wd('/');

        I2C1602_wc(0x80+3+aia);
        I2C1602_wd((mon)/10+'0');
        I2C1602_wc(0x80+4+aia);
        I2C1602_wd((mon%10)+'0');

        I2C1602_wc(0x80+5+aia);
        I2C1602_wd('/');

        I2C1602_wc(0x80+6+aia);
        I2C1602_wd((day)/10+'0');
        I2C1602_wc(0x80+7+aia);
        I2C1602_wd((day%10)+'0');

        I2C1602_wc(0xc0+bia);
        I2C1602_wd((hou)/10+'0');
        I2C1602_wc(0xc0+1+bia);
        I2C1602_wd((hou%10)+'0');
        fps++;
        if(Filcker!=sec){
            wc(0xc0+14);
            wd(fps/10+'0');
            wc(0xc0+15);
            wd(fps%10+'0');
            fps=0;
            Filcker=sec;
            I2C1602_wc(0xc0+2+bia);
            I2C1602_wd(' ');

        }else{
            I2C1602_wc(0xc0+2+bia);
            I2C1602_wd(':');

        }
        if(filcker!=time){
            filcker=time;
            wc(0x80+8);
            wd(' ');
            wc(0x80+14+cia);
            wd(' ');
        }else{
            wc(0x80+8);
            wd('<');
            wc(0x80+15);
            wd('>');

        }

        I2C1602_wc(0xc0+3+bia);
        I2C1602_wd((min)/10+'0');
        I2C1602_wc(0xc0+4+bia);
        I2C1602_wd((min%10)+'0');

        I2C1602_wc(0xc0+6+bia);
        I2C1602_wd((sec)/10+'0');
        I2C1602_wc(0xc0+7+bia);
        I2C1602_wd((sec%10)+'0');

        int i=0;
        if(sec/30>0){
            I2C1602_wc(0xc0+5);
            I2C1602_wd(' ');
            I2C1602_wc(0xc0);
            I2C1602_wd('H');
        }else{
            I2C1602_wc(0xc0);
            I2C1602_wd(' ');
            I2C1602_wc(0xc0+5);
            I2C1602_wd('L');
        }
        for(i=0;i<4;i++){
        I2C1602_wc(0xc0+4-i);
        if((HEX2BCD(sec)>>i)&BIT0!=0x00){
            I2C1602_wd('>');
        }else{
            I2C1602_wd(' ');
        }
        }


        wc(0x80+11);
        wd(time%10+'0');
        wc(0x80+10);
        wd((time%100)/10+'0');
        wc(0x80+9);
        wd(time/100+'0');
        wc(0x80+12);
        wd('.');
        if(pause==0){
            tmp_TAR=TAR;
        }
        wc(0x80+13);
        wd((tmp_TAR*10)/32768+'0');
        wc(0x80+14);
        wd((((tmp_TAR)*100)/32768)%10+'0');

	}

}
