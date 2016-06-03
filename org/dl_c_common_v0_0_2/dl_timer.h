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

#ifndef __INC_DL_TIMER
#define __INC_DL_TIMER

#include "dl_base.h"
#include "dl_mem.h"
#include "dl_time.h" // for second level date/time

#ifdef DL_WIN32
#include <windows.h>
#include <time.h>
#endif

#ifdef DL_UNIX
#include <sys/types.h>
#include <sys/time.h>
#endif

/******************************************************************************/
//
// TYPES
//

struct DL_TIMER_S
{
	DL_UINT32 sec;
	DL_UINT32 msec;
};
typedef struct DL_TIMER_S DL_TIMER;

/******************************************************************************/

// Starts/Initialises the specified timer instance
void DL_TIMER_Start ( DL_TIMER *oTimer );

/******************************************************************************/

// Returns the duration of the specified timer (in Ms)
// NB can be called multiple times, to obtain durations at different points
//    for the same timer instance
DL_UINT32 DL_TIMER_GetDuration ( DL_TIMER iTimer );

/******************************************************************************/

#endif /* __INC_DL_TIMER */
