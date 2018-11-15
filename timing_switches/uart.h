/*
 * uart.h
 *
 *  Created on: 2018年10月31日
 *      Author: Cal
 */

#ifndef UART_H_
#define UART_H_

//串口波特率计算，当BRCLK=CPU_F时用下面的公式可以计算，否则要根据设置加入分频系数
#define baud           1480                                //设置波特率的大小

#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //波特率计算公式
#define baud_h         (uchar)(baud_setting>>8)            //提取高位
#define baud_l         (uchar)(baud_setting)               //低位

#define ulong unsigned long
#define uchar unsigned char
#define uint unsigned int

void UART_Init();



#endif /* UART_H_ */
