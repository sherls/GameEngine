#ifndef _DISPLAY_SKELETON_H_
#define _DISPLAY_SKELETON_H_

#include "DisplayCommon.h"

#if defined( DUMP_FBX_INFO ) && defined( DISPLAY_ANIMATION_INFORMATION )
namespace Tools
{
	namespace DisplaySkeleton
	{
		void DisplaySkeleton( FbxNode *i_node );
	}
}
#endif	// #if defined( DUMP_FBX_INFO ) && defined( DISPLAY_ANIMATION_INFORMATION )

#endif	// #ifndef _DISPLAY_SKELETON_H_