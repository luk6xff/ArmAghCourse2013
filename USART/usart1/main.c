#include "keyboard.h"
#include "servo.h"
#include "uart.h"

////////////////////////////////////////////////////

int main (){
	KeyboardInit(); 
	Servo_Init(30000);
	Servo_Callib();
	UART_InitWithInt(9600);
	
	while(1){

		switch(eKeyboard_Read()){
			case BUTTON_1:Servo_Callib(); break;
			case BUTTON_2:Servo_GoTo(50); break;
			case BUTTON_3:Servo_GoTo(100); break;
			case BUTTON_4:Servo_GoTo(150); break;
			default: break;
		}
	}
}
