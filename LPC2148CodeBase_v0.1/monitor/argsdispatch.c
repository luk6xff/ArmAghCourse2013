/**************************************************************************/
/*! 
    \file     argsdispatch.c
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 November 2009
    \version  1.00
*/
/**************************************************************************/

#include <stdio.h>
#include <string.h>

#include "monitor.h"
#include "argsdispatch.h"

extern const commandList_t *activeCommandList;

/**************************************************************************/
/*! 

*/
/**************************************************************************/
int argsDispatch (const commandList_t *cl, int argc, char **argv)
{
  activeCommandList = cl;

  while (cl->command)
  {
    if (!strcmp (cl->command, argv [0]))
    {
      int ret = 0;

      if ((argc == 2) && !strcmp (argv [1], "?"))
        printf ("%s\r\n", cl->parameters);
      else if ((argc - 1) < cl->minArgs)
        printf ("Too few arguments to command (%d expected)\r\n", cl->minArgs);
      else if (cl->cmdType == CMDTYPE_CMDLIST)
        argsDispatch (cl->commandList, argc - 1, &argv [1]);
      else if ((argc - 1) > cl->maxArgs)
        printf ("Too many arguments to command (%d maximum)\r\n", cl->maxArgs);
      else
        ret = (*cl->handler) (argc - 1, &argv [1]);

      return ret;
    }

    cl++;
  }

  if (!cl->command)
    printf ("Unknown command \"%s\"\r\n", argv [0]);

  return 0;
}
