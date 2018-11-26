#ifndef GUI_H
#define GUI_H

#include"TFT.h"

void GUIline(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int line_color);   //画线函数
void GUIfull(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int full_colour) ;
void plotC(unsigned int x,unsigned int y,unsigned int xc,int yc,unsigned int yc_colour) ;
void GUIcircle(unsigned int xc,unsigned int yc,unsigned int r,unsigned int circle_colour) ;
void GUIDotline(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int line_color);   //画虚线函数
void CLR_Screen(unsigned int bColor);
void DisplayDesk();
void GUIsquare2pix(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int full_colour);
#endif
