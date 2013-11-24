
#include "led.h"
#include "uart.h"

////////////////////////////////////////////////////
char cOdebranyZnak;
int main (){
	
	Led_Init();
  UART_InitWithInt(9600);
	while(1){
	 	switch(cOdebranyZnak){
			case '1':Led_On(0); break;
			case '2':Led_On(1); break;
			case '3':Led_On(2); break;
			case '4':Led_On(3); break;
			case 'c':Led_On(4); break;
			default: break;
		}
	}
}
