/**************************************************************************/
/*! 
    \file     uart1.h
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 September 2009
    \version  1.00
*/
/**************************************************************************/

#ifndef _UART1_H_
#define _UART1_H_

#include "FreeRTOS.h"
#include "queue.h"

signed portBASE_TYPE uart1Init (unsigned portLONG ulWantedBaud, unsigned portBASE_TYPE uxQueueLength);
void uart1Deinit (void);
signed portBASE_TYPE uart1GetChar (signed portCHAR *pcRxedChar, portTickType xBlockTime);
signed portBASE_TYPE uart1PutChar (signed portCHAR cOutChar, portTickType xBlockTime);
signed portBASE_TYPE uart1PutString (const signed portCHAR * const pcString, portTickType xBlockTime);
void uart1GetRxQueue (xQueueHandle *qh);

#endif
