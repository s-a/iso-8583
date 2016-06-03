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
/* dl_iso8583_defs_1993.c - ISO8583 1993 Definitions                          */
/*                                                                            */
/******************************************************************************/

#include "dl_iso8583_defs_1993.h"

/******************************************************************************/

static DL_ISO8583_FIELD_DEF _iso8583_1993_fields[] = {
/*   0 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Message Type Indicator
/*   1 */ {kDL_ISO8583_BMP, 16,kDL_ISO8583_CONTVAR}, // Bitmap
/*   2 */ {kDL_ISO8583_N  , 19,kDL_ISO8583_LLVAR  }, // Primary Account Number
/*   3 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Processing Code
/*   4 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Amount, Txn
/*   5 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Amount, Reconciliation
/*   6 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Amount, Cardholder Billing
/*   7 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Date and Time, Transmission
/*   8 */ {kDL_ISO8583_N  ,  8,kDL_ISO8583_FIXED  }, // Amount, Cardholder Billing Fee
/*   9 */ {kDL_ISO8583_N  ,  8,kDL_ISO8583_FIXED  }, // Conversion Rate, Reconciliation
/*  10 */ {kDL_ISO8583_N  ,  8,kDL_ISO8583_FIXED  }, // Conversion Rate, Cardholder Billing
/*  11 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Systems Trace Audit Number
/*  12 */ {kDL_ISO8583_N  , 12,kDL_ISO8583_FIXED  }, // Date and Time, Local Txn
/*  13 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Date, Effective
/*  14 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Date, Expiration
/*  15 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Date, Settlement
/*  16 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Date, Conversion
/*  17 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Date, Capture
/*  18 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Merchant Type
/*  19 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Acquiring Inst
/*  20 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Primary Account Number
/*  21 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Forwarding Inst
/*  22 */ {kDL_ISO8583_AN , 12,kDL_ISO8583_FIXED  }, // Point of Service Data Code
/*  23 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Card Sequence Number
/*  24 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Function Code
/*  25 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Message Reason Code
/*  26 */ {kDL_ISO8583_N  ,  4,kDL_ISO8583_FIXED  }, // Card Acceptor Business Code
/*  27 */ {kDL_ISO8583_N  ,  1,kDL_ISO8583_FIXED  }, // Approval Code Length
/*  28 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Date, Reconciliation
/*  29 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Reconciliation Indicator
/*  30 */ {kDL_ISO8583_N  , 24,kDL_ISO8583_FIXED  }, // Amounts, Original
/*  31 */ {kDL_ISO8583_ANS, 99,kDL_ISO8583_LLVAR  }, // Acquirer Reference Data
/*  32 */ {kDL_ISO8583_N  , 11,kDL_ISO8583_LLVAR  }, // Acquirer Inst Id Code
/*  33 */ {kDL_ISO8583_N  , 11,kDL_ISO8583_LLVAR  }, // Forwarding Inst Id Code
/*  34 */ {kDL_ISO8583_NS , 28,kDL_ISO8583_LLVAR  }, // Primary Account Number, Extended
/*  35 */ {kDL_ISO8583_Z  , 37,kDL_ISO8583_LLVAR  }, // Track 2 Data
/*  36 */ {kDL_ISO8583_Z  ,104,kDL_ISO8583_LLLVAR }, // Track 3 Data
/*  37 */ {kDL_ISO8583_ANP, 12,kDL_ISO8583_FIXED  }, // Retrieval Reference Number
/*  38 */ {kDL_ISO8583_ANP,  6,kDL_ISO8583_FIXED  }, // Approval Code
/*  39 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Action Code
/*  40 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Service Code
/*  41 */ {kDL_ISO8583_ANS,  8,kDL_ISO8583_FIXED  }, // Card Acceptor Terminal Id
/*  42 */ {kDL_ISO8583_ANS, 15,kDL_ISO8583_FIXED  }, // Card Acceptor Id Code
/*  43 */ {kDL_ISO8583_ANS, 99,kDL_ISO8583_LLVAR  }, // Card Acceptor Name/Location
/*  44 */ {kDL_ISO8583_ANS, 99,kDL_ISO8583_LLVAR  }, // Additional Response Data
/*  45 */ {kDL_ISO8583_ANS, 76,kDL_ISO8583_LLVAR  }, // Track 1 Data
/*  46 */ {kDL_ISO8583_ANS,204,kDL_ISO8583_LLLVAR }, // Amounts, Fees
/*  47 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Additional Data - National
/*  48 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Additional Data - Private
/*  49 */ {kDL_ISO8583_AN ,  3,kDL_ISO8583_FIXED  }, // Currency Code, Txn
/*  50 */ {kDL_ISO8583_AN ,  3,kDL_ISO8583_FIXED  }, // Currency Code, Reconciliation
/*  51 */ {kDL_ISO8583_AN ,  3,kDL_ISO8583_FIXED  }, // Currency Code, Cardholder Billing
/*  52 */ {kDL_ISO8583_B  ,  8,kDL_ISO8583_FIXED  }, // Personal Id Number (PIN) Data
/*  53 */ {kDL_ISO8583_B  , 48,kDL_ISO8583_LLVAR  }, // Security Related Control Information
/*  54 */ {kDL_ISO8583_ANS,120,kDL_ISO8583_LLLVAR }, // Amounts, Additional
/*  55 */ {kDL_ISO8583_B  ,255,kDL_ISO8583_LLLVAR }, // IC Card System Related Data
/*  56 */ {kDL_ISO8583_N  , 35,kDL_ISO8583_LLVAR  }, // Original Data Elements
/*  57 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Authorization Life Cycle Code
/*  58 */ {kDL_ISO8583_N  , 11,kDL_ISO8583_LLVAR  }, // Authorizing Agent Inst Id Code
/*  59 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Transport Data
/*  60 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/*  61 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/*  62 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/*  63 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/*  64 */ {kDL_ISO8583_B  ,  8,kDL_ISO8583_FIXED  }, // Message Authentication Code Field
/*  65 */ {kDL_ISO8583_B  ,  8,kDL_ISO8583_FIXED  }, // Reserved for ISO use
/*  66 */ {kDL_ISO8583_ANS,204,kDL_ISO8583_LLLVAR }, // Amounts, Original Fees
/*  67 */ {kDL_ISO8583_N  ,  2,kDL_ISO8583_FIXED  }, // Extended Payment Data
/*  68 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Receiving Inst
/*  69 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Settlement Inst
/*  70 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Authorizing Agent Inst
/*  71 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Message Number
/*  72 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Data Record
/*  73 */ {kDL_ISO8583_N  ,  6,kDL_ISO8583_FIXED  }, // Date, Action
/*  74 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Credits, Number
/*  75 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Credits, Reversal Number
/*  76 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Debits, Number
/*  77 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Debits, Reversal Number
/*  78 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Transfer, Number
/*  79 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Transfer, Reversal Number
/*  80 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Inquiries, Number
/*  81 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Authorizations, Number
/*  82 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Inquiries, Reversal Number
/*  83 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Payments, Number
/*  84 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Payments, Reversal Number
/*  85 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Fee Collections, Number
/*  86 */ {kDL_ISO8583_N  , 16,kDL_ISO8583_FIXED  }, // Credits, Amount
/*  87 */ {kDL_ISO8583_N  , 16,kDL_ISO8583_FIXED  }, // Credits, Reversal Amount
/*  88 */ {kDL_ISO8583_N  , 16,kDL_ISO8583_FIXED  }, // Debits, Amount
/*  89 */ {kDL_ISO8583_N  , 16,kDL_ISO8583_FIXED  }, // Debits, Reversal Amount
/*  90 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Authorizations, Reversal Number
/*  91 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Txn Destination Inst
/*  92 */ {kDL_ISO8583_N  ,  3,kDL_ISO8583_FIXED  }, // Country Code, Txn Originator Inst
/*  93 */ {kDL_ISO8583_N  , 11,kDL_ISO8583_LLVAR  }, // Txn Destination Inst Id Code
/*  94 */ {kDL_ISO8583_N  , 11,kDL_ISO8583_LLVAR  }, // Txn Originator Inst Id Code
/*  95 */ {kDL_ISO8583_ANS, 99,kDL_ISO8583_LLVAR  }, // Card Issuer Reference Data
/*  96 */ {kDL_ISO8583_B  ,999,kDL_ISO8583_LLLVAR }, // Key Management Data
/*  97 */ {kDL_ISO8583_XN , 17,kDL_ISO8583_FIXED  }, // Amount, Net Reconciliation
/*  98 */ {kDL_ISO8583_ANS, 25,kDL_ISO8583_FIXED  }, // Payee
/*  99 */ {kDL_ISO8583_AN , 11,kDL_ISO8583_LLVAR  }, // Settlement Inst Id Code
/* 100 */ {kDL_ISO8583_N  , 11,kDL_ISO8583_LLVAR  }, // Receiving Inst Id Code
/* 101 */ {kDL_ISO8583_ANS, 17,kDL_ISO8583_LLVAR  }, // File Name
/* 102 */ {kDL_ISO8583_ANS, 28,kDL_ISO8583_LLVAR  }, // Account Id 1
/* 103 */ {kDL_ISO8583_ANS, 28,kDL_ISO8583_LLVAR  }, // Account Id 2
/* 104 */ {kDL_ISO8583_ANS,100,kDL_ISO8583_LLLVAR }, // Txn Description
/* 105 */ {kDL_ISO8583_N  , 16,kDL_ISO8583_FIXED  }, // Credits, Chargeback Amount
/* 106 */ {kDL_ISO8583_N  , 16,kDL_ISO8583_FIXED  }, // Debits, Chargeback Amount
/* 107 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Credits, Chargeback Number
/* 108 */ {kDL_ISO8583_N  , 10,kDL_ISO8583_FIXED  }, // Debits, Chargeback Number
/* 109 */ {kDL_ISO8583_ANS, 84,kDL_ISO8583_LLVAR  }, // Credits, Fee Amounts
/* 110 */ {kDL_ISO8583_ANS, 84,kDL_ISO8583_LLVAR  }, // Debits, Fee Amounts
/* 111 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 112 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 113 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 114 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 115 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for ISO use
/* 116 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 117 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 118 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 119 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 120 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 121 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 122 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for National use
/* 123 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 124 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 125 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 126 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 127 */ {kDL_ISO8583_ANS,999,kDL_ISO8583_LLLVAR }, // Reserved for Private use
/* 128 */ {kDL_ISO8583_B  ,  8,kDL_ISO8583_FIXED  }  // Message Authentication Code Field
};

/******************************************************************************/

void DL_ISO8583_DEFS_1993_GetHandler ( DL_ISO8583_HANDLER *oHandler )
{
	DL_ISO8583_COMMON_SetHandler(
		_iso8583_1993_fields,
		(DL_UINT8)(sizeof(_iso8583_1993_fields)/sizeof(DL_ISO8583_FIELD_DEF)),
		oHandler);

	return;
}

/******************************************************************************/
