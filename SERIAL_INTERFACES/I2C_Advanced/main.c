#include<LPC21xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uart.h"
#include "command_decoder.h"
#include "timer_interrupts.h"
#include "led.h"
#include "string.h"
#include "pcf8574.h"
#include "i2c.h"

unsigned char ucTokenNr;
struct Token asToken[MAX_TOKEN_NR];
/*

int main(void)
{
	I2C_Init();
	char uartReceiverBuffer[RECEIVER_SIZE];
  UART_InitWithInt(9600);
	
		while(1)
    {
   
 			if(eReciever_GetStatus()==READY)
        {
            Reciever_GetStringCopy(uartReceiverBuffer);
            DecodeMsg(uartReceiverBuffer);
            if((ucTokenNr!=0)&&(asToken[0].eType==KEYWORD)) 
            {
							switch(asToken[0].uValue.eKeyword)
                {
								case PORTSET:
                     if((asToken[1].eType==NUMBER)&&(asToken[1].uValue.uiNumber<=0x00FF))
												PCF8574_Write(asToken[1].uValue.uiNumber);
                    break;
                default:
                    break;
                }
            }
        }
    }
		return 0;
}
*/
////////////////////////////////////////////////////


static void setPortPcf8574(void){
  static unsigned char counter;
	counter++;
	if(counter==0xFF){
		counter=0;
	}
  PCF8574_Write(counter);
	Led_StepRight();

}

static void readPortPcf8574(void){
  static unsigned char counter;
	counter++;
	if(counter==0xFF){
		counter=0;
	}
  PCF8574_Read();
	PCF8574_Write(getPCF8574_Input());
	Led_StepRight();

}

int main(void)
{
	I2C_Init();
	Led_Init(); //for debug
	char uartReceiverBuffer[RECEIVER_SIZE];
  //UART_InitWithInt(9600);
	 Timer0Interrupts_Init(1000000, setPortPcf8574/*readPortPcf8574*/);
	 while(1){};

		return 0;
}
