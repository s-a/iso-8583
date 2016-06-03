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
/* dl_iso8583.c - An implementation of the ISO-8583 message protocol          */
/*                                                                            */
/******************************************************************************/

#ifndef __INC_DL_ISO8583
#define __INC_DL_ISO8583

#include "dl_base.h"
#include "dl_err.h"
#include "dl_str.h"

#include "dl_iso8583_common.h"
#include "dl_iso8583_fields.h"

/******************************************************************************/

// Initialises an ISO8583 message
// returns: none
void DL_ISO8583_MSG_Init ( DL_UINT8       *_iStaticBuf,
				           DL_UINT16       _iStaticBufSize,
				           DL_ISO8583_MSG *ioMsg );

// Frees a previously initialised ISO8583 message, de-allocating memory as req
// returns: none
void DL_ISO8583_MSG_Free ( DL_ISO8583_MSG *ioMsg );

/******************************************************************************/

// allocates and sets the specified (string) field
// NB iField range is 0..kDL_ISO8583_MAX_FIELD_IDX
// returns: error code
DL_ERR DL_ISO8583_MSG_SetField_Str ( DL_UINT16       iField,
					                 const DL_UINT8 *iDataStr,
						             DL_ISO8583_MSG *ioMsg );

// allocates and sets the specified field
// NB iField range is 0..kDL_ISO8583_MAX_FIELD_IDX
// NB iDataLen indicates the length of the data (0..n)
// NB iData can be NULL if iDataLen is 0
// returns: error code
DL_ERR DL_ISO8583_MSG_SetField_Bin ( DL_UINT16       iField,
					                 const DL_UINT8 *_iData,
					                 DL_UINT16       _iDataLen,
					                 DL_ISO8583_MSG *ioMsg );

/******************************************************************************/

// NB iField range is 0..kDL_ISO8583_MAX_FIELD_IDX
// returns: 1 if the field is set / 0 otherwise
int DL_ISO8583_MSG_HaveField ( DL_UINT16             iField,
					           const DL_ISO8583_MSG *iMsg );

/******************************************************************************/

// NB iField range is 0..kDL_ISO8583_MAX_FIELD_IDX
// outputs: oPtr - static pointer to field data
// returns: error code
DL_ERR DL_ISO8583_MSG_GetField_Str ( DL_UINT16              iField,
							         const DL_ISO8583_MSG  *iMsg,
									 DL_UINT8             **oPtr );

// NB iField range is 0..kDL_ISO8583_MAX_FIELD_IDX
// outputs: oPtr     - static pointer to field data
//			oByteLen - byte length of field data
// returns: error code
DL_ERR DL_ISO8583_MSG_GetField_Bin ( DL_UINT16              iField,
								     const DL_ISO8583_MSG  *iMsg,
									 DL_UINT8             **oPtr,
									 DL_UINT16             *oByteLen );

/******************************************************************************/

// NB doesn't check for overwrite of 'oByteArr'!
// returns: error code
DL_ERR DL_ISO8583_MSG_Pack ( const DL_ISO8583_HANDLER *iHandler,
					         const DL_ISO8583_MSG     *iMsg,
		                     DL_UINT8                 *ioByteArr,
			                 DL_UINT16                *oNumBytes );

// NB 'ioMsg' must be initialised (using 'DL_ISO_MSG_Init') before calling
// returns: error code
DL_ERR DL_ISO8583_MSG_Unpack ( const DL_ISO8583_HANDLER *iHandler,
					           const DL_UINT8           *iByteArr,
			                   DL_UINT16                 iByteArrSize,
			                   DL_ISO8583_MSG           *ioMsg );

/******************************************************************************/

// outputs the contents of the ISO8583 message to the specified file
// inputs : iOutFile - output stream
//          _iEolStr - EOL terminator string, defaults to '\n' if NULL
//          iHandler - ISO8583 handler instance
//          iMsg     - ISO8583 message
// returns: none
void DL_ISO8583_MSG_Dump ( FILE                     *iOutFile,
					       const char               *_iEolStr,
					       const DL_ISO8583_HANDLER *iHandler,
					       const DL_ISO8583_MSG     *iMsg );

/******************************************************************************/

#endif /* __INC_DL_ISO8583 */
