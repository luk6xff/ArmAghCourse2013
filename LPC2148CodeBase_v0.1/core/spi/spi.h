/**************************************************************************/
/*! 
    \file     spi.h
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 September 2009
    \version  1.00
*/
/**************************************************************************/

#ifndef _SPI_H_
#define _SPI_H_

#include "FreeRTOS.h"

void spiInit (void);
U8 spiPut (U8 valueIn);

#endif
