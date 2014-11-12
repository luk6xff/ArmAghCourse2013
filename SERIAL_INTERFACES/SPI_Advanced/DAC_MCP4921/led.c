#include<LPC21xx.h>
#include "stdbool.h"
#include "led.h"

#define LED0_bm 1<<16
#define LED1_bm 1<<17
#define LED2_bm 1<<18
#define LED3_bm 1<<19

void Led_On(unsigned char ucLedlndeks);
typedef enum{LEFT, RIGHT}Direction;

void Led_Init(void)
{
    IO1DIR|=LED0_bm|LED1_bm|LED2_bm|LED3_bm;
    IO1SET=LED0_bm;
}

///////////////////////////////////////////////////////////////////////
void Led_On(unsigned char ucLedlndeks)
{
    IO1CLR=(LED0_bm | LED1_bm | LED2_bm | LED3_bm);
    switch(ucLedlndeks)
    {
    case 0:
        IO1SET|=LED0_bm;
        break;
    case 1:
        IO1SET|=LED1_bm;
        break;
    case 2:
        IO1SET|=LED2_bm;
        break;
    case 3:
        IO1SET|=LED3_bm;
        break;

    }
}

///////////////////////////////////////////////////////////////////////

static void LedStep(Direction eDirection)
{
    static unsigned char cStateLed;

  switch(eDirection){
    case LEFT:
    {
        cStateLed++;
        Led_On(cStateLed%4);
        break;
    }
    case RIGHT:
    {
        cStateLed--;
        Led_On(cStateLed%4);
        break;
    }
   }
}

///////////////////////////////////////////////////////////////////////
void Led_StepLeft(void)
{
    LedStep(LEFT);
}
///////////////////////////////////////////////////////////////////////
void Led_StepRight(void)
{
    LedStep(RIGHT);
}

