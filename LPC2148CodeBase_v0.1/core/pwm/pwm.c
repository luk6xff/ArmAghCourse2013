/**************************************************************************/
/*! 
    \file     pwm.c
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 September 2009
    \version  1.00

    Enables the Pulse Width Modulator (PWM), and allows users to set
    the duty cycle, pulse width and pulse frequency (per second).

    The following code will cause the pulse width modulator to create
    a 5mS pulse 96 times per second:
    \code 
    #include <pwm/pwm.h>

    void pwmStart(void)
    {
      pwmPulseWidthInMicroseconds(5000);
      pwmFrequency(96);
      pwmInit();
    }
    \endcode
*/
/**************************************************************************/

#include "FreeRTOS.h"
#include "pwm.h"

// Default values
static int pwmPulseWidth = 20000;
static int pwmPercentage = 50;

/**************************************************************************/
/*! 
    Sets the duty cycle (a percentage between 0 and 100)
*/
/**************************************************************************/
int pwmDutyCycle (int percentage)
{
  if ((percentage < 0) || (percentage > 100))
    return -1;

  PWM_MR5 = ((configCPU_CLOCK_HZ / pwmPulseWidth) * (pwmPercentage = percentage)) / 100;
  PWM_LER = PWM_LER_M5L;

  return 0;
}

/**************************************************************************/
/*! 
    Sets the length of each pulse in microseconds
*/
/**************************************************************************/
int pwmPulseWidthInMicroseconds (int us)
{
  unsigned int count = (configCPU_CLOCK_HZ / 1000000) * us;

  if (count > PWM_MR0)
    return -1;

  PWM_MR5 = count;
  PWM_LER = PWM_LER_M5L;

  return 0;
}

/**************************************************************************/
/*! 
    Sets the number of pulses per second
*/
/**************************************************************************/
int pwmFrequency (int frequency)
{
  if ((frequency < 1) || (frequency >= (int) configCPU_CLOCK_HZ))
    return -1;

  PWM_MR0 = (configCPU_CLOCK_HZ / (pwmPulseWidth = frequency));
  PWM_MR5 = ((configCPU_CLOCK_HZ / pwmPulseWidth) * pwmPercentage) / 100;
  PWM_LER = PWM_LER_M5L | PWM_LER_M0L;

  return 0;
}

/**************************************************************************/
/*! 
    Initialises the pulse width modulator
*/
/**************************************************************************/
void pwmInit (void)
{
  SCB_PCONP |= SCB_PCONP_PCPWM0;
  PCB_PINSEL1 = (PCB_PINSEL1 & ~PCB_PINSEL1_P021_MASK) | PCB_PINSEL1_P021_PWM5;

  PWM_TCR  = PWM_TCR_CR;
  PWM_PR   = 0;
  PWM_MR0  = (configCPU_CLOCK_HZ / pwmPulseWidth);
  PWM_MCR |= PWM_MCR_MR0R;
  PWM_PCR |= PWM_PCR_ENA5;
  PWM_TCR  = (PWM_TCR_CE | PWM_TCR_PWME);

  pwmDutyCycle (50);
}
