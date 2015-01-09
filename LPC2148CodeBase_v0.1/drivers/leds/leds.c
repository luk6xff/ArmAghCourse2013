/**************************************************************************/
/*! 
    \file     leds.c
    \author   K. Townsend (microBuilder.eu)
    \date     26 November 2009
    \version  1.00

    Causes the LED to blink on and off at a specific rate

*/
/**************************************************************************/

#include "leds.h"

//
//  If CFG_LED isn't defined don't allocate a task handle slot for it
//
#ifndef CFG_LED
#define TASKHANDLE_LED 0
#endif

// Default values
static portBASE_TYPE ledsMinimumDelay = 5;    // Minimum delay in ticks when toggling LED state (1 tick = 10mS)
static portBASE_TYPE ledsDelayInTicks = 100;  // Default delay in ticks when toggling LED state

// ===============
// Private Methods
// ===============

static int getPinState(int pinNumber)
{
  // Read the current state of all pins in GPIO block 1
  int pinBlockState = LEDS_REG_IOPIN;
  // Read the value of 'pinNumber'
  int pinState = (pinBlockState & (1 << pinNumber)) ? 1 : 0;
  // Return the value of pinState
  return pinState;
}

// ===============
// Public Methods
// ===============

/**************************************************************************/
/*! 
    Sets the delay in ticks between each LED state change (where 1 tick
    typically = 10mS).  A minimum delay has been defined to prevent the
    code from consuming too much time in the task scheduler.
*/
/**************************************************************************/
void ledsSetDelay(portBASE_TYPE ticks)
{
  ledsDelayInTicks = ticks < ledsMinimumDelay ? ledsMinimumDelay : ticks;
}

/**************************************************************************/
/*! 
    Causes the LED to blink continuously at specified rate (default: 1Hz).
*/
/**************************************************************************/
static portTASK_FUNCTION(vLEDTask, pvParameters __attribute__((unused)))
{
  // Set LED as output
  LEDS_REG_IODIR |= (1 << LEDS_LED);

  // Turn LED off (set it 'high')
  LEDS_REG_IOSET |= (1 << LEDS_LED);

  for (;;)
  {
    if (getPinState(LEDS_LED))
    {
      LEDS_REG_IOCLR |= (1 << LEDS_LED);
    }
    else
    {
      LEDS_REG_IOSET |= (1 << LEDS_LED);
    }

    // Wait specified number of ticks 
    vTaskDelay(ledsDelayInTicks);
  }
}


/**************************************************************************/
/*! 
    Starts the led task.
*/
/**************************************************************************/
signed portBASE_TYPE ledsTaskStart (void)
{
  return xTaskCreate (vLEDTask,  (const signed portCHAR * const) "LED", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1), &taskHandles [TASKHANDLE_LED]);
}

/**************************************************************************/
/*! 
    Stop the led task.
*/
/**************************************************************************/
signed portBASE_TYPE ledsTaskStop (void)
{
  if (!taskHandles [TASKHANDLE_LED])
    return 0;
  
  vTaskDelete (taskHandles [TASKHANDLE_LED]);
  taskHandles [TASKHANDLE_LED] = NULL;

  // Turn LED off
  LEDS_REG_IODIR |= (1 << LEDS_LED);
  LEDS_REG_IOSET |= (1 << LEDS_LED);

  return 1;
}

/**************************************************************************/
/*! 
    Returns the current delay between LED state changes.
*/
/**************************************************************************/
unsigned portBASE_TYPE ledsGetDelay(void)
{
  return ledsDelayInTicks;
}