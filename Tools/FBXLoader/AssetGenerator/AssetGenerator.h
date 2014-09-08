#ifndef _ASSET_GENERATOR_H_
#define _ASSET_GENERATOR_H_

#include <fbxsdk.h>

namespace Tools
{
	namespace AssetGenerator
	{
		bool GenerateMeshFile( FbxMesh *i_mesh, const char *i_filename );
		bool GenerateSceneFile( FbxScene *i_scene, const char *i_filename );
		bool GenerateMaterialFile( FbxGeometry *i_geometry, const char *i_filename, int &i_materialID );
	}
}

#endif	// #ifndef _ASSET_GENERATOR_H_