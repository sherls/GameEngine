#ifndef _DISPLAY_ANIMATION_H_
#define _DISPLAY_ANIMATION_H_

#include "DisplayCommon.h"

#if defined( DUMP_FBX_INFO ) && defined( DISPLAY_ANIMATION_INFORMATION )
namespace Tools
{
	namespace DisplayAnimation
	{
		void DisplayAnimation( FbxScene *i_scene );
		void DisplayDefaultAnimation( FbxNode *i_node );
	}
}
#endif	// #if defined( DUMP_FBX_INFO ) && defined( DISPLAY_ANIMATION_INFORMATION )

#endif	// #ifndef _DISPLAY_ANIMATION_H_