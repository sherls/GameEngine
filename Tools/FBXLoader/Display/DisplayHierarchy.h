#ifndef _DISPLAY_HIERARCHY_H_
#define _DISPLAY_HIERARCHY_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayHierarchy
	{
#ifdef DUMP_FBX_INFO
		void DisplayHierarchy( FbxScene *i_scene );
		void DisplayHierarchy( FbxNode *i_node, int i_depth );
#else
		inline void DisplayHierarchy( FbxScene *i_scene ){ }
		inline void DisplayHierarchy( FbxNode *i_node, int i_depth ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif	// #ifndef _DISPLAY_HIERARCHY_H_