/**************************************************************************/
/*! 
    \file     rtcISR.c
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     9 November 2009
    \version  1.00
*/
/**************************************************************************/

#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#if defined CFG_MONITOR_USBSER
#include "../usbser/usbser.h"
#elif defined CFG_MONITOR_UART0
#include "../uart/uart0.h"
#else
#error "Eeek!  No console devices defined!"
#endif

#include "rtcISR.h"
#include "rtc.h"

static xQueueHandle consoleQueue = NULL;

void rtcISRInit (void)
{
#if defined CFG_MONITOR_USBSER
  usbserGetRxQueue (&consoleQueue);
#elif defined CFG_MONITOR_UART0
  uart0GetRxQueue (&consoleQueue);
#else
#error "Eeek!  No console devices defined!"
#endif
}

static void rtcISR_Handler (void)
{
  portBASE_TYPE higherPriorityTaskWoken = pdFALSE;

  RTC_CCR = (RTC_CCR_CLKEN | RTC_CCR_CLKSRC);
  SCB_PCONP |= SCB_PCONP_PCRTC;

  if (RTC_ILR & RTC_ILR_RTCCIF)
  {
    U8 c = 0xff;

    if (consoleQueue)
      xQueueSendFromISR (consoleQueue, &c, &higherPriorityTaskWoken);

    RTC_ILR = RTC_ILR_RTCCIF;
  }

  if (RTC_ILR & RTC_ILR_RTCALF)
  {
    U8 c = 0xfe;

    if (consoleQueue)
      xQueueSendFromISR (consoleQueue, &c, &higherPriorityTaskWoken);

    RTC_ILR = RTC_ILR_RTCALF;
  }

  VIC_VectAddr = (unsigned portLONG) 0;

  RTC_CCR = (RTC_CCR_CLKEN | RTC_CCR_CLKSRC);
  SCB_PCONP &= ~SCB_PCONP_PCRTC;

  if (higherPriorityTaskWoken)
    portYIELD_FROM_ISR ();
}

void rtcISR (void) __attribute__ ((naked));
void rtcISR (void)
{
  portSAVE_CONTEXT ();
  rtcISR_Handler ();
  portRESTORE_CONTEXT ();
}
