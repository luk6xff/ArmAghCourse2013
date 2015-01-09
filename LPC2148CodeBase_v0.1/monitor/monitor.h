/**************************************************************************/
/*! 
    \file     monitor.h
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 November 2009
    \version  1.00
*/
/**************************************************************************/

#ifndef _MONITOR_H_
#define _MONITOR_H_

#include "FreeRTOS.h"
#include "task.h"

typedef enum
{
  CMDTYPE_CMDLIST = 0,
  CMDTYPE_FUNCTION
}
cmdType_e;

typedef struct commandList_s
{
  const portCHAR *command;
  portCHAR minArgs;
  portCHAR maxArgs;
  cmdType_e cmdType;
  union
  {
    const void *trickGCC;
    int (*handler) (int argc, portCHAR **argv);
    struct commandList_s *commandList;
  };
  const portCHAR *description;
  const portCHAR *parameters;
}
commandList_t;

signed portBASE_TYPE monitorTaskStart (void);

#endif
