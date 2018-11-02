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

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void){
    if(!scale){
        scale=1000;
    }
    P6OUT=~P6OUT;
    if(!(P1IN&BIT0)){
        scale>>=1;
    }else if(!(P1IN&BIT1)){
        scale<<=1;
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
	        CCTL1=OUTMOD_3; //��Ϊ0������CCR1��Ϊ1������CCR0��Ϊ0

	        CCR2=12580;
	        CCTL2=OUTMOD_7;//��Ϊ1������CCR1��Ϊ0������CCR0��Ϊ1

	        P6DIR=0xff;
	        P1DIR &= 0;
	        P1SEL=0x00;
	            P1IES |= BIT0+BIT1+BIT2; //0:�����أ�    1���½���
	            P1IE |= BIT0+BIT1+BIT2;  //�ж�����1Ϊ����
	            P1IFG &= 0;  //�жϱ�־��0Ϊ�ɽ����ж�
	            _EINT();    //�������ж�

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
	LCD_SetPos(0,239,285,285);
	    for(lx=0;lx<240;lx++){
	        Write_Data(0x00,0x1f);
	}
	LCD_SetPos(0,239,20,20);
	    for(lx=0;lx<240;lx++){
	        Write_Data(0xff,0xff);
	    }

	    LCD_SetPos(0,120,300,300);
	                    for(lx=0;lx<240;lx++){

	                            Write_Data(0xf8,0x1f);
	                    }
	unsigned long loop=0;
	while(1){
	    //����
	    /////////////////////////////////////////////////////////////
	    for(loop=0;loop<61;loop++){
	        ly=(adcTrans());

	        wave1[loop]=ly>>4;
	        //wave11[i/2]=ly>>4;
	        for(lx=0;lx<scale;lx++){
	            delay_us(1);
	        }
	    }

	    ///////////////////////////////////////////////////////////////

//������
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

	                height=280-wave1[lx/4];

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

	                                            height=280-wave1[lx/4];

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
	        //��1/2��1/4��3/4��
	                //////////////////////////////////////////////////////////////////
	                        LCD_SetPos(0,239,153,153);
	                        for(lx=0;lx<240;lx++){
	                            if(lx%3){
	                                Write_Data(0x07,0xe0);
	                            }else{
	                            Write_Data(0x00,0x00);
	                        }
	                    }
	                        LCD_SetPos(0,239,86,86);
	                                        for(lx=0;lx<240;lx++){
	                                            if(!(lx%3)){
	                                                Write_Data(0xf8,0x1f);
	                                            }else{
	                                            Write_Data(0x00,0x00);
	                                        }
	                                    }
	                                        LCD_SetPos(0,239,219,219);
	                                                        for(lx=0;lx<240;lx++){
	                                                            if(!(lx%3)){
	                                                                Write_Data(0xff,0xe0);
	                                                            }else{
	                                                            Write_Data(0x00,0x00);
	                                                        }
	                                                    }
	        ///////////////////////////////////////////////////////////////////////////////////////////////
	}
	
	return 0;
}
