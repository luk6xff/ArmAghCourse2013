#include "keyboard.h"
#include "servo.h"
#include "uart.h"

////////////////////////////////////////////////////
char cOdebranyZnak;
int main (){
	KeyboardInit(); 
	Servo_Init(10000);
	Servo_Callib();
	UART_InitWithInt(9600);
	unsigned char ucDegreeCounter=0;
	while(1){
	/* ver1.0
		switch(eKeyboard_Read()){
			case BUTTON_1:Servo_Callib(); break;
			case BUTTON_2:Servo_GoTo(50); break;
			case BUTTON_3:Servo_GoTo(100); break;
			case BUTTON_4:Servo_GoTo(150); break;
			default: break;
		}
		 	switch(cOdebranyZnak){
			case '1':Servo_GoTo(50); break;
			case '2':Servo_GoTo(100); break;
			case '3':Servo_GoTo(150); break;
			case 'c':Servo_Callib(); break;
			default: break;
		}
		*/
		//ver2.0
		/*
		if(eKeyboard_Read()==BUTTON_1||cOdebranyZnak=='c')
		{
				Servo_Callib();
		}
		else if(eKeyboard_Read()==BUTTON_2||cOdebranyZnak=='1')
		{
				Servo_GoTo(50);
		}
		else if(eKeyboard_Read()==BUTTON_3||cOdebranyZnak=='2')
		{
				Servo_GoTo(100);
		}
		else if(eKeyboard_Read()==BUTTON_4||cOdebranyZnak=='3')
		{
				Servo_GoTo(150);
		}
		else;
		*/
		switch(cOdebranyZnak){
			case '1':
				if(ucDegreeCounter==200)ucDegreeCounter=0; // przemyslec co po przekroczeniu 200
				ucDegreeCounter+=50;
				cOdebranyZnak='0';// przeanalizowac na sprzecie cOdebranyZnak='0';
				Servo_GoTo(ucDegreeCounter); break;
			case 'c': Servo_Callib();cOdebranyZnak='0'; break;
			default: break;
    }
		}
	}

