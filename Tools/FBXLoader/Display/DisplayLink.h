#ifndef _DISPLAY_LINK_H_
#define _DISPLAY_LINK_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayLink
	{
#ifdef DUMP_FBX_INFO
		void DisplayLink( FbxGeometry *i_geometry );
#else
		inline void DisplayLink( FbxGeometry *i_geometry ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif	// #ifndef _DISPLAY_LINK_H_