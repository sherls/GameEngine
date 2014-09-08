#ifndef _DISPLAY_LIGHT_H_
#define _DISPLAY_LIGHT_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayLight
	{
#ifdef DUMP_FBX_INFO
		void DisplayLight( FbxNode *i_Node );
#else
		inline void DisplayLight( FbxNode *i_Node ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif	// #ifndef _DISPLAY_LIGHT_H_