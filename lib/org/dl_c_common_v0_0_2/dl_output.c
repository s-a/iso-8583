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

#include "dl_output.h"

/******************************************************************************/
//
// CONSTANTS
//

#define kDL_OUTPUT_HEX_COLS		16

/******************************************************************************/

void DL_OUTPUT_Hex ( FILE           *iOutFile,
					 const char     *_iEolStr,
					 const DL_UINT8 *iPtr,
					 DL_UINT32       iNumBytes )
{
	DL_UINT32  rowIdx,
		       colIdx;
	char      *tmpEOL  = _iEolStr == NULL ? "\n" : _iEolStr;

	for ( rowIdx=0 ; rowIdx<(iNumBytes+kDL_OUTPUT_HEX_COLS-1)/kDL_OUTPUT_HEX_COLS ; rowIdx++ )
	{
		fprintf(iOutFile,"%08lxh ",rowIdx*kDL_OUTPUT_HEX_COLS);

		/* output hex characters */
		for ( colIdx=0 ; colIdx<kDL_OUTPUT_HEX_COLS ; colIdx++ )
		{
			DL_UINT32 offset = (rowIdx * kDL_OUTPUT_HEX_COLS) + colIdx;

			if ( offset >= iNumBytes )
				fprintf(iOutFile,"   ");
			else
				fprintf(iOutFile,"%02x ",(int)(iPtr[offset]));
		} /* end-for (colIdx) */

		/* output ascii characters (if printable) */
		for ( colIdx=0 ; colIdx<kDL_OUTPUT_HEX_COLS ; colIdx++ )
		{
			DL_UINT32 offset = (rowIdx * kDL_OUTPUT_HEX_COLS) + colIdx;

			if ( offset >= iNumBytes )
				fprintf(iOutFile," ");
			else if ( (iPtr[offset] >= 33) && (iPtr[offset] <= 126) )
				fprintf(iOutFile,"%c",(char)(iPtr[offset]));
			else
				fprintf(iOutFile,".");
		} /* end-for (colIdx) */

		fprintf(iOutFile,"%s",tmpEOL);
	} /* end-for (rowIdx) */

	return;
}

/******************************************************************************/
