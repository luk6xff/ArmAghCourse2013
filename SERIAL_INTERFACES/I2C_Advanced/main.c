#include<LPC21xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uart.h"
#include "command_decoder.h"
#include "string.h"
#include "pcf8574.h"

unsigned char ucTokenNr;
struct Token asToken[MAX_TOKEN_NR];


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

////////////////////////////////////////////////////
