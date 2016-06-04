/******************************************************************************/
/*                                                                            */
/* Copyright (C) 2001-2007 Oscar Sanderson                                    */
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
/* Base Types / Constants                                                     */
/*                                                                            */
/******************************************************************************/

#ifndef __INC_DL_BASE
#define __INC_DL_BASE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

/******************************************************************************/
//
// OPERATING SYSTEM / ENVIRONMENT SETTINGS
//

// DL_WIN32	(Target = Windows 32)
// DL_UNIX	(Target = UNIX)

#ifdef WIN32
/* Win32 */
#define DL_WIN32
#else
/* Unix (assumed) */
#define DL_UNIX
#endif

#if 0
#ifdef DL_WIN32
/* Include Windows header file */
#include <windows.h>
#endif /* DL_WIN32 */
#endif

// Debug
#ifdef DEBUG
#define DL_DEBUG
#endif

/******************************************************************************/
//
// TYPES
//

#ifdef DL_WIN32
typedef unsigned char   DL_UINT8;
typedef char            DL_SINT8;
typedef unsigned short	DL_UINT16;
typedef short			DL_SINT16;
typedef unsigned int	DL_UINT32;
typedef int				DL_SINT32;
#elif defined DL_UNIX
typedef unsigned char   DL_UINT8;
typedef char            DL_SINT8;
typedef unsigned int	DL_UINT16;
typedef int 			DL_SINT16;
typedef unsigned long	DL_UINT32;
typedef long			DL_SINT32;
#else
// Unsupported platform
#error Platform Not Supported
#endif

// NB not used by all library code!
typedef DL_UINT8 DL_CHAR;
typedef DL_UINT32 DL_UCHAR; // unicode character

/******************************************************************************/
//
// CONSTANTS (INTEGER TYPES)
//

#define DL_SIZE_OF_UINT8	3	/* 0..255    */
#define DL_SIZE_OF_SINT8	4	/* -127..128 */
#define DL_SIZE_OF_UINT16	5	/* 0-65535   */
#define DL_SIZE_OF_SINT16	6

#define DL_SIZE_OF_UINT24	8

#define DL_SIZE_OF_UINT32	10
#define DL_SIZE_OF_SINT32	11

#define DL_MAX_UINT8		0xff
#define DL_MAX_UINT16		0xffff
#define DL_MAX_UINT24		0xffffffL
#define DL_MAX_UINT32		0xffffffffL

/******************************************************************************/
//
// CONSTANTS (ASCII CHARACTERS)
//

#define kDL_ASCII_NULL			0x00
#define kDL_ASCII_HT			0x09
#define kDL_ASCII_LF			0x0A
#define kDL_ASCII_CR			0x0D
#define kDL_ASCII_SP			0x20

#define kDL_ASCII_QUOTE			0x22
#define kDL_ASCII_APOSTROPHE	0x27
#define kDL_ASCII_FORWARDSLASH	0x2F
#define kDL_ASCII_BACKSLASH		0x5C

/******************************************************************************/
//
// MACROS (BIT MANIPULATION)
//

#define DL_BIT_TEST(value,bit)\
 ((((DL_UINT32)(value)) & ((DL_UINT32)1 << (bit))) ? 1 : 0)

#define DL_BIT_SET(value,bit)\
 (((DL_UINT32)(value)) |= ((DL_UINT32)1 << (bit)))

#define DL_BIT_CLEAR(value,bit)\
 (((DL_UINT32)(value)) &= ~((DL_UINT32)1 << (bit)))

/******************************************************************************/
//
// MACROS (BYTE-ARR -> INTEGER)
//

// converts 2 bytes to UINT16 - assumes logical ordering of the integer
#define DL_BYTES_TO_UINT16(byteArr)\
 ((((DL_UINT16)((byteArr)[0]) & DL_MAX_UINT8) << 8) +\
   ((DL_UINT16)((byteArr)[1]) & DL_MAX_UINT8))

// converts 3 bytes to UINT24 (ie 32) - assumes logical ordering of the integer
#define DL_BYTES_TO_UINT24(byteArr)\
 ((((DL_UINT32)((byteArr)[0]) & DL_MAX_UINT8) << 16) +\
  (((DL_UINT32)((byteArr)[1]) & DL_MAX_UINT8) <<  8) +\
   ((DL_UINT32)((byteArr)[2]) & DL_MAX_UINT8))

// converts 4 bytes to UINT32 - assumes logical ordering of the integer
#define DL_BYTES_TO_UINT32(byteArr)\
 ((((DL_UINT32)((byteArr)[0]) & DL_MAX_UINT8) << 24) +\
  (((DL_UINT32)((byteArr)[1]) & DL_MAX_UINT8) << 16) +\
  (((DL_UINT32)((byteArr)[2]) & DL_MAX_UINT8) <<  8) +\
   ((DL_UINT32)((byteArr)[3]) & DL_MAX_UINT8))

/******************************************************************************/
//
// MACROS (INTEGER -> BYTE-ARR)
//

// writes the UINT16 to the specified memory address (logical ordering)
// NB this macro DOES NOT advance the pointer
#define DL_UINT16_TO_BYTES(iVal,oArr)\
{ (oArr)[0]=((iVal) >> 8) & DL_MAX_UINT8;\
  (oArr)[1]= (iVal)       & DL_MAX_UINT8; }

// writes the UINT24 to the specified memory address (logical ordering)
// NB this macro DOES NOT advance the pointer
#define DL_UINT24_TO_BYTES(iVal,oArr)\
 {(oArr)[0]=((iVal) >> 16) & DL_MAX_UINT8;\
  (oArr)[1]=((iVal) >>  8) & DL_MAX_UINT8;\
  (oArr)[2]= (iVal)        & DL_MAX_UINT8;};

// writes the UINT32 to the specified memory address (logical ordering)
// NB this macro DOES NOT advance the pointer
#define DL_UINT32_TO_BYTES(iVal,oArr)\
 {(oArr)[0]=((iVal) >> 24) & DL_MAX_UINT8;\
  (oArr)[1]=((iVal) >> 16) & DL_MAX_UINT8;\
  (oArr)[2]=((iVal) >>  8) & DL_MAX_UINT8;\
  (oArr)[3]= (iVal)        & DL_MAX_UINT8;};

/******************************************************************************/
//
// MACROS (ASCII-HEX <--> NIBBLE)
//

#define DL_ASCHEX_2_NIBBLE(aschex)\
 ( ((aschex)>='a') ? (((aschex)-'a')%6)+10 : (((aschex)>='A') ? (((aschex)-'A')%6)+10 : ((aschex)-'0')%10) )

#define DL_NIBBLE_2_ASCHEX(nibble)\
 ( ((nibble)>9) ? (((nibble)-10)%6)+'A' : ((nibble)+'0') )

/******************************************************************************/
//
// MACROS (INTEGER COMPARISON)
//

#define MIN(x,y) ((x)<(y)?(x):(y))

#define MAX(x,y) ((x)>(y)?(x):(y))

/******************************************************************************/
//
// MACROS (INTEGER ADDITION)
//

// adds the amount indicated by 'addition' to 'initial' unless it would exceed
// the value indicated by 'limit', in which case the returned amount is limited
#define DL_UINT32_AddWithLimit(initial,addition,limit)\
 ((((DL_UINT32)(initial)>=(DL_UINT32)(limit))||\
   ((DL_UINT32)(addition)>=(DL_UINT32)(limit))||\
   (((DL_UINT32)(limit)-(DL_UINT32)(initial))<(DL_UINT32)(addition)))\
   ? (DL_UINT32)limit : (DL_UINT32)(initial)+(DL_UINT32)(addition))

/******************************************************************************/

#endif /* __INC_DL_BASE */
