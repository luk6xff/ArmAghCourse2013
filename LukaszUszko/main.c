#include<LPC21xx.h>
#include "led.h"
#include "keyboard.h"
//#include <time.h>       /* time */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>






void InitTimer0(void);
void WaitOnTimer0(unsigned int uiTime);

void vDelay(int iCounter);

void vDelay(int iCounter){
	 int iCounterLoop;
	for( iCounterLoop=0; iCounterLoop<(iCounter*834*6);iCounterLoop++ );
}

/* AUTOMATY
typedef enum{STATE0, STATE1, STATE2,STATE3,STATE4, STATE5} LedState;
LedState eLedState ;

typedef enum{LEFTN, RIGHTN} nLedState;
nLedState eLedStateN;

typedef enum{STOP, MOVE} mLedState;
mLedState eLedStateM;

typedef enum{_STOP, _LEFT,_RIGHT} bLedState;
bLedState eLedStateB;


unsigned char ucMoveCounter;

*/


int main(void){

   Led_Init();
   //InitTimer0();
	 KeyboardInit();
	while(1){
		

		
	
		///Led_BlinkSnake();
		//vDelay(100);
/* TU WROCICCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC		
		
		while(1){
		WaitOnTimer0(100);
		vToggleBitPort1(LED1_bm);
		}
		
		*/
		/*
		if(ReadButton1()){Led_On(LED0_bm); Led_Off(LED1_bm);}
		else {Led_Off(LED0_bm); Led_On(LED1_bm);}
		*/
//zad 1.16		
		
	/*	
		switch(ReadButtonEn1()){
			case RELEASED:{Led_On(LED0_bm); Led_Off(LED1_bm);break;} 
			case PRESSED:{Led_Off(LED0_bm); Led_On(LED1_bm);break;} 

}
	*/
		
		
		
 //KeyboardInit();
 /*
switch(eKeyboard_Read()){

	case RELEASED:{Led_On(4);break;} 
	case BUTTON_1:{Led_On(0);break;}
	case BUTTON_2:{Led_On(1);break;}
	case BUTTON_3:{Led_On(2);break;}
  case BUTTON_4:{Led_On(3);break;}


}
	*/
	/*
	int iCounter;
	for(iCounter=0;iCounter<9;iCounter++){
	  Led_StepLeft();}
	 for(iCounter=0;iCounter<9;iCounter++){
	Led_StepRight();}
	 
	 
	 switch(eKeyboard_Read()){
case BUTTON_1:{Led_StepRight();break;}
	case BUTTON_2:{ Led_StepLeft();break;}
  default:break;}
	
	*/
	
///////////////////AUTOMATY//////////////////////
//2

/*
eLedState = STATE0;
while (1) {
    switch (eLedState) 
		{
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

*/	
	

//3	
/*
ucMoveCounter=0;
eLedStateN=LEFTN;
while(1){
 switch(eLedStateN){
	case LEFTN:{
	
	 if(++ucMoveCounter<=3)
		 {
		   eLedStateN=LEFTN;Led_StepLeft();
		 }
     else
		 {
		   eLedStateN = RIGHTN;
     }
  break;
  }
  case RIGHTN:{
	 if(--ucMoveCounter>0)
	   {
	   eLedStateN=RIGHTN;Led_StepRight();
		 }
  else
	  {
     eLedStateN = LEFTN;
    }
  break;
  }
 }
vDelay(500);
}
*/



//4
/*
ucMoveCounter = 0;
eLedStateM = MOVE;
while (1) {
    switch (eLedStateM) {
    case MOVE:{
     if (++ucMoveCounter <= 3) {
		      eLedStateM = MOVE;
		      Led_StepLeft();
	    } else {
          eLedStateM = STOP;
	    }
    break;
	  }
    case STOP:{
			 eLedStateM = STOP;
       break;
	    }
    }
    vDelay(500);
}
*/



//5
/*
ucMoveCounter = 0;
eLedStateM = STOP;
while (1) {
    switch (eLedStateM) {
    case MOVE:{

	    if (++ucMoveCounter <= 3) {
		    eLedStateM = MOVE;
		    Led_StepLeft();
	    } 
			else {
        eLedStateM = STOP;
	    }
			break;
	   }
    case STOP:{
	    if (ReadButton1()) {
		    eLedStateM = MOVE;
	    } 
			else
		    eLedStateM = STOP;
      break;
	   }
    }
    vDelay(500);
}



*/




//6
/*
ucMoveCounter = 0;
eLedStateN = RIGHTN;
while (1) {
   switch (eLedStateN) {
     case LEFTN:{
	    if (ReadButton2()) {
		      eLedStateN = RIGHTN;
	      } else Led_StepLeft();
		        
	    break;
	   }
    case RIGHTN:{
	    if (ReadButton1()) {
		      eLedStateN = LEFTN;
	      } else Led_StepRight();
		    
	    break;
	   }
    }
    vDelay(500);
}
*/


//6
/*
ucMoveCounter=0;
eLedStateN=RIGHTN;
while(1){
switch(eLedStateN){
case LEFTN:{
	if(ReadButton2()){eLedStateN=RIGHTN;}
else Led_StepLeft();
break;
}
case RIGHTN:{
	if(ReadButton1()){eLedStateN=LEFTN;}
else Led_StepRight();
break;
}
}
vDelay(500);
}
*/



//7
/*
eLedStateB = _STOP;
while (1) {
    switch (eLedStateB) {
    case _STOP:{
	    if (ReadButton1()) {
		      eLedStateB = _LEFT;
	    } else if (ReadButton3()) {
		      eLedStateB = _RIGHT;
	    } else;
	    break;
	 }
    case _RIGHT:{
	    if (ReadButton2()) {
		      eLedStateB = _STOP;
	    }
			else Led_StepRight();
      break;
	 }
    case _LEFT:{
	    if (ReadButton2()) {
		      eLedStateB = _STOP;
	    } else
		  Led_StepLeft();
	    break;
	 }
  }
  vDelay(500);
}


*/


////////////////////////////////////////////////////////////////KONIEC AUTOAMTOW//////////////////////////////////////////////////////////////////////////
/*
IO1CLR=LED0_bm;
WaitOnTimer0(10000);
IO1SET=LED0_bm;
WaitOnTimer0(10000);
*/
}
return 0;
}


////////////////////////////////////////////////////////////////TIMER/////////////////////////////////////////////////////////////////////////////////////

void InitTimer0(void){
T0TCR=1<<1; //enable Timer
T0PR=15; //prescaler 1us
T0TCR=1<<0; //enable Timer
	}

void WaitOnTimer0(unsigned int uiTime){

		T0MR0=uiTime;
	T0MCR|=1<<1|1<<0;
	while((T0IR&(1<<0))==0);

//T0TCR=2; //reset Timer
//T0TCR=1;
}

