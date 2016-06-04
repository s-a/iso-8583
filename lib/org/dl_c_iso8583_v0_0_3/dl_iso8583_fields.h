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
/* ISO8583 field packers / unpackers                                          */
/*                                                                            */
/******************************************************************************/

#ifndef __INC_DL_ISO8583_FIELDS
#define __INC_DL_ISO8583_FIELDS

#include "dl_base.h"
#include "dl_err.h"
#include "dl_mem.h"

#include "dl_iso8583_common.h"

/******************************************************************************/

DL_ERR _DL_ISO8583_FIELD_Pack ( DL_UINT16                  iField,
								const DL_ISO8583_MSG      *iMsg,
								const DL_ISO8583_HANDLER  *iHandler,
								DL_UINT8                 **ioPtr );

DL_ERR _DL_ISO8583_FIELD_Unpack ( DL_UINT16                  iField,
								  DL_ISO8583_MSG            *ioMsg,
								  const DL_ISO8583_HANDLER  *iHandler,
								  DL_UINT8                 **ioPtr );

/******************************************************************************/

#endif /* __INC_DL_ISO8583_FIELDS */
