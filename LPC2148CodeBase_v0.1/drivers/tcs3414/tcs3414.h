/**************************************************************************/
/*! 
    \file     tcs3414.h
    \author   K. Townsend (microBuilder.eu)
    \date     25 September 2009
    \version  1.00
*/
/**************************************************************************/

#ifndef _TCS3414_H_
#define _TCS3414_H_

#include "FreeRTOS.h"
#include "task.h"

#include "core/i2c/i2c.h"

#define TCS3414_ADDRESS                           ((unsigned int) (0x72))   // 011 1001 shifted left 1 bit = 0x72

#define TCS3414_COMMAND_WORD                      ((unsigned int) (0xA0))   // To use word transactions add 101x xxxx to commands
#define TCS3414_COMMAND_BYTES                     ((unsigned int) (0x80))   // To use byte transactions add 100x xxxx to commands

#define TCS3414_REGISTER_CONTROL                  ((unsigned int) (0x00))
#define TCS3414_REGISTER_TIMING                   ((unsigned int) (0x01))
#define TCS3414_REGISTER_INTERRUPT                ((unsigned int) (0x02))
#define TCS3414_REGISTER_INTSOURCE                ((unsigned int) (0x03))
#define TCS3414_REGISTER_PARTNO_REVID             ((unsigned int) (0xA4))   // 0xA0 + 0x04 = 0xA4
#define TCS3414_REGISTER_GAIN                     ((unsigned int) (0x07))
#define TCS3414_REGISTER_LOWTHRESHOLD_LOWBYTE     ((unsigned int) (0x08))
#define TCS3414_REGISTER_LOWTHRESHOLD_HIGHBYTE    ((unsigned int) (0x09))
#define TCS3414_REGISTER_HIGHTHRESHOLD_LOWBYTE    ((unsigned int) (0x0A))
#define TCS3414_REGISTER_HIGHTHRESHOLD_HIGHBYTE   ((unsigned int) (0x0B))
#define TCS3414_REGISTER_GREENLOW                 ((unsigned int) (0xB0))   // 0xA0 + 0x10 = 0xB0
#define TCS3414_REGISTER_GREENHIGH                ((unsigned int) (0xB1))   // 0xA0 + 0x11 = 0xB1
#define TCS3414_REGISTER_REDLOW                   ((unsigned int) (0xB2))
#define TCS3414_REGISTER_REDHIGH                  ((unsigned int) (0xB3))
#define TCS3414_REGISTER_BLUELOW                  ((unsigned int) (0xB4))
#define TCS3414_REGISTER_BLUEHIGH                 ((unsigned int) (0xB5))
#define TCS3414_REGISTER_CLEARLOW                 ((unsigned int) (0xB6))
#define TCS3414_REGISTER_CLEARHIGH                ((unsigned int) (0xB7))

/**************************************************************************/
/*! 
    The Gain setting (bit [5:4] in the GAIN register) multiplies the output
    by the specified amount, allowing you to adjust it's sensitivity and
    dynamic range.
*/
/**************************************************************************/
typedef enum
{
  tcs3414Gain_1 = 0x00,
  tcs3414Gain_4 = 0x10,
  tcs3414Gain_16 = 0x20,
  tcs3414Gain_64 = 0x30
} 
tcs3414Gain_t;
#define TCS3414_GAIN_MASK                         ((unsigned int) (0x30))

/**************************************************************************/
/*! 
    The Prescalar (bits [2:0] in the GAIN register) divides down the
    output by the specified amount, allowing you to adjust it's
    sensitivity and dynamic range.
*/
/**************************************************************************/
typedef enum
{
  tcs3414Prescalar_1 = 0x00,
  tcs3414Prescalar_2 = 0x01,
  tcs3414Prescalar_4 = 0x02,
  tcs3414Prescalar_8 = 0x03,
  tcs3414Prescalar_16 = 0x04,
  tcs3414Prescalar_32 = 0x05,
  tcs3414Prescalar_64 = 0x06
} 
tcs3414Prescalar_t;
#define TCS3414_PRESCALAR_MASK                    ((unsigned int) (0x07))

/**************************************************************************/
/*! 
    A list of all available 'colours' that can be measured by the IC.
*/
/**************************************************************************/
typedef enum
{
  tcs3414Colours_Red,
  tcs3414Colours_Green,
  tcs3414Colours_Blue,
  tcs3414Colours_Clear
} 
tcs3414Colours_t;

signed portBASE_TYPE tcs3414TaskStart (void);

int tcs3414Init(void);
int tcs3414GetColour(tcs3414Colours_t colour, int *value);
int tcs3414GetICRevision(int *value);
int tcs3414SetGainAndPrescalar(tcs3414Gain_t gain, tcs3414Prescalar_t prescalar);

#endif