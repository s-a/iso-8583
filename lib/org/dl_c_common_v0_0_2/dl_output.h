/******************************************************************************/
/*                                                                            */
/* Copyright (C) 2007-2007 Oscar Sanderson                                    */
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
/* Output Functions                                                           */
/*                                                                            */
/******************************************************************************/

#ifndef __INC_DL_OUTPUT
#define __INC_DL_OUTPUT

#include "dl_base.h"

/******************************************************************************/

/* outputs the hex dump of the specified data (iPtr/iNumBytes) to the */
/* specified stream (iOutFile)                                        */
/* NB '_iEolStr' defaults to '\n' if NULL                             */
/* returns: n/a                                                       */
void DL_OUTPUT_Hex ( FILE           *iOutFile,
					 const char     *_iEolStr,
					 const DL_UINT8 *iPtr,
					 DL_UINT32       iNumBytes );

/******************************************************************************/

#endif /* __INC_DL_OUTPUT */
