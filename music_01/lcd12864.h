/*
 * 12864.h
 *
 *  Created on: 2018��10��30��
 *      Author: Cal
 */

#ifndef LCD12864_H_
#define LCD12864_H_
#include<msp430f169.h>
#include"music.h"
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

//12864Ӧ��ָ�
#define CLEAR_SCREEN    0x01                  //����ָ�������ACֵΪ00H
#define AC_INIT     0x02                  //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD   0x06                  //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE    0x30                  //����ģʽ��8λ����ָ�
#define DISPLAY_ON  0x0c                  //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF 0x08                  //��ʾ��
#define CURSE_DIR   0x14                  //�α������ƶ�:AC=AC+1
#define SET_CG_AC   0x40                  //����AC����ΧΪ��00H~3FH
#define SET_DD_AC   0x80                      //����DDRAM AC
#define FUN_MODEK   0x36                  //����ģʽ��8λ��չָ�

void lcd12864_wc(unsigned char);
void lcd12864_wd(unsigned char);
void lcd12864_init();
void lcd12864_wstr(unsigned char,unsigned char*);
void menu_01();



//ȫ�ֱ���

unsigned char menu_status;
unsigned char key_value;

#endif /* 12864_H_ */
