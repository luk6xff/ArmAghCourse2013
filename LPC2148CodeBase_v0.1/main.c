/**************************************************************************/
/*! 
    \file     main.c
    \author   K. Townsend
    \date     23 September 2009
    \version  1.00
*/
/**************************************************************************/

#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "monitor/monitor.h"
#include "core/cpu/cpu.h"

#if defined CFG_LED && CFG_LED_RUNLEDONSTARTUP == 1
#include "drivers/leds/leds.h"
#endif

#if defined CFG_RTC
#include "core/rtc/rtc.h"
#endif

#ifdef CFG_MONITOR_UART0
  #include "core/uart/uart0.h"
#endif

#ifdef CFG_MONITOR_USBSER
  #include "core/usbser/usbser.h"
#endif

/**************************************************************************/
/*! 
    Starts the main monitor task and performs any initialisation activity
*/
/**************************************************************************/
static portTASK_FUNCTION(vStartupTask, pvParameters __attribute__((unused))) 
{
  for (;;)
  {    
    #if defined CFG_RTC
    // Initialise RTC
    rtcInit();
    #endif

    #if defined CFG_LED && CFG_LED_RUNLEDONSTARTUP == 1
    // Start LED flasher (for testing purposes)
    ledsTaskStart();
    #endif

    // Start monitor task
    monitorTaskStart();

    // Remove startup task.  The monitor will only start after the current
    // task is deleted since 'Startup' has a higher priority.
    vTaskDelete(NULL);
  }
}

/**************************************************************************/
/*! 
    The application entry point.  Initialise the CPU, open the monitor
    'port', create the startup task and begin the task scheduler.
*/
/**************************************************************************/
int main(void)
{
  // Setup the system clocks and put all peripherals in an appropriate default state
  cpuSetupHardware();

  #ifdef CFG_MONITOR_UART0
  // Configure UART0 for Monitor input/output
  uart0Init(9600, 80);
  #endif

  #ifdef CFG_MONITOR_USBSER
  // Configure USB Serial Port for Monitor input/output (9600/8N1)
  usbserInit();
  #endif

  // Create the startup task
  xTaskCreate (vStartupTask, (const signed portCHAR * const)"Startup", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES, &taskHandles[TASKHANDLE_STARTUP]);

  // Start the task scheduler
  vTaskStartScheduler();

  // You should only ever reach this point if there isn't enough memory to start the scheduler
  return 0;
}

/**************************************************************************/
/*! 
    Configures Timer1 for FreeRTOS run time stat monitoring.  To enable
    run time stats set configGENERATE_RUN_TIME_STATS to '1' in
    FreeRTOSConfig.h.

    \note For more information on run time stats monitoring see: 
    http://www.freertos.org/rtos-run-time-stats.html
*/
/**************************************************************************/
#if configGENERATE_RUN_TIME_STATS == 1
void vConfigureTimerForRunTimeStats( void )
{
    const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

    // Power up timer1
    SCB_PCONP |= SCB_PCONP_PCTIM1;
	
    // Reset the timer
    T1_TCR = TCR_COUNT_RESET;
	
    // Just count up
    T1_CTCR = CTCR_CTM_TIMER;

    // Prescale to a frequency that is good enough to get a decent resolution,
    // but not so fast that it overflows all the time.
    T1_PR =  ( configCPU_CLOCK_HZ / 10000UL ) - 1UL;

    // Start the counter
    T1_TCR = TCR_COUNT_ENABLE;
}
#endif


/**************************************************************************/
/*! 
    Hook when a stack overflow occurs if configCHECK_FOR_STACK_OVERFLOW
    is set to 1 or 2 in FreeRTOSConfig.h

    \note For more information on stack overflow in FreeRTOS see: 
    http://www.freertos.org/Stacks-and-stack-overflow-checking.html
*/
/**************************************************************************/
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	/* Check pcTaskName for the name of the offending task, or pxCurrentTCB
	if pcTaskName has itself been corrupted. */
	( void ) pxTask;
	( void ) pcTaskName;
	for( ;; );
}

/**************************************************************************/
/*! 
    Redirects printf output to either UART0 or USBSER (as defined in
    ProjectConfig.h)
*/
/**************************************************************************/
void __putchar(char c) 
{
  // Use UART0 for printf output
  #ifdef CFG_MONITOR_UART0
  uart0PutChar(c, 5);
  #endif

  // Use USB Serial for printf output
  #ifdef CFG_MONITOR_USBSER
  usbserPutChar(c, 5);
  #endif
}
