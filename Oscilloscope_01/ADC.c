/*
 * ADC.c
 *
 *  Created on: 2018年11月1日
 *      Author: Cal
 */
#include"ADC.h"
void adcInit(){

    P6SEL|=0x01;
    ADC12CTL0=ADC12ON+REFON+REF2_5V;//+SHT0_15;
    //ADC12CTL0&=(~(SHT0_15));

    ADC12CTL1=SHP+CSTARTADD_0+ADC12SSEL_2;//+CONSEQ_2;//P6.1,adc12ssel2使用mclk
    ADC12MCTL0=SREF_1;
    ADC12MCTL0|=INCH_0+EOS;//P6.1
    unsigned int i;
    for(i=0xFFF;i>0;i--);
    ADC12CTL0 |= ENC;
}

unsigned int adcTrans(){
    unsigned int dat;
    ADC12CTL0 |= ADC12SC;
    while((ADC12IFG & BIT0)==0);//P6.1
    dat=(long)ADC12MEM0;//P6.1
    return dat;
}
