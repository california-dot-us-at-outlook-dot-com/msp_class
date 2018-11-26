/*
 * TFT.h
 *
 *  Created on: 2018年11月1日
 *      Author: Cal
 */

#ifndef TFT_H_
#define TFT_H_
#include <msp430f169.h>
#include"timeConfig.h"

//2.8寸TFT彩屏显示控制相关硬件配置
#define RS_CLR          P5OUT &= ~BIT5           //RS置低
#define RS_SET          P5OUT |=  BIT5           //RS置高

#define RW_CLR          P5OUT &= ~BIT6           //RW置低
#define RW_SET          P5OUT |=  BIT6           //RW置高

#define RD_CLR          P5OUT &= ~BIT7           //E置低
#define RD_SET          P5OUT |=  BIT7           //E置高

#define CS_CLR          P5OUT &= ~BIT0            //CS置低
#define CS_SET          P5OUT |=  BIT0            //CS置高

#define RST_CLR         P5OUT &= ~BIT3            //RST置低
#define RST_SET         P5OUT |=  BIT3            //RST置高

#define LE_CLR          P5OUT &= ~BIT1            //LE置低
#define LE_SET          P5OUT |=  BIT1            //LE置高

//2.8寸TFT彩屏触摸屏控制相关硬件配置
#define PEN_CLR         P2OUT &= ~BIT0           //PEN置低,触碰触摸屏时，Penirq引脚由未触摸时的高电平变为低电平
#define PEN_SET         P2OUT |=  BIT0           //PEN置高
#define PEN             (P2IN & 0x01)            //P2.0输入的值

#define TPDO_CLR    P2OUT &= ~BIT1           //TPDO置低
#define TPDO_SET    P2OUT |=  BIT1           //TPDO置高
#define TPDOUT          ((P2IN>>1)&0x01)         //P2.1输入的值

#define BUSY_CLR    P2OUT &= ~BIT3           //BUSY置低
#define BUSY_SET    P2OUT |=  BIT3           //BUSY置高

#define TPDI_CLR    P2OUT &= ~BIT4            //TPDI置低
#define TPDI_SET    P2OUT |=  BIT4            //TPDI置高

#define TPCS_CLR    P2OUT &= ~BIT5            //TPCS置低
#define TPCS_SET    P2OUT |=  BIT5            //TPCS置高

#define TPCLK_CLR   P2OUT &= ~BIT6            //TPCLK置低
#define TPCLK_SET   P2OUT |=  BIT6            //TPCLK置高

//彩屏/12864液晶/1602液晶的数据口，三液晶共用
#define DataDIR         P4DIR                     //数据口方向
#define DataPort        P4OUT                     //P4口为数据口




void Write_Data(unsigned char,unsigned char);
void  Write_Data_U16(unsigned int y);
void Write_Cmd(unsigned char ,unsigned char);
void Write_Cmd_Data(unsigned char,unsigned int );
void LCD_SetPos(unsigned int ,unsigned int,unsigned int,unsigned int);
void TFT_Initial();

void displayNums(unsigned int ,unsigned int,unsigned char);
void clearNums(unsigned int ,unsigned int,unsigned char);

//颜色代码，TFT显示用
#define White          0xFFFF                                                               //显示颜色代码
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0


#endif /* TFT_H_ */
