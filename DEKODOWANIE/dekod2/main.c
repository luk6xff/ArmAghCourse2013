#include "servo.h"
#include "uart.h"

////////////////////////////////////////////////////


int main ()
{
    char uartReceiverBuffer[RECEIVER_SIZE];
		Servo_Init(10000);
    Servo_Callib();
    UART_InitWithInt(9600);

    while(1)
    {
				if(eReciever_GetStatus()==READY)
        {
    					Reciever_GetStringCopy(uartReceiverBuffer);
            if (eCompareString("callib", uartReceiverBuffer)==EQUAL)
            {
                Servo_Callib();
            }
            else if (eCompareString("left", uartReceiverBuffer)==EQUAL)
            {
                Servo_GoTo(50);
            }
            else if (eCompareString("right", uartReceiverBuffer)==EQUAL)
            {
                Servo_GoTo(150);
            }
				}
		}
}


