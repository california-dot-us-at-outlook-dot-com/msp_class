/*
 * 12864.c
 *
 *  Created on: 2018��10��30��
 *      Author: Cal
 */
#include"lcd12864.h"
#include"timeConfig.h"
#include"variables.h"
void lcd12864_wc(unsigned char Data){
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataPort=Data;
    delay_ms(2);
    EN_CLR;
    delay_ms(2);
}

void lcd12864_wd(unsigned char Data){
    RS_SET;
    RW_CLR;
    EN_SET;
    DataPort=Data;
    delay_ms(2);
    EN_CLR;
    delay_ms(2);
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
    lcd12864_wstr(0x80,"  ���õ���·����");
    lcd12864_wstr(0x90,"  ���õ�һ·����");
    lcd12864_wstr(0x88,"  ���õڶ�·����");
    lcd12864_wstr(0x98,"  ���õ���·����");

}

void menu_01x(){
    unsigned char i=0;
    unsigned char one;
    unsigned char ten;
    lcd12864_wc(CLEAR_SCREEN);
    for(i=0;i<4;i++){
        if(timeValues[status-20][i*4]==0){
            lcd12864_wstr(0x81+i*8,"��");
        }else if(timeValues[status-20][i*4]==1){
            lcd12864_wstr(0x81+i*8,"��");
        }else if(timeValues[status-20][i*4]==2){
            lcd12864_wstr(0x81+i*8,"��");
        }else{
            lcd12864_wstr(0x81+i*8,"��");
        }
        lcd12864_wc(0x81+i*8);
        lcd12864_wd((timeValues[status-20][i*4])/10+'0');

        lcd12864_wd((timeValues[status-20][i*4])%10+'0');
    }

    for(i=0;i<4;i++){
        ten=(timeValues[status-20][i*4+1])/10;
        one=(timeValues[status-20][i*4+1])%10;
        lcd12864_wc(0x83+i*8);
        lcd12864_wd(ten+'0');
        //lcd12864_wd(':');
        lcd12864_wd(one+'0');
        //lcd12864_wstr(0x84," :");
    }
    for(i=0;i<4;i++){
            ten=(timeValues[status-20][i*4+2])/10;
            one=(timeValues[status-20][i*4+2])%10;
            lcd12864_wc(0x84+i*8);
            lcd12864_wd(':');
            lcd12864_wd(ten+'0');
            //lcd12864_wd(':');
            lcd12864_wd(one+'0');
        }

    for(i=0;i<4;i++){
        //lcd12864_wc(0x86);
        lcd12864_wstr(0x86+i*8,"OK");
    }
}
void setCursor(){
    //unsigned char **cur=(unsigned char **)malloc(sizeof(unsigned char*));
    unsigned char i=0;
    unsigned char cur=cursor;
    if(keyValue==2 && cursor>0){
        cursor-=1;
    }else if(keyValue==8 && cursor<3){
        cursor+=1;
    }
    if(cur!=cursor || (keyValue==2&&cur==3) || (keyValue==8&&cur==0)){
    for(i=0;i<4;i++){
        lcd12864_wstr(0x80," ");
        lcd12864_wstr(0x90," ");
        lcd12864_wstr(0x88," ");
        lcd12864_wstr(0x98," ");
    }
    if(cursor==0){
        lcd12864_wstr(0x80,">");
    }else if(cursor==1){
        lcd12864_wstr(0x90,">");
    }else if(cursor==2){
        lcd12864_wstr(0x88,">");
    }else if(cursor==3){
        lcd12864_wstr(0x98,">");
    }
    }
}

void menu_02(){
    lcd12864_wc(CLEAR_SCREEN);          //����
    lcd12864_wstr(0x82,"");
    lcd12864_wstr(0x90,"");
    lcd12864_wstr(0x88,"");
    lcd12864_wstr(0x98,"");
}
