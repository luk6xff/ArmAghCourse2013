#include "led.h"
#include "keyboard.h"
#include "timer_interrupts.h"
#include "servo.h"
void Automat(void);
LedState eLedState = _CALLIB;

////////////////////////////////////////////////////
int main (){
	unsigned int iMainLoopCtr;
	KeyboardInit(); 
	Led_Init(); 
  DetectorInit(); 
  Timer1Interrupts_Init(250000,&Automat); 
	
	while(1){
	 	iMainLoopCtr++;
	}
}
////////////////////////////////////////////////////

void Automat(void)
{
		static unsigned char ucWaitCounter;
		switch (eLedState){
				case _CALLIB:
        {
            if (eReadDetector()==ACTIVE)
            {
                eLedState = _STOP;
            }
            else
                Led_StepLeft();
            break;
        }
			  case _STOP:
        {
            if (eKeyboard_Read()==BUTTON_1)
            {
                eLedState = _LEFT;
            }
            else if (eKeyboard_Read()==BUTTON_3)
            {
                eLedState = _RIGHT;
            }
            else;
            break;
        }
        case _RIGHT:
        {
            if (eKeyboard_Read()==BUTTON_2)
            {
                eLedState = _IDLE;
            }
            else Led_StepRight();
            break;
        }
        case _LEFT:
        {
            if (eKeyboard_Read()==BUTTON_2)
            {
                eLedState = _IDLE;
            }
            else
                Led_StepLeft();
            break;
        }
				case _IDLE:
        {
            if (ucWaitCounter<10)
            {
                ucWaitCounter++;
            }
            else 
            {
                ucWaitCounter=0;
								eLedState = _STOP;							
            }
            break;
        }
    }
}


