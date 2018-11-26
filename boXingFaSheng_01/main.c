#include <msp430.h> 

#include"timerA1_int.h"
#include"DMA.h"
#include"TFT.h"
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	TFT_Initial();
	P6DIR=0xff;
	timerA1_int_init(TASSEL_2,ID_3,65535);
	DMA_Init();
	while(1);
	return 0;
}
