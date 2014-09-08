#ifndef _TARGET_H_
#define _TARGET_H_

namespace Utilities
{
	#ifdef _WIN32
		#include "Target.Win32.h"
	#endif

	#ifndef CACHE_ALIGN
		#error "CACHE_ALIGN is not defined"
	#endif	// #ifndef CACHE_ALIGN
}	// namespace GameEngine

#endif	// #ifndef _TARGET_H_