#include "Common.h"
#include "Display/DisplayCommon.h"

void Tools::Common::InitializeSDKObjects( FbxManager*& i_manager, FbxScene*& i_scene )
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	i_manager = FbxManager::Create();
	if( !i_manager )
	{
		FBXSDK_printf( "Error: Unable to create FBX Manager!\n" );
		exit( 1 );
	}
	else
		FBXSDK_printf( "Autodesk FBX SDK version %s\n", i_manager->GetVersion() );

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create( i_manager, IOSROOT );
	i_manager->SetIOSettings( ios );

	//Load plugins from the executable directory (optional)
	FbxString path = FbxGetApplicationDirectory();
	i_manager->LoadPluginsDirectory( path.Buffer() );

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	i_scene = FbxScene::Create( i_manager, "My Scene" );
	if( !i_scene )
	{
		FBXSDK_printf( "Error: Unable to create FBX scene!\n" );
		exit( 1 );
	}
}

void Tools::Common::DestroySDKObjects( FbxManager *i_manager, bool i_bExitStatus )
{
	//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
	if( i_manager )
	{
		i_manager->GetIOSettings()->Destroy();
		i_manager->Destroy();
		i_manager = NULL;
	}

	if( i_bExitStatus )
		FBXSDK_printf("Program Success!\n");
}

bool Tools::Common::SaveScene( FbxManager* i_manager, FbxDocument* i_scene, const char* i_filename, int i_fileformat, bool i_bEmbedMedia )
{
	int lMajor, lMinor, lRevision;
	bool lStatus = true;

	// Create an exporter.
	FbxExporter* lExporter = FbxExporter::Create(i_manager, "");

	if( i_fileformat < 0 || i_fileformat >= i_manager->GetIOPluginRegistry()->GetWriterFormatCount() )
	{
		// Write in fall back format in less no ASCII format found
		i_fileformat = i_manager->GetIOPluginRegistry()->GetNativeWriterFormat();

		//Try to export in ASCII if possible
		int lFormatIndex, lFormatCount = i_manager->GetIOPluginRegistry()->GetWriterFormatCount();

		for (lFormatIndex=0; lFormatIndex<lFormatCount; lFormatIndex++)
		{
			if (i_manager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
			{
				FbxString lDesc =i_manager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
				const char *lASCII = "ascii";
				if (lDesc.Find(lASCII)>=0)
				{
					i_fileformat = lFormatIndex;
					break;
				}
			}
		} 
	}

	// Set the export states. By default, the export states are always set to 
	// true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below 
	// shows how to change these states.
	i_manager->GetIOSettings()->SetBoolProp(EXP_FBX_MATERIAL,        true);
	i_manager->GetIOSettings()->SetBoolProp(EXP_FBX_TEXTURE,         true);
	i_manager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED,        i_bEmbedMedia);
	i_manager->GetIOSettings()->SetBoolProp(EXP_FBX_SHAPE,           true);
	i_manager->GetIOSettings()->SetBoolProp(EXP_FBX_GOBO,            true);
	i_manager->GetIOSettings()->SetBoolProp(EXP_FBX_ANIMATION,       true);
	i_manager->GetIOSettings()->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	// Initialize the exporter by providing a filename.
	if(lExporter->Initialize(i_filename, i_fileformat, i_manager->GetIOSettings()) == false)
	{
		FBXSDK_printf("Call to FbxExporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
		return false;
	}

	FbxManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
	FBXSDK_printf("FBX file format version %d.%d.%d\n\n", lMajor, lMinor, lRevision);

	// Export the scene.
	lStatus = lExporter->Export(i_scene); 

	// Destroy the exporter.
	lExporter->Destroy();
	return lStatus;
}

bool Tools::Common::LoadScene( FbxManager* i_manager, FbxDocument* i_scene, const char* i_filename )
{
	int fileMajor, fileMinor, fileRevision;
	int SDKMajor, SDKMinor, SDKRevision;
	bool status;
	char password[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion( SDKMajor, SDKMinor, SDKRevision );

	// Create an importer.
	FbxImporter* importer = FbxImporter::Create( i_manager, "" );

	// Initialize the importer by providing a filename.
	const bool importStatus = importer->Initialize( i_filename, -1, i_manager->GetIOSettings() );
	importer->GetFileVersion( fileMajor, fileMinor, fileRevision );

	if( !importStatus )
	{
		FbxString error = importer->GetStatus().GetErrorString();
		FBXSDK_printf( "Call to FbxImporter::Initialize() failed.\n" );
		FBXSDK_printf( "Error returned: %s\n\n", error.Buffer() );

		if( importer->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion )
		{
			FBXSDK_printf( "FBX file format version for this FBX SDK is %d.%d.%d\n", SDKMajor, SDKMinor, SDKRevision );
			FBXSDK_printf( "FBX file format version for file '%s' is %d.%d.%d\n\n", i_filename, fileMajor, fileMinor, fileRevision );
		}

		return false;
	}

	FBXSDK_printf( "FBX file format version for this FBX SDK is %d.%d.%d\n", SDKMajor, SDKMinor, SDKRevision );

	if( importer->IsFBX() )
	{
		FBXSDK_printf( "FBX file format version for file '%s' is %d.%d.%d\n\n", i_filename, fileMajor, fileMinor, fileRevision );

#ifdef DISPLAY_ANIMATION_INFORMATION
		int animStackCount;

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.
		DisplayCommon::DisplayString( "--------------------\nAnimation Stack Information\n--------------------" );

		animStackCount = importer->GetAnimStackCount();

		DisplayCommon::DisplayInt( "    Number of Animation Stacks: ", animStackCount );
		DisplayCommon::DisplayString( "    Current Animation Stack: ", importer->GetActiveAnimStackName() );

		for( int i = 0; i < animStackCount; i++ )
		{
			FbxTakeInfo *takeInfo = importer->GetTakeInfo( i );

			DisplayCommon::DisplayInt( "    Animation Stack ", i );
			DisplayCommon::DisplayString( "         Name: ", takeInfo->mName );
			DisplayCommon::DisplayString( "         Description: ", takeInfo->mDescription );

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			DisplayCommon::DisplayString( "         Import Name: ", takeInfo->mImportName );

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			if( takeInfo->mSelect )
				DisplayCommon::DisplayString( "         Import State: true\n" );
			else
				DisplayCommon::DisplayString( "         Import State: false\n" );
		}
#endif	// #ifdef DISPLAY_ANIMATION

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		i_manager->GetIOSettings()->SetBoolProp( IMP_FBX_MATERIAL,        true );
		i_manager->GetIOSettings()->SetBoolProp( IMP_FBX_TEXTURE,         true );
		i_manager->GetIOSettings()->SetBoolProp( IMP_FBX_LINK,            true );
		i_manager->GetIOSettings()->SetBoolProp( IMP_FBX_SHAPE,           true );
		i_manager->GetIOSettings()->SetBoolProp( IMP_FBX_GOBO,            true );
		i_manager->GetIOSettings()->SetBoolProp( IMP_FBX_ANIMATION,       true );
		i_manager->GetIOSettings()->SetBoolProp( IMP_FBX_GLOBAL_SETTINGS, true );
	}

	// Import the scene.
	status = importer->Import( i_scene );

	if( status == false && importer->GetStatus().GetCode() == FbxStatus::ePasswordError )
	{
		FBXSDK_printf( "Please enter password: " );

		password[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf( "%s", password );
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(password);

		i_manager->GetIOSettings()->SetStringProp( IMP_FBX_PASSWORD, lString );
		i_manager->GetIOSettings()->SetBoolProp( IMP_FBX_PASSWORD_ENABLE, true );

		status = importer->Import( i_scene );

		if( status == false && importer->GetStatus().GetCode() == FbxStatus::ePasswordError )
		{
			FBXSDK_printf( "\nPassword is wrong, import aborted.\n" );
		}
	}

	// Destroy the importer.
	importer->Destroy();
	importer = NULL;

	return status;
}
