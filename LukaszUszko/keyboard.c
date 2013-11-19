#include<LPC21xx.h>
#include "stdbool.h"
#include "keyboard.h"


#define BUTTON1_bm 1<<4
#define BUTTON2_bm 1<<5
#define BUTTON3_bm 1<<6
#define BUTTON4_bm 1<<7

void vToggleBitPort1(int ucPin);


//////////////////////////////////////////////////////
void vToggleBitPort1(int ucPin){
	if(ucPin<0||ucPin>0xFFFF){
			return;
 }
	else{
				if(IO1PIN&(ucPin)){
						IO1CLR=ucPin;
					}
				else IO1SET=ucPin;
		}
}




bool ReadButton1(void){

	if(IO0PIN&BUTTON1_bm){
		 return false;  
			}
	else return true;

}

///////////////////////////////////////////////////////////////////////


  void ButtonInit(void){
	IODIR0&=~(BUTTON1_bm);
}

///////////////////////////////////////////////////////////////////////


ButtonState ReadButtonEn1(void){

    if(IO0PIN&BUTTON1_bm){ return RELEASED;  }
    else{ return PRESSED;}

}





KeyboardState eKeyboard_Read(void){

  if(!(IO0PIN&(BUTTON1_bm))) return BUTTON_1;
  else if (!(IO0PIN&(BUTTON2_bm))) return BUTTON_2;
	else if (!(IO0PIN&(BUTTON3_bm))) return BUTTON_3;
	else if (!(IO0PIN&(BUTTON4_bm))) return BUTTON_4;
  else return RELEASED_;
}



void KeyboardInit(void){
	IODIR0&=~(BUTTON1_bm|BUTTON2_bm|BUTTON3_bm|BUTTON4_bm);
}

