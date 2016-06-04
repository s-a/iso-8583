//
// DL ISO8583 library - benchmark test
//

#include "dl_timer.h"

#include "dl_iso8583.h"
#include "dl_iso8583_defs_1987.h"


// indicates the number of pack/unpack iterations performed
#define kITERATIONS		1000000

// indicates the buffer size (in bytes)
#define kBUFFER_SIZE	1000

// if defined then ISO messages will use a static memory buffer, otherwise
// dynamic memory allocation will be used
#define USE_STATIC_MEMORY


int main ( void )
{
	DL_TIMER           t;
	DL_ISO8583_HANDLER isoHandler;
	DL_ISO8583_MSG     isoMsg;
	DL_UINT8           buf[kBUFFER_SIZE];
	DL_UINT16          bufSize;
	long               i;
#ifdef USE_STATIC_MEMORY
	DL_UINT8           sBuf[kBUFFER_SIZE];
#endif

	DL_ISO8583_DEFS_1987_GetHandler(&isoHandler);

	/* initialise ISO message */
#ifdef USE_STATIC_MEMORY
	DL_ISO8583_MSG_Init(sBuf,kBUFFER_SIZE,&isoMsg);
#else
	DL_ISO8583_MSG_Init(NULL,0,&isoMsg);
#endif

	/* set ISO message fields */
	(void)DL_ISO8583_MSG_SetField_Str(0,"0800",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(2,"454000000000003",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(3,"000000",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(28,"C1200",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(11,"000001",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(34,"12345",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(41,"12345678",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(42,"123456789012345",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(12,"0000",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(13,"0101",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(14,"0505",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(15,"0202",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(60,"TEST DATA",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(61,"LONGER TEST DATA",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(62,"The quick brown fox jumped over the lazy dog",&isoMsg);
	(void)DL_ISO8583_MSG_SetField_Str(70,"301",&isoMsg);

	/* output ISO message content */
	DL_ISO8583_MSG_Dump(stdout,NULL,&isoHandler,&isoMsg);

	DL_TIMER_Start(&t);

	for ( i=0 ; i<kITERATIONS ; i++ )
	{
		(void)DL_ISO8583_MSG_Pack(&isoHandler,&isoMsg,buf,&bufSize);

		/* destroy ISO Msg */
		DL_ISO8583_MSG_Free(&isoMsg);

		/* initialise ISO message (for unpack) */
#ifdef USE_STATIC_MEMORY
		DL_ISO8583_MSG_Init(sBuf,kBUFFER_SIZE,&isoMsg);
#else
		DL_ISO8583_MSG_Init(NULL,0,&isoMsg);
#endif

		(void)DL_ISO8583_MSG_Unpack(&isoHandler,buf,bufSize,&isoMsg);
	}

	printf("----> TPS = %1.2f\n\n\n",(((float)(kITERATIONS) / (float)DL_TIMER_GetDuration(t)) * 1000));

	/* destroy ISO Msg */
	DL_ISO8583_MSG_Free(&isoMsg);

	return 0;
}
