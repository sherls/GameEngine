#ifndef _DISPLAY_NURB_H_
#define _DISPLAY_NURB_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayNURB
	{
#ifdef DUMP_FBX_INFO
		void DisplayNURB( FbxNode *i_node );
#else
		inline void DisplayNURB( FbxNode *i_node ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif // #ifndef _DISPLAY_NURB_H_