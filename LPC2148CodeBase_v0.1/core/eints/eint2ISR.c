//
//  $Id: eint2ISR.c 128 2008-10-11 14:54:27Z jcw $
//  $Revision: 128 $
//  $Author: jcw $
//  $Date: 2008-10-11 10:54:27 -0400 (Sat, 11 Oct 2008) $
//  $HeadURL: http://tinymicros.com/svn_public/arm/lpc2148_demo/trunk/eints/eint2ISR.c $
//


#include "FreeRTOS.h"
#ifdef CFG_UIP
#include "task.h"
#include "semphr.h"
#endif

#include "eint2ISR.h"

//
//  
//
#ifdef CFG_UIP
extern xSemaphoreHandle xENC28J60Semaphore;
#endif

//
//
//
static void eint2ISR_Handler (void)
{
#ifdef CFG_UIP
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
#endif

  SCB_EXTINT |= SCB_EXTINT_EINT2;

#ifdef CFG_UIP
  xSemaphoreGiveFromISR (xENC28J60Semaphore, &xHigherPriorityTaskWoken);

	VIC_VectAddr = (unsigned portLONG) 0;

  if (xHigherPriorityTaskWoken)
    portYIELD_FROM_ISR ();
#endif
}

void eint2ISR (void) __attribute__ ((naked));
void eint2ISR (void)
{
  portSAVE_CONTEXT ();
  eint2ISR_Handler ();
  portRESTORE_CONTEXT ();
}

