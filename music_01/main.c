#include"lcd12864.h"

/**
 * main.c
 */
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

int main(void)
{

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	char a[2]="��";

	lcd12864_init();

	menu_01();

	return 0;
}
