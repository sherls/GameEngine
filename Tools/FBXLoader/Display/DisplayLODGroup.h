#ifndef _DISPLAY_LOD_GROUP_H_
#define _DISPLAY_LOD_GROUP_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayLODGroup
	{
#ifdef DUMP_FBX_INFO
		void DisplayLODGroup( FbxNode *i_node );
#else
		inline void DisplayLODGroup( FbxNode *i_node ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif	// #ifndef _DISPLAY_LOD_GROUP_H_