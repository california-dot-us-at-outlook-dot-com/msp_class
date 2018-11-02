#include <msp430.h> 


/**
 * main.c
 */

#include"TFT.h"
#include"ADC.h"
#include<stdlib.h>
unsigned char *wave1;
unsigned char *wave11;
unsigned long timeDiv;
unsigned long scale=1000;
unsigned char pause=0;

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void){
    P6OUT=~P6OUT;
    if(!(P1IN&BIT0)){
        scale+=200;
    }else if(!(P1IN&BIT1)){
        scale-=200;
    }else if(!(P1IN&BIT2)){
        pause=!pause;
    }

    P1IFG &= 0;
}


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	P1DIR |= 0x0f;
	    P1SEL |= 0x0f;
	    CCR0=32767;
	    TACTL=TASSEL_2+MC_1;

	        CCR1=12580;
	        CCTL1=OUTMOD_3; //先为0，遇到CCR1变为1，遇到CCR0变为0

	        CCR2=12580;
	        CCTL2=OUTMOD_7;//先为1，遇到CCR1变为0，遇到CCR0变为1

	        P6DIR=0xff;
	        P1DIR &= 0;
	        P1SEL=0x00;
	            P1IES |= BIT0+BIT1+BIT2; //0:上升沿；    1：下降沿
	            P1IE |= BIT0+BIT1+BIT2;  //中断允许，1为允许
	            P1IFG &= 0;  //中断标志，0为可接受中断
	            _EINT();    //开启总中断

	wave1=(unsigned char *)malloc(sizeof(char)*61);
	//wave11=(unsigned char*)malloc(sizeof(char)*61);
	adcInit();
	TFT_Initial();
	P6DIR=0xff;
	P6OUT=0xf0;
	unsigned int lx;
	unsigned int ly;
	unsigned char i=0;

	LCD_SetPos(0,240,0,320);
	for(lx=0;lx<240;lx++){
	    for(ly=0;ly<360;ly++){
	        //LCD_SetPos(lx,lx,wave1[lx],wave1[lx]);
	        Write_Data(0,0x00);
	    }
	}
	LCD_SetPos(0,239,285,285);
	    for(lx=0;lx<240;lx++){
	        Write_Data(0x00,0x1f);
	}
	LCD_SetPos(0,239,20,20);
	    for(lx=0;lx<240;lx++){
	        Write_Data(0xff,0xff);
	    }
	while(1){
	    ly=(adcTrans());
	    //delay_us(1);
	    //wave1[i]=ly>20?(ly>>4):20;
	    wave1[i]=ly>>4;
	    //wave1[i]=255;
	    //wave11[i]=ly>>4;
	    for(lx=0;lx<scale;lx++){
	        delay_us(1);
	    }
	    i++;
	    if(i>60){
	        i=0;
	        unsigned char ramp;
	        unsigned int height;
	        unsigned char upordown;
	        for(lx=0;lx<240;lx+=4){
	            if(wave1[lx/4]<wave1[lx/4+1]){
	                ramp=wave1[lx/4+1]-wave1[lx/4];
	                upordown=1;
	            }else{
	                ramp=wave1[lx/4]-wave1[lx/4+1];
	                upordown=0;
	            }
                ramp=ramp>4?ramp:4;

	                height=wave1[lx/4]+25;

	            if(upordown){
	                LCD_SetPos(lx,lx+4,height,height+ramp);
	            }else{
                    LCD_SetPos(lx,lx+4,height-ramp,height);

	            }
	            for(i=0;i<ramp;i++){
	                Write_Data(0xf8,0x00);
	                Write_Data(0xf8,0x00);
	                Write_Data(0xf8,0x00);
	                Write_Data(0xf8,0x00);
	                Write_Data(0xf8,0x00);
	            }
	        }
	        delay_ms(500);
	        while(pause);
	        for(lx=0;lx<240;lx+=4){
	            if(wave1[lx/4]<wave1[lx/4+1]){
	                ramp=wave1[lx/4+1]-wave1[lx/4];
	                upordown=1;
	            }else{
	                ramp=wave1[lx/4]-wave1[lx/4+1];
	                upordown=0;
	            }
	            ramp=ramp>4?ramp:4;

	                                            height=wave1[lx/4]+25;

	                        if(upordown){
	                                            LCD_SetPos(lx,lx+4,height,height+ramp);
	                                        }else{
	                                            LCD_SetPos(lx,lx+4,height-ramp,height);

	                                        }	            for(i=0;i<ramp;i++){
	                Write_Data(0,0x00);
	                Write_Data(0,0);
	                Write_Data(0,0x00);
	                Write_Data(0,0);
	                Write_Data(0,0x00);
	            }
	        }
	        i=0;
	    }
	}
	
	return 0;
}
