/**
 ****************************************************************************************************
 * \file		Debug.cpp
 * \brief		Debug message implementation
 ****************************************************************************************************
*/
#ifdef _DEBUG

#include <Windows.h>
#include <assert.h>
#include <stdio.h>

#include "Debug.h"
#include "../UtilitiesTypes.h"

namespace Utilities
{
UINT32	u32DbgGroup = 0;
UINT32	u32DbgLevel = 0;

/**
 ****************************************************************************************************
	\fn			void DEBUG_SET_GROUP( int UINT32Group, DBG_STATE enDbgState )
 ****************************************************************************************************
*/
void DEBUG_SET_GROUP( UINT32 u32Group, DBG_STATE enDbgState )
{
	if( enDbgState == DBG_ON )
		u32DbgGroup |= u32Group;
	else
		u32DbgGroup &= (~u32Group);
}

/**
 ****************************************************************************************************
	\fn			void DEBUG_SET_LEVEL( int u32Level, DBG_STATE enDbgState )
 ****************************************************************************************************
*/
void DEBUG_SET_LEVEL( UINT32 u32Level, DBG_STATE enDbgState )
{
	if (enDbgState == DBG_ON)
		u32DbgLevel |= u32Level;
	else
		u32DbgLevel &= (~u32Level);
}

/**
 ****************************************************************************************************
	\fn			void ConsolePrint( const char *fmt, ... )
 ****************************************************************************************************
*/
void ConsolePrint( const char *fmt, ... )
{
	const char *p = fmt;
	//UINT8 count = 0;

	//assert( fmt != NULL );

	//while(*p)
	//{
	//	count++;
	//	++p;
	//}
	
	const unsigned int str_length = 256;
	//char *str = (char*)malloc( sizeof(char) * count );
	//char *str = new char[count];
	char str[str_length];
    va_list args;

    va_start(args, fmt);

	vsprintf_s( str, str_length, fmt, args );
	//vsprintf_s( str, count, fmt, args );

	va_end( args );

	OutputDebugStringA( str );

	//delete []str;
}
}
#endif	// #ifdef _DEBUG