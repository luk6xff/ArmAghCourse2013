/**************************************************************************/
/*! 
    \file     uart0ISR.h
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 September 2009
    \version  1.00
*/
/**************************************************************************/

#ifndef _UART0ISR_H_
#define _UART0ISR_H_

#include "FreeRTOS.h"
#include "queue.h"

void uart0ISRCreateQueues (unsigned portBASE_TYPE uxQueueLength, xQueueHandle *pxRX0Queue, xQueueHandle *pxTX0Queue, portLONG volatile **pplTHREEmptyFlag);
void uart0ISR (void);

#endif
