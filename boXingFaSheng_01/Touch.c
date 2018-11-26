/*
 * Touch.c
 *
 *  Created on: 2018年11月15日
 *      Author: Cal
 */




/*===================================================================================================
文件功能描述：320x240TFT触摸板控制程序，实现触摸位置坐标采集。
====================================================================================================*/

//#define unsigned int ;
#include<msp430.h>
#include"Touch.h"
#include"GUI.h"
//#include"Config.h"
struct struct1 coordinate;      //定义一个名为Tp_pix的结构变量,用于存放触摸点采样结果

//Xmin、max和Ymin、Ymax分别是触摸屏横纵坐标的最小/最大值,如果偏差大时，用户可重新标定

//================================================================================================
//  实现功能：延时
//================================================================================================
void delay(unsigned int i)
{
    while(i--);
}
//================================================================================================
//  实现功能：   ADS7843启动
//================================================================================================
void start_7843()
{
    P2DIR |= BIT3 + BIT4 + BIT5 + BIT6;
    //P3DIR |= 0xff;
    //P4DIR |=0xff;
    TPCLK_CLR;
    TPCS_SET;
    TPDI_SET;
    TPCLK_SET;
    TPCS_CLR;

}

//================================================================================================
//  实现功能：   写8位命令到触摸控制IC
//  输入参数：  temp 需要写入的8位控制命令
//================================================================================================
void Write_7843(unsigned char temp)                         //SPI写8位命令到触摸控制IC
{
    unsigned char i=0;

    for(i=0;i<8;i++)                                    //循环8次写入一字节
        {
            if(temp&0x80)TPDI_SET;else TPDI_CLR;    //判断最高位是否为1,为1则向数据位写1
            TPCLK_CLR; delay(1);                //送一个脉冲，上升沿有效，将DIN位数据送入到IC
            TPCLK_SET; delay(1);
            temp<<=1;                           //待写数据左移1位，准备好写下一位数据
        }
}

//================================================================================================
//  实现功能：   从触摸控制IC读8位数据到控制器
//  返回参数：  temp 需要写入的8位控制命令
//================================================================================================
unsigned int Read_7843()                          //SPI 读数据
{
    unsigned char i=0;
    unsigned int temp=0;

    for(i=0;i<12;i++)                         //循环12次读取12位结果
     {
       temp<<=1;                                  //temp左移一位，准备读取下一位
       TPCLK_SET; delay(1);                   //下降沿有效
       TPCLK_CLR; delay(1);
       if(TPDOUT) temp++;                     //判断控制IC送出的一位数据是否为1,如果为1,赋给temp的最低位
     }
    return(temp);                                 //返回结果
}

//================================================================================================
//  实现功能：读取触摸点X轴和Y轴电压值
//  返回参数：pix 读取到的触摸点电压值
//================================================================================================
struct struct1 AD7843()
{
    struct struct1 pix;
    TPCS_CLR;

    Write_7843(0x90);                 //送控制字 10010000 即用差分方式读X坐标，详细请见有关资料
    TPCLK_SET; delay(1);
    TPCLK_CLR; delay(1);
    pix.y=Read_7843();

    Write_7843(0xD0);                 //送控制字 11010000 即用差分方式读Y坐标 详细请见有关资料
    TPCLK_SET; delay(1);
    TPCLK_CLR; delay(1);
    pix.x=Read_7843();

    TPCS_SET;
    return pix;
}

//================================================================================================
//  实现功能：软件滤波，滤掉波动过大的采样点
//  返回参数：flag 采集数据是否有效标志,flag=1;则数据有效
//================================================================================================
unsigned char pix_filter(struct struct1 pix1,struct struct1 pix2)
{
    unsigned char flag=0;
    int x=pix1.x>pix2.x?pix1.x-pix2.x:pix2.x-pix1.x;  //X轴两次采样绝对值
    int y=pix1.y>pix2.y?pix1.y-pix2.y:pix2.y-pix1.y;  //Y轴两次采样绝对值
    if(x<10&&y<10)                                    //软件滤波，2次取样的值相差太大的视为噪声
    {
        flag=1;
        coordinate.x=(pix1.x+pix2.x)/2;           //求两次采样平均值
        coordinate.y=(pix1.y+pix2.y)/2;
    }
    return flag;
}

//================================================================================================
//  实现功能：读取采集结果,两次取均值
//================================================================================================
unsigned char Getpix() //取采样值，此处使用软件滤波，2次取样的值相差太大的视为噪声
{
    unsigned char flag=0;
    struct struct1 pix1;
    struct struct1 pix2;
    if (PEN==0)
        {
            pix1=AD7843();
            pix2=AD7843();

            if(pix_filter(pix1,pix2)==1) //得到当前TP的取样值，此处使用软件滤波，2次取样的值相差太大的视为噪声
                {
                  if((coordinate.x>Xmin)&&(coordinate.y>Ymin))
                   {
                    ltx=(unsigned int)(240.0*(coordinate.x-Xmin)/(Xmax-Xmin));       //坐标转换，即根据采样值计算实际坐标值
                    lty=(unsigned int)(320.0*(coordinate.y-Ymin)/(Ymax-Ymin));   //Xmin、max和Ymin、Ymax分别是触摸屏横纵坐标的最小/最大值
                    flag=1;
                    LCD_X=ltx;
                    LCD_Y=lty;
                   }
                }
        }

    return flag;

}


/*
 *
 *
 *
 * 按键高亮
 */
/*
void GUIsquare2pix(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int full_colour)
{
    unsigned int i,j;

    for(i=0;i<2;i++)
            {
                GUIline(x0+i,y0,x0+i,y1,full_colour);
                GUIline(x0,y0+i,x1,y0+i,full_colour);
                GUIline(x0,y1-i,x1,y1-i,full_colour);
                GUIline(x1-i,y0,x1-i,y1,full_colour);
            }
}
*/

/*
 *
 *
 *
 * 识别按键
 *
 */

void Detect_TP(void)
{

     if(Getpix()==1)
      {
       /*第一列4个按钮11,21,31,41*/
    if((LCD_X>=xs(0))&& (LCD_X<=xe(0)))     //依据公式参考桌面绘制时的公式，main函数中，xs和xe，横坐标起点和终点
    {
    if((LCD_Y>=ys(0)) &&(LCD_Y<=ye(0)))  //依据公式，ys和ye，纵坐标起点和终点
    {
      Button_11=1;
      GUIsquare2pix(xs(0),ys(0),xe(0),ye(0),Red);
      delay_ms(1);
      delay_ms(1);
      delay_ms(1);
      while(PEN==0);
      GUIsquare2pix(xs(0),ys(0),xe(0),ye(0),Black);
    }

        else if((LCD_Y>=ys(1)) &&(LCD_Y<=ye(1)))

        {

          Button_21=1;

          GUIsquare2pix(xs(0),ys(1),xe(0),ye(1),Red);

          delay_ms(1);

          delay_ms(1);

          delay_ms(1);

          while(PEN==0);

          GUIsquare2pix(xs(0),ys(1),xe(0),ye(1),Black);

        }

        else if((LCD_Y>=ys(2)) &&(LCD_Y<=ye(2)))

        {

          Button_31=1;

          GUIsquare2pix(xs(0),ys(2),xe(0),ye(2),Red);

          delay_ms(1);

          delay_ms(1);

          delay_ms(1);

          while(PEN==0);

          GUIsquare2pix(xs(0),ys(2),xe(0),ye(2),Black);

        }

        else if((LCD_Y>=ys(3)) &&(LCD_Y<=ye(3)))

        {

          Button_41=1;

          GUIsquare2pix(xs(0),ys(3),xe(0),ye(3),Red);

          delay_ms(1);

          delay_ms(1);

          delay_ms(1);

          while(PEN==0);

          GUIsquare2pix(xs(0),ys(3),xe(0),ye(3),Black);

        }

        }
      /*第二列4个按钮12,22,32,42*/
        if((LCD_X>=xs(1))&& (LCD_X<=xe(1)))     //依据公式参考桌面绘制时的公式，main函数中，xs和xe，横坐标起点和终点

        {

          if((LCD_Y>=ys(0)) &&(LCD_Y<=ye(0)))  //依据公式，ys和ye，纵坐标起点和终点

          {

            Button_12=1;

            GUIsquare2pix(xs(1),ys(0),xe(1),ye(0),Red);

            delay_ms(1);

            delay_ms(1);

            delay_ms(1);

            while(PEN==0);

            GUIsquare2pix(xs(1),ys(0),xe(1),ye(0),Black);

          }


          else if((LCD_Y>=ys(1)) &&(LCD_Y<=ye(1)))

          {

            Button_22=1;

            GUIsquare2pix(xs(1),ys(1),xe(1),ye(1),Red);

            delay_ms(1);

            delay_ms(1);

            delay_ms(1);

            while(PEN==0);

            GUIsquare2pix(xs(1),ys(1),xe(1),ye(1),Black);

          }

          else if((LCD_Y>=ys(2)) &&(LCD_Y<=ye(2)))

          {
            Button_32=1;

            GUIsquare2pix(xs(1),ys(2),xe(1),ye(2),Red);

            delay_ms(1);

            delay_ms(1);

            delay_ms(1);

            while(PEN==0);

            GUIsquare2pix(xs(1),ys(2),xe(1),ye(2),Black);

          }
          else if((LCD_Y>=ys(3)) &&(LCD_Y<=ye(3)))

          {
            Button_42=1;

            GUIsquare2pix(xs(1),ys(3),xe(1),ye(3),Red);

            delay_ms(1);

            delay_ms(1);

            delay_ms(1);

            while(PEN==0);

            GUIsquare2pix(xs(1),ys(3),xe(1),ye(3),Black);

          }

        }

        /*第三列4个按钮13,23,33,43*/
      if((LCD_X>=xs(2))&& (LCD_X<=xe(2)))     //依据公式参考桌面绘制时的公式，main函数中，xs和xe，横坐标起点和终点

      {

        if((LCD_Y>=ys(0)) &&(LCD_Y<=ye(0)))  //依据公式，ys和ye，纵坐标起点和终点

        {

          Button_13=1;

          GUIsquare2pix(xs(2),ys(0),xe(2),ye(0),Red);

          delay_ms(1);

          delay_ms(1);

          delay_ms(1);

          while(PEN==0);

          GUIsquare2pix(xs(2),ys(0),xe(2),ye(0),Black);

        }

        else if((LCD_Y>=ys(1)) &&(LCD_Y<=ye(1)))

        {

          Button_23=1;

          GUIsquare2pix(xs(2),ys(1),xe(2),ye(1),Red);

          delay_ms(1);

          delay_ms(1);

          delay_ms(1);

          while(PEN==0);

          GUIsquare2pix(xs(2),ys(1),xe(2),ye(1),Black);

        }

        else if((LCD_Y>=ys(2)) &&(LCD_Y<=ye(2)))

        {

          Button_33=1;

          GUIsquare2pix(xs(2),ys(2),xe(2),ye(2),Red);

          delay_ms(1);

          delay_ms(1);

          delay_ms(1);

          while(PEN==0);

          GUIsquare2pix(xs(2),ys(2),xe(2),ye(2),Black);

        }
        else if((LCD_Y>=ys(3)) &&(LCD_Y<=ye(3)))

        {

          Button_43=1;

          GUIsquare2pix(xs(2),ys(3),xe(2),ye(3),Red);

          delay_ms(1);

          delay_ms(1);

          delay_ms(1);

          while(PEN==0);

          GUIsquare2pix(xs(2),ys(3),xe(2),ye(3),Black);

        }

      }


      }

}
