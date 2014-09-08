/**
 ****************************************************************************************************
 * \file		FBXLoader.h
 * \brief		FBXLoader class implementation
 ****************************************************************************************************
*/

#include <string>
#include <crtdbg.h>

#include "FBXLoader.h"

// FBX SDK
#include <fbxsdk.h>

// Tools/FbxLoader
#include <Common.h>
#include <AssetGenerator/AssetGenerator.h>
#include <OctreeGenerator/OctreeGenerator.h>

bool FBX::FBXLoader::OpenFBXLoader( const char *i_filename, wxTextCtrl *i_infoWindow, std::string &o_sceneFile )
{
	FbxManager *sdkManager = NULL;
	FbxScene *scene = NULL;

	Tools::OctreeGenerator::Initialize();

	// Prepare the FBX SDK
	Tools::Common::InitializeSDKObjects( sdkManager, scene );

	// Load the scene
	bool result = Tools::Common::LoadScene( sdkManager, scene, i_filename );
	if( result )
	{
		FbxAxisSystem sceneAxisSystem = scene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem ourAxisSystem( FbxAxisSystem::eDirectX );
		if( sceneAxisSystem != ourAxisSystem )
			ourAxisSystem.ConvertScene( scene );

		// Convert Unit System to what is used in this example, if needed
		FbxSystemUnit sceneSystemUnit = scene->GetGlobalSettings().GetSystemUnit();
		if( sceneSystemUnit.GetScaleFactor() != 1.0 )
		{
			//The unit in this example is centimeter.
			FbxSystemUnit::m.ConvertScene( scene );
		}

		// Convert mesh, NURBS and patch into triangle mesh
		FbxGeometryConverter geometryConverter( sdkManager );
		result = geometryConverter.Triangulate( scene, true );
		if( result )
			i_infoWindow->WriteText( "Success triangulate the meshes.\n" );
		else
		{
			wxMessageBox( "<<<ERROR>>> Operation aborted! <<<ERROR>>>\nReason: Failed to triangulate the meshes." ,
				"Failed to triangulate the meshes", wxOK | wxICON_ERROR, i_infoWindow );
			return FAIL;
		}

		// Split meshes per material, so that we only have one material per mesh (for VBO support)
		result = geometryConverter.SplitMeshesPerMaterial( scene, true );
		if( result )
		{
			i_infoWindow->WriteText( "Success splitting the meshes per material.\n" );
		}
		else
		{
			wxMessageBox( "<<<ERROR>>> Operation aborted! <<<ERROR>>>\nReason: Failed splitting the meshes per material." ,
				"Failed splitting the meshes per material", wxOK | wxICON_ERROR, i_infoWindow );
			return FAIL;
		}

		std::string *scnTargetStr = new std::string( i_filename );
		scnTargetStr->replace( scnTargetStr->end()-3, scnTargetStr->end(), "scn" );
		size_t from = scnTargetStr->find_last_of('\\');
		*scnTargetStr = scnTargetStr->substr( from + 1, scnTargetStr->length() - from );
		char *targetName = new char[ scnTargetStr->length() + 30 ];
		sprintf_s( targetName, scnTargetStr->length() + 30, "..\\..\\External\\Data\\Scenes\\%s", scnTargetStr->c_str() );
		o_sceneFile = std::string( scnTargetStr->c_str() );

		i_infoWindow->WriteText( "Generating scene file and all the related files. This may takes a while. Please wait patiently.\n" );
		Tools::AssetGenerator::GenerateSceneFile( scene, targetName );

		delete [] targetName;
		delete scnTargetStr;
	}

	if( scene )
	{
		scene->Destroy();
		scene = NULL;
	}
	Tools::Common::DestroySDKObjects( sdkManager, result );

	std::string *octreeTargetStr = new std::string( i_filename );
	*octreeTargetStr = octreeTargetStr->substr( 0, octreeTargetStr->length() - 4 );
	UINT32 startPos = octreeTargetStr->find_last_of("\\" );
	*octreeTargetStr = octreeTargetStr->substr(startPos+1, octreeTargetStr->size());
	Tools::OctreeGenerator::GenerateOctree( octreeTargetStr->c_str() );

	Tools::OctreeGenerator::ShutDown();

	delete octreeTargetStr;

	return SUCCESS;
}