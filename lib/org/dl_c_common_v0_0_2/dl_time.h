/******************************************************************************/
/*                                                                            */
/* Copyright (C) 2004-2007 Oscar Sanderson                                    */
/*                                                                            */
/* This software is provided 'as-is', without any express or implied          */
/* warranty.  In no event will the author(s) be held liable for any damages   */
/* arising from the use of this software.                                     */
/*                                                                            */
/* Permission is granted to anyone to use this software for any purpose,      */
/* including commercial applications, and to alter it and redistribute it     */
/* freely, subject to the following restrictions:                             */
/*                                                                            */
/* 1. The origin of this software must not be misrepresented; you must not    */
/*    claim that you wrote the original software. If you use this software    */
/*    in a product, an acknowledgment in the product documentation would be   */
/*    appreciated but is not required.                                        */
/*                                                                            */
/* 2. Altered source versions must be plainly marked as such, and must not be */
/*    misrepresented as being the original software.                          */
/*                                                                            */
/* 3. This notice may not be removed or altered from any source distribution. */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* Date/Time module providing functionality for both UTC and Local values.    */
/* Uses a standard Epoch of 1/1/1970 00:00:00                                 */
/*                                                                            */
/* Provides full support if WIN32 is defined, otherwise defaults to GMT       */
/*                                                                            */
/******************************************************************************/

#ifndef __INC_DL_TIME
#define __INC_DL_TIME

#include "dl_base.h"
#include "dl_mem.h"
#include "dl_str.h"

#include <time.h> // included for 'time'

#ifdef DL_WIN32
#include <windows.h>
#include <winbase.h>
#endif /* DL_WIN32 */

#ifdef DL_UNIX
#include <sys/types.h>
#endif /* DL_UNIX */

/******************************************************************************/
//
// CONSTANTS
//

// indicates the length of a YYYYMMDDHHMISS format timestamp
#define kDL_TIME_TIMESTAMP_LEN		14

// indicates the length of a 'DD/MM/YYYY (HH:MI:SS)' format string
#define kDL_TIME_FORMAT_STR_LEN		21

// Day-of-Week definitions
#define kDL_TIME_DOW_SUNDAY			0
#define kDL_TIME_DOW_MONDAY			1
#define kDL_TIME_DOW_TUESDAY		2
#define kDL_TIME_DOW_WEDNESDAY		3
#define kDL_TIME_DOW_THURSDAY		4
#define kDL_TIME_DOW_FRIDAY			5
#define kDL_TIME_DOW_SATURDAY		6

// Month definitions
#define kDL_TIME_MON_JANUARY		1
#define kDL_TIME_MON_FEBRUARY		2
#define kDL_TIME_MON_MARCH			3
#define kDL_TIME_MON_APRIL			4
#define kDL_TIME_MON_MAY			5
#define kDL_TIME_MON_JUNE			6
#define kDL_TIME_MON_JULY			7
#define kDL_TIME_MON_AUGUST			8
#define kDL_TIME_MON_SEPTEMBER		9
#define kDL_TIME_MON_OCTOBER		10
#define kDL_TIME_MON_NOVEMBER		11
#define kDL_TIME_MON_DECEMBER		12

/******************************************************************************/
//
// TYPES
//

// Note: supports either Absolute or Day-in-Month modes
struct DL_TIME_S
{
	// Day-in-month mode:
	//
	// Identified by 'Year' = 0
	// 'dayOfWeek' indicates Sunday->Saturday
	// 'day' indicates week (1-5, where 5=last)
	//
	// e.g. Last Thursday of April...
	//
	//        Year     : 0
	//        Month    : 4 (April)
	//        Day      : 5 (Last)
	//        DayOfWeek: 4 (Thursday)

	int year;   /* 1970-2106 */
	int month;  /* 1-12, as per kDL_TIME_MON_* */
	int day;    /* 1-31 */
	int hour;   /* 0-23 */
	int minute; /* 0-59 */
	int second; /* 0-59 */

	int _dayOfWeek; /* 0-6, as per kDL_TIME_DOW_* */
};
typedef struct DL_TIME_S DL_TIME;

/******************************************************************************/

// returns: number of UTC seconds since epoch
DL_UINT32 DL_TIME_GetUTCSeconds ( void );

/******************************************************************************/

// converts UTC seconds to local time structure
void DL_TIME_ConvUTCSecondsToLocalStruct ( DL_UINT32  iUtcSecs,
										   DL_TIME   *oData );

// converts UTC seconds to local timestamp
void DL_TIME_ConvUTCSecondsToLocalTimestamp ( DL_UINT32 iUtcSecs,
											  char      oTimestamp[] );

// converts UTC seconds to UTC time structure
void DL_TIME_ConvUTCSecondsToUTCStruct ( DL_UINT32  iUtcSecs,
										 DL_TIME   *oData );

// converts UTC seconds to UTC timestamp
void DL_TIME_ConvUTCSecondsToUTCTimestamp ( DL_UINT32 iUtcSecs,
										    char      oTimestamp[] );

/******************************************************************************/

// converts UTC timestamp to UTC seconds
// returns: 1 if okay, 0 if error (ie invalid timestamp)
int DL_TIME_ConvUTCTimestampToUTCSeconds ( const char  iTimestamp[],
										   DL_UINT32  *oUtcSecs );

/******************************************************************************/

// converts local timestamp to UTC seconds
// returns: 1 if okay, 0 if error (ie invalid timestamp)
int DL_TIME_ConvLocalTimestampToUTCSeconds ( const char  iTimestamp[],
											 DL_UINT32  *oUtcSecs );

/******************************************************************************/

// converts UTC seconds to a local time formatted string
// NB ioFormatStr must point to at least kDL_TIME_FORMAT_STR_LEN byte of memory
void DL_TIME_ConvUTCSecondsToLocalFormatStr ( DL_UINT32  iUtcSecs,
										      char      *ioFormatStr );

/******************************************************************************/

// adds the months (iNumMonths) to the date/time value (iSeconds)
// outputs: oSeconds - updated value with months added
// returns: 1 if success / 0 if error (ie would exceed max date/time)
int _DL_TIME_AddMonths ( DL_UINT32  iSeconds,
						 DL_UINT16  iNumMonths,
						 DL_UINT32 *oSeconds );

/******************************************************************************/

// outputs the number of days based on the specified year and month
void _DL_TIME_GetDaysInMonth ( int  iMonth,
						       int  iYear,
							   int *oDaysInMonth );

/******************************************************************************/

#endif /* __INC_DL_TIME */
