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


int main(void)
{
    Led_Init();
    KeyboardInit();
    while(1)
    {
////////////////////////////////////////////////
      switch(ReadButton1()){
        case RELEASED:
        {
            Led_On(0);
            break;
        }
        case PRESSED:
        {
            Led_On(1);
            break;
        }
      }
////////////////////////////////////////////////

      switch(eKeyboard_Read()){
        case RELEASED:
        {
            Led_On(4);
            break;
        }
        case BUTTON_1:
        {
            Led_On(0);
            break;
        }
        case BUTTON_2:
        {
            Led_On(1);
            break;
        }
        case BUTTON_3:
        {
            Led_On(2);
            break;
        }
        case BUTTON_4:
        {
            Led_On(3);
            break;
        }
			}

////////////////////////////////////////////////
        unsigned char ucLedCounter;
        for(ucLedCounter=0; ucLedCounter<9; ucLedCounter++)
        {
            Led_StepLeft();
        }
        for(ucLedCounter=0; ucLedCounter<9; ucLedCounter++)
        {
            Led_StepRight();
        }
////////////////////////////////////////////////

      switch(eKeyboard_Read()){
        case BUTTON_1:
        {
            Led_StepRight();
            break;
        }
        case BUTTON_2:
        {
            Led_StepLeft();
            break;
        }
        default:
            break;
        }
		}
}
		
	
