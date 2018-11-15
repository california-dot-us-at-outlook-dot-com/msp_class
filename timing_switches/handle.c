/*
 * handle.c
 *
 *  Created on: 2018Äê10ÔÂ31ÈÕ
 *      Author: Cal
 */
#include<msp430f169.h>
#include"handle.h"
#include"timeConfig.h"
#include"variables.h"
#include"lcd12864.h"


void I2c_Reg_Init(){

    P3SEL|=0x0a;
    P3DIR&=~0x0a;
    U0CTL|=I2C+SYNC;
    U0CTL&=~I2CEN;
    I2CTCTL=I2CSSEL_2;

    //I2CNDAT=0x01;
    I2CSA=(0xa0>>1);
    U0CTL|=I2CEN;

}

void I2c_Reg_Write(unsigned char addr,unsigned char dat){
    //I2CNDAT=2;
    I2CNDAT=0x02;
    //I2CSA=(0xd0>>1);

    U0CTL |= MST;
    I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
    //I2CNDAT=0x02;
    //P6OUT=~1;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=addr;
    delay_ms(9);
    //I2CNDAT=0x01;
    //P6OUT=~2;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=dat;
    delay_ms(9);
    //P6OUT=~3;
    while((I2CTCTL&I2CSTP)==0x02);
}

unsigned char I2c_Reg_Read(unsigned char addr){
    I2CNDAT=0x01;
    unsigned char ctlbyte;
    U0CTL |= MST;
    I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=addr;
    delay_ms(9);
    U0CTL |= MST;
    I2CIFG &= ~ARDYIFG;
    I2CTCTL &= ~I2CTRX;
    I2CTCTL = I2CSTT+I2CSTP;
    while((I2CIFG&RXRDYIFG)==0);
    ctlbyte=I2CDRB;
    delay_ms(9);
    while((I2CTCTL&I2CSTP)==0x02);
    return ctlbyte;
}



//I2C2
void Sda2(unsigned char a){
    if(a==0){
        port2Out &= (~SDA2);
    }else{
        port2Out |= SDA2;
    }
}
void Scl2(unsigned char a){
    if(a==0){
        port2Out &= (~SCL2);
    }else{
        port2Out |= SCL2;
    }
}


void start2(){
    port2Dout;
    Sda2(1);
    delay_us(10);
    Scl2(1);
    delay_us(10);
    Sda2(0);
    delay_us(10);
    Scl2(0);
    delay_us(10);
}

void stop2(){
    port2Dout;
    Sda2(0);
    delay_us(10);
    Scl2(1);
    delay_us(10);
    Sda2(1);
    delay_us(10);
}

unsigned char w2(unsigned char d){
    unsigned char a=0,b=0;
    port2Dout;
    for(a=0;a<8;a++){
        Sda2(d>>7);
        d=(d<<1);
        delay_us(10);
        Scl2(1);
        delay_us(10);
        Scl2(0);
        delay_us(10);
    }
    Sda2(1);
    delay_us(10);
    Scl2(1);
    portSda2In;

    while(port2In&SDA2){
        b++;
        if(b>200){
            Scl2(0);
            delay_us(10);
            return 0;
        }
    }

    Scl2(0);
    delay_us(10);
    return 1;
}

unsigned char r2(){
    unsigned char a=0,d=0;
    portSda2Out;
    Sda2(1);
    portSda2In;
    delay_us(10);
    for(a=0;a<8;a++){
        Scl2(1);
        delay_us(10);
        d <<= 1;
        d |= (port2In&SDA2);
        delay_us(10);
        Scl2(0);
        delay_us(10);
    }
    return d;
}

void I2c_2_Write(unsigned char addr,unsigned char dat){
    start2();
    w2(0xa0);
    w2(addr);
    w2(dat);
    stop2();
}

unsigned char I2c_2_Read(unsigned char addr){
    unsigned char i=0;
    start2();
    w2(0xa0);
    w2(addr);
    start2();
    w2(0xa1);
    i=r2();
    stop2();
    return i;
//            r2();
}

unsigned char readAt(unsigned char i){
    //I2c_Reg_Read(i);
    P6OUT=~i;
    return I2c_2_Read(i);

}
unsigned char writeAt(unsigned char i,unsigned char j){
    I2c_2_Write(i,j);
    return 0;
}

void jumpTo01x(){
    status=20;
    status+=cursor;
    menu_01x();
}
