/*
 * DMA.c
 *
 *  Created on: 2018Äê11ÔÂ26ÈÕ
 *      Author: Cal
 */
#include"DMA.h"

const unsigned char sin[32]={0,1,2,3,4,5,6,};

void DMA_Init(){
    DMA0SA=&sin;
   // DMA0DA=DAC12_0DAT;
    DMA0DA=&P6OUT;
    DMA0SZ=0x05;//data size
    DMA0CTL=DMADSTBYTE+DMASRCBYTE+DMADT_4+DMASRCINCR_3+DMAEN;
}




void DMA_trans(){
     //DMA1CTL /= DMAREQ;
    //DMA0CTL |= DMAREQ;
    P6OUT = (P6OUT&(~BIT7))|((~P6OUT)&BIT7);

}



