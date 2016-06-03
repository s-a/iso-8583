/******************************************************************************/
/*                                                                            */
/* Copyright (C) 2005-2007 Oscar Sanderson                                    */
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
/* Timer Module - Providing millisecond level timer capabilities              */
/*                                                                            */
/* NB Rollover occurs after 49.7 days due to UINT32 precision                 */
/*                                                                            */
/******************************************************************************/

#include "dl_timer.h"

/******************************************************************************/

// returns: number of milliseconds
static DL_UINT32 getMSec ( void );

/******************************************************************************/

void DL_TIMER_Start ( DL_TIMER *oTimer )
{
	/* init outputs */
	DL_MEM_memset(oTimer,0,sizeof(DL_TIMER));

	/* record seconds */
	oTimer->sec = DL_TIME_GetUTCSeconds();

	/* record micro-seconds */
	oTimer->msec = getMSec();

	return;
}

/******************************************************************************/

DL_UINT32 DL_TIMER_GetDuration ( DL_TIMER iTimer )
{
	DL_UINT32 durMs    = 0;
	DL_TIMER  tmpTimer;

	/* start temp timer (to determine current time) */
	DL_TIMER_Start(&tmpTimer);

	/* calculate millisecond difference between current and original timer */
	durMs = ( ((tmpTimer.sec - iTimer.sec) * 1000) +
		      ((tmpTimer.msec - iTimer.msec) % 1000) )
			& DL_MAX_UINT32;

	return durMs;
}

/******************************************************************************/

static DL_UINT32 getMSec ( void )
{
	DL_UINT32 ret = 0;

#if defined(DL_WIN32)
	{
		ret = (DL_UINT32)(GetTickCount() % 1000);
	}
#elif defined(DL_UNIX)
	{
		struct timeval tv;
		struct timezone tz;

		gettimeofday(&tv, &tz);

		ret = (DL_UINT32)((tv.tv_usec / 1000) % 1000);
	}
#else
#error Platform Not Supported
#endif

	return ret;
}

/******************************************************************************/
