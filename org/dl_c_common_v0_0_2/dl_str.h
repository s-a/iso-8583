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
/* String manipulation module                                                 */
/*                                                                            */
/******************************************************************************/

#ifndef __INC_DL_STR
#define __INC_DL_STR

#include "dl_base.h"
#include "dl_err.h"
#include "dl_mem.h"

#include <string.h>

/******************************************************************************/

extern DL_CHAR kDL_STR_EmptyStr[1];

/******************************************************************************/
//
// MACROS
//

/* returns an empty string if the pointer is NULL */
#define DL_STR_SafeStr(str)\
 ((str)==NULL?kDL_STR_EmptyStr:(str))

/* accepts a pointer to a numeric - outputs safe values (ie 0 if NULL) */
#define DL_STR_SafeNum(a)\
 ((a)==NULL?0:*(a))

#define DL_STR_StrChr	strchr

/* returns 1 if character is whitespace / 0 otherwise */
/* NB whitespace: 0x09-0x0D or 0x20 */
#define DL_STR_IsWS(iCh)\
 ((((iCh)>=kDL_ASCII_HT)&&((iCh)<=kDL_ASCII_CR))||((iCh)==kDL_ASCII_SP) )

/******************************************************************************/

/* returns: static pointer to the request environment variable OR empty */
/*          string if not found / set                                   */
DL_CHAR *DL_STR_GetEnv ( const DL_CHAR *iEnvStr );

/******************************************************************************/

/* returns: length of string */
int DL_STR_StrLen ( const DL_CHAR *iStr );

/* returns length of string excluding whitespace characters */
int DL_STR_StrLenExWS ( const DL_CHAR *iStr );

/******************************************************************************/

/* compares two strings                                     */
/* returns: -1 if str1<str2, 0 if str1=str2, 1 if str1>str2 */
int DL_STR_StrCmp ( const DL_CHAR *iStr1,
			        const DL_CHAR *iStr2,
				    int            iIgnoreCase );

/* compares two string up to maximum length (iMaxChars)     */
/* returns: -1 if str1<str2, 0 if str1=str2, 1 if str1>str2 */
/* NB compares at most 'maxCompareChars'                    */
int DL_STR_StrNCmp ( const DL_CHAR *iStr1,
				     const DL_CHAR *iStr2,
				     int            iIgnoreCase,
				     int            iMaxChars );

/******************************************************************************/

/* copies a string (iFromPtr) to another string (ioToPtr) up to a maximum */
/* length (iMaxChars)                                                     */
/* returns: n/a                                                           */
/* NB if 'ioToPtr' is NULL then empty string is copied                    */
void DL_STR_StrCpy ( DL_CHAR       *ioToPtr,
				     const DL_CHAR *iFromPtr,
				     int            iMaxChars );

/* outputs an allocated copy of the string (caller must free) */
/* NB if 'iStr' is NULL then empty string is output           */
/* returns: error code                                        */
DL_ERR DL_STR_StrDup ( const DL_CHAR  *iStr,
					   DL_CHAR       **oStr );

/* as per 'DL_STR_StrDup' but limited to 'iMaxChars' */
/* NB if 'iStr' is NULL then empty string is output  */
/* returns: error code                               */
DL_ERR DL_STR_StrNDup ( const DL_CHAR  *iStr,
					    DL_UINT32       iMaxChars,
						DL_CHAR       **oStr );

/* concatenates the two specified strings and outputs an allocated string */
/* NB caller must free                                                    */
/* returns: error code                                                    */
DL_ERR DL_STR_StrCat ( const DL_CHAR  *iStr1,
					   const DL_CHAR  *iStr2,
					   DL_CHAR       **oStr );

/******************************************************************************/

/* trims any occurances of the specified character from the left of the */
/* string and shifts the string to the left                             */
void DL_STR_LTrim ( DL_CHAR *ioStr,
				    DL_CHAR  iTrimCh );

/* trims any occurances of the specified character from the right of the */
/* string                                                                */
void DL_STR_RTrim ( DL_CHAR *ioStr,
					DL_CHAR  iTrimCh );

/* converts all lowercase characters to uppercase */
void DL_STR_ToUpper ( DL_CHAR *ioStr );

/* moves past whitespace characters                           */
/* returns: pointer to character following whitespace or NULL */
DL_CHAR *DL_STR_SkipWS ( const DL_CHAR *iStr );

/******************************************************************************/

/* indicates if the specified string comprises only of digits */
/* NULL is treated as a non-numeric string                    */
/* returns: 1 if numeric / 0 otherwise                        */
int DL_STR_IsNumeric ( const DL_CHAR *iStr );

/* returns: 1 if string is valid / otherwise 0                     */
/* minLen: indicates minimum length of the string (-1 if no check) */
/* maxLen: indicates maximum length of the string (-1 if no check) */
/* validChars: string of valid characters (NULL if no check)       */
int DL_STR_Validate ( const DL_CHAR *iStr,
					  int            iMinLen,
				      int            iMaxLen,
					  const DL_CHAR *iValidChars );

/* returns: 1 if the string (iStr) contains any of the characters in */
/*          'iContains', 0 otherwise                                 */
int DL_STR_Contains ( const DL_CHAR *iStr,
					  const DL_CHAR *iContains );

/******************************************************************************/

/* encapsulates the input string (iStr) using the encapsulation character  */
/* (iEncapChar) - outputting an allocated string (oStr) - caller must free */
/* returns: error code                                                     */
DL_ERR DL_STR_EncapsulateStr ( const DL_CHAR  *iStr,
							   DL_CHAR         iEncapChar,
							   DL_CHAR       **oStr );

/* gets the (escaped) content of an encapsulated string                  */
/* outputs: oDataStr - content of encapsulated string (caller MUST free) */
/*          oNextPtr - pointer to next character OR 'iStr' if error      */
/* returns: error code                                                   */
DL_ERR DL_STR_GetEncapsulatedStr ( const DL_CHAR  *iStr,
								   DL_CHAR         iEncapChar,
								   DL_CHAR       **oDataStr,
								   DL_CHAR       **oNextPtr );

/******************************************************************************/

/* reads characters (from 'iStr') into the specified buffer (oBuffer) until   */
/* an invalid character is encountered (based on 'iValidChars') or the buffer */
/* if full (i.e. contains 'iBufferSize'-1 characters)                         */
/* returns: pointer to next character in 'iStr'                               */
DL_CHAR *DL_STR_ReadToBuffer ( const DL_CHAR *iStr,
						       const DL_CHAR *iValidChars,
						       int            iBufferSize,
						       DL_CHAR       *oBuffer );

/******************************************************************************/

#endif /* __INC_DL_STR */
