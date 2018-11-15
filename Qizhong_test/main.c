#include <msp430.h> 


/**
 * main.c
 */

#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

//���ڲ����ʼ��㣬��BRCLK=CPU_Fʱ������Ĺ�ʽ���Լ��㣬����Ҫ�������ü����Ƶϵ��
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (unsigned int)((unsigned long)CPU_F/((unsigned long)baud))  //�����ʼ��㹫ʽ
#define baud_h         (unsigned char)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (unsigned char)(baud_setting)               //��λ

unsigned char *tx;
unsigned char *rx;


//����/12864Һ��/1602Һ�������ݿڣ���Һ������
#define DataDIR         P4DIR                     //���ݿڷ���
#define DataPort        P4OUT                     //P4��Ϊ���ݿ�

//12864/1602Һ�����ƹܽ�
#define RS_CLR          P5OUT &= ~BIT5           //RS�õ�
#define RS_SET          P5OUT |=  BIT5           //RS�ø�

#define RW_CLR          P5OUT &= ~BIT6           //RW�õ�
#define RW_SET          P5OUT |=  BIT6           //RW�ø�

#define EN_CLR          P5OUT &= ~BIT7           //E�õ�
#define EN_SET          P5OUT |=  BIT7           //E�ø�

#define PSB_CLR         P5OUT &= ~BIT0            //PSB�õͣ����ڷ�ʽ
#define PSB_SET         P5OUT |=  BIT0            //PSB�øߣ����ڷ�ʽ

#define RST_CLR         P5OUT &= ~BIT1            //RST�õ�
#define RST_SET         P5OUT |= BIT1             //RST�ø�

void UART_Init()
{
  U0CTL|=SWRST;               //��λSWRST
  U0CTL|=CHAR;                //8λ����ģʽ
  U0TCTL|=SSEL1;              //SMCLKΪ����ʱ��
  U0BR1=baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0=baud_l;               //N=UBR+(UxMCTL)/8
  U0MCTL=0x00;                //΢���Ĵ���Ϊ0��������9600bps
  ME1|=UTXE0;                 //UART0����ʹ��
  ME1|=URXE0;                 //UART0����ʹ��
  U0CTL&=~SWRST;
  IE1|=URXIE0;                //�����ж�ʹ��λ

  P3SEL|= BIT4;               //����IO��Ϊ��ͨI/Oģʽ
  P3DIR|= BIT4;               //����IO�ڷ���Ϊ���
  P3SEL|= BIT5;
}
void Send_Byte(unsigned char data)
{
  while((IFG1&UTXIFG0)==0);          //���ͼĴ����յ�ʱ��������
    U0TXBUF=data;
}

unsigned char i=0;

#pragma vector=USART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
  unsigned char data=0;
  //data=U0RXBUF;                       //���յ������ݴ�����
  rx[i]=U0RXBUF;
  P6OUT++;
  if(rx[i]=='\n'){
      i=0;
      display();
  }else if(rx[i]==0){

  }else{
      i++;
  }
  Send_Byte(0x74);                    //�����յ��������ٷ��ͳ�ȥ
  //P6OUT=~data;
}

void Clock_Init()
{
  unsigned char i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG;
}

void intInit(){
    P1DIR &= 0;
    P1IES |= BIT0+BIT1; //0:�����أ�    1���½���
    P1IE |= BIT0+BIT2;  //�ж�����1Ϊ����
    P1IFG &= 0;  //�жϱ�־��0Ϊ�ɽ����ж�
}

void wd(unsigned char data){
    RS_SET;
    RW_CLR;
    EN_SET;
    DataPort=data;
    delay_ms(5);
    EN_CLR;
}

void wc(unsigned char data){
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataPort=data;
    delay_ms(5);
    EN_CLR;
}

void init1602(void){    P4DIR=0xff;
P4OUT=0x00;
P5SEL=0x00;
P5DIR=0xff;
    wc(0x38);
    delay_ms(5);
    wc(0x08);
    delay_ms(5);
    wc(0x01);
    delay_ms(5);
    wc(0x06);
    delay_ms(5);
    wc(0x0c);
    delay_ms(5);
}

void display(){
    unsigned char loop=0;
    for(loop=0;loop<16;loop++){
        wc(0x80+loop);
        wd(' ');
    }
    for(loop=0;rx[loop]!='\n';loop++){
        wc(0x80+loop);
        wd(rx[loop]);
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void){

}


void init_spi(){
    U0CTL |= SWRST;
    U0CTL |= CHAR+SYNC+MM;
    U0TCTL |= SSEL1+SSEL0+STC+CKPH;
    //U0TCTL=CKPH;
    //U0TCTL &= ~CKPL;
    U0BR0=0x02;
    U0BR1=0x00;
    ME1 |= USPIE0;
    U0CTL&=~SWRST;
    IE1&=~UTXIE0;
    IE1&=~URXIE0;
    P3SEL|=0x0e;
    P3DIR|=0x01;
}

void spi_sendByte(unsigned char dat){
    U0TXBUF=dat;
    while((IFG1&UTXIFG0)==0);
    //IFG1&=~UTXIFG0;
}


unsigned char SPI_Read_Byte(void)

{

    IFG2 &= 0xCF;

    U1TXBUF = 0xFF;

    while(!(IFG2 & URXIFG1)); //�ж�URXIFG1Ϊ������ɱ�־

    return U0RXBUF;

}

unsigned char spi_RW_reg(unsigned char dat){
    U0TXBUF=dat;
    while(!(IFG1&UTXIFG0));
    delay_ms(9);
    return U0RXBUF;
}

unsigned char onePressed=0;
unsigned char findPressed(){
    unsigned char a=1;
    unsigned char b=0;
    unsigned char num=0;
    unsigned char num1=0;
    for(a=1;a<0x10;a<<=1){
        P2DIR=0xff;
        P2OUT=0x00;
        P2DIR=0x0f;
        P2OUT=a;
        if(P2IN!=a){
            //P6OUT=~P5IN;
            b=1;
            if(onePressed){
                break;
            }
            unsigned char tmp=(P2IN>>4);
            for(;tmp;tmp>>=1){
                num++;
            }
            num1=a;
            if(b){break;}
        }if(b){break;}
        P2DIR=0xff;
        P2OUT=0x00;
    }
    if(!b){
        onePressed=0;
    }else{
        onePressed=1;
    }
    return (num)*4+num1;
}

void init_spi_slave(){
      P3SEL = 0x0E;                             // Setup P3 for SPI mode
      U0CTL = CHAR + SYNC + SWRST;              // 8-bit, SPI, Slave
      U0TCTL = CKPL + STC;                      // Polarity, UCLK, 3-wire
      ME1 = USPIE0;                             // Module enable
      U0CTL &= ~SWRST;                          // SPI enable
      IE1 |= URXIE0;//+UTXIE0;                            // Recieve interrupt enable
      //_EINT();                                  // Enable interrupts

}

/*
#pragma vector=USART0TX_VECTOR
__interrupt void SPI0_tx (void){
    unsigned char data=0;
    Send_Byte(0x74);
}
*/

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	P6DIR=0xff;
	P6OUT=0x00;
	init1602();
	tx=(unsigned char*)malloc(sizeof(unsigned char)*32);
	rx=(unsigned char*)malloc(sizeof(unsigned char)*32);

	//UART_Init();
    //init_spi();
    init_spi_slave();
	//intInit();


	_EINT();
	Send_Byte(0x34);
	unsigned char tmp;
	unsigned char t;
	unsigned char i;
	while(1){
	    if(i>16){
	        i=16;
	    }
	    tmp=findPressed()+'0';

	    if(tmp!='0'&&tmp!='='){
	        t=tmp;wc(0xc0+i);
	        wd(tmp);
	    }
	    if(tmp=='='){
	        for(tmp=0;tmp<i;tmp++){
	            //Send_Byte(tx[tmp]);
	            //spi_RW_reg(tx[tmp]);
	            wc(0xc0+tmp);
	            wd(' ');
	        }
	        //spi_RW_reg('\n');
	        i=0;
	    }else if(tmp!='0'){
	        tx[i]=t;
	        i++;
	    }
	}
	return 0;
}
