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

#include "dl_iso8583_fields.h"

/******************************************************************************/
//
// FIELD HANDLER PROTOTYPES
//

DL_ERR _pack_iso_ASCHEX ( DL_UINT16                    iField,
						  const DL_ISO8583_MSG        *iMsg,
						  const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						  DL_UINT8                   **ioPtr );

// unpacks ISO Numeric (bcd format)
// NB if iSize is odd then we have a padding char on left
//    (but don't include when unpacking)
DL_ERR _unpack_iso_ASCHEX ( DL_UINT16                    iField,
						    DL_ISO8583_MSG              *ioMsg,
						    const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						    DL_UINT8                   **ioPtr );

DL_ERR _pack_iso_ASCII ( DL_UINT16                    iField,
						 const DL_ISO8583_MSG        *iMsg,
						 const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						 DL_UINT8                   **ioPtr );

DL_ERR _unpack_iso_ASCII ( DL_UINT16                    iField,
						   DL_ISO8583_MSG              *ioMsg,
						   const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						   DL_UINT8                   **ioPtr );

DL_ERR _pack_iso_BINARY ( DL_UINT16                    iField,
						  const DL_ISO8583_MSG        *iMsg,
						  const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						  DL_UINT8                   **ioPtr );

DL_ERR _unpack_iso_BINARY ( DL_UINT16                    iField,
						    DL_ISO8583_MSG              *ioMsg,
						    const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						    DL_UINT8                   **ioPtr );

DL_ERR _pack_iso_BITMAP ( DL_UINT16                    iField,
						  const DL_ISO8583_MSG        *iMsg,
						  const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						  DL_UINT8                   **ioPtr );

DL_ERR _unpack_iso_BITMAP ( DL_UINT16                    iField,
						    DL_ISO8583_MSG              *ioMsg,
						    const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						    DL_UINT8                   **ioPtr );

/******************************************************************************/
//
// LENGTH HANDLER PROTOTYPES
//

// outputs the variable length element
// iVarLenType - e.g. kDL_ISO8583_LLVAR
static DL_ERR VarLen_Put ( DL_UINT8    iVarLenType,
						   DL_UINT32   iActLen,
						   DL_UINT32  *ioReqLen,
						   DL_UINT8  **ioPtr );

// determines variable length element
static DL_ERR VarLen_Get ( const DL_UINT8 **ioPtr,
			               DL_UINT8         iVarLenDigits,
				           DL_UINT16        iMaxValue,
				           DL_UINT16       *oLen );

/******************************************************************************/
//
// TYPES
//

struct DL_ISO8583_TYPE_S
{
	DL_ERR    (*unpackFunc)(DL_UINT16,DL_ISO8583_MSG*,const DL_ISO8583_FIELD_DEF*,DL_UINT8**);
	DL_ERR    (*packFunc  )(DL_UINT16,const DL_ISO8583_MSG*,const DL_ISO8583_FIELD_DEF*,DL_UINT8**);
};
typedef struct DL_ISO8583_TYPE_S DL_ISO8583_TYPE;

/******************************************************************************/
//
// VARIABLES
//

static DL_ISO8583_TYPE fieldTypeArr[] = {
/* ISO_N    */ {_unpack_iso_ASCHEX,_pack_iso_ASCHEX},
/* ISO_NS   */ {_unpack_iso_BINARY,_pack_iso_BINARY},
/* ISO_XN   */ {_unpack_iso_ASCHEX,_pack_iso_ASCHEX},
/* ISO_A    */ {_unpack_iso_ASCII ,_pack_iso_ASCII },
/* ISO_AN   */ {_unpack_iso_ASCII ,_pack_iso_ASCII },
/* ISO_ANS  */ {_unpack_iso_ASCII ,_pack_iso_ASCII },
/* ISO_ANSB */ {_unpack_iso_ASCII ,_pack_iso_ASCII },
/* ISO_ANP  */ {_unpack_iso_ASCII ,_pack_iso_ASCII },
/* ISO_B    */ {_unpack_iso_BINARY,_pack_iso_BINARY},
/* ISO_Z    */ {_unpack_iso_BINARY,_pack_iso_BINARY},
/* ISO_BMAP */ {_unpack_iso_BITMAP,_pack_iso_BITMAP} };

/******************************************************************************/
//
// MACROS
//

// gets the field type details
#define GetFieldType(fieldType)\
 (&fieldTypeArr[fieldType])

/******************************************************************************/

DL_ERR _DL_ISO8583_FIELD_Pack ( DL_UINT16                  iField,
								const DL_ISO8583_MSG      *iMsg,
								const DL_ISO8583_HANDLER  *iHandler,
								DL_UINT8                 **ioPtr )
{
	DL_ERR                err          = kDL_ERR_NONE;
	DL_ISO8583_FIELD_DEF *fieldDefPtr  = DL_ISO8583_GetFieldDef(iField,iHandler);
	DL_ISO8583_TYPE      *fieldTypePtr = GetFieldType(fieldDefPtr->fieldType);

	err = fieldTypePtr->packFunc(iField,iMsg,fieldDefPtr,ioPtr);

	return err;
}

/******************************************************************************/

DL_ERR _DL_ISO8583_FIELD_Unpack ( DL_UINT16                  iField,
								  DL_ISO8583_MSG            *ioMsg,
								  const DL_ISO8583_HANDLER  *iHandler,
								  DL_UINT8                 **ioPtr )
{
	DL_ERR                err          = kDL_ERR_NONE;
	DL_ISO8583_FIELD_DEF *fieldDefPtr  = DL_ISO8583_GetFieldDef(iField,iHandler);
	DL_ISO8583_TYPE      *fieldTypePtr = GetFieldType(fieldDefPtr->fieldType);

	err = fieldTypePtr->unpackFunc(iField,ioMsg,fieldDefPtr,ioPtr);

	return err;
}

/******************************************************************************/

DL_ERR _pack_iso_ASCHEX ( DL_UINT16                    iField,
						  const DL_ISO8583_MSG        *iMsg,
						  const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						  DL_UINT8                   **ioPtr )
{
	DL_ERR                err           = kDL_ERR_NONE;
	DL_UINT8             *tmpPtr        = *ioPtr;
	DL_ISO8583_MSG_FIELD *fieldPtr      = ((DL_ISO8583_MSG*)iMsg)->field + iField;
	DL_UINT32             actLen        = fieldPtr->len;
	DL_UINT8             *dataPtr       = fieldPtr->ptr;
	DL_UINT32             reqLen        = iFieldDefPtr->len;
	DL_UINT32             wholeActBytes = 0;
	DL_UINT32             wholeReqBytes = 0;
	DL_UINT32             i;

	/* variable length handling */
	err = VarLen_Put(iFieldDefPtr->varLen,actLen,&reqLen,&tmpPtr);

	if ( !err )
	{
		if ( actLen > reqLen ) /* too long */
		{
			err = kDL_ERR_OTHER;
		}
		else
		{
			/* determine numbers of bytes for required / actual lengths      */
			/* NB 'required bytes' are rounded up, 'actual' are rounded down */
			wholeActBytes = actLen / 2;
			wholeReqBytes = (reqLen + 1) / 2;

			/* output left padding (00h) bytes - where required */
			/* NB less one if the actual length has an odd number of digits */
			i = wholeReqBytes - wholeActBytes;
			if ( actLen % 2 )
				i--;
			DL_MEM_memset(tmpPtr,0,i);
			tmpPtr += i;

			/* handle partial digit - if required */
			if ( actLen % 2 ) /* have partial digit */
			{
				*tmpPtr++ = (DL_UINT8)DL_ASCHEX_2_NIBBLE(dataPtr[0]);
				dataPtr++;
			}

			/* handle complete digit pairs */
			for ( i=0 ; i<wholeActBytes ; i++,dataPtr+=2 )
			{
				*tmpPtr++ = (DL_UINT8)((DL_ASCHEX_2_NIBBLE(dataPtr[0]) << 4) |
										DL_ASCHEX_2_NIBBLE(dataPtr[1])         );
			} /* end-for */
		}
	}

	*ioPtr = tmpPtr;

	return err;
}

/******************************************************************************/

// unpacks ISO Numeric (bcd format)
// NB if iSize is odd then we have a padding char on left
//    (but don't include when unpacking)
// NB doesn't remove any leading padding (0 nibbles)
DL_ERR _unpack_iso_ASCHEX ( DL_UINT16                    iField,
						    DL_ISO8583_MSG              *ioMsg,
						    const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						    DL_UINT8                   **ioPtr )
{
	DL_ERR     err        = kDL_ERR_NONE;
	DL_UINT8  *tmpPtr     = *ioPtr;
	DL_UINT16  size       = 0;
	DL_UINT8  *tmpDataPtr = NULL;

	/* variable length handling */
	err = VarLen_Get(&tmpPtr,iFieldDefPtr->varLen,iFieldDefPtr->len,&size);

	/* allocate field */
	if ( !err )
		err = _DL_ISO8583_MSG_AllocField(iField,size,ioMsg,&tmpDataPtr);

	if ( !err )
	{
		DL_UINT8 ch;

		/* if size is 'odd' then ignore the leading nibble, as this is a pad character */
		if ( size % 2 ) /* odd */
		{
			ch = *tmpPtr & 0x0f;
			*tmpDataPtr++ = DL_NIBBLE_2_ASCHEX(ch);
			tmpPtr++;
			size -= 1;
		}

		size /= 2;
		while ( size-- > 0 )
		{
			ch = (*tmpPtr >> 4) & 0xf;
			*tmpDataPtr++ = DL_NIBBLE_2_ASCHEX(ch);
			ch = *tmpPtr & 0xf;
			*tmpDataPtr++ = DL_NIBBLE_2_ASCHEX(ch);
			tmpPtr++;
		}
		
		*tmpDataPtr = kDL_ASCII_NULL; /* null terminate */
	}

	*ioPtr = tmpPtr;

	return err;
}

/******************************************************************************/

DL_ERR _pack_iso_ASCII ( DL_UINT16                    iField,
						 const DL_ISO8583_MSG        *iMsg,
						 const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						 DL_UINT8                   **ioPtr )
{
	DL_ERR                err      = kDL_ERR_NONE;
	DL_UINT8             *tmpPtr   = *ioPtr;
	DL_ISO8583_MSG_FIELD *fieldPtr = ((DL_ISO8583_MSG*)iMsg)->field + iField;
	DL_UINT32             actLen   = fieldPtr->len;
	DL_UINT8             *dataPtr  = fieldPtr->ptr;
	DL_UINT32             reqLen   = iFieldDefPtr->len;

	/* variable length handling */
	err = VarLen_Put(iFieldDefPtr->varLen,actLen,&reqLen,&tmpPtr);

	if ( !err )
	{
		if ( actLen > reqLen ) /* too long */
		{
			err = kDL_ERR_OTHER;
		}
		else if ( actLen == reqLen ) /* exact size */
		{
			/* copy up to 'required' amount */
			DL_MEM_memcpy(tmpPtr,dataPtr,reqLen);
			tmpPtr += reqLen;
		}
		else /* shorter - so need to right pad (space) */
		{
			/* copy what data we have (actual length) */
			DL_MEM_memcpy(tmpPtr,dataPtr,actLen);
			/* right pad as required */
			DL_MEM_memset(tmpPtr+actLen,(int)kDL_ASCII_SP,reqLen-actLen);
			tmpPtr += reqLen;
		}
	}

	*ioPtr = tmpPtr;

	return err;
}

/******************************************************************************/

// NB doesn't remove any trailing padding (spaces)
DL_ERR _unpack_iso_ASCII ( DL_UINT16                    iField,
						   DL_ISO8583_MSG              *ioMsg,
						   const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						   DL_UINT8                   **ioPtr )
{
	DL_ERR     err        = kDL_ERR_NONE;
	DL_UINT8  *tmpPtr     = *ioPtr;
	DL_UINT16  size       = 0;
	DL_UINT8  *tmpDataPtr = NULL;

	/* variable length handling */
	err = VarLen_Get(&tmpPtr,iFieldDefPtr->varLen,iFieldDefPtr->len,&size);

	/* allocate field */
	if ( !err )
		err = _DL_ISO8583_MSG_AllocField(iField,size,ioMsg,&tmpDataPtr);

	if ( !err )
	{
		DL_MEM_memcpy(tmpDataPtr,tmpPtr,size); 
		tmpPtr     += size;
		tmpDataPtr += size;

		*tmpDataPtr = kDL_ASCII_NULL; /* null terminate */
	}

	*ioPtr = tmpPtr;

	return err;
}

/******************************************************************************/

DL_ERR _pack_iso_BINARY ( DL_UINT16                    iField,
						  const DL_ISO8583_MSG        *iMsg,
						  const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						  DL_UINT8                   **ioPtr )
{
	DL_ERR                err      = kDL_ERR_NONE;
	DL_UINT8             *tmpPtr   = *ioPtr;
	DL_ISO8583_MSG_FIELD *fieldPtr = ((DL_ISO8583_MSG*)iMsg)->field + iField;
	DL_UINT32             actLen   = fieldPtr->len;
	DL_UINT8             *dataPtr  = fieldPtr->ptr;
	DL_UINT32             reqLen   = iFieldDefPtr->len;

	/* variable length handling */
	err = VarLen_Put(iFieldDefPtr->varLen,actLen,&reqLen,&tmpPtr);

	if ( !err )
	{
		if ( actLen > reqLen ) /* too long */
		{
			err = kDL_ERR_OTHER;
		}
		else if ( actLen == reqLen ) /* exact size */
		{
			/* copy up to 'required' amount */
			DL_MEM_memcpy(tmpPtr,dataPtr,reqLen);
			tmpPtr += reqLen;
		}
		else /* shorter - so need to right pad (space) */
		{
			/* copy what data we have (actual length) */
			DL_MEM_memcpy(tmpPtr,dataPtr,actLen);
			/* right pad as required */
			DL_MEM_memset(tmpPtr+actLen,(int)0,reqLen-actLen);
			tmpPtr += reqLen;
		}
	}

	*ioPtr = tmpPtr;

	return err;
}

/******************************************************************************/

// NB doesn't remove any trailing padding (0's)
DL_ERR _unpack_iso_BINARY ( DL_UINT16                    iField,
						    DL_ISO8583_MSG              *ioMsg,
						    const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						    DL_UINT8                   **ioPtr )
{
	DL_ERR     err        = kDL_ERR_NONE;
	DL_UINT8  *tmpPtr     = *ioPtr;
	DL_UINT16  size       = 0;
	DL_UINT8  *tmpDataPtr = NULL;

	/* variable length handling */
	err = VarLen_Get(&tmpPtr,iFieldDefPtr->varLen,iFieldDefPtr->len,&size);

	/* allocate field */
	if ( !err )
		err = _DL_ISO8583_MSG_AllocField(iField,size,ioMsg,&tmpDataPtr);

	if ( !err )
	{
		DL_MEM_memcpy(tmpDataPtr,tmpPtr,size); 
		tmpPtr     += size;
		tmpDataPtr += size;

		*tmpDataPtr = kDL_ASCII_NULL; /* null terminate */
	}

	*ioPtr = tmpPtr;

	return err;
}

/******************************************************************************/

DL_ERR _pack_iso_BITMAP ( DL_UINT16                    iField,
						  const DL_ISO8583_MSG        *iMsg,
						  const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
						  DL_UINT8                   **ioPtr )
{
	DL_ERR     err         = kDL_ERR_NONE;
	DL_UINT8  *tmpPtr      = *ioPtr;
	DL_UINT16  curFieldIdx = iField;
	int        i;

	/* for each possible bitmap segment */
	for ( i=0 ; i<((kDL_ISO8583_MAX_FIELD_IDX-iField+1)+63)/64 ; i++ )
	{
		DL_UINT32 ms=0,
				  ls=0;
		int       j;

		/* move to next field */
		curFieldIdx++;

		for ( j=0 ; j<31 ; j++,curFieldIdx++ )
		{
			ms <<= 1;
			if ( (curFieldIdx <= kDL_ISO8583_MAX_FIELD_IDX) &&
				 (NULL != iMsg->field[curFieldIdx].ptr) )
				ms++;
		}

		for ( j=0 ; j<32 ; j++,curFieldIdx++ )
		{
			ls <<= 1;
			if ( (curFieldIdx <= kDL_ISO8583_MAX_FIELD_IDX) &&
				 (NULL != iMsg->field[curFieldIdx].ptr) )
				ls++;
		}

		/* output bitmap segment (if required) */
		if ( 0 == i )
		{
			/* NB 1st segment is always output */
			DL_UINT32_TO_BYTES(ms,tmpPtr);
			DL_UINT32_TO_BYTES(ls,tmpPtr+4);
			tmpPtr += 8;
		}
		else
		{
			if ( ms || ls )
			{
				/* set continuation bit of previous segment */
				*(tmpPtr-8) |= 0x80;

				DL_UINT32_TO_BYTES(ms,tmpPtr);
				DL_UINT32_TO_BYTES(ls,tmpPtr+4);
				tmpPtr += 8;
			}
			else
			{
				/* no fields present, so don't output */
				break;
			}
		}
	} /* end-for(i) */

	*ioPtr = tmpPtr;

	return err;
}

/******************************************************************************/

DL_ERR _unpack_iso_BITMAP ( DL_UINT16                    iField,
					        DL_ISO8583_MSG              *ioMsg,
					        const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
					        DL_UINT8                   **ioPtr )
{
	DL_ERR     err    = kDL_ERR_NONE;
	DL_UINT8  *tmpPtr = *ioPtr;

	{
		DL_UINT16 curFieldIdx = iField;

		/* for each bitmap segment (8 bytes) */
		do
		{
			DL_UINT32 ms,ls;
			int       j;

			/* get bitmap segment (8 bytes) */
			ms = DL_BYTES_TO_UINT32(tmpPtr);
			ls = DL_BYTES_TO_UINT32(tmpPtr+4);
			tmpPtr += 8;

			/* move to next field */
			curFieldIdx++;

			/* ms part */
			for ( j=30 ; j>=0 ; j--,curFieldIdx++ )
			{
				if ( DL_BIT_TEST(ms,j) )
				{
					if ( curFieldIdx > kDL_ISO8583_MAX_FIELD_IDX )
						return kDL_ERR_OTHER;

					/* set length to non-zero value to indicate field presence */
					ioMsg->field[curFieldIdx].len = 1;
				}
			} /* end-for(j) */

			/* ls part */
			for ( j=31 ; j>=0 ; j--,curFieldIdx++ )
			{
				if ( DL_BIT_TEST(ls,j) )
				{
					if ( curFieldIdx > kDL_ISO8583_MAX_FIELD_IDX )
						return kDL_ERR_OTHER;

					/* set length to non-zero value to indicate field presence */
					ioMsg->field[curFieldIdx].len = 1;
				}
			} /* end-for(j) */

			/* stop if no more bitmap segments */
			if ( 0 == DL_BIT_TEST(ms,31) )
				break;
		} while ( !err );
	}

	*ioPtr = tmpPtr;

	return err;
}

/******************************************************************************/

// returns the bcd encoded value - based on decValue (0..99)
#define output_bcd_byte(decValue)\
 ((DL_UINT8)((((decValue)/10)<<4)|((decValue)%10)))

// outputs the variable length element
// iVarLenType - e.g. kDL_ISO8583_LLVAR
static DL_ERR VarLen_Put ( DL_UINT8    iVarLenType,
						   DL_UINT32   iActLen,
						   DL_UINT32  *ioReqLen,
						   DL_UINT8  **ioPtr )
{
	DL_ERR    err    = kDL_ERR_NONE;
	DL_UINT8 *tmpPtr = *ioPtr;

	switch ( iVarLenType )
	{
		case kDL_ISO8583_FIXED:
			/* do nothing */
			break;
		case kDL_ISO8583_LLVAR:
			iActLen   %= 100;
			*ioReqLen  = iActLen;
			*tmpPtr++    = output_bcd_byte(iActLen);
			break;
		case kDL_ISO8583_LLLVAR:
			iActLen   %= 1000;
			*ioReqLen  = iActLen;
			*tmpPtr++    = output_bcd_byte(iActLen/100);
			*tmpPtr++    = output_bcd_byte(iActLen%100);
			break;
		case kDL_ISO8583_LLLLVAR:
			iActLen   %= 10000;
			*ioReqLen  = iActLen;
			*tmpPtr++    = output_bcd_byte(iActLen/100);
			*tmpPtr++    = output_bcd_byte(iActLen%100);
			break;
		default:
			/* [ERROR] unsupported length type */
			err = kDL_ERR_OTHER;
	} /* end-switch */

	*ioPtr = tmpPtr;

	return err;
}

/******************************************************************************/

// determines variable length element
static DL_ERR VarLen_Get ( const DL_UINT8 **ioPtr,
			               DL_UINT8         iVarLenDigits,
				           DL_UINT16        iMaxValue,
				           DL_UINT16       *oLen )
{
	DL_ERR    err    = kDL_ERR_NONE;
	DL_UINT8 *tmpPtr = (DL_UINT8*)*ioPtr;

	/* init outputs */
	*oLen = iMaxValue;

	if ( kDL_ISO8583_FIXED != iVarLenDigits )
	{
		*oLen = 0;

		if ( iVarLenDigits % 2 )
			iVarLenDigits++;

		while ( iVarLenDigits > 0 )
		{
			*oLen = (*oLen * 100) +
					((((int)(*tmpPtr) >> 4) & 0xf) * 10) +
					((int)(*tmpPtr) & 0xf);
			iVarLenDigits -= 2;
			tmpPtr++;
		} /* end-while */

		/* limit if exceeds max */
		*oLen = MIN(iMaxValue,*oLen);
	}

	*ioPtr = tmpPtr;

	return err;
}

/******************************************************************************/
