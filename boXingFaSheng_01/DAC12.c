/*
 * DAC12.c
 *
 *  Created on: 2018Äê11ÔÂ26ÈÕ
 *      Author: Cal
 */
#include"timeConfig.h"
#include"DAC12.h"


void Init_DAC12_0(void){
    P6SEL |= 0b00000001;
    P6DIR |= 0b00000001;
    ADC12CTL0 = REF2_5V + REFON;
    DAC12_0CTL = DAC12IR + DAC12AMP_5 + DAC12ENC;
    DAC12_0DAT = 0x0000;
}

void Init_DAC12_1(void){
    P6SEL |= 0b00000010;
    P6DIR |= 0b00000010;
    ADC12CTL1 = REF2_5V + REFON;
    DAC12_1CTL = DAC12IR + DAC12AMP_5 + DAC12ENC;
    DAC12_1DAT = 0x0000;
}
