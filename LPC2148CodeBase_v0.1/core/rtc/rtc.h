/**************************************************************************/
/*! 
    \file     rtc.h
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     9 November 2009
    \version  1.00
*/
/**************************************************************************/

#ifndef _RTC_H_
#define _RTC_H_

#include <time.h>

//
//  Define this for using non-vectored IRQs.  Undef it if a regular vectored
//  IRQ is preferred.
//
#define RTC_NONVECTOREDIRQ

//
//
//
void rtcInit (void);
time_t rtcGetEpochSeconds (unsigned int *milliseconds);
void rtcSetEpochSeconds (time_t now);
int rtcSetAlarm (struct tm *tm);
struct tm *rtcGetAlarmTm (struct tm *tm);
time_t rtcGetAlarmEpochSeconds (void);
int rtcPeriodicAlarm (int mode);

#endif
