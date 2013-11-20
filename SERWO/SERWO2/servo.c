#include<LPC21xx.h>
#include "servo.h"
#include "led.h"
#include "timer_interrupts.h"

#define DETECTOR_OUT 1<<10

void DetectorInit(void)
{
    IODIR0&=~(DETECTOR_OUT);
}

ServoStateDetector eReadDetector(void){
	if(!(IO0PIN&(DETECTOR_OUT))) return INACTIVE;
	else return ACTIVE;
}

	
	
	

