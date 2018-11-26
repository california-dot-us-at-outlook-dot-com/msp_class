/*
 * timerA1_int.c
 *
 *  Created on: 2018Äê11ÔÂ14ÈÕ
 *      Author: Cal
 */

#include"timerA1_int.h"
#include"DMA.h"

void timerA1_int_init(unsigned int clk,unsigned int ID,unsigned int time){
    TACTL = clk + MC_1 + TAIE + ID;           // SMCLK, contmode, interrupt
    TACCR0=time;
    _EINT();
}


#pragma vector=TIMERA1_VECTOR
__interrupt void TimerA(){
    switch(TAIV){
        case 2:break;
        case 4:break;
        case 10:DMA_trans();break;
    }
}
