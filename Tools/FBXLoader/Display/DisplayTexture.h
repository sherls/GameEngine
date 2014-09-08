#ifndef _DISPLAY_TEXTURE_H_
#define _DISPLAY_TEXTURE_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayTexture
	{
#ifdef DUMP_FBX_INFO
		void FindAndDisplayTextureInfoByProperty( FbxProperty i_property, bool& i_displayHeader, int i_materialIndex );
		void DisplayTexture( FbxGeometry* i_geometry );
		void DisplayTextureInfo( FbxTexture* i_texture, int i_blendMode );
#else
		inline void FindAndDisplayTextureInfoByProperty( FbxProperty i_property, bool& i_displayHeader, int i_materialIndex ){ }
		inline void DisplayTexture( FbxGeometry* i_geometry ){ }
		inline void DisplayTextureInfo( FbxTexture* i_texture, int i_blendMode ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif	// #ifndef _DISPLAY_TEXTURE_H_