/**************************************************************************/
/*! 
    \file     pwm.h
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 September 2009
    \version  1.00
*/
/**************************************************************************/

#ifndef _PWM_H_
#define _PWM_H_

int pwmDutyCycle (int percentage);
int pwmPulseWidthInMicroseconds (int us);
int pwmFrequency (int frequency);
void pwmInit (void);

#endif
