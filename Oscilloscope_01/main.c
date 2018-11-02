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
long scale=1000;
unsigned char pause=0;
unsigned char middle=0;//使波形居中
#define bia -10//bia最小为-10

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void){

    if(!(P1IN&BIT0)){
        scale>>=1;
    }else if(!(P1IN&BIT1)){
        scale<<=1;
    }else if(!(P1IN&BIT2)){
        pause=!pause;
    }else if(!(P1IN&BIT3)){
        middle=!middle;
    }
    if(!scale){
        scale=1000;
    }
    if(scale>65536){
        scale=1000;
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
	            P1IES |= BIT0+BIT1+BIT2+BIT3; //0:上升沿；    1：下降沿
	            P1IE |= BIT0+BIT1+BIT2+BIT3;  //中断允许，1为允许
	            P1IFG &= 0;  //中断标志，0为可接受中断
	            _EINT();    //开启总中断

	wave1=(unsigned char *)malloc(sizeof(char)*61);
	wave11=(unsigned char*)malloc(sizeof(char)*61);
	adcInit();
	TFT_Initial();
	P6DIR=0xff;
	P6OUT=0xf0;
	unsigned int lx;
	unsigned int ly;
//	unsigned char i=0;

	LCD_SetPos(0,240,0,320);
	for(lx=0;lx<240;lx++){
	    for(ly=0;ly<360;ly++){
	        //LCD_SetPos(lx,lx,wave1[lx],wave1[lx]);
	        Write_Data(0,0x00);
	    }
	}
	LCD_SetPos(0,239,275+bia,275+bia);
	    for(lx=0;lx<240;lx++){
	        Write_Data(0x00,0x1f);
	}
	LCD_SetPos(0,239,10+bia,10+bia);
	    for(lx=0;lx<240;lx++){
	        Write_Data(0xff,0xff);
	    }

	    LCD_SetPos(0,120,300,300);
	                    for(lx=0;lx<240;lx++){

	                            Write_Data(0xf8,0x1f);
	                    }
	unsigned long loop=0;
	unsigned char waveTop=0;
	unsigned char waveBottom=0;
	unsigned char waveMax=0;
	unsigned char waveMin=0;
	while(1){
	    int bias=128-((waveTop-waveBottom)/2);
	    //采样
	    /////////////////////////////////////////////////////////////

	    for(loop=0;loop<61;loop++){
	        ly=(adcTrans());
	        if(middle){
	            wave1[loop]=(ly>>4)+bias;
	        }else{
	            wave1[loop]=ly>>4;
	            waveMax=(wave1[loop]>waveMax?wave1[loop]:waveMax);
	            waveMin=(wave1[loop]<waveMin?wave1[loop]:waveMin);
	        }

	        //wave11[i/2]=ly>>4;
	        for(lx=0;lx<scale;lx++){
	            delay_us(1);
	        }
	    }
	    waveTop=waveMax;
	    waveBottom=waveMin;
	    ///////////////////////////////////////////////////////////////

//画曲线
	                                            ////////////////////////////////////////////////////////////////
	        unsigned char ramp;
	        unsigned int height;
	        unsigned char upordown;
	        for(lx=0;lx<240;lx+=4){
	            if(wave1[lx/4]<wave1[lx/4+1]){
	                ramp=wave1[lx/4+1]-wave1[lx/4];
	                upordown=0;
	            }else{
	                ramp=wave1[lx/4]-wave1[lx/4+1];
	                upordown=1;
	            }
                ramp=ramp>4?ramp:4;

	                height=bia+270-wave1[lx/4];

	            if(upordown){
	                LCD_SetPos(lx,lx+4,height,height+ramp);
	            }else{
                    LCD_SetPos(lx,lx+4,height-ramp,height);

	            }
	            for(loop=0;loop<ramp;loop++){
	                Write_Data(0xf8,0x00);
	                Write_Data(0xf8,0x00);
	                Write_Data(0xf8,0x00);
	                Write_Data(0xf8,0x00);
	                Write_Data(0xf8,0x00);
	            }
	        }

	        for(loop=0;loop<scale;loop++){
	            delay_us(100);
	        }
	        while(pause);
	        for(lx=0;lx<240;lx+=4){
	            if(wave1[lx/4]<wave1[lx/4+1]){
	                ramp=wave1[lx/4+1]-wave1[lx/4];
	                upordown=0;
	            }else{
	                ramp=wave1[lx/4]-wave1[lx/4+1];
	                upordown=1;
	            }
	            ramp=ramp>4?ramp:4;

	                                            height=bia+270-wave1[lx/4];

	                        if(upordown){
	                                            LCD_SetPos(lx,lx+4,height,height+ramp);
	                                        }else{
	                                            LCD_SetPos(lx,lx+4,height-ramp,height);

	                                        }	            for(loop=0;loop<ramp;loop++){
	                Write_Data(0,0x00);
	                Write_Data(0,0);
	                Write_Data(0,0x00);
	                Write_Data(0,0);
	                Write_Data(0,0x00);
	            }
	        }
	        /////////////////////////////////////////////////////////////////////////////
	        //画1/2、1/4、3/4线
	                //////////////////////////////////////////////////////////////////
	                        LCD_SetPos(0,239,143+bia,143+bia);
	                        for(lx=0;lx<240;lx++){
	                            if(lx%3){
	                                Write_Data(0x07,0xe0);
	                            }else{
	                            Write_Data(0x00,0x00);
	                        }
	                    }
	                        LCD_SetPos(0,239,76+bia,76+bia);
	                                        for(lx=0;lx<240;lx++){
	                                            if(!(lx%3)){
	                                                Write_Data(0xf8,0x1f);
	                                            }else{
	                                            Write_Data(0x00,0x00);
	                                        }
	                                    }
	                                        LCD_SetPos(0,239,209+bia,209+bia);
	                                                        for(lx=0;lx<240;lx++){
	                                                            if(!(lx%3)){
	                                                                Write_Data(0xf0,0xe0);
	                                                            }else{
	                                                            Write_Data(0x00,0x00);
	                                                        }
	                                                    }
	        ///////////////////////////////////////////////////////////////////////////////////////////////
	}
	
	return 0;
}
