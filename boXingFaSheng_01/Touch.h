/*
 * Touch.h
 *
 *  Created on: 2018年11月15日
 *      Author: Cal
 */

#ifndef TOUCH_H_
#define TOUCH_H_

#include"timeConfig.h"


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


struct struct1                          //定义一个类型名为struct1的结构,包含两个成员x和y
{
    unsigned int x;                         //成员x
    unsigned int y;                         //成员y
};

#define Xmin           0x0133
#define Xmax           0x0EDA
#define Ymin           0x00D0
#define Ymax           0x0E2F

extern struct struct1 coordinate;      //定义一个名为Tp_pix的结构变量,用于存放处理后的触摸坐标参数
//extern unsigned int  ltx,lty;

void start_7843();
void Write_7843(unsigned char temp);          //SPI写8位命令到触摸控制IC
unsigned int Read_7843();                     //SPI 读数据
struct struct1 AD7843() ;
unsigned char pix_filter(struct struct1 pix1,struct struct1 pix2);
unsigned char Getpix();
void Detect_TP(void);

unsigned int  ltx,lty;            //根据采集到的参数转换为TFT上的实际坐标值
unsigned int LCD_X,LCD_Y;

/*触摸按钮等状态标志位*/
unsigned char   Button_Flag;          //按键按下标志

unsigned char   Button_11;                    //第一行第一列按钮
unsigned char   Button_12;
unsigned char   Button_13;

unsigned char   Button_21;                //第二行第一列按钮
unsigned char   Button_22;
unsigned char   Button_23;

unsigned char   Button_31;                //第三行第一列按钮
unsigned char   Button_32;
unsigned char   Button_33;

unsigned char   Button_41;                    //第四行第一列按钮
unsigned char   Button_42;
unsigned char   Button_43;

unsigned char   Button_Back;                  //退格按钮
unsigned char   Button_Clear;                 //清除按钮



#define xs(j)   (j*60+15*j+15)                    //按钮横坐标起点
#define ys(i)   (80+i*50+10*i)                                      //按钮纵坐标起点
#define xe(j)   (j*60+15*j+15+60)                 //按钮横坐标终点
#define ye(i)   (80+i*50+10*i+50)                //按钮纵坐标终点


#endif /* TOUCH_H_ */
