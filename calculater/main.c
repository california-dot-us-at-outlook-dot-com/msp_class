#include <msp430.h> 
#include"Config.h"
#define SDA1 BIT0   //��I2C1602
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

/**
 * main.c
 */
unsigned char tmp=0;
unsigned char num=0;
unsigned char num1=0;
long left=0;
unsigned char operation=0;
unsigned char operation_pos=0;
long right=0;
long result=0;
unsigned char cur=0;
unsigned char cur1=0x80;
unsigned char onePress=0;
void findPressed(){
    unsigned char a=1;
    unsigned char b=0;
    for(a=1;a<0x10;a<<=1){
        P5DIR=0xff;
        P5OUT=0x00;
        P5DIR=0x0f;
        P5OUT=a;
        num=0;
        num1=0;
        if(P5IN!=a){
            //P6OUT=~P5IN;
            b=1;
            tmp=P5IN>>4;
            for(;tmp;tmp>>=1){
                num++;
            }
            for(tmp=(P5IN&0x0f);tmp;tmp>>=1){
                num1++;
            }

            num=((num-1)*4)+(num1-1);
            if(num<10&&onePress==0){
                if(cur==0){
                    left=left*10+num;
                }else{
                    right=right*10+num;
                }
                I2C1602_wc(cur1);
                cur1++;
                I2C1602_wd(num+48);
            }else if(num>=10 && onePress==0){
                if(num==10 && cur==0){
                    operation=0;
                    operation_pos=cur1;
                    cur=1;
                    I2C1602_wc(cur1);
                                    cur1++;
                                    I2C1602_wd('+');
                }else if(num==10){
                    operation=0;
                    I2C1602_wc(operation_pos);
                    I2C1602_wd('+');
                }
                if(num==11 && cur==0){
                    operation=1;
                    operation_pos=cur1;
                    cur=1;
                    I2C1602_wc(cur1);
                                    cur1++;
                                    I2C1602_wd('-');
                }else if(num==11){
                    operation=1;
                    I2C1602_wc(operation_pos);
                    I2C1602_wd('-');
                }
                if(num==12 && cur==0){
                    operation=2;
                    operation_pos=cur1;
                    cur=1;
                    I2C1602_wc(cur1);
                                    cur1++;
                                    I2C1602_wd('x');
                }else if(num==12){
                    operation=2;
                    I2C1602_wc(operation_pos);
                    I2C1602_wd('x');
                }
                if(num==13 && cur==0){
                    operation=3;
                    operation_pos=cur1;
                    cur=1;
                    I2C1602_wc(cur1);
                                    cur1++;
                                    I2C1602_wd('/');
                }else if(num==13){
                    operation=3;
                    I2C1602_wc(operation_pos);
                    I2C1602_wd('/');
                }
                if(num==14){
                    cur=0;
                    if(operation==0){
                        result=left+right;
                    }
                    if(operation==1){
                        result=left-right;
                    }
                    if(operation==2){
                        result=left*right;
                    }
                    if(operation==3){
                        result=left/right;
                    }
                    left=0;
                    right=0;
                    P6OUT=result;
                    I2C1602_wc(cur1);
                                    cur1++;
                                    I2C1602_wd('=');
                                    I2C1602_wc(cur1);
                                                    cur1++;
                                                    I2C1602_wd(result/100+48);
                                                    I2C1602_wc(cur1);
                                                                    cur1++;
                                                                    I2C1602_wd((result%100)/10+48);
                                                                    I2C1602_wc(cur1);
                                                                                    cur1++;
                                                                                    I2C1602_wd(result%10+48);
                if(cur1>=0xc0){
                    for(cur1=0x80;cur1<16+0x80;cur1++){
                                            I2C1602_wc(cur1);
                                            I2C1602_wd(' ');
                                        }
                    cur1=0x80;
                }else{
                    for(cur1=0xc0;cur1<16+0xc0;cur1++){
                                                                I2C1602_wc(cur1);
                                                                I2C1602_wd(' ');
                                                            }
                                        cur1=0xc0;

                }
                }
                if(num==15){
                    left=0;
                    right=0;
                    operation=0;
                    cur=0;
                    cur1=0x80;
                    for(num=0;num<16;num++){
                        I2C1602_wc(num+0x80);
                        I2C1602_wd(' ');
                        I2C1602_wc(num+0xc0);
                        I2C1602_wd(' ');
                    }
                }
            }
            onePress=1;
        }
    }

    if(b==0){
        P6OUT=0xff;
        onePress=0;
    }
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    I2C1602_init();
    P6DIR=0xff;
    P6OUT=0xff;
    P5DIR=0xff;
    P5OUT=0x00;
    P5DIR=0x0f;
    while(1){
        findPressed();
    }



    return 0;
}
