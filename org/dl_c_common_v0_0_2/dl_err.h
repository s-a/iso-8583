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
/* Error code definitions                                                     */
/*                                                                            */
/******************************************************************************/

#ifndef __INC_DL_ERR
#define __INC_DL_ERR

#include "dl_base.h"

/******************************************************************************/
//
// TYPES
//

typedef DL_UINT32	DL_ERR;

/******************************************************************************/
//
// GENERIC ERROR CODES
//

#define kDL_ERR_NONE					(DL_ERR)0

#define kDL_ERR_OTHER					(DL_ERR)1
#define kDL_ERR_NOT_IMPLEMENTED			(DL_ERR)2

/******************************************************************************/
//
// ALLOCATED ERROR CODES
//

// 0000-0019	<RESERVED>
// 0020-0999
// 1000-1019	dl_mem
// 1020-1039	dl_cgi
// 1040-1059	dl_hash
// 1060-1079	dl_sess
// 1080-1099	<RFU>
// 1100-1119	dl_file
// 1120-1139	dl_dblib
// 1140-1159	dl_socket
// 1160-1179	dl_smtp
// 1180-2499	<RFU>
// 2500-2519	dl_avl
// 2520-2999	<RFU>
// 3000-3019	dl_threadpool
// 3020-3999	<RFU>
// 4000-4019	dl_staticQ

/******************************************************************************/

#endif /* __INC_DL_ERR */
