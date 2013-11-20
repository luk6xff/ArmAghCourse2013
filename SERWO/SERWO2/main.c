#include "led.h"
#include "keyboard.h"
#include "timer_interrupts.h"
#include "servo.h"
void Automat(void);
LedState eLedState = _CALLIB;
struct Servo sServo; 
////////////////////////////////////////////////////
int main (){
	unsigned int iMainLoopCtr;
	KeyboardInit(); 
  Led_Init(); 
  DetectorInit(); 
	sServo.eState=CALLIB; 
	Timer1Interrupts_Init(60000,&Automat); 
	
	while(1){
	 	 iMainLoopCtr++;
			switch(eKeyboard_Read()){
			case BUTTON_1:sServo.eState=CALLIB; break;
			case BUTTON_2:sServo.uiDesiredPosition=50; break;
			case BUTTON_3:sServo.uiDesiredPosition=100; break;
			case BUTTON_4:sServo.uiDesiredPosition=150; break;
			default: break;
		}
	 }
}


void Automat(void)
{
    switch (sServo.eState)
    {
    case CALLIB:
    {
        if (eReadDetector()==ACTIVE)
        {
            sServo.uiCurrentPosition=0;
            sServo.uiDesiredPosition=0;
            sServo.eState = IDDLE;
        }
        else
            Led_StepLeft();
        break;
    }
    case IDDLE:
    {
        if(sServo.uiCurrentPosition==sServo.uiDesiredPosition)
        {
            sServo.eState = IDDLE;
        }
        else sServo.eState = IN_PROGRESS;

        break;
    }
    case IN_PROGRESS:
    {
        if(sServo.uiCurrentPosition==sServo.uiDesiredPosition)
        {
            sServo.eState = IDDLE;
        }
        else if(sServo.uiCurrentPosition<sServo.uiDesiredPosition)
        {
            Led_StepRight();
            sServo.uiCurrentPosition++;
        }
        else
        {
            Led_StepLeft();
            sServo.uiCurrentPosition--;
        }
        break;
    }
    }
}

