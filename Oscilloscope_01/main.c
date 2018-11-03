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
unsigned long cloop=65536;

#define bia 3//bia最小为-10

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

unsigned long timeA=0;
unsigned int tmpTAR=0;
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A(void){
    if(cloop!=35536){
        timeA++;
    }
}


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	P1DIR |= 0x0f;
	    P1SEL |= 0x0f;
	    TACCR0=10000;
	    TACTL=TASSEL_2+MC_1+ID_3;
	    TACCTL0=CCIE;



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
	LCD_SetPos(0,239,277+bia,277+bia);
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



	//画刻度
	unsigned char i;
	for(i=0;i<240;i+=24){
	    LCD_SetPos(i,i,275+bia,277+bia);
	    Write_Data(0xf8,0x00);
	    Write_Data(0xf8,0x00);
	    Write_Data(0xf8,0x00);
	}
	//3.3V
	displayNums(2,0,3);
	displayNums(10,0,10);
	displayNums(18,0,3);
	displayNums(26,0,11);
//0V
	displayNums(2,275,0);
	displayNums(10,275,11);




	unsigned long loop=0;
	unsigned char waveTop=0;//绝对电压最大值
	unsigned char waveBottom=0;
	unsigned char waveMax=0;//相对电压最大
	unsigned char waveMin=255;
	unsigned long waveAver=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	while(1){
	    //int bias=128-((waveMax-waveMin)/2);
	    //采样
	    /////////////////////////////////////////////////////////////
	    waveMax=0;
	    waveMin=255;
	    waveAver=0;
	    timeA=0;
	    TAR=0;
	    for(cloop=0;cloop<61;cloop++){
	        ly=(adcTrans());
	      //  if(middle){
	        //    wave1[loop]=(ly>>4)+bias;
	          //  waveTop=waveMax-bias;
	            //waveBottom=waveMin-bias;
	        //}else{
	            wave1[cloop]=ly>>4;
	            waveAver+=wave1[cloop];
	            waveMax=(wave1[cloop]>waveMax?wave1[cloop]:waveMax);
	            waveMin=(wave1[cloop]<waveMin?wave1[cloop]:waveMin);
	          //  waveTop=waveMax;
	            //waveBottom=waveMin;
	        //}

	        //wave11[i/2]=ly>>4;
	        for(lx=0;lx<scale;lx++){
	            delay_us(1);
	        }
	    }
	    cloop=65536;
	    tmpTAR=TAR;
	    waveAver=waveAver/60;
	    //waveTop=waveMax;
	    //waveBottom=waveMin;
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

	        //写电压最大值
	        displayNums(48,280+bia,waveMax/77);
	        displayNums(56,280+bia,10);
	        displayNums(64,280+bia,(waveMax%77)/7.7);
	        displayNums(72,280+bia,((waveMax%77)%8)/0.77);
	        displayNums(80,280+bia,11);

	        //写电压最小值
	                    displayNums(48,298+bia,waveMin/77);
	                    displayNums(56,298+bia,10);
	                    displayNums(64,298+bia,(waveMin%77)/7.7);
	                    displayNums(72,298+bia,((waveMin%77)%8)/0.77);
	                    displayNums(80,298+bia,11);

	                    //写峰峰值
	                                            displayNums(48+40,288+bia,(waveMax-waveMin)/77);
	                                            displayNums(56+40,288+bia,10);
	                                            displayNums(64+40,288+bia,((waveMax-waveMin)%77)/7.7);
	                                            displayNums(72+40,288+bia,(((waveMax-waveMin)%77)%8)/0.77);
	                                            displayNums(80+40,288+bia,11);

	        //写电压平均值
	        displayNums(96+50,288+bia,waveAver/77);
	                    displayNums(104+50,288+bia,10);
	                    displayNums(112+50,288+bia,(waveAver%77)/7.7);
	                    displayNums(120+50,288+bia,((waveAver%77)%8)/0.77);
	                    displayNums(128+50,288+bia,11);


	        //写时间间隔
	                    displayNums(200,281,timeA/100);
	                    displayNums(208,281,(timeA%100)/10);
	                    displayNums(216,281,(timeA%10));
	                    displayNums(224,281,12);
	                    displayNums(232,281,14);

	                    displayNums(200,300,tmpTAR/1000);
	                    displayNums(208,300,(tmpTAR%1000)/100);
	                    displayNums(216,300,0);
                        displayNums(224,300,13);
                        displayNums(232,300,14);
//////////////显示的延时
	        ////////////////////////////////////////////////////////////////////////
	        for(loop=0;loop<scale;loop++){
	            delay_us(100);
	        }
	        /////////////////////////////////////////////////
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
