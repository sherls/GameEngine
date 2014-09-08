#ifndef _DISPLAY_MATERIAL_H_
#define _DISPLAY_MATERIAL_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayMaterial
	{
#ifdef DUMP_FBX_INFO
		void DisplayMaterial( FbxGeometry *i_geometry );
#else
		inline void DisplayMaterial( FbxGeometry *i_geometry ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif	// #ifndef _DISPLAY_MATERIAL_H_