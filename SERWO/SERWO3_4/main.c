#include "keyboard.h"
#include "servo.h"

////////////////////////////////////////////////////

int main (){
	unsigned int iMainLoopCtr;
	KeyboardInit(); 
	
	Servo_Init(30000);
	Servo_Callib();
	
	while(1){
	 	iMainLoopCtr++;
		
		switch(eKeyboard_Read()){
			case BUTTON_1:Servo_Callib(); break;
			case BUTTON_2:Servo_GoTo(50); break;
			case BUTTON_3:Servo_GoTo(100); break;
			case BUTTON_4:Servo_GoTo(150); break;
			default: break;
		}
	}
}
