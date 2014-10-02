#include<LPC21xx.h>
#include "servo.h"
#include "led.h"
#include "timer_interrupts.h"

#define DETECTOR_OUT 1<<10

typedef enum
{
    ACTIVE,
    INACTIVE
} ServoStateDetector;

enum ServoState {CALLIB, IDDLE, IN_PROGRESS};
struct Servo
{
    enum ServoState eState;
    unsigned int uiCurrentPosition;
    unsigned int uiDesiredPosition;
};

struct Servo sServo;

/////////////////////////////////////////////////
static void DetectorInit(void)
{
    IODIR0&=~(DETECTOR_OUT);
}

static ServoStateDetector eReadDetector(void)
{
    if(!(IO0PIN&(DETECTOR_OUT))) return INACTIVE;
    else return ACTIVE;
}


static void Automat(void)
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

void Servo_Init(unsigned int uiServoFrequency)
{
    Led_Init();
    DetectorInit();
    Timer1Interrupts_Init(uiServoFrequency,&Automat);
}

void Servo_Callib(void)
{
    sServo.eState=CALLIB;
}

void Servo_GoTo(unsigned int uiPosition)
{
    sServo.uiDesiredPosition=uiPosition;
}
