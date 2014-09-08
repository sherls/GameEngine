/**
 ****************************************************************************************************
 * \file		Debug.h
 * \brief		Debug message implementation header
 ****************************************************************************************************
*/

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "../UtilitiesTypes.h"

/*****************************************************************************************************************
Debug Groups (Up to 32 groups)
*****************************************************************************************************************/
#define DBG_UI			0x00000001	/**< User interface */
#define DBG_MATH		0x00000002	/**< Math related debug */
#define DBG_RING_BUFFER	0x00000004	/**< Ring buffer related debug */
#define DBG_NAME		0x00000008	/**< Actor name related debug */
#define DBG_COLLISION	0x00000010	/**< Collision related debug */
#define DBG_TIME		0x00000020	/**< Time related debug */
#define DBG_RENDERER	0x00000040	/**< Renderer related debug */

/*******************************************************************************************************************
Debug Levels (Up to 32 levels)
*******************************************************************************************************************/
#define D_FUNC		0x00000001	/**< Function tracing. Open if want to trace which function cause crash */
#define D_TRACE		0x00000002	/**< Tracing use */
#define D_ERR		0x00000004	/**< Error */
#define D_EVENT		0x00000008	/**< User input event or internal system event */
#define D_INFO		0x00000010	/**< Information */
#define D_UNIT_TEST	0x00000020	/**< Unit test */
#define D_PROFILER	0x00000040	/**< Profiler related debug */

namespace Utilities
{
	typedef enum DBG_STATE
	{
		DBG_ON,
		DBG_OFF
	} DBG_STATE;

	extern UINT32 u32DbgGroup;
	extern UINT32 u32DbgLevel;

/*****************************************************************************************************************
Debug setting functions
*****************************************************************************************************************/
#ifdef _DEBUG
	void DEBUG_SET_GROUP( UINT32 u32Group, DBG_STATE enDbgState );
	void DEBUG_SET_LEVEL( UINT32 u32Level, DBG_STATE enDbgState );
	void ConsolePrint( const char *str, ... );
#endif	// #ifdef _DEBUG
}	// namespace GameEngine

/*****************************************************************************************************************
Debug Macros
******************************************************************************************************************/
#ifdef _DEBUG
	#define DEBUG_MSG( a, b, fmt, ... )		if ( (a&Utilities::u32DbgGroup)&&(b&u32DbgLevel) ) Utilities::ConsolePrint( fmt, __VA_ARGS__ )
	#define DBG_MSG_LEVEL( a, fmt, ... )	if( a&Utilities::u32DbgLevel ) Utilities::ConsolePrint( fmt, __VA_ARGS__ )
	#define DBG_CONFIG_ERROR( a, b )		DBG_MSG_LEVEL( D_ERR, "[ERROR] Couldn't find %s in %s\n", a, b )
#else
	#define DEBUG_MSG( a, b, fmt, ... )		void( 0 )
	#define DBG_MSG_LEVEL( a, fmt, ... )	void( 0 )
	#define DBG_CONFIG_ERROR( a, b )		void( 0 )
#endif

#define FUNCTION_START					DBG_MSG_LEVEL( D_FUNC, "********************START %s FUNCTION********************\n", __FUNCTION__ )
#define FUNCTION_FINISH					DBG_MSG_LEVEL( D_FUNC, "********************END OF %s FUNCTION********************\n", __FUNCTION__ )

#endif	// #ifndef _DEBUG_H_