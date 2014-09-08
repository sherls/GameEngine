#ifndef _DISPLAY_MESH_H_
#define _DISPLAY_MESH_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayMesh
	{
#ifdef DUMP_FBX_INFO
		void DisplayMesh( FbxNode *i_node );
#else
		inline void DisplayMesh( FbxNode *i_node ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif	// #ifndef _DISPLAY_MESH_H_


