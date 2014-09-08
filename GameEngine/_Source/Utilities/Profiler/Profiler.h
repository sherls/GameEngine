/**
 ****************************************************************************************************
 * \file		Time.h
 * \brief		The header of Time utility function
 ****************************************************************************************************
*/

#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <map>
#include <string>
#include <assert.h>

// Utilities header
#include <Time/Time.h>
#include <Singleton/Singleton.h>

#include "../GameEngineTypes.h"

namespace GameEngine
{
	class Profiler
	{
		friend Utilities::Singleton<Profiler>;

		typedef struct _accumulator
		{
			unsigned int	m_Count;
			unsigned int	m_Sum;
			unsigned int	m_Max;
			unsigned int	m_Min;

			_accumulator()
			{
				m_Count = 0;
				m_Sum = 0;
				m_Max = 0;
				m_Min = UINT_MAX;
			}
		} S_ACCUMULATOR;

		std::map<std::string, S_ACCUMULATOR> _accumulators;

		Profiler(){}
		~Profiler(){}

	public:
		void AddTiming( const char *i_pName, UINT32 i_ms );
		void PrintAccumulators( void );
	};

	class ScopedTimer
	{
		const char*				_scopeName;
		Utilities::TICK		_start;

	public:
		ScopedTimer( const char *i_name );
		~ScopedTimer( void );
	} ;
}	// namespace GameEngine

typedef Utilities::Singleton<GameEngine::Profiler> g_Profiler;

#if defined( ENABLE_PROFILING )
	#define PROFILE_SCOPE_BEGIN(str)	{ GameEngine::ScopedTimer __Timer( str );
	#define PROFILE_SCOPE_END()			}

	#define PROFILE_UNSCOPED(str)		GameEngine::ScopedTimer __Timer( str );
	#define PROFILE_PRINT_RESULTS()		g_Profiler::Get().PrintAccumulators();
#else
	#define PROFILE_SCOPE_BEGIN(str)	__noop
	#define PROFILE_SCOPE_END			__noop
	#define PROFILE_UNSCOPED(str)		__noop
	#define PROFILE_PRINT_RESULTS()		__noop
#endif // ENABLE_PROFILING

#endif // _PROFILING_H_