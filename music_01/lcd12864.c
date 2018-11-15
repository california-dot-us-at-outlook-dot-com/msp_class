/*
 * 12864.c
 *
 *  Created on: 2018年10月30日
 *      Author: Cal
 */
#include"lcd12864.h"

void lcd12864_wc(unsigned char Data){
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataPort=Data;
    delay_ms(5);
    EN_CLR;
    delay_ms(5);
}

void lcd12864_wd(unsigned char Data){
    RS_SET;
    RW_CLR;
    EN_SET;
    DataPort=Data;
    delay_ms(5);
    EN_CLR;
    delay_ms(5);
}

void lcd12864_init(){
    P4SEL=0x00;
    P4DIR=0xFF;
    P5SEL=0x00;
    P5DIR=0xFF;
    PSB_SET;
    RST_SET;

    lcd12864_wc(FUN_MODE);            //显示模式设置
    delay_ms(5);
    lcd12864_wc(FUN_MODE);          //显示模式设置
    delay_ms(5);
    lcd12864_wc(DISPLAY_ON);            //显示开
    delay_ms(5);
    lcd12864_wc(CLEAR_SCREEN);          //清屏
    delay_ms(5);
}

void lcd12864_wstr(unsigned char comm,unsigned char* dat ){
    lcd12864_wc(comm);
    unsigned char i=0;
    for(i=0;dat[i];i++){
        lcd12864_wd(dat[i]);
    }
}

void menu_01(){
    lcd12864_wc(CLEAR_SCREEN);          //清屏
    lcd12864_wstr(0x82,"主菜单");
    lcd12864_wstr(0x90,"1.练琴");
    lcd12864_wstr(0x88,"2.创作自己的曲子");
    lcd12864_wstr(0x98,"3.已保存的曲子");
}

void menu_011(){
    lcd12864_wc(CLEAR_SCREEN);          //清屏
    //do somethings
    lcd12864_wstr(0x98,"右下角是返回键");
}

void menu_012(){
    lcd12864_wc(CLEAR_SCREEN);          //清屏
    //显示音符
    lcd12864_wstr(0x98,"保存");
    lcd12864_wstr(0x9c,"退出");
}

void menu_0121a(){
    lcd12864_wc(CLEAR_SCREEN);          //清屏
    lcd12864_wstr(0x80,"将被保存为");
    //do something
    lcd12864_wstr(0x90,"第");
    //do something
    lcd12864_wstr(0x94,"首歌曲");
    lcd12864_wstr(0x88,"1.确定  | 2.取消");
}

void menu_0121b(){
    lcd12864_wc(CLEAR_SCREEN);          //清屏
    lcd12864_wstr(0x80,"空间不足");
    //do something
    lcd12864_wstr(0x90,"将曲子覆盖：");
    //do something
    lcd12864_wstr(0x88,"1 | 2 | 3 | 4");
    lcd12864_wstr(0x98,"右下角返回键");
}

void menu_013(){
    lcd12864_wc(CLEAR_SCREEN);          //清屏
    lcd12864_wstr(0x80,"");

}

void menu_013x(){
    lcd12864_wc(CLEAR_SCREEN);          //清屏
    lcd12864_wstr(0x80,"可以执行：");
    lcd12864_wstr(0x90,"    1.播放");
    lcd12864_wstr(0x88,"    2.删除");
    lcd12864_wstr(0x98,"    3.返回上级");

}
