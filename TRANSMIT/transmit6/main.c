#include "servo.h"
#include "uart.h"
#include "command_decoder.h"
#include "string.h"
#include "watch.h"
////////////////////////////////////////////////////
unsigned char ucTokenNr;
struct Token asToken[MAX_TOKEN_NR];

static void displayWatchValue(void);
static void displayCalculatedValue(unsigned int uiValue);

int main ()
{
    char uartReceiverBuffer[RECEIVER_SIZE];
		WatchInit();
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
                case CALC:
                     if((asToken[1].eType==NUMBER)&&(asToken[1].uValue.uiNumber<0xFFFF))
											 displayCalculatedValue(asToken[1].uValue.uiNumber); 
                    break;
                default:
                    break;
                }
            }
        }
			 
				if(Transmiter_GetStatus()==FREE){
						displayWatchValue();
				}
    }
}



static void displayWatchValue(void) {
	char cDataStr[2][15]= {"\nsec " ,"\nmin "};

	if(WatchAreSecondsChanged()){
		AppendUIntToString(WatchGetSeconds(),cDataStr[0]);
		Transmiter_SendString((char*)cDataStr[0]);
		
  }
	if(WatchAreMinutesChanged()){
		AppendUIntToString(WatchGetMinutes(),cDataStr[1]);
		Transmiter_SendString((char*)cDataStr[1]);
  }

}


static void displayCalculatedValue(unsigned int uiValue){
	char cDataStr[15]= {"\ncalc "};
	uiValue= uiValue*2;
	AppendUIntToString(uiValue,cDataStr);
	Transmiter_SendString((char*)cDataStr);
}













