/**************************************************************************/
/*! 
    \file     uart0.h
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 September 2009
    \version  1.00
*/
/**************************************************************************/

#ifndef _UART0_H_
#define _UART0_H_

#include "FreeRTOS.h"
#include "queue.h"

signed portBASE_TYPE uart0Init (unsigned portLONG ulWantedBaud, unsigned portBASE_TYPE uxQueueLength);
void uart0Deinit (void);
signed portBASE_TYPE uart0GetChar (signed portCHAR *pcRxedChar, portTickType xBlockTime);
signed portBASE_TYPE uart0PutChar (signed portCHAR cOutChar, portTickType xBlockTime);
signed portBASE_TYPE uart0PutString (const signed portCHAR * const pcString, portTickType xBlockTime);
void uart0GetRxQueue (xQueueHandle *qh);

#endif
