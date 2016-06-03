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
/* dl_iso8583_defs_1987.c - ISO8583 1987 Definitions                          */
/*                                                                            */
/******************************************************************************/

#include "dl_iso8583_defs_1987.h"

/******************************************************************************/

static DL_ISO8583_FIELD_DEF _iso8583_1987_fields[] = {
/*   0 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Message Type Indicator
/*   1 */ {kDL_ISO8583_BMP, 16,kDL_ISO8583_CONTVAR}, // Bitmap
/*   2 */ {kDL_ISO8583_N  , 19,kDL_ISO8583_LLVAR  }, // Primary Account Number
/*   3 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Processing Code
/*   4 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Amount, Txn
/*   5 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Amount, Settlement
/*   6 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Amount, Cardholder Billing
/*   7 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Date and Time, Transmission
/*   8 */ {kDL_ISO8583_N  ,  8,kDL_ISO8583_FIXED  }, // Amount, Cardholder Billing Fee
/*   9 */ {kDL_ISO8583_N  ,  8,kDL_ISO8583_FIXED  }, // Conversion Rate, Settlement
/*  10 */ {kDL_ISO8583_N  ,  8,kDL_ISO8583_FIXED  }, // Conversion Rate, Cardholder Billing
/*  11 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Systems Trace Audit Number
/*  12 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Time, Local Txn
/*  13 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Date, Local Txn
/*  14 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Date, Expiration
/*  15 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Date, Settlement
/*  16 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Date, Conversion
/*  17 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Date, Capture
/*  18 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Merchant Type
/*  19 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Acquiring Inst
/*  20 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Primary Account Number
/*  21 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Forwarding Inst
/*  22 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Point of Service Entry Mode
/*  23 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Application PAN number
/*  24 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Network International Identifier
/*  25 */ {kDL_ISO8583_N  ,  2,kDL_ISO8583_FIXED  }, // Point of Service Condition Code
/*  26 */ {kDL_ISO8583_N  ,  2,kDL_ISO8583_FIXED  }, // Point of Service PIN Capture Code
/*  27 */ {kDL_ISO8583_N  ,  1,kDL_ISO8583_FIXED  }, // Authorization Identification Response Length
/*  28 */ {kDL_ISO8583_XN ,  9,kDL_ISO8583_FIXED  }, // Amount, Txn Fee
/*  29 */ {kDL_ISO8583_XN ,  9,kDL_ISO8583_FIXED  }, // Amount, Settlement Fee
/*  30 */ {kDL_ISO8583_XN ,  9,kDL_ISO8583_FIXED  }, // Amount, Txn Processing Fee
/*  31 */ {kDL_ISO8583_XN ,  9,kDL_ISO8583_FIXED  }, // Amount, Settlement Processing Fee
/*  32 */ {kDL_ISO8583_N  , 11,kDL_ISO8583_LLVAR  }, // Acquirer Inst Id Code
/*  33 */ {kDL_ISO8583_N  , 11,kDL_ISO8583_LLVAR  }, // Forwarding Inst Id Code
/*  34 */ {kDL_ISO8583_NS , 28,kDL_ISO8583_LLVAR  }, // Primary Account Number, Extended
/*  35 */ {kDL_ISO8583_Z  , 37,kDL_ISO8583_LLVAR  }, // Track 2 Data
/*  36 */ {kDL_ISO8583_AN ,104,kDL_ISO8583_LLLVAR }, // Track 3 Data
/*  37 */ {kDL_ISO8583_AN , 12,kDL_ISO8583_FIXED  }, // Retrieval Reference Number
/*  38 */ {kDL_ISO8583_AN ,  6,kDL_ISO8583_FIXED  }, // Approval Code
/*  39 */ {kDL_ISO8583_AN ,  2,kDL_ISO8583_FIXED  }, // Response Code
/*  40 */ {kDL_ISO8583_ANS,  3,kDL_ISO8583_FIXED  }, // Service Restriction Code
/*  41 */ {kDL_ISO8583_ANS,  8,kDL_ISO8583_FIXED  }, // Card Acceptor Terminal Id
/*  42 */ {kDL_ISO8583_ANS, 15,kDL_ISO8583_FIXED  }, // Card Acceptor Id Code
/*  43 */ {kDL_ISO8583_ANS, 40,kDL_ISO8583_FIXED  }, // Card Acceptor Name/Location
/*  44 */ {kDL_ISO8583_ANS, 25,kDL_ISO8583_LLVAR  }, // Additional Response Data
/*  45 */ {kDL_ISO8583_ANS, 76,kDL_ISO8583_LLVAR  }, // Track 1 Data
/*  46 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Additional Data - ISO
/*  47 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Additional Data - National
/*  48 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Additional Data - Private
/*  49 */ {kDL_ISO8583_ANS,  3,kDL_ISO8583_FIXED  }, // Currency Code, Txn
/*  50 */ {kDL_ISO8583_AN ,  3,kDL_ISO8583_FIXED  }, // Currency Code, Settlement
/*  51 */ {kDL_ISO8583_AN ,  3,kDL_ISO8583_FIXED  }, // Currency Code, Cardholder Billing
/*  52 */ {kDL_ISO8583_B  ,  8,kDL_ISO8583_FIXED  }, // Personal Id Number (PIN) Data
/*  53 */ {kDL_ISO8583_N  , 16,kDL_ISO8583_FIXED  }, // Security Related Control Information
/*  54 */ {kDL_ISO8583_ANS,120,kDL_ISO8583_LLLVAR }, // Amounts, Additional
/*  55 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/*  56 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/*  57 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/*  58 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/*  59 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/*  60 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/*  61 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/*  62 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/*  63 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/*  64 */ {kDL_ISO8583_B  ,  8,kDL_ISO8583_FIXED  }, // Message Authentication Code Field
/*  65 */ {kDL_ISO8583_B  ,  8,kDL_ISO8583_FIXED  }, // Reserved for ISO use
/*  66 */ {kDL_ISO8583_N  ,  1,kDL_ISO8583_FIXED  }, // Code, Settlement
/*  67 */ {kDL_ISO8583_N  ,  2,kDL_ISO8583_FIXED  }, // Extended Payment Code
/*  68 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Receiving Inst
/*  69 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Settlement Inst
/*  70 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Network Management Information Code
/*  71 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Message Number
/*  72 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Message Number Last
/*  73 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Date, Action
/*  74 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Credits, Number
/*  75 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Credits, Reversal Number
/*  76 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Debits, Number
/*  77 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Debits, Reversal Number
/*  78 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Transfer, Number
/*  79 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Transfer, Reversal Number
/*  80 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Inquiries, Number
/*  81 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Authorization, Number
/*  82 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Credits, Processing Fee
/*  83 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Credits, Txn Fee
/*  84 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Debits, Processing Fee
/*  85 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Debits, Txn Fee
/*  86 */ {kDL_ISO8583_N  , 15,kDL_ISO8583_FIXED  }, // Credits, Amount
/*  87 */ {kDL_ISO8583_N  , 15,kDL_ISO8583_FIXED  }, // Credits, Reversal Amount
/*  88 */ {kDL_ISO8583_N  , 15,kDL_ISO8583_FIXED  }, // Debits, Amount
/*  89 */ {kDL_ISO8583_N  , 15,kDL_ISO8583_FIXED  }, // Debits, Reversal Amount
/*  90 */ {kDL_ISO8583_N  , 42,kDL_ISO8583_FIXED  }, // Original Data Elements
/*  91 */ {kDL_ISO8583_ANS,  1,kDL_ISO8583_FIXED  }, // File Update Code
/*  92 */ {kDL_ISO8583_N  ,  2,kDL_ISO8583_FIXED  }, // File Security Code
/*  93 */ {kDL_ISO8583_N  ,  5,kDL_ISO8583_FIXED  }, // Response Indicator
/*  94 */ {kDL_ISO8583_ANS,  7,kDL_ISO8583_FIXED  }, // Service Indicator
/*  95 */ {kDL_ISO8583_ANS, 42,kDL_ISO8583_FIXED  }, // Amount, Replacement
/*  96 */ {kDL_ISO8583_B  ,  8,kDL_ISO8583_FIXED  }, // Message Security Code
/*  97 */ {kDL_ISO8583_XN , 17,kDL_ISO8583_FIXED  }, // Amount, Net Settlement
/*  98 */ {kDL_ISO8583_ANS, 25,kDL_ISO8583_FIXED  }, // Payee
/*  99 */ {kDL_ISO8583_N  , 11,kDL_ISO8583_LLVAR  }, // Settlement Inst Id Code
/* 100 */ {kDL_ISO8583_N  , 11,kDL_ISO8583_LLVAR  }, // Receiving Inst Id Code
/* 101 */ {kDL_ISO8583_ANS, 17,kDL_ISO8583_LLVAR  }, // File Name
/* 102 */ {kDL_ISO8583_ANS, 28,kDL_ISO8583_LLVAR  }, // Account Id 1
/* 103 */ {kDL_ISO8583_ANS, 28,kDL_ISO8583_LLVAR  }, // Account Id 2
/* 104 */ {kDL_ISO8583_ANS,100,kDL_ISO8583_LLLVAR }, // Txn Description
/* 105 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 106 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 107 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 108 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 109 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 110 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 111 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 112 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 113 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 114 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 115 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 116 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 117 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 118 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 119 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 120 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 121 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 122 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 123 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 124 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 125 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 126 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 127 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 128 */ {kDL_ISO8583_B  ,  8,kDL_ISO8583_FIXED  }  // Message Authentication Code Field
};

/******************************************************************************/

void DL_ISO8583_DEFS_1987_GetHandler ( DL_ISO8583_HANDLER *oHandler )
{
	DL_ISO8583_COMMON_SetHandler(
		_iso8583_1987_fields,
		(DL_UINT8)(sizeof(_iso8583_1987_fields)/sizeof(DL_ISO8583_FIELD_DEF)),
		oHandler);

	return;
}

/******************************************************************************/
