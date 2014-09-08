/**
 ****************************************************************************************************
 * \file		Time.h
 * \brief		The header of Time utility function
 ****************************************************************************************************
*/

#ifndef _TIME_H_
#define _TIME_H_

#include "../Target/Target.h"
#include "../UtilitiesTypes.h"

namespace Utilities
{
	namespace Time
	{
		bool Initialize( void );
		void OnNewFrame( void );
		float GetTotalSecondsElapsed( void );
		float GetTimeElapsedThisFrame_ms( void );

		UINT32 GetCurrentCounter_ms( void );
		UINT32 TickDifferenceToMs( UINT32 i_u32Ticks );
		TICK GetCurrentTick( void );
		UINT32 GetDifferenceTick( TICK i_start, TICK i_end );
		UINT32 GetDifferenceTick_ms( TICK i_start, TICK i_end );
	}	// namespace Time
}	// namespace GameEngine

#endif	// #ifndef _TIME_H_