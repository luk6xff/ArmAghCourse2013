/**************************************************************************/
/*! 
    \file     adc.c
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 September 2009
    \version  1.00

    Powers up and initialises Analog to Digital Convertor 0.
    
    The LPC2148 contains two 8 channel 4.5MHz, 10-bit ADCs which are
    capable of measuring analog 'voltage' inputs (ADC0 and ADC1).  The
    conversion process results in values between 0 and 1023, depending on
    where the analog voltage being measured was between analog 'GND' and
    analog 'VREF' (0 to 3.3V in most cases).

    The ADC can be used as follows:
    \code
    #include "adc/adc.h"

    int main(void)
    {
      // Initialise ADC 0, Channel 3
      adcInit0_3();
    
      // Constantly read the results of ADC0.3
      unsigned int results = 0;
      while (1)
      {
        results = adcRead0_3();
        // Results now contains the ADC value
      }
    }
    \endcode

*/
/**************************************************************************/

#include "FreeRTOS.h"
#include "adc.h"

/**************************************************************************/
/*! 
    Reads ADC 0.1 and returns to converted results.
*/
/**************************************************************************/
int adcRead0_1 (void)
{
  // Deselects all channels and stops all conversions
  AD0_CR &= ~(AD_CR_START_MASK | AD_CR_SELMASK);  

  // Select channel 1
  AD0_CR |=  (AD_CR_START_NONE | AD_CR_SEL1);

  // Manually start converting now (rather than waiting on an external input)
  AD0_CR |=   AD_CR_START_NOW;                    

  // Wait for the conversion to complete
  while (!(AD0_DR1 & AD_DR_DONE))
    ;

  // Return the processed results
  return ((AD0_DR1 & AD_DR_RESULTMASK)    // Read bits 6..15 on AD0 data register 1 (which contains our conversion results)
          >> AD_DR_RESULTSHIFT);          // shift them to the end of a 32-bit value (giving us a 10-bit number)   
}

/**************************************************************************/
/*! 
    Reads ADC 0.2 and returns to converted results.
*/
/**************************************************************************/
int adcRead0_2 (void)
{
  // Deselects all channels and stops all conversions
  AD0_CR &= ~(AD_CR_START_MASK | AD_CR_SELMASK);  

  // Select channel 2
  AD0_CR |=  (AD_CR_START_NONE | AD_CR_SEL2);

  // Manually start converting now (rather than waiting on an external input)
  AD0_CR |=   AD_CR_START_NOW;                    

  // Wait for the conversion to complete
  while (!(AD0_DR2 & AD_DR_DONE))
    ;

  // Return the processed results
  return ((AD0_DR2 & AD_DR_RESULTMASK)    // Read bits 6..15 on AD0 data register 2 (which contains our conversion results)
          >> AD_DR_RESULTSHIFT);          // shift them to the end of a 32-bit value (giving us a 10-bit number)   
}

/**************************************************************************/
/*! 
    Reads ADC 0.3 and returns to converted results.
*/
/**************************************************************************/
int adcRead0_3 (void)
{
  // Deselects all channels and stops all conversions
  AD0_CR &= ~(AD_CR_START_MASK | AD_CR_SELMASK);  

  // Select channel 3
  AD0_CR |=  (AD_CR_START_NONE | AD_CR_SEL3);     

  // Manually start converting now (rather than waiting on an external input)
  AD0_CR |=   AD_CR_START_NOW;                    

  // Wait for the conversion to complete
  while (!(AD0_DR3 & AD_DR_DONE))
    ;

  // Return the processed results
  return ((AD0_DR3 & AD_DR_RESULTMASK)    // Read bits 6..15 on AD0 data register 3 (which contains our conversion results)
          >> AD_DR_RESULTSHIFT);          // shift them to the end of a 32-bit value (giving us a 10-bit number)   
}

/**************************************************************************/
/*! 
    Applies power and initialises Analog to Digital Converter 0.1 (ADC0.1).

    \warning This code assumes that PCLK = 48MHz!
*/
/**************************************************************************/
void adcInit0_1 (void)
{
  // Force pin 0.28 to function as AD0.1
  PCB_PINSEL1 = (PCB_PINSEL1 & ~PCB_PINSEL1_P028_MASK) | PCB_PINSEL1_P028_AD01;

  // Enable power for ADC0
  SCB_PCONP |= SCB_PCONP_PCAD0;

  // Initialise ADC converter
  AD0_CR = AD_CR_CLKS10                     // 10-bit precision
         | AD_CR_PDN                        // Exit power-down mode
         | ((11 - 1) << AD_CR_CLKDIVSHIFT)  // 4.36MHz Clock (48.0MHz / 11)
         | AD_CR_SEL1;                      // Use channel 1
}

/**************************************************************************/
/*! 
    Applies power and initialises Analog to Digital Converter 0.2 (ADC0.2).

    \warning This code assumes that PCLK = 48MHz!
*/
/**************************************************************************/
void adcInit0_2 (void)
{
  // Force pin 0.29 to function as AD0.2
  PCB_PINSEL1 = (PCB_PINSEL1 & ~PCB_PINSEL1_P029_MASK) | PCB_PINSEL1_P029_AD02;

  // Enable power for ADC0
  SCB_PCONP |= SCB_PCONP_PCAD0;

  // Initialise ADC converter
  AD0_CR = AD_CR_CLKS10                     // 10-bit precision
         | AD_CR_PDN                        // Exit power-down mode
         | ((11 - 1) << AD_CR_CLKDIVSHIFT)  // 4.36MHz Clock (48.0MHz / 11)
         | AD_CR_SEL2;                      // Use channel 2
}

/**************************************************************************/
/*! 
    Applies power and initialises Analog to Digital Converter 0.3 (ADC0.3).

    \warning This code assumes that PCLK = 48MHz!
*/
/**************************************************************************/
void adcInit0_3 (void)
{
  // Force pin 0.30 to function as AD0.3
  PCB_PINSEL1 = (PCB_PINSEL1 & ~PCB_PINSEL1_P030_MASK) | PCB_PINSEL1_P030_AD03;

  // Enable power for ADC0
  SCB_PCONP |= SCB_PCONP_PCAD0;

  // Initialise ADC converter
  AD0_CR = AD_CR_CLKS10                     // 10-bit precision
         | AD_CR_PDN                        // Exit power-down mode
         | ((11 - 1) << AD_CR_CLKDIVSHIFT)  // 4.36MHz Clock (48.0MHz / 11)
         | AD_CR_SEL3;                      // Use channel 3
}
