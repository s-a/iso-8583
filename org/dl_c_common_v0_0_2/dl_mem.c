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

#include "dl_mem.h"

/******************************************************************************/

// allocates a chunk of memory
// returns: error code
DL_ERR DL_MEM_malloc ( DL_UINT32   iNumBytes,
					   void      **oPtr )
{
	DL_ERR err = 0;

	err = DL_MEM_callocWithInit(1,iNumBytes,oPtr);

	return err;
}

/* based on calloc - but does not indicate an error if 0 items requested
   NB also init's the array elements to 0 on success
   returns: 1 if ok / 0 otherwise */
DL_ERR DL_MEM_callocWithInit ( DL_UINT32   numItems,
							   size_t      itemSize,
							   void      **out )
{
	DL_ERR err = 0;

	/* init output params */
	*out = NULL;

	/* attempt to allocate memory - if numItems > 0 */
	if ( numItems > 0 )
	{
		/* allocate array - with error check */
		if ( (*out = (void*)calloc(numItems,itemSize)) == NULL )
		{
			err = kDL_ERR_MEM_ALLOC;
		}
		else /* init array elements (to 0) */
		{
			DL_MEM_memset(*out,0,numItems*itemSize);
		}
	}

	/* cleanup (on error) */
	if ( err )
	{
		DL_MEM_free(*out);
	}

	return err;
}

/******************************************************************************/
