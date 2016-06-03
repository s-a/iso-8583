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

#include "dl_time.h"

/******************************************************************************/
//
// CONSTANTS
//

#define kMIN_YEAR				1970
#define kMAX_YEAR				2106

#define kTIMESTAMP_MIN_VALUE	"19700101000000"
#define kTIMESTAMP_MAX_VALUE	"21060207062815"

#define kSECONDS_IN_MINUTE		60
#define kMINUTES_IN_HOUR		60
#define kHOURS_IN_DAY			24
#define kDAYS_IN_NORMAL_YEAR	365

#define kSECONDS_IN_HOUR		(kMINUTES_IN_HOUR * kSECONDS_IN_MINUTE)
#define kSECONDS_IN_DAY			(kHOURS_IN_DAY * kSECONDS_IN_HOUR)
#define kSECONDS_IN_NORMAL_YEAR	(kDAYS_IN_NORMAL_YEAR * kSECONDS_IN_DAY)
#define kSECONDS_IN_LEAP_YEAR	(kSECONDS_IN_NORMAL_YEAR + kSECONDS_IN_DAY)

// indicates the day of week for 1/1/1970 (eg Thursday)
#define kFIRST_DAY_1970			kDL_TIME_DOW_THURSDAY

/******************************************************************************/
//
// TYPES
//

struct TZ_INFO_S
{
	/* seconds to be added to UTC to get 'base' localtime */
	DL_SINT32 baseBias;

	/* indicates if DST is used */
	int       hasDST; /* 0:No, else Yes */

	/* DST */
	DL_SINT32 dstBias;
	DL_TIME   dstStart;
	DL_TIME   dstEnd;
};
typedef struct TZ_INFO_S TZ_INFO;

/******************************************************************************/
//
// MACROS
//

/* indicates whether the specified year is a leap year */
#define isLeapYear(year)\
((year%4)?0:((year%100)?1:((year%400)?0:1)))

#define TZ_INFO_BASE_BIAS(tzInfo)	((tzInfo)->baseBias)
#define TZ_INFO_HAS_DST(tzInfo)		((tzInfo)->hasDST)
#define TZ_INFO_DST_BIAS(tzInfo)	((tzInfo)->dstBias)

/******************************************************************************/
//
// PRIVATE VARIABLES
//

static int     _haveTZ = 0;
static TZ_INFO _tz;

/******************************************************************************/

// NB only loads time-zone details on the first execution
static void _get_tz_info ( void );

/******************************************************************************/

static void DL_TIME_Populate ( int      iYear,
							   int      iMonth,
							   int      iDay,
							   int      iHour,
							   int      iMinute,
							   int      iSecond,
							   int      _iDayOfWeek,
							   DL_TIME *oData );

/******************************************************************************/

static void seconds_to_struct ( DL_UINT32  iSeconds,
							    DL_TIME   *oData );

/******************************************************************************/

/* NB '_year' should be specified if 'day-in-month' mode used */
static void struct_to_timestamp ( int            _iYear,
								  const DL_TIME *iData,
								  char          *oTimestampStr );

/******************************************************************************/

/* NB '_year' should be specified if 'day-in-month' mode used */
static void struct_to_seconds ( int            _iYear,
							    const DL_TIME *iData,
							    DL_UINT32     *oSeconds );

/* converts a timestamp (YYYYMMDDHHMISS) to the date/time structure */
/* returns: 1 if ok / 0 otherwise (eg invalid timestamp)            */
static int timestamp_to_struct ( const char *iTimestampStr,
								 DL_TIME    *oData );

/******************************************************************************/

// ensures that any 'Day-in-Month' values are converted to absolute
static void rationalise_struct ( int            _iYear,
								 const DL_TIME *iData,
								 DL_TIME       *oData );

static int calc_day_of_week ( int iYear,
							  int iMonth,
							  int iDay );

static int calc_days_in_month ( int iYear,
							    int iMonth );

static void calc_dst_start_end ( int        _iYear,
								 DL_UINT32 *iDstStart,
								 DL_UINT32 *iDstEnd );

/******************************************************************************/

DL_UINT32 DL_TIME_GetUTCSeconds ( void )
{
	DL_UINT32 ret = 0;

	ret = (DL_UINT32)(time(NULL) & DL_MAX_UINT32);

	return ret;
}

/******************************************************************************/

void DL_TIME_ConvUTCSecondsToLocalStruct ( DL_UINT32  iUtcSecs,
										   DL_TIME   *oData )
{
	/* get time-zone specific settings */
	_get_tz_info();

	/* apply base Bias to UTC value */
	iUtcSecs += TZ_INFO_BASE_BIAS(&_tz);

	/* convert seconds to struct */
	seconds_to_struct(iUtcSecs,oData);

	/* perform Std/DST adjustment (where required) */
	if ( TZ_INFO_HAS_DST(&_tz) )
	{
		int       inDST    = 0;
		DL_UINT32 dstStart = 0,
		          dstEnd   = 0;

		calc_dst_start_end(oData->year,&dstStart,&dstEnd);

		if ( ((dstStart <= dstEnd) &&  (iUtcSecs >= dstStart) && (iUtcSecs < dstEnd))  ||
		     ((dstStart >  dstEnd) && ((iUtcSecs >= dstStart) || (iUtcSecs < dstEnd))) )
		{
			inDST = 1;
		}

		if ( inDST )
		{
			/* apply DST bias */
			iUtcSecs += TZ_INFO_DST_BIAS(&_tz);
		}

		/* convert seconds to struct */
		seconds_to_struct(iUtcSecs,oData);
	}

	return;
}

/******************************************************************************/

void DL_TIME_ConvUTCSecondsToLocalTimestamp ( DL_UINT32 iUtcSecs,
											  char      oTimestamp[] )
{
	DL_TIME st;

	/* convert seconds (utc) to struct (local) */
	DL_TIME_ConvUTCSecondsToLocalStruct(iUtcSecs,&st);

	/* convert struct to timestamp */
	struct_to_timestamp(0,&st,oTimestamp);

	return;
}

/******************************************************************************/

void DL_TIME_ConvUTCSecondsToUTCStruct ( DL_UINT32  iUtcSecs,
										 DL_TIME   *oData )
{
	/* convert seconds to struct */
	seconds_to_struct(iUtcSecs,oData);

	return;
}

/******************************************************************************/

void DL_TIME_ConvUTCSecondsToUTCTimestamp ( DL_UINT32 iUtcSecs,
										    char      oTimestamp[] )
{
	DL_TIME st;

	/* convert seconds (utc) to struct (utc) */
	DL_TIME_ConvUTCSecondsToUTCStruct(iUtcSecs,&st);

	/* convert struct to timestamp */
	struct_to_timestamp(0,&st,oTimestamp);

	return;
}

/******************************************************************************/

int DL_TIME_ConvUTCTimestampToUTCSeconds ( const char  iTimestamp[],
										   DL_UINT32  *oUtcSecs )
{
	int     ok = 1;
	DL_TIME st;

	/* init outputs */
	*oUtcSecs = 0;

	/* timestamp (utc) to struct (utc) */
	ok = timestamp_to_struct(iTimestamp,&st);

	if ( ok )
	{
		/* struct (utc) to seconds (utc) */
		struct_to_seconds(0,&st,oUtcSecs);
	}

	return ok;
}

/******************************************************************************/

int DL_TIME_ConvLocalTimestampToUTCSeconds ( const char  iTimestamp[],
											 DL_UINT32  *oUtcSecs )
{
	int     ok = 1;
	DL_TIME st;

	/* init outputs */
	*oUtcSecs = 0;

	/* get time-zone specific settings */
	_get_tz_info();

	/* timestamp (local) to struct (local) */
	ok = timestamp_to_struct(iTimestamp,&st);

	if ( ok )
	{
		/* struct (local) to seconds (local) */
		struct_to_seconds(0,&st,oUtcSecs);

		/* perform Std/DST adjustment (where required) */
		if ( TZ_INFO_HAS_DST(&_tz) )
		{
			int       inDST    = 0;
			DL_UINT32 dstStart = 0,
				      dstEnd   = 0;

			calc_dst_start_end(st.year,&dstStart,&dstEnd);

			dstStart += TZ_INFO_DST_BIAS(&_tz);
			dstEnd   -= TZ_INFO_DST_BIAS(&_tz);

			if ( ((dstStart <= dstEnd) &&  (*oUtcSecs >= dstStart) && (*oUtcSecs < dstEnd))  ||
			     ((dstStart >  dstEnd) && ((*oUtcSecs >= dstStart) || (*oUtcSecs < dstEnd))) )
			{
				inDST = 1;
			}

			if ( inDST )
			{
				/* remove DST bias */
				*oUtcSecs -= TZ_INFO_DST_BIAS(&_tz);
			}
		}

		/* remove base Bias to UTC value */
		*oUtcSecs -= TZ_INFO_BASE_BIAS(&_tz);
	}

	return ok;
}

/******************************************************************************/

void DL_TIME_ConvUTCSecondsToLocalFormatStr ( DL_UINT32  iUtcSecs,
										      char      *ioFormatStr )
{
	DL_TIME st;

	/* init outputs */
	sprintf(ioFormatStr,"");

	/* convert UTC seconds to Local structure */
	DL_TIME_ConvUTCSecondsToLocalStruct(iUtcSecs,&st);

	/* populate buffer */
	sprintf(ioFormatStr,"%02d/%02d/%04d (%02d:%02d:%02d)",
			st.day,
			st.month,
			st.year,
			st.hour,
			st.minute,
			st.second);

	return;
}

/******************************************************************************/

int _DL_TIME_AddMonths ( DL_UINT32  iSeconds,
						 DL_UINT16  iNumMonths,
						 DL_UINT32 *oSeconds )
{
	int     ok = 1;
	DL_TIME dt;

	/* convert to nice date/time format */
	DL_TIME_ConvUTCSecondsToUTCStruct(iSeconds,&dt);

	if ( ok )
	{
		int years  = iNumMonths / 12;
		int months = iNumMonths % 12;

		/* add whole years */
		dt.year += years;

		/* add partial months (may cause year overflow) */
		if ( (dt.month + months) <= 12 ) /* same year */
		{
			dt.month += months;
		}
		else /* next year */
		{
			dt.year++;
			dt.month = (dt.month + months) - 12;
		}

		/* adjust 'day' if exceeds maximum for month/year combination */
		dt.day = MAX(dt.day,calc_days_in_month(dt.year,dt.month));
	}

	/* convert back to seconds and pass to caller */
	if ( ok )
	{
		/* struct (utc) to seconds (utc) */
		struct_to_seconds(0,&dt,oSeconds);
	}

	/* check for seconds overflow */
	if ( ok && (iSeconds > *oSeconds) )
	{
		/* overflow because input is greater than output */
		ok = 0;
	}

	return ok;
}

/******************************************************************************/

void _DL_TIME_GetDaysInMonth ( int  iMonth,
						       int  iYear,
							   int *oDaysInMonth )
{
	/* init outputs */
	*oDaysInMonth     = 0;

	/* determine 'days in month' */
	*oDaysInMonth = calc_days_in_month(iYear,iMonth);

	return;
}

/******************************************************************************/

static void _get_tz_info ( void )
{
	/* do nothing if we have already loaded the time zone details */
	if ( _haveTZ )
		return;

	/* init structure */
	DL_MEM_memset(&_tz,0,sizeof(TZ_INFO));

#if defined(DL_WIN32)
	{
		TIME_ZONE_INFORMATION winTZ;
		DWORD                 rc;
		int                   tempOk = 1;

		/* get timezone */
		rc = GetTimeZoneInformation(&winTZ);

		if ( rc == TIME_ZONE_ID_UNKNOWN )
		{
			/* timezone does NOT have DST */
			_tz.hasDST = 0;
		}
		else if ( (rc == TIME_ZONE_ID_STANDARD) ||
			      (rc == TIME_ZONE_ID_DAYLIGHT) )
		{
			/* timezone has DST */
			_tz.hasDST = 1;
		}
		else /* unexpected error */
		{
			/* indicate local error */
			/* NB timezone will default to UTC with no DST */
			tempOk = 0;
		}

		if ( tempOk )
		{
			/* base bias */
			_tz.baseBias = (winTZ.Bias * -1) * 60;

			/* DST bias */
			_tz.dstBias = (winTZ.DaylightBias * -1) * 60;

			/* DST start */
			DL_TIME_Populate(winTZ.DaylightDate.wYear,
			                 winTZ.DaylightDate.wMonth,
						     winTZ.DaylightDate.wDay,
							 winTZ.DaylightDate.wHour,
							 winTZ.DaylightDate.wMinute,
							 winTZ.DaylightDate.wSecond,
							 winTZ.DaylightDate.wDayOfWeek,
							 &(_tz.dstStart));

			/* DST end */
			DL_TIME_Populate(winTZ.StandardDate.wYear,
				             winTZ.StandardDate.wMonth,
							 winTZ.StandardDate.wDay,
							 winTZ.StandardDate.wHour,
							 winTZ.StandardDate.wMinute,
							 winTZ.StandardDate.wSecond,
							 winTZ.StandardDate.wDayOfWeek,
							 &(_tz.dstEnd));
		}

		/* indicate TZ details loaded */
		_haveTZ = 1;
	}
#elif defined(DL_UNIX)
	// NB for unix we default to GMT
    _tz.hasDST   = 0;
    _tz.baseBias = 0;
    _tz.dstBias  = 0;
#else
#error Platform Not Supported
#endif

	return;
}

/******************************************************************************/

static void DL_TIME_Populate ( int      iYear,
							   int      iMonth,
							   int      iDay,
							   int      iHour,
							   int      iMinute,
							   int      iSecond,
							   int      _iDayOfWeek,
							   DL_TIME *oData )
{
	DL_MEM_memset(oData,0,sizeof(DL_TIME));

	oData->year       = iYear;
	oData->month      = iMonth;
	oData->day        = iDay;
	oData->hour       = iHour;
	oData->minute     = iMinute;
	oData->second     = iSecond;
	oData->_dayOfWeek = _iDayOfWeek;

	return;
}

/******************************************************************************/

static void seconds_to_struct ( DL_UINT32  iSeconds,
							    DL_TIME   *oData )
{
    int       i;
    int       done;
    DL_UINT32 tempUInt32;
	int       year   = kMIN_YEAR,
		      month  = 1,
			  day    = 1,
			  hour   = 0,
			  minute = 0,
			  second = 0,
			  DoW    = 0;

    /* init ouput parameters */
	DL_MEM_memset(oData,0,sizeof(DL_TIME));

    /* force input range to 32-bits */
    iSeconds &= DL_MAX_UINT32;

	/* calculate the 'day-of-week'                             */
	/* NB performed first as simplest to use the seconds value */
	DoW = (int)(((iSeconds / kSECONDS_IN_DAY) + kFIRST_DAY_1970) % 7);

    /* calculate the year (1970-2106) */
    done = 0;
    do
    {
        /* determine the number of seconds for this year */
        tempUInt32 = isLeapYear(year) ? kSECONDS_IN_LEAP_YEAR : kSECONDS_IN_NORMAL_YEAR;

        if ( iSeconds >= tempUInt32 )
        {
            year++;
            iSeconds -= tempUInt32;
        }
        else
        {
            done = 1;
        }
    } while ( !done );
    
    /* calculate 'month' (1-12) */
    for ( i=0,done=0 ; (i<12) && (!done) ; i++ )
    {
        /* determine number of seconds for month (i+1) */
        tempUInt32 = (calc_days_in_month(year,month) * kSECONDS_IN_DAY);
        
        if ( iSeconds >= tempUInt32 )
        {
            month++;
            iSeconds -= tempUInt32;
        }
        else
        {
            done = 1;
        }
    } /* end-for */
    
    /* calculate 'day' (1-n) */
    day      += iSeconds / kSECONDS_IN_DAY;
    iSeconds -= (day-1)  * kSECONDS_IN_DAY;

    /* calculate 'hours' (0-n) */
    hour       = iSeconds / kSECONDS_IN_HOUR;
    iSeconds  -= hour     * kSECONDS_IN_HOUR;

    /* calculate 'minutes' (0-n) */
    minute    = iSeconds / kSECONDS_IN_MINUTE;
    iSeconds -= minute   * kSECONDS_IN_MINUTE;

    /* calculate 'seconds' (0-n) */
    second = iSeconds;

	/* populate structure */
	DL_TIME_Populate(year,month,day,hour,minute,second,DoW,oData);

    return;
}

/******************************************************************************/

/* NB '_year' should be specified if 'day-in-month' mode used */
static void struct_to_timestamp ( int            _iYear,
								  const DL_TIME *iData,
								  char          *oTimestampStr )
{
	DL_TIME tmpSt;

	rationalise_struct(_iYear,iData,&tmpSt);

	sprintf(oTimestampStr,"%04d%02d%02d%02d%02d%02d",
			tmpSt.year,tmpSt.month,tmpSt.day,tmpSt.hour,tmpSt.minute,tmpSt.second);

	return;
}

/******************************************************************************/

/* NB '_year' should be specified if 'day-in-month' mode used */
static void struct_to_seconds ( int            _iYear,
							    const DL_TIME *iData,
							    DL_UINT32     *oSeconds )
{
	DL_TIME tmpSt;
	int     i;

	/* init output params */
	*oSeconds = 0;

	rationalise_struct(_iYear,iData,&tmpSt);

	/* process 'year' */
	for ( i=kMIN_YEAR ; i<tmpSt.year ; i++ )
	{
		*oSeconds += kSECONDS_IN_NORMAL_YEAR;

		if ( isLeapYear(i) ) /* is a leap year */
			*oSeconds += kSECONDS_IN_DAY;
	}

	/* process 'month' */
	for ( i=1 ; i<tmpSt.month ; i++ )
		*oSeconds += calc_days_in_month(tmpSt.year,i) * kSECONDS_IN_DAY;

	/* process 'day' */
	*oSeconds += (tmpSt.day - 1) * kSECONDS_IN_DAY;

	/* process 'hours' */
	*oSeconds += tmpSt.hour * kSECONDS_IN_HOUR;

	/* process 'minutes' */
	*oSeconds += tmpSt.minute * kSECONDS_IN_MINUTE;

	/* process 'seconds' */
	*oSeconds += tmpSt.second;

	/* limit output size */
	*oSeconds &= DL_MAX_UINT32;

	return;
}

/******************************************************************************/

/* converts a timestamp (YYYYMMDDHHMISS) to the date/time structure */
/* returns: 1 if ok / 0 otherwise (eg invalid timestamp)            */
static int timestamp_to_struct ( const char *iTimestampStr,
								 DL_TIME    *oData )
{
	int ok     = 1;
	int year   = 0,
		month  = 0,
		day    = 0,
		hour   = 0,
		minute = 0,
		second = 0;

	/* init output params */
	DL_MEM_memset(oData,0,sizeof(DL_TIME));

	/* check 'timestamp' format (high-level) */
	ok = DL_STR_Validate(iTimestampStr,kDL_TIME_TIMESTAMP_LEN,kDL_TIME_TIMESTAMP_LEN,"0123456789");

	/* check that date/time string is within the required range */
    if ( ok && ((DL_STR_StrCmp(iTimestampStr,kTIMESTAMP_MIN_VALUE,0) == -1) ||
                (DL_STR_StrCmp(iTimestampStr,kTIMESTAMP_MAX_VALUE,0) ==  1)) )
	{
		ok = 0;
	}

	/* decompose timestamp into individual parts */
	if ( ok )
	{
		/* split string into date/time parts */
		sscanf(iTimestampStr,"%04d%02d%02d%02d%02d%02d",
			   &year,&month,&day,&hour,&minute,&second);
	}

	/* NB min/max year has already been validated */

	/* validate 'month' (1-12) */
	if ( ok && !((month >= 1) && (month <= 12)) )
		ok = 0;

	/* validate 'day' (for given month) */
	if ( ok && !((day >= 1) && (day <= calc_days_in_month(year,month))) )
		ok = 0;

	/* validate 'hour' (0-23) */
	if ( ok && !((hour >= 0) && (hour <= 23)) )
		ok = 0;

	/* validate 'minute' (0-59) */
	if ( ok && !((minute >= 0) && (minute <= 59)) )
		ok = 0;

	/* validate 'second' (0-59) */
	if ( ok && !((second >= 0) && (second <= 59)) )
		ok = 0;

	if ( ok )
	{
		DL_TIME_Populate(year,month,day,hour,minute,second,
						 calc_day_of_week(year,month,day),
						 oData);
	}

	return ok;
}

/******************************************************************************/

// ensures that any 'Day-in-Month' values are converted to absolute
static void rationalise_struct ( int            _iYear,
								 const DL_TIME *iData,
								 DL_TIME       *oData )
{
	int i;

	/* copy struct to local memory */
	DL_MEM_memcpy(oData,iData,sizeof(DL_TIME));

	/* perform 'day-in-month' processing (where required) */
	if ( iData->year == 0 )
	{
		int DoW   = 0;
		int first = 1;

		/* set 'Year */
		oData->year = _iYear;

		/* check that valid '_year' was provided by caller */
		if ( !((_iYear >= kMIN_YEAR) && (_iYear <= kMAX_YEAR)) ) /* invalid */
			_iYear = kMIN_YEAR; /* default to min valid year */

		/* determine 1st day of '_year' */
		for ( DoW=kFIRST_DAY_1970,i=kMIN_YEAR ; i<_iYear ; i++ )
		{
			DoW = (DoW + kDAYS_IN_NORMAL_YEAR + (isLeapYear(i) ? 1 : 0)) % 7;
		}

		/* determine 1st day of 'month' */
		for ( i=1 ; i<iData->month ; i++ )
		{
			DoW = (DoW + calc_days_in_month(_iYear,i)) % 7;
		}

		/* get 1st desired day in month */
		if ( iData->_dayOfWeek > DoW )
		{
			first += iData->_dayOfWeek - DoW;
		}
		else if ( iData->_dayOfWeek < DoW )
		{
			first += 7 - (DoW - iData->_dayOfWeek);
		}

		/* determine 'day' */
		if ( iData->day >= 5 ) /* last day of month */
		{
			oData->day = (((calc_days_in_month(_iYear,iData->month) - first) / 7) * 7) + first;
		}
		else if ( iData->day > 0 ) /* day 1-4 */
		{
			oData->day = first + ((iData->day - 1) * 7);
		}
		else /* invalid */
		{
			oData->day = first;
		}
	}

	return;
}

/******************************************************************************/

static int calc_day_of_week ( int iYear,
							  int iMonth,
							  int iDay )
{
	int       DoW     = 0;
	int       i;
	DL_UINT32 numDays = kFIRST_DAY_1970;

	/* calculate days for full years */
	for ( i=kMIN_YEAR ; i<iYear ; i++ )
	{
		numDays += kDAYS_IN_NORMAL_YEAR;

		if ( isLeapYear(i) )
			numDays++;
	}

	/* calculate days for full months */
	for ( i=1 ; i<iMonth ; i++ )
	{
		numDays += calc_days_in_month(iYear,i);
	}

	/* calculate days for month days */
	if ( iDay > 1 )
	{
		numDays += iDay - 1;
	}

	DoW = (int)(numDays % 7);

	return DoW;
}

/******************************************************************************/

static int calc_days_in_month ( int iYear,
							    int iMonth )
{
	int days = 0;
    int daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

	if ( (iMonth == 2) && isLeapYear(iYear) )
	{
		days = 29;
	}
	else if ( (iMonth >= 1) && (iMonth <= 12) )
	{
		days = daysInMonth[iMonth-1];
	}

	return days;
}

/******************************************************************************/

static void calc_dst_start_end ( int        _iYear,
								 DL_UINT32 *iDstStart,
								 DL_UINT32 *iDstEnd )
{
	TZ_INFO tmpTZ;

	/* init output params */
	*iDstStart = 0;
	*iDstEnd   = 0;

	_get_tz_info();

	/* copy TZ details */
	DL_MEM_memcpy(&tmpTZ,&_tz,sizeof(TZ_INFO));

	/* set 'year' to appropriate value if 'absolute' date                        */
	/* NB required to prevent the calculation of start/end values for wrong year */
	if ( tmpTZ.dstStart.year != 0 )
		tmpTZ.dstStart.year = _iYear;
	if ( tmpTZ.dstEnd.year != 0 )
		tmpTZ.dstEnd.year = _iYear;

	/* convert start/end values to raw seconds */
	struct_to_seconds(_iYear,&(tmpTZ.dstStart),iDstStart);
	struct_to_seconds(_iYear,&(tmpTZ.dstEnd)  ,iDstEnd  );

	return;
}

/******************************************************************************/
