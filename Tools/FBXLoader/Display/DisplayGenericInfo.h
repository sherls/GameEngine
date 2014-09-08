#ifndef _DISPLAY_GENERIC_INFO_H_
#define _DISPLAY_GENERIC_INFO_H_

#include "DisplayCommon.h"

#if defined( DUMP_FBX_INFO ) && defined( DISPLAY_GENERIC_INFORMATION )
namespace Tools
{
	namespace DisplayGenericInfo
	{
		void DisplayGenericInfo( FbxScene *i_scene );
	}
}
#endif	// #if defined( DUMP_FBX_INFO ) && defined( DISPLAY_GENERIC_INFORMATION )

#endif	// #ifndef _DISPLAY_GENERIC_INFO_H_