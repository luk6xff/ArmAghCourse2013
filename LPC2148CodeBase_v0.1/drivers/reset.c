/**************************************************************************/
/*! 
    \file     reset.c
    \author   K. Townsend (microBuilder.eu)
    \date     24 November 2009
    \version  1.00
*/
/**************************************************************************/

#include "reset.h"

/**************************************************************************/
/*! 
    Causes the device to reset by starting the watchdog and then not
    feeding it.  This is the only way to reset the device in code.
*/
/**************************************************************************/
void reset (void)
{
  WD_TC = 0xff;
  WD_MOD = 0x0f;
  WD_FEED = WD_FEED_FEED1;
  WD_FEED = WD_FEED_FEED2;
}
