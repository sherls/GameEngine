#ifndef _DISPLAY_SHAPE_H_
#define _DISPLAY_SHAPE_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayShape
	{
#ifdef DUMP_FBX_INFO
		void DisplayShape( FbxGeometry *i_geometry );
#else
		inline void DisplayShape( FbxGeometry *i_geometry ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif // #ifndef _DISPLAY_SHAPE_H_