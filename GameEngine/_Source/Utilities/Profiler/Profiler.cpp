/**
 ****************************************************************************************************
 * \file		Time.h
 * \brief		The header of Time utility function
 ****************************************************************************************************
*/

// Utilities header
#include <Debug/Debug.h>

#include "Profiler.h"

/**
 ****************************************************************************************************
	\fn			void AddTiming( const char *i_name)
	\brief		Update time on new frame
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Profiler::AddTiming( const char * i_name, UINT32 i_u32Ms )
{
	S_ACCUMULATOR &myAccumulator = _accumulators[i_name];

	FUNCTION_START;

	if( i_u32Ms < myAccumulator.m_Min )
		myAccumulator.m_Min = i_u32Ms;
	if( i_u32Ms > myAccumulator.m_Max )
		myAccumulator.m_Max = i_u32Ms;

	myAccumulator.m_Count++;
	myAccumulator.m_Sum += i_u32Ms;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void PrintAccumulators( void )
	\brief		Print out accumulators
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Profiler::PrintAccumulators( void )
{
	std::map<std::string, S_ACCUMULATOR>::iterator iter;

	FUNCTION_START;

	for( iter = _accumulators.begin(); iter != _accumulators.end(); ++iter )
	{
		float average = iter->second.m_Count ? ((float) iter->second.m_Sum ) / iter->second.m_Count : 0.0f;
		DBG_MSG_LEVEL( D_PROFILER, "[%s] Count: %d Sum: %d Min: %d Max: %d Ave: %f\n", iter->first.c_str(), iter->second.m_Count, iter->second.m_Sum, iter->second.m_Min, iter->second.m_Max, average );
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			ScopedTimer( const char *i_name )
	\brief		Default constructor of ScopedTimer class
	\param		i_name name of scoped time
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::ScopedTimer::ScopedTimer( const char *i_name ) :
	_start( Utilities::Time::GetCurrentTick() ),
	_scopeName( i_name )
{
	FUNCTION_START;

#ifdef _DEBUG
	DBG_MSG_LEVEL( D_PROFILER, "Scoped Timer %s started at Tick %lld\n", _scopeName ? _scopeName : "Unnamed", _start );
#endif
	assert( i_name );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			~ScopedTimer( void )
	\brief		Destructor of ScopedTimer class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::ScopedTimer::~ScopedTimer( void )
{
	Utilities::TICK end = Utilities::Time::GetCurrentTick();

	FUNCTION_START;

#ifdef _DEBUG
	DBG_MSG_LEVEL( D_PROFILER, "Scoped Timer %s ended at Tick %lld ran for %d \n", _scopeName ? _scopeName : "Unnamed", end, Utilities::Time::GetDifferenceTick_ms(_start, end) );
#endif

	g_Profiler::Get().AddTiming( _scopeName, Utilities::Time::GetDifferenceTick_ms(_start, end) );

	FUNCTION_FINISH;
}