#include<LPC21xx.h>
#include "stdbool.h"
#include "keyboard.h"

#define BUTTON1_bm 1<<4
#define BUTTON2_bm 1<<5
#define BUTTON3_bm 1<<6
#define BUTTON4_bm 1<<7


void KeyboardInit(void)
{
    IODIR0&=~(BUTTON1_bm|BUTTON2_bm|BUTTON3_bm|BUTTON4_bm);
}


KeyboardState eKeyboard_Read(void)
{
    if((IO0PIN&(BUTTON1_bm))==0) return BUTTON_1;
    else if ((IO0PIN&BUTTON2_bm)==0) return BUTTON_2;
    else if ((IO0PIN&BUTTON3_bm)==0) return BUTTON_3;
    else if ((IO0PIN&BUTTON4_bm)==0) return BUTTON_4;
    else return RELEASED_;
}








