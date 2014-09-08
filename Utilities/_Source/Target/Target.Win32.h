/**
 ****************************************************************************************************
 * \file		Target.Win32.h
 * \brief		Configuration for Windows
 ****************************************************************************************************
*/

#ifndef _TARGET_WIN32_H_
#define _TARGET_WIN32_H_

#define CACHE_LINE		64
#define CACHE_ALIGN		__declspec(align(CACHE_LINE))

typedef __int64 TICK;

#endif	// #ifndef _TARGET_WIN32_H_