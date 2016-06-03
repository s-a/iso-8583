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

#ifndef __INC_DL_MEM
#define __INC_DL_MEM

#include "dl_base.h"
#include "dl_err.h"

#include <string.h>

/******************************************************************************/
//
// ERROR CODES (1000-1019)
//

#define kDL_ERR_MEM_ALLOC	(DL_ERR)(1000+0)

/******************************************************************************/

#define DL_MEM_free(ptr)\
{ if ( (ptr) != NULL ) { free(ptr) ; (ptr) = NULL ; } }

/******************************************************************************/

#define DL_MEM_memset(ptr,value,numBytes)\
 ((void)memset((void*)(ptr),(int)(value),(size_t)(numBytes)))

#define DL_MEM_memcpy(toPtr,fromPtr,numBytes)\
 ((void)memcpy((void*)(toPtr),(void*)(fromPtr),(size_t)(numBytes)))

#define DL_MEM_memcmp(aPtr,bPtr,len)\
 (memcmp((void*)(aPtr),(void*)(bPtr),(size_t)(len)))

/******************************************************************************/

// allocates a chunk of memory
// returns: error code
DL_ERR DL_MEM_malloc ( DL_UINT32   iNumBytes,
					   void      **oPtr );

/* based on calloc - but does not indicate an error if 0 items requested
   NB also init's the array elements to 0 on success */
// returns: error code
DL_ERR DL_MEM_callocWithInit ( DL_UINT32   numItems,
							   size_t      itemSize,
							   void      **out );

/******************************************************************************/

#endif /* __INC_DL_MEM */
