#include "led.h"
#include "keyboard.h"
#include "timer_interrupts.h"
#include "servo.h"
//void Automat(void);
//LedState eLedState = _CALLIB;//cw1
//struct Servo sServo; //cw2
////////////////////////////////////////////////////
int main (){
	unsigned int iMainLoopCtr;
	KeyboardInit(); //cw2
	//Led_Init(); //cw1 i 2
  //DetectorInit(); cw1 i 2
	//sServo.eState=CALLIB; //cw2
	//Timer1Interrupts_Init(250000,&Automat); // cw1 i 2
	
	/////cw3
	Servo_Init(80000);
	Servo_Callib();
	
	while(1){
	 	iMainLoopCtr++;
		/*cw 2
		  switch(eKeyboard_Read()){
			case BUTTON_1:sServo.eState=CALLIB; break;
			case BUTTON_2:sServo.uiDesiredPosition=5; break;
			case BUTTON_3:sServo.uiDesiredPosition=10; break;
			case BUTTON_4:sServo.uiDesiredPosition=15; break;
			default: break;
		}*/
		
		
		//cw3
		switch(eKeyboard_Read()){
			case BUTTON_1:Servo_Callib(); break;
			case BUTTON_2:Servo_GoTo(5); break;
			case BUTTON_3:Servo_GoTo(10); break;
			case BUTTON_4:Servo_GoTo(15); break;
			default: break;
		}
	}
	
}
////////////////////////////////////////////////////
/* cw1
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
*/

/*
///cw2
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
*/
