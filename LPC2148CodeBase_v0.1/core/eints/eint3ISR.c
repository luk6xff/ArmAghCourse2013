//
//  $Id: eint3ISR.c 158 2008-10-17 01:31:32Z jcw $
//  $Revision: 158 $
//  $Author: jcw $
//  $Date: 2008-10-16 21:31:32 -0400 (Thu, 16 Oct 2008) $
//  $HeadURL: http://tinymicros.com/svn_public/arm/lpc2148_demo/trunk/eints/eint3ISR.c $
//

#include "FreeRTOS.h"

#include "eint2ISR.h"

//
//
//
static void eint3ISR_Handler (void)
{
  SCB_EXTINT |= SCB_EXTINT_EINT3;

  // Do something

  VIC_VectAddr = (unsigned portLONG) 0;
}

void eint3ISR (void) __attribute__ ((naked));
void eint3ISR (void)
{
  portSAVE_CONTEXT ();
  eint3ISR_Handler ();
  portRESTORE_CONTEXT ();
}
