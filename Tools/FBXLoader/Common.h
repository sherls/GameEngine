#ifndef _COMMON_H_
#define _COMMON_H_

#include <fbxsdk.h>

namespace Tools
{
	namespace Common
	{
		void InitializeSDKObjects( FbxManager*& i_manager, FbxScene*& i_scene );
		void DestroySDKObjects( FbxManager* i_manager, bool i_bExitStatus );
		void CreateAndFillIOSettings( FbxManager* i_manager );

		bool SaveScene( FbxManager* i_manager, FbxDocument* i_scene, const char* i_filename, int i_fileFormat = -1, bool i_bEmbedMedia = false );
		bool LoadScene( FbxManager* i_manager, FbxDocument* i_scene, const char* i_filename_source );
	}
}

#endif	// #ifndef _COMMON_H_