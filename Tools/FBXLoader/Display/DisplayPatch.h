#ifndef _DISPLAY_PATCH_H_
#define _DISPLAY_PATCH_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayPatch
	{
#ifdef DUMP_FBX_INFO
		void DisplayPatch( FbxNode *i_node );
#else
		inline void DisplayPatch( FbxNode *i_node ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif	// #ifndef _DISPLAY_PATCH_H_