#include<LPC21xx.h>
#include "led.h"
#include "keyboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void vDelay(int iCounter)
{
    int iCounterLoop;
    for( iCounterLoop=0; iCounterLoop<(iCounter*834*6); iCounterLoop++ );
}

void Automat2(void);
void Automat3(void);
void Automat4(void);
void Automat5(void);
void Automat6(void);
void Automat7(void);

int main(void)
{
    Led_Init();
    KeyboardInit();

    while(1)
    {
       /* Automat2();
        Automat3();
        Automat4();
        Automat5();
        Automat6();*/
        Automat7();
    }
    return 0;
}


void Automat2(void)
{
    typedef enum {STATE0, STATE1, STATE2,STATE3,STATE4, STATE5} LedState;
    LedState eLedState = STATE0;
    while (1)
    {
      switch (eLedState){
        case STATE0:
            eLedState = STATE1;
            Led_StepRight();
            break;
        case STATE1:
            eLedState = STATE2;
            Led_StepRight();
            break;
        case STATE2:
            eLedState = STATE3;
            Led_StepRight();
            break;
        case STATE3:
            eLedState = STATE4;
            Led_StepLeft();
            break;
        case STATE4:
            eLedState = STATE5;
            Led_StepLeft();
            break;
        case STATE5:
            eLedState = STATE0;
            Led_StepLeft();
            break;
        }
     vDelay(500);
    }
}
void Automat3(void)
{
    unsigned char ucLedMoveCounter=0;

    typedef enum {LEFTN, RIGHTN} LedState;
    LedState eLedState=LEFTN;
    while(1)
    {
       switch(eLedState){
        case LEFTN:
        {

            if(++ucLedMoveCounter<=3)
            {
                eLedState=LEFTN;
                Led_StepLeft();
            }
            else
            {
                eLedState = RIGHTN;
            }
            break;
        }
        case RIGHTN:
        {
            if(--ucLedMoveCounter>0)
            {
                eLedState=RIGHTN;
                Led_StepRight();
            }
            else
            {
                eLedState= LEFTN;
            }
            break;
        }
      }
     vDelay(500);
    }
}
void Automat4(void)
{

    unsigned char ucLedMoveCounter = 0;
    typedef enum {STOP, MOVE} LedState;
    LedState eLedState = MOVE;
    while (1)
    {
      switch (eLedState){
        case MOVE:
        {
            if (++ucLedMoveCounter <= 3)
            {
                eLedState = MOVE;
                Led_StepLeft();
            }
            else
            {
                eLedState = STOP;
            }
            break;
        }
        case STOP:
        {
            eLedState = STOP;
            break;
        }
      }
     vDelay(500);
    }
}
void Automat5(void)
{
    unsigned char ucLedMoveCounter = 0;
    typedef enum {STOP, MOVE} LedState;
    LedState eLedState = STOP;
    while (1)
    {
      switch (eLedState){
        case MOVE:
        {

            if (++ucLedMoveCounter <= 3)
            {
                eLedState = MOVE;
                Led_StepLeft();
            }
            else
            {
                eLedState = STOP;
            }
            break;
        }
        case STOP:
        {
            if (eKeyboard_Read()==BUTTON_1)
            {
                eLedState = MOVE;
            }
            else
                eLedState = STOP;
            break;
        }
      }
     vDelay(500);
    }
}
void Automat6(void)
{
    typedef enum {STOP,RIGHT} LedState;
    LedState eLedState = RIGHT;
    while (1)
    {
      switch (eLedState){
        case STOP:
        {
            if (eKeyboard_Read()==BUTTON_2)
            {
                eLedState = RIGHT;
            }
            break;
        }
        case RIGHT:
        {
            if (eKeyboard_Read()==BUTTON_1)
            {
                eLedState = STOP;
            }
            else Led_StepRight();

            break;
        }
      }
     vDelay(500);
    }
}
void Automat7(void)
{
		unsigned char ucWaitCounter=0;
		typedef enum {STOP, LEFT,RIGHT,IDLE} LedState;
    LedState eLedState = STOP;
    while (1)
    {
      switch (eLedState){
        case STOP:
        {
            if (eKeyboard_Read()==BUTTON_1)
            {
                eLedState = LEFT;
            }
            else if (eKeyboard_Read()==BUTTON_3)
            {
                eLedState = RIGHT;
            }
            else;
            break;
        }
        case RIGHT:
        {
            if (eKeyboard_Read()==BUTTON_2)
            {
                eLedState = IDLE;
            }
            else Led_StepRight();
            break;
        }
        case LEFT:
        {
            if (eKeyboard_Read()==BUTTON_2)
            {
                eLedState = IDLE;
            }
            else
                Led_StepLeft();
            break;
        }
				case IDLE:
        {
            if (ucWaitCounter<10)
            {
                ucWaitCounter++;
            }
            else 
            {
                ucWaitCounter=0;
								eLedState = STOP;							
            }
            break;
        }
      }
     vDelay(250);
    }
}


