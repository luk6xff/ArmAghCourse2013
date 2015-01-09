/**************************************************************************/
/*! 
    \file     leds.h
    \author   K. Townsend (microBuilder.eu)
    \date     26 November 2009
    \version  1.00
*/
/**************************************************************************/

#ifndef _LEDS_H_
#define _LEDS_H_

#include "FreeRTOS.h"
#include "task.h"

// To try to make the code a bit more portable, the registers and LED pin
// have been defined here.  If a pin in GPIO block 1 is used, for example,
// you simply need to change the IODIR, IOSET and IOCLR register
// definitions from GPIO0_FIODIR to GPIO1_FIODIR, etc.

#define LEDS_LED        22            // The IO pin that the LED is attached to
#define LEDS_REG_IOPIN  GPIO0_FIOPIN  // IOPIN register corresponding to LEDS_LED pin
#define LEDS_REG_IODIR  GPIO0_FIODIR  // IODIR register corresponding to LEDS_LED pin
#define LEDS_REG_IOSET  GPIO0_FIOSET  // IOSET register corresponding to LEDS_LED pin
#define LEDS_REG_IOCLR  GPIO0_FIOCLR  // IOCLR register corresponding to LEDS_LED pin

void ledsSetDelay(portBASE_TYPE ticks);
signed portBASE_TYPE ledsTaskStart (void);
signed portBASE_TYPE ledsTaskStop (void);
unsigned portBASE_TYPE ledsGetDelay(void);

#endif
