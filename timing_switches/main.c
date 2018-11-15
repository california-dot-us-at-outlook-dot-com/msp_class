#include <msp430.h> 
#include"timeConfig.h"
#include"variables.h"
#include"uart.h"
#include"lcd12864.h"
#include<stdlib.h>
#include"handle.h"
//#include"lcd12864.h"

/**
 * main.c
 */

void portInit(){
    P4DIR=0xff;
    P4OUT=0xff;
    delay_ms(100);
}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	P6DIR=0xff;
	portInit();
	P6OUT=1;
	c=(unsigned char*)malloc(sizeof(unsigned char)*8);
	timeValues=(unsigned char**)malloc(sizeof(unsigned char*)*4);
	unsigned char i=0;
	unsigned char j=0;
	P6OUT=2;
	status=20;
	for(i=0;i<255;i++){
	    writeAt(i,46);
	    //delay_ms(10);
	}
	P6OUT=3;
	for(i=0;i<2;i++){
	    timeValues[i]=(unsigned char*)malloc(sizeof(unsigned char)*4*4);
	}
	P6OUT=4;
	UART_Init();
	lcd12864_init();

	for(i=0;i<2;i++){
	    for(j=0;j<(4*4);j++){
	        timeValues[i][j]=readAt(i*16+j);
	    }
	}
	P6OUT=5;
	menu_01x();
	while(1){
	    P6OUT=keyValue;
	    //P6OUT=U0RXBUF;
	}
	return 0;
}


#pragma vector=USART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
    unsigned char data=0;


    data=U0RXBUF;
    //U1RXBUF=0;
  /*
    if(a==9){
        P6OUT=~data;
    }
    a++;
  */
    //Send_Byte(data);
    if(data==')' && b==0 ){
        //P6OUT=data;
        c[0]=data;
        b=1;

    }else if(b<8 && b>0){
        c[b]=data;
        b++;
    }

if(b==8){
    b=0;
    unsigned char key_1[8]={41,185,187,187,185,169,171,0b10111011};
        unsigned char key_2[8]={41,185,187,187,105,105,171,187};
        unsigned char key_3[8]={41,185,187,187,0b01101101,0b01101011,0b00101011,0b01101011};
        unsigned char key_4[8]={41,185,187,187,0b01101001,
                                0b01101001,
                                 0b01101011,
                                 0b10111011};
        unsigned char key_5[8]={41,185,187,187,0b10111001,
                                0b00101011,
                                0b00101011,
                                0b10111011

        };
        unsigned char key_6[8]={41,185,187,187,0b10101101,
                                0b01101011,
                                0b01101011,
                                0b10101101,

        };
        unsigned char key_7[8]={41,185,187,187,0b00101101,
                                0b10101101,
                                0b01101101,
                                0b01101011,

        };
        unsigned char key_8[8]={41,185,187,187,0b00101101,
                                0b10101101,
                                0b01101101,
                                0b01101011,

        };
        unsigned char key_9[8]={41,185,187,187,0b10101101,
                                0b00111001,
                                0b10101101,
                                0b01101011,

        };
        unsigned char key_0[8]={41,185,187,187,0b01101101,
                                0b00101011,
                                0b00111001,
                                0b10111011,

        };
        unsigned char if_1=1;
            unsigned char if_2=1;
            unsigned char if_3=1;
                unsigned char if_4=1;
                unsigned char if_5=1;
                    unsigned char if_6=1;
                    unsigned char if_7=1;
                        unsigned char if_8=1;
                        unsigned char if_9=1;
                            unsigned char if_0=1;

                            unsigned char cc=0;

                            for(cc=0;cc<8;cc++){
                                            if(c[cc]!=key_1[cc]){
                                                if_1=0;
                                            }
                                            if(c[cc]!=key_2[cc]){
                                                if_2=0;
                                            }
                                            if(c[cc]!=key_3[cc]){
                                                                if_3=0;
                                                            }
                                            if(c[cc]!=key_4[cc]){
                                                                if_4=0;
                                                            }
                                            if(c[cc]!=key_5[cc]){
                                                                if_5=0;
                                                            }
                                            if(c[cc]!=key_6[cc]){
                                                                if_6=0;
                                                            }
                                            if(c[cc]!=key_7[cc]){
                                                                if_7=0;
                                                            }
                                            if(c[cc]!=key_8[cc]){
                                                                if_8=0;
                                                            }
                                            if(c[cc]!=key_9[cc]){
                                                                if_9=0;
                                                            }
                                            if(c[cc]!=key_0[cc]){
                                                                if_0=0;
                                                            }
                                        }
                                        if(if_1){
                                            keyValue=1;
                                        }else if(if_2){
                                            keyValue=2;
                                        }else if(if_3){
                                            keyValue=3;
                                        }else if(if_4){
                                            keyValue=4;
                                        }else if(if_5){
                                            keyValue=5;
                                        }else if(if_6){
                                            keyValue=6;
                                        }/*else if(if_7){
                                            P6OUT=~7;
                                        }*/else if(if_8){
                                            keyValue=8;
                                        }else if(if_9){
                                            keyValue=9;
                                        }else if(if_0){
                                            keyValue=0;
                                        }


                                        if(keyValue==2||keyValue==8){
                                            setCursor();
                                        }
                                        if(keyValue==5 && status==0){
                                            jumpTo01x();
                                        }

}

}

