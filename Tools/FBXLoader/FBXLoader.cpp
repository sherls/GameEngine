/**
 ****************************************************************************************************
 * \file		cSceneBuilder.cpp
 * \brief		Implementation of cSceneBuilder class
 ****************************************************************************************************
*/

#include <fstream>
#include <crtdbg.h>

// External header
#include <fbxsdk.h>

#include "Common.h"
#include "FBXLoader.h"
#include "Display/DisplayMesh.h"
#include "Display/DisplayNURB.h"
#include "Display/DisplayLight.h"
#include "Display/DisplayPatch.h"
#include "Display/DisplayCommon.h"
#include "Display/DisplayCamera.h"
#include "Display/DisplayMarker.h"
#include "Display/DisplayLODGroup.h"
#include "Display/DisplaySkeleton.h"
#include "Display/DisplayHierarchy.h"
#include "Display/DisplayAnimation.h"
#include "Display/DisplayGenericInfo.h"
#include "Display/DisplayUserProperties.h"
#include "Display/DisplayGlobalSettings.h"
#include "Display/DisplayPivotsAndLimits.h"

#include "AssetGenerator/AssetGenerator.h"
#include "OctreeGenerator/OctreeGenerator.h"

#ifdef DUMP_FBX_INFO
namespace Tools
{
	void DisplayMetaData( FbxScene *i_scene, const char *i_fileName_target );
	void DisplayContent( FbxScene *i_scene );
	void DisplayContent( FbxNode *i_node );
}
#endif	// #ifdef DUMP_FBX_INFO

// Inherited Implementation
//=========================
bool Tools::FBXLoader::Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const
{
#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	// #ifdef _DEBUG

#ifdef DUMP_FBX_INFO
	std::string *message;
	std::ofstream dumpInfo;
	std::string *dumpInfoFilename = new std::string( i_fileName_source );

	*dumpInfoFilename = dumpInfoFilename->substr( 0, dumpInfoFilename->length() - 4 );
	*dumpInfoFilename += std::string("_dump.txt");
	dumpInfo.open( dumpInfoFilename->c_str() );
	if( dumpInfo.fail() )
	{
		message = new std::string( "Couldn't create target file " );
		*message += *dumpInfoFilename + std::string( ":  " ) + GetLastWindowsError();
		FormatAndOutputErrorMessage( *message );

		delete message;
		return false;
	}
	dumpInfo.clear();

	DisplayCommon::Initialize( *dumpInfoFilename );
#endif	// #ifdef DUMP_FBX_INFO

	FbxManager *sdkManager = NULL;
	FbxScene *scene = NULL;
	bool bCreateOctTree = false;

	std::string *sourceName = new std::string( i_fileName_source );
	if( sourceName->find("Collision") != std::string::npos || sourceName->find("collision") != std::string::npos )
		bCreateOctTree = true;

	OctreeGenerator::Initialize();

	// Prepare the FBX SDK
	Common::InitializeSDKObjects( sdkManager, scene );

	// Load the scene
	bool result = Common::LoadScene( sdkManager, scene, i_fileName_source );
	if( result )
	{
		FbxAxisSystem sceneAxisSystem = scene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem ourAxisSystem( FbxAxisSystem::eDirectX );
		if( sceneAxisSystem != ourAxisSystem )
		{
			ourAxisSystem.ConvertScene( scene );
		}

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
			LogMessage( "Success triangulate the meshes" );
		else
			LogMessage( "Failed triangulate the meshes" );

		// Split meshes per material, so that we only have one material per mesh (for VBO support)
		result = geometryConverter.SplitMeshesPerMaterial( scene, true );
		if( result )
			LogMessage( "Success splitting the meshes per material" );
		else
			LogMessage( "Failed splitting the meshes per material" );

		AssetGenerator::GenerateSceneFile( scene, i_fileName_target );

#ifdef DUMP_FBX_INFO
#ifdef DISPLAY_METADATA_INFORMATION
		DisplayMetaData( scene, i_fileName_source );
#endif	// #ifdef DISPLAY_METADATA

#ifdef DISPLAY_GLOBAL_LIGHT_SETTING_INFORMATION
		DisplayGlobalSettings::DisplayGlobalLightSettings( &scene->GetGlobalSettings() );
#endif	// #ifdef DISPLAY_GLOBAL_LIGHT_SETTING_INFORMATION

#ifdef DISPLAY_GLOBAL_CAMERA_SETTING_INFORMATION
		DisplayGlobalSettings::DisplayGlobalCameraSettings( &scene->GetGlobalSettings() );
#endif	// #ifdef DISPLAY_GLOBAL_CAMERA_SETTING_INFORMATION

#ifdef DISPLAY_GLOBAL_TIME_SETTING_INFORMATION
		DisplayGlobalSettings::DisplayGlobalTimeSettings( &scene->GetGlobalSettings() );
#endif	// #ifdef DISPLAY_GLOBAL_TIME_SETTING

#ifdef DISPLAY_HIERARCHY_INFORMATION
		DisplayHierarchy::DisplayHierarchy( scene );
#endif	// #ifdef DISPLAY_HIERARCHY

		DisplayContent( scene );

#ifdef DISPLAY_ANIMATION_INFORMATION
		DisplayAnimation::DisplayAnimation( scene );
#endif	// #ifdef DISPLAY_ANIMATION

#ifdef DISPLAY_GENERIC_INFORMATION
		DisplayGenericInfo::DisplayGenericInfo( scene );
#endif	// #ifdef DISPLAY_GENERIC_INFORMATION
#endif	// #ifdef DUMP_FBX_INFO
	}

#ifdef DUMP_FBX_INFO
	DisplayCommon::ShutDown();
#endif	// #ifdef DUMP_FBX_INFO

	if( scene )
	{
		scene->Destroy();
		scene = NULL;
	}
	Common::DestroySDKObjects( sdkManager, result );

#ifdef DUMP_FBX_INFO
	dumpInfo.close();

	if( dumpInfoFilename )
		delete dumpInfoFilename;
#endif	// #ifdef DUMP_FBX_INFO

	if( bCreateOctTree )
	{
		*sourceName = sourceName->substr( 0, sourceName->length() - 4 );
		UINT32 startPos = sourceName->find_last_of("/" );
		sourceName = new std::string( sourceName->substr(startPos+1, sourceName->size()) );
		OctreeGenerator::GenerateOctree( sourceName->c_str() );
	}

	OctreeGenerator::ShutDown();

	delete sourceName;

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif	// #ifdef _DEBUG
	return true;
}

#ifdef DUMP_FBX_INFO
#ifdef DISPLAY_METADATA
void Tools::DisplayMetaData( FbxScene *i_scene, const char *i_fileName_target )
{
	FbxDocumentInfo* sceneInfo = i_scene->GetSceneInfo();
	if( sceneInfo )
	{
		DisplayCommon::DisplayString( "\n\n--------------------\nMeta-Data\n--------------------" );
		DisplayCommon::DisplayString( "    Title: ", sceneInfo->mTitle );
		DisplayCommon::DisplayString( "    Subject: ", sceneInfo->mSubject );
		DisplayCommon::DisplayString( "    Author: ", sceneInfo->mAuthor );
		DisplayCommon::DisplayString( "    Keywords: ", sceneInfo->mKeywords );
		DisplayCommon::DisplayString( "    Revision: ", sceneInfo->mRevision );
		DisplayCommon::DisplayString( "    Comment: ", sceneInfo->mComment );

		FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
		if (thumbnail)
		{
			DisplayCommon::DisplayString( "    Thumbnail:" );

			switch( thumbnail->GetDataFormat() )
			{
			case FbxThumbnail::eRGB_24:
				DisplayCommon::DisplayString( "        Format: RGB" );
				break;
			case FbxThumbnail::eRGBA_32:
				DisplayCommon::DisplayString( "        Format: RGBA" );
				break;
			}

			switch (thumbnail->GetSize())
			{
			case FbxThumbnail::eNotSet:
				DisplayCommon::DisplayDouble( "        Size: no dimensions specified (", (double)thumbnail->GetSizeInBytes(), " bytes)" );
				break;
			case FbxThumbnail::e64x64:
				DisplayCommon::DisplayDouble( "        Size: 64 x 64 pixels (", (double)thumbnail->GetSizeInBytes(), " bytes)" );
				break;
			case FbxThumbnail::e128x128:
				DisplayCommon::DisplayDouble( "        Size: 128 x 128 pixels (", (double)thumbnail->GetSizeInBytes(), " bytes)" );
			}
		}
	}
}
#endif	// #ifdef DISPLAY_METADATA

void Tools::DisplayContent( FbxScene *i_scene )
{
	FbxNode *node = i_scene->GetRootNode();

	DisplayCommon::DisplayString( "\n\n--------------------\nNode Content\n--------------------" );

	if( node )
	{
		for( int i = 0; i < node->GetChildCount(); i++ )
		{
			DisplayContent( node->GetChild(i) );
		}
	}
}

void Tools::DisplayContent( FbxNode *i_node )
{
	FbxNodeAttribute::EType attributeType;

	if( i_node->GetNodeAttribute() == NULL )
	{
		DisplayCommon::DisplayString( "Null Node Attribute\n" );
	}
	else
	{
		attributeType = ( i_node->GetNodeAttribute()->GetAttributeType() );

		switch( attributeType )
		{
		case FbxNodeAttribute::eMarker:
			DisplayMarker::DisplayMarker( i_node );
			break;

#ifdef DISPLAY_ANIMATION_INFORMATION
		case FbxNodeAttribute::eSkeleton:
			DisplaySkeleton::DisplaySkeleton( i_node );
			break;
#endif	// #ifdef DISPLAY_ANIMATION_INFORMATION

		case FbxNodeAttribute::eMesh:
			DisplayMesh::DisplayMesh( i_node );
			break;

		case FbxNodeAttribute::eNurbs:
			DisplayNURB::DisplayNURB( i_node );
			break;

		case FbxNodeAttribute::ePatch:
			DisplayPatch::DisplayPatch( i_node );
			break;

		case FbxNodeAttribute::eCamera:
			DisplayCamera::DisplayCamera( i_node );
			break;

		case FbxNodeAttribute::eLight:
			DisplayLight::DisplayLight( i_node );
			break;

		case FbxNodeAttribute::eLODGroup:
			DisplayLODGroup::DisplayLODGroup( i_node );
			break;
		}
	}

#ifdef DISPLAY_PROPERTY_INFORMATION
	DisplayUserProperties::DisplayUserProperties( i_node );
#endif	// #ifdef DISPLAY_PROPERTY_INFORMATION
	//DisplayTarget( i_node );
#ifdef DISPLAY_PIVOT_INFORMATION
	DisplayPivotAndLimits::DisplayPivotsAndLimits( i_node );
#endif	// #ifdef DISPLAY_PIVOT_INFORMATION
	//DisplayTransformPropagation( i_node );
	//DisplayGeometricTransform( i_node );

	for( int i = 0; i < i_node->GetChildCount(); i++ )
	{
		DisplayContent( i_node->GetChild(i) );
	}
}
#endif	// #ifdef DUMP_FBX_INFO