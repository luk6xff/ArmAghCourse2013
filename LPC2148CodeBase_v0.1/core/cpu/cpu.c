/**************************************************************************/
/*! 
    \file     cpu.c
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 September 2009
    \version  1.00

    Initialises the CPU, setting the core and peripheral clocks (CCLK and
    PCLK), setting all pins to GPIO by default, and disabling power to all
    peripherals (so that power isn't wasted on unused peripherals).
    By default, the cpu is set to operate at 48MHz (4 x 12MHz XTAL).

    The following code will initialise the CPU:
    \code 
    #include "cpu/cpu.h"

    int main(void)
    {
      cpuSetupHardware();
    }
    \endcode
*/
/**************************************************************************/

#include "FreeRTOS.h"
#include "cpu.h"

/**************************************************************************/
/*! 
    Initialises the CPU, setting the core and peripheral clocks (CCLK and
    PCLK) to 48MHz, and setting all pins to fast GPIO and input by default.
    This method disables power to all onboard peripherals, which is
    generally a good practice to avoid wasting power with unused
    peripherals.  Before using any peripherals (such as I2C, UART, USB,
    etc.) be sure to power them up using the appropriate PCONP register.
*/
/**************************************************************************/
void cpuSetupHardware (void)
{
  //  Use fast I/O on both ports
  SCB_SCS = SCB_SCS_GPIO0M | SCB_SCS_GPIO1M;

  //  Configure pin functions.  All pins are set to GPIO, except the Debug
  //  port (P1.26) and the Trace port (P1.25..P1.16) which are required
  //  for JTAG.  If you are not using JTAG and wish to use the ports as
  //  GPIO uncomment the last line.
  PCB_PINSEL0 = PCB_PINSEL0_ALL_GPIO;
  PCB_PINSEL1 = PCB_PINSEL1_ALL_GPIO;
  // PCB_PINSEL2 = (PCB_PINSEL2_P13626_GPIO | PCB_PINSEL2_P12516_GPIO);

  //  Set all GPIO to input (safer than an output, which may be driving a high
  //  into a closed switch such as BSL).  
  GPIO0_FIODIR = ~GPIO_IO_ALL;
  GPIO1_FIODIR = ~GPIO_IO_JTAG;

  //  Setup the PLL to multiply the 12Mhz XTAL input by 4, divide by 1
  SCB_PLLCFG = (SCB_PLLCFG_MUL4 | SCB_PLLCFG_DIV1);

  //  Activate the PLL by turning it on then feeding the correct sequence of bytes
  SCB_PLLCON  = SCB_PLLCON_PLLE;
  SCB_PLLFEED = SCB_PLLFEED_FEED1;
  SCB_PLLFEED = SCB_PLLFEED_FEED2;

  //  Wait for the PLL to lock...
  while (!(SCB_PLLSTAT & SCB_PLLSTAT_PLOCK))
    ;

  //  ...before connecting it using the feed sequence again
  SCB_PLLCON  = SCB_PLLCON_PLLC | SCB_PLLCON_PLLE;
  SCB_PLLFEED = SCB_PLLFEED_FEED1;
  SCB_PLLFEED = SCB_PLLFEED_FEED2;

  //  Setup and turn on the MAM.  Three cycle access is used due to the fast
  //  PLL used.  It is possible faster overall performance could be obtained by
  //  tuning the MAM and PLL settings.
  MAM_TIM = MAM_TIM_3;
  MAM_CR = MAM_CR_FULL;

  //  Setup the peripheral bus to be the same as the PLL output (48Mhz)
  SCB_VPBDIV = SCB_VPBDIV_100;

  //  Disable power to all modules (to avoid wasting power)
  SCB_PCONP = SCB_PCONP_ALLOFF;

  //  Make sure all interrupts are disabled
  VIC_IntEnClr = VIC_IntEnClr_MASK;
}

/**************************************************************************/
/*! 
    Disables the Phase Lock Loop (PLL)
*/
/**************************************************************************/
void cpuPLLDisable (void)
{
  SCB_PLLCON  = 0;
  SCB_PLLFEED = SCB_PLLFEED_FEED1;
  SCB_PLLFEED = SCB_PLLFEED_FEED2;
  SCB_PLLCFG =  0;
}


