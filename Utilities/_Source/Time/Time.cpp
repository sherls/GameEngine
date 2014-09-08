/**
 ****************************************************************************************************
 * \file		Time.h
 * \brief		The implementation of Time utility function
 ****************************************************************************************************
*/

#include <assert.h>
#include <windows.h>

#include "Time.h"
#include "../Debug/Debug.h"

namespace Utilities
{
	namespace Time
	{
		double g_frequency_s;
		LARGE_INTEGER g_countsPerSecond;
		LARGE_INTEGER g_totalCountsElapsed_duringRun;
		LARGE_INTEGER g_totalCountsElapsed_previousFrame;
		LARGE_INTEGER g_totalCountsElapsed_atInitialization;
	}	// namespace Time
}	// namespace GameEngine

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize time
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
bool Utilities::Time::Initialize( void )
{
	FUNCTION_START;

	// Get the frequency of the high-resolution performance counter
	if( !QueryPerformanceFrequency(&g_countsPerSecond) )
	{
		FUNCTION_FINISH;
		return false;
	}
	else
		g_frequency_s = 1.0f / static_cast<double>( g_countsPerSecond.QuadPart );

	// Make sure it's supported on this hardware
	if( g_countsPerSecond.QuadPart != 0 )
	{
		if( QueryPerformanceCounter(&g_totalCountsElapsed_atInitialization) )
		{
			FUNCTION_FINISH;
			return true;
		}
		else
		{
			FUNCTION_FINISH;
			return false;
		}
	}
	else
	{
		FUNCTION_FINISH;
		return false;
	}
}

/**
 ****************************************************************************************************
	\fn			void OnNewFrame( void )
	\brief		Update time on new frame
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::Time::OnNewFrame( void )
{
	FUNCTION_START;

	// Update the previous frame
	g_totalCountsElapsed_previousFrame = g_totalCountsElapsed_duringRun;

	// Update current frame
	LARGE_INTEGER totalCountsElapsed;
	BOOL result = QueryPerformanceCounter( &totalCountsElapsed );
	assert( result  );
	g_totalCountsElapsed_duringRun.QuadPart = totalCountsElapsed.QuadPart - g_totalCountsElapsed_atInitialization.QuadPart;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			float GetTotalSecondsElapsed( void )
	\brief		Get total seconds elapsed
	\param		NONE
	\return		float
	\retval		total elapsed time in seconds
 ****************************************************************************************************
*/
float Utilities::Time::GetTotalSecondsElapsed( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
	return static_cast<float>(
		static_cast<double>(g_totalCountsElapsed_duringRun.QuadPart * g_frequency_s)
	);
}

/**
 ****************************************************************************************************
	\fn			float GetTimeElapsedThisFrame_ms( NONE )
	\brief		Get total time elapsed during previous frame
	\param		NONE
	\return		float
	\retval		Total time elapsed during previous frame
 ****************************************************************************************************
*/
float Utilities::Time::GetTimeElapsedThisFrame_ms( void )
{
	float lastFrame;
	FUNCTION_START;

	lastFrame = static_cast<float>(
		static_cast<double>(g_totalCountsElapsed_duringRun.QuadPart - g_totalCountsElapsed_previousFrame.QuadPart)
		* g_frequency_s * 1000
	);

	FUNCTION_FINISH;
	return lastFrame;
}

/**
 ****************************************************************************************************
	\fn			UINT32 GetCurrentCounter_ms( void )
	\brief		Get current counter
	\param		NONE
	\return		UINT32
	\retval		Current high-resolution performance counter
 ****************************************************************************************************
*/
UINT32 Utilities::Time::GetCurrentCounter_ms( void )
{
	FUNCTION_START;
	FUNCTION_FINISH;
	return g_totalCountsElapsed_duringRun.LowPart - g_totalCountsElapsed_previousFrame.LowPart;
}

/**
 ****************************************************************************************************
	\fn			UINT32 TickDifferenceToMs( UINT32 i_u32Ticks )
	\brief		Get tick differences in ms
	\param		i_u32Ticks tick to be converted to ms
	\return		UINT32
	\retval		Tick differences in ms
 ****************************************************************************************************
*/
UINT32 Utilities::Time::TickDifferenceToMs( UINT32 i_u32Ticks )
{
	FUNCTION_START;

	if( g_countsPerSecond.QuadPart == 0LL )
		Initialize();

	TICK i64Diff = static_cast<TICK>(i_u32Ticks) * 1000;

	FUNCTION_FINISH;
	return static_cast<UINT32>( i64Diff * g_frequency_s );
}

/**
 ****************************************************************************************************
	\fn			TICK GetCurrentTick( void )
	\brief		Get current tick
	\param		NONE
	\return		TICK
	\retval		Current high-resolution performance counter tick
 ****************************************************************************************************
*/
__int64 Utilities::Time::GetCurrentTick( void )
{
	LARGE_INTEGER counter;

	FUNCTION_START;

	BOOL bSuccess = QueryPerformanceCounter( &counter );
	assert( bSuccess );

	FUNCTION_FINISH;

	return counter.QuadPart;
}

/**
 ****************************************************************************************************
	\fn			UINT32 GetDifferenceTick( TICK i_start, TICK i_end )
	\brief		Get tick difference
	\param		i_start start tick
	\param		i_end end tick
	\return		UINT32
	\retval		The tick difference
 ****************************************************************************************************
*/
UINT32 Utilities::Time::GetDifferenceTick( TICK i_start, TICK i_end )
{
	FUNCTION_START;
	FUNCTION_FINISH;
	return static_cast<UINT32>( i_end, i_start );
}

/**
 ****************************************************************************************************
	\fn			UINT32 GetDifferenceTick_ms( TICK i_start, TICK i_end )
	\brief		Get the tick difference in ms
	\param		i_start start tick
	\param		i_end end tick
	\return		UINT32
	\retval		The tick difference in ms
 ****************************************************************************************************
*/
UINT32 Utilities::Time::GetDifferenceTick_ms( TICK i_start, TICK i_end )
{
	FUNCTION_START;
	FUNCTION_FINISH;
	return TickDifferenceToMs( static_cast<UINT32>(i_end - i_start) );
}