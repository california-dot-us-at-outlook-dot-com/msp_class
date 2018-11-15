/*
 * uart.h
 *
 *  Created on: 2018��10��31��
 *      Author: Cal
 */

#ifndef UART_H_
#define UART_H_

//���ڲ����ʼ��㣬��BRCLK=CPU_Fʱ������Ĺ�ʽ���Լ��㣬����Ҫ�������ü����Ƶϵ��
#define baud           1480                                //���ò����ʵĴ�С

#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ

#define ulong unsigned long
#define uchar unsigned char
#define uint unsigned int

void UART_Init();



#endif /* UART_H_ */
