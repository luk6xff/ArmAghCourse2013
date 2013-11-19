#include<LPC21xx.h>
#include "stdbool.h"
#include "led.h"

#define LED0_bm 1<<16 
#define LED1_bm 1<<17
#define LED2_bm 1<<18
#define LED3_bm 1<<19

///////////////////////////////////////////////////////////////////////

void Led_On(unsigned int ucLedIndeks){
	
	IO1CLR=LED0_bm|LED1_bm|LED2_bm|LED2_bm;
	switch(ucLedIndeks){
    case 0:ucLedIndeks=LED0_bm; break;
		case 1:ucLedIndeks=LED1_bm;	break;
    case 2:ucLedIndeks=LED2_bm;	break;
	  case 3:ucLedIndeks=LED2_bm;	break;
		}
  IO1SET|=ucLedIndeks;
}

///////////////////////////////////////////////////////////////////////

static void LedStep(Direction eDirection){
	static  int uiStateLed;
	
	switch(eDirection){
		case LEFT:{
			uiStateLed++;
			if(uiStateLed>3){
					uiStateLed=0;
			}
			Led_On(LED0_bm<<uiStateLed);
		break;
		}
		case RIGHT:{uiStateLed--;
			if(uiStateLed<0){
					uiStateLed=3;
			}
	    Led_On(LED0_bm<<uiStateLed);
		break;
		}
	}
}
///////////////////////////////////////////////////////////////////////
void Led_Init(void){ //p.12
	IO1DIR|=LED0_bm|LED1_bm|LED2_bm|LED3_bm; 
	IO1SET=LED0_bm;
}
///////////////////////////////////////////////////////////////////////
void Led_StepLeft(void){LedStep(LEFT);}
///////////////////////////////////////////////////////////////////////
void Led_StepRight(void){LedStep(RIGHT);}
