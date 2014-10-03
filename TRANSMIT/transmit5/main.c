#include "servo.h"
#include "uart.h"
#include "command_decoder.h"
#include "string.h"
#include "watch.h"
////////////////////////////////////////////////////
unsigned char ucTokenNr;
struct Token asToken[MAX_TOKEN_NR];

static void displayWatchValue(void);

int main ()
{
   /* char uartReceiverBuffer[RECEIVER_SIZE];
    Servo_Init(10000);
    Servo_Callib();*/
		WatchInit();
    UART_InitWithInt(9600);
    
	
		while(1)
    {
     /*
 			if(eReciever_GetStatus()==READY)
        {
            Reciever_GetStringCopy(uartReceiverBuffer);
            DecodeMsg(uartReceiverBuffer);
            if((ucTokenNr!=0)&&(asToken[0].eType==KEYWORD)) 
            {
                switch(asToken[0].uValue.eKeyword)
                {
                case CALLIB:
                    Servo_Callib();
                    break;
                case GOTO:
                    if((asToken[1].eType==NUMBER)&&(asToken[1].uValue.uiNumber<200))Servo_GoTo(asToken[1].uValue.uiNumber); // tu czy po goto byla liczba oraz czy wartosc mniejsza od 200
                    break;
                default:
                    break;
                }
            }
        }
			*/
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
















