#ifndef _DISPLAY_MARKER_H_
#define _DISPLAY_MARKER_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayMarker
	{
#ifdef DUMP_FBX_INFO
		void DisplayMarker( FbxNode *i_node );
#else
		inline void DisplayMarker( FbxNode *i_node ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif	// #ifndef _DISPLAY_MARKER_H_