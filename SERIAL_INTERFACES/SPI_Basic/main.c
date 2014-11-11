#include<LPC21xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "spi.h"
#include "timer_interrupts.h"
#include "led.h"
#include "keyboard.h"

static void setDacValues(void){
	 static  unsigned char uiAdcSendDataCounter=0;
	 unsigned int uiAdcValue=0;
	 uiAdcSendDataCounter++;
		
	 if(uiAdcSendDataCounter==1){
			uiAdcValue=0;
		}
	 else if(uiAdcSendDataCounter==2){
			uiAdcValue=4096/2;
		}
	 else {
			uiAdcValue=4095;
			uiAdcSendDataCounter=0;
		}
	 DAC_MCP4921_Set(uiAdcValue);
}

int main(void)
{
	
	Timer0Interrupts_Init(250000, setDacValues);
	while(1){};

  return 0;
}

////////////////////////////////////////////////////
