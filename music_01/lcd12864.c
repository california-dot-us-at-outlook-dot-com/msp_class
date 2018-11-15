/*
 * 12864.c
 *
 *  Created on: 2018��10��30��
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

    lcd12864_wc(FUN_MODE);            //��ʾģʽ����
    delay_ms(5);
    lcd12864_wc(FUN_MODE);          //��ʾģʽ����
    delay_ms(5);
    lcd12864_wc(DISPLAY_ON);            //��ʾ��
    delay_ms(5);
    lcd12864_wc(CLEAR_SCREEN);          //����
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
    lcd12864_wc(CLEAR_SCREEN);          //����
    lcd12864_wstr(0x82,"���˵�");
    lcd12864_wstr(0x90,"1.����");
    lcd12864_wstr(0x88,"2.�����Լ�������");
    lcd12864_wstr(0x98,"3.�ѱ��������");
}

void menu_011(){
    lcd12864_wc(CLEAR_SCREEN);          //����
    //do somethings
    lcd12864_wstr(0x98,"���½��Ƿ��ؼ�");
}

void menu_012(){
    lcd12864_wc(CLEAR_SCREEN);          //����
    //��ʾ����
    lcd12864_wstr(0x98,"����");
    lcd12864_wstr(0x9c,"�˳�");
}

void menu_0121a(){
    lcd12864_wc(CLEAR_SCREEN);          //����
    lcd12864_wstr(0x80,"��������Ϊ");
    //do something
    lcd12864_wstr(0x90,"��");
    //do something
    lcd12864_wstr(0x94,"�׸���");
    lcd12864_wstr(0x88,"1.ȷ��  | 2.ȡ��");
}

void menu_0121b(){
    lcd12864_wc(CLEAR_SCREEN);          //����
    lcd12864_wstr(0x80,"�ռ䲻��");
    //do something
    lcd12864_wstr(0x90,"�����Ӹ��ǣ�");
    //do something
    lcd12864_wstr(0x88,"1 | 2 | 3 | 4");
    lcd12864_wstr(0x98,"���½Ƿ��ؼ�");
}

void menu_013(){
    lcd12864_wc(CLEAR_SCREEN);          //����
    lcd12864_wstr(0x80,"");

}

void menu_013x(){
    lcd12864_wc(CLEAR_SCREEN);          //����
    lcd12864_wstr(0x80,"����ִ�У�");
    lcd12864_wstr(0x90,"    1.����");
    lcd12864_wstr(0x88,"    2.ɾ��");
    lcd12864_wstr(0x98,"    3.�����ϼ�");

}
