#include "DisplayCamera.h"

#ifdef DUMP_FBX_INFO

namespace Tools
{
	namespace DisplayCamera
	{
		//void DisplayCamera( FbxCamera *i_camera, char *i_name, FbxNode *i_targetNode = NULL, FbxNode *i_targetUpNode = NULL );
		void DisplayDefaultAnimationValues( FbxCamera *i_camera );
		void DisplayRenderOptions( FbxCamera *i_camera );
		void DisplayCameraViewOptions( FbxCamera *i_camera );
		void DisplayBackgroundProperties( FbxCamera *i_camera );
		void DisplayApertureAndFilmControls( FbxCamera *i_camera );
		void DisplayViewingAreaControls( FbxCamera *i_camera );
		void DisplayCameraPositionAndOrientation( FbxCamera *i_camera, FbxNode *i_targetNode, FbxNode *i_upTargetNode );
	}
}

void Tools::DisplayCamera::DisplayCamera( FbxNode *i_node )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nCamera\n--------------------" );
	DisplayCamera( (FbxCamera*) i_node->GetNodeAttribute(), (char *) i_node->GetName(), i_node->GetTarget(), i_node->GetTargetUp() );
}

void Tools::DisplayCamera::DisplayCamera( FbxCamera *i_camera, char *i_name, FbxNode *i_targetNode, FbxNode *i_targetUpNode )
{
	DisplayCommon::DisplayString( "Camera Name: ", i_name );
	if( !i_camera )
	{
		DisplayCommon::DisplayString( "NOT FOUND" );
		return;
	}
	DisplayCommon::DisplayMetaDataConnections( i_camera );

	DisplayCameraPositionAndOrientation( i_camera, i_targetNode, i_targetUpNode );

	const char* lProjectionTypes[] = { "Perspective", "Orthogonal" };

	DisplayCommon::DisplayString( "    Projection Type: ", lProjectionTypes[i_camera->ProjectionType.Get()] );

	DisplayViewingAreaControls( i_camera );

	// If camera projection type is set to FbxCamera::eOrthogonal, the 
	// aperture and film controls are not relevant.
	if( i_camera->ProjectionType.Get() != FbxCamera::eOrthogonal )
	{
		DisplayApertureAndFilmControls( i_camera );
	}

	DisplayBackgroundProperties( i_camera );
	DisplayCameraViewOptions( i_camera );
	DisplayRenderOptions( i_camera );
	DisplayDefaultAnimationValues( i_camera );
}

void Tools::DisplayCamera::DisplayCameraPositionAndOrientation( FbxCamera *i_camera, FbxNode *i_targetNode, FbxNode *i_targetUpNode )
{
	DisplayCommon::DisplayString( "    Camera Position and Orientation" );
	DisplayCommon::Display3DVector( "        Position: ", i_camera->Position.Get() );

	if( i_targetNode )
		DisplayCommon::DisplayString( "        Camera Interest: ",(char *) i_targetNode->GetName() );
	else
		DisplayCommon::Display3DVector( "        Default Camera Interest Position: ", i_camera->InterestPosition.Get() );

	if( i_targetUpNode )
		DisplayCommon::DisplayString( "        Camera Up Target: ", (char *) i_targetUpNode->GetName() );
	else
		DisplayCommon::Display3DVector( "        Up Vector: ", i_camera->UpVector.Get() );

	DisplayCommon::DisplayDouble( "        Roll: ", i_camera->Roll.Get() );
}

void Tools::DisplayCamera::DisplayViewingAreaControls( FbxCamera *i_camera )
{
	DisplayCommon::DisplayString( "    Viewing Area Controls" );

	const char* lCameraFormat[] = { "Custom", "D1 NTSC", "NTSC", "PAL", "D1 PAL", 
		"HD", "640x480", "320x200", "320x240", "128x128",
		"Full Screen" };

	DisplayCommon::DisplayString( "        Format: ", lCameraFormat[i_camera->GetFormat()] );

	const char* lAspectRatioModes[] = { "Window Size", "Fixed Ratio", "Fixed Resolution", 
		"Fixed Width", "Fixed Height" };

	DisplayCommon::DisplayString( "        Aspect Ratio Mode: ", lAspectRatioModes[i_camera->GetAspectRatioMode()] );

	// If the ratio mode is eWINDOW_SIZE, both width and height values aren't relevant.
	if( i_camera->GetAspectRatioMode() != FbxCamera::eWindowSize )
	{
		DisplayCommon::DisplayDouble( "        Aspect Width: ", i_camera->AspectWidth.Get() );
		DisplayCommon::DisplayDouble( "        Aspect Height: ", i_camera->AspectHeight.Get() );
	}

	DisplayCommon::DisplayDouble( "        Pixel Ratio: ", i_camera->PixelAspectRatio.Get() );
	DisplayCommon::DisplayDouble( "        Near Plane: ", i_camera->NearPlane.Get() );
	DisplayCommon::DisplayDouble( "        Far Plane: ", i_camera->FarPlane.Get() );
	DisplayCommon::DisplayBool( "        Mouse Lock: ", i_camera->LockMode.Get() );
}

void Tools::DisplayCamera::DisplayApertureAndFilmControls( FbxCamera *i_camera )
{
	DisplayCommon::DisplayString( "    Aperture and Film Controls" );

	const char* cameraApertureFormats[] = { "Custom", 
		"16mm Theatrical",
		"Super 16mm",
		"35mm Academy",
		"35mm TV Projection",
		"35mm Full Aperture",
		"35mm 1.85 Projection",
		"35mm Anamorphic",
		"70mm Projection",
		"VistaVision",
		"Dynavision",
		"Imax" };

	DisplayCommon::DisplayString( "        Aperture Format: ", cameraApertureFormats[i_camera->GetApertureFormat()] );

	const char* cameraApertureModes[] = { "Horizontal and Vertical", "Horizontal", "Vertical", "Focal Length" };

	DisplayCommon::DisplayString( "        Aperture Mode: ", cameraApertureModes[i_camera->GetApertureMode()] );

	DisplayCommon::DisplayDouble( "        Aperture Width: ", i_camera->GetApertureWidth(), " inches" );
	DisplayCommon::DisplayDouble( "        Aperture Height: ", i_camera->GetApertureHeight(), " inches" );
	DisplayCommon::DisplayDouble( "        Squeeze Ratio: ", i_camera->GetSqueezeRatio() );
	DisplayCommon::DisplayDouble( "        Focal Length: ", i_camera->FocalLength.Get(), "mm" );
	DisplayCommon::DisplayDouble( "        Field of View: ", i_camera->FieldOfView.Get(), " degrees" );
}

void Tools::DisplayCamera::DisplayBackgroundProperties( FbxCamera *i_camera )
{
	DisplayCommon::DisplayString( "    Background Properties" );

	DisplayCommon::DisplayString( "        Background File Name: \"", (char *) i_camera->GetBackgroundFileName(), "\"" );

	const char* backgroundDisplayModes[] = { "Disabled", "Always", "When Media" };

	DisplayCommon::DisplayString( "        Background Display Mode: ", backgroundDisplayModes[i_camera->ViewFrustumBackPlaneMode.Get()] );

	DisplayCommon::DisplayBool( "        Foreground Matte Threshold Enable: ", i_camera->ShowFrontplate.Get() );

	// This option is only relevant if background drawing mode is set to eFOREGROUND or eBACKGROUND_AND_FOREGROUND.
	if( i_camera->ForegroundOpacity.Get() )
	{
		DisplayCommon::DisplayDouble( "        Foreground Matte Threshold: ", i_camera->BackgroundAlphaTreshold.Get() );
	}

	FbxString backgroundPlacementOptions;

	if( i_camera->GetBackPlateFitImage() )
	{
		backgroundPlacementOptions += " Fit,";
	}
	if( i_camera->GetBackPlateCenter() )
	{
		backgroundPlacementOptions += " Center,";
	}
	if( i_camera->GetBackPlateKeepRatio() )
	{
		backgroundPlacementOptions += " Keep Ratio,";
	}
	if( i_camera->GetBackPlateCrop() )
	{
		backgroundPlacementOptions += " Crop,";
	}
	if( !backgroundPlacementOptions.IsEmpty() )
	{
		FbxString lString =  backgroundPlacementOptions.Left( backgroundPlacementOptions.GetLen() - 1 );
		DisplayCommon::DisplayString( "        Background Placement Options: ",lString.Buffer() );
	}

	DisplayCommon::DisplayDouble( "        Background Distance: ", i_camera->BackPlaneDistance.Get() );

	const char* cameraBackgroundDistanceModes[] = { "Relative to Interest", "Absolute from Camera" };

	DisplayCommon::DisplayString( "        Background Distance Mode: ", cameraBackgroundDistanceModes[i_camera->BackPlaneDistanceMode.Get()] );
}

void Tools::DisplayCamera::DisplayCameraViewOptions( FbxCamera *i_camera )
{
	DisplayCommon::DisplayString( "    Camera View Options" );

	DisplayCommon::DisplayBool( "        View Camera Interest: ", i_camera->ViewCameraToLookAt.Get() );
	DisplayCommon::DisplayBool( "        View Near Far Planes: ", i_camera->ViewFrustumNearFarPlane.Get() );
	DisplayCommon::DisplayBool( "        Show Grid: ", i_camera->ShowGrid.Get() );
	DisplayCommon::DisplayBool( "        Show Axis: ", i_camera->ShowAzimut.Get() );
	DisplayCommon::DisplayBool( "        Show Name: ", i_camera->ShowName.Get() );
	DisplayCommon::DisplayBool( "        Show Info on Moving: ", i_camera->ShowInfoOnMoving.Get() );
	DisplayCommon::DisplayBool( "        Show Time Code: ", i_camera->ShowTimeCode.Get()) ;
	DisplayCommon::DisplayBool( "        Display Safe Area: ", i_camera->DisplaySafeArea.Get() );

	const char* lSafeAreaStyles[] = { "Round", "Square" };
	FbxColor color;
	FbxDouble3 c;

	DisplayCommon::DisplayString( "        Safe Area Style: ", lSafeAreaStyles[i_camera->SafeAreaDisplayStyle.Get()] );
	DisplayCommon::DisplayBool( "        Show Audio: ", i_camera->ShowAudio.Get() );

	c = i_camera->BackgroundColor.Get();
	color = FbxColor( c[0], c[1], c[2] );
	DisplayCommon::DisplayColor( "        Background Color: ", color );

	c = i_camera->AudioColor.Get();
	color = FbxColor( c[0], c[1], c[2] );
	DisplayCommon::DisplayColor( "        Audio Color: ", color );

	DisplayCommon::DisplayBool( "        Use Frame Color: ", i_camera->UseFrameColor.Get() );

	c = i_camera->FrameColor.Get();
	color = FbxColor( c[0], c[1], c[2] );
	DisplayCommon::DisplayColor( "        Frame Color: ", color );
}

void Tools::DisplayCamera::DisplayRenderOptions( FbxCamera *i_camera )
{
	DisplayCommon::DisplayString( "    Render Options" );

	const char* cameraRenderOptionsUsageTimes[] = { "Interactive", "At Render" };

	DisplayCommon::DisplayString( "        Render Options Usage Time: ", cameraRenderOptionsUsageTimes[i_camera->UseRealTimeDOFAndAA.Get()] );
	DisplayCommon::DisplayBool( "        Use Antialiasing: ", i_camera->UseAntialiasing.Get() );
	DisplayCommon::DisplayDouble( "        Antialiasing Intensity: ", i_camera->AntialiasingIntensity.Get() );

	const char* lCameraAntialiasingMethods[] = { "Oversampling Antialiasing", "Hardware Antialiasing" };

	DisplayCommon::DisplayString( "        Antialiasing Method: ", lCameraAntialiasingMethods[i_camera->AntialiasingMethod.Get()] );

	// This option is only relevant if antialiasing method is set to eOVERSAMPLING_ANTIALIASING.
	if( i_camera->AntialiasingMethod.Get() == FbxCamera::eAAOversampling )
	{
		DisplayCommon::DisplayInt( "        Number of Samples: ", i_camera->FrameSamplingCount.Get() );
	}

	const char* cameraSamplingTypes[] = { "Uniform", "Stochastic" };

	DisplayCommon::DisplayString( "        Sampling Type: ", cameraSamplingTypes[i_camera->FrameSamplingType.Get()] );
	DisplayCommon::DisplayBool( "        Use Accumulation Buffer: ", i_camera->UseAccumulationBuffer.Get() );
	DisplayCommon::DisplayBool( "        Use Depth of Field: ", i_camera->UseDepthOfField.Get() );

	const char* cameraFocusDistanceSources[] = { "Camera Interest", "Specific Distance" };

	DisplayCommon::DisplayString( "        Focus Distance Source: ", cameraFocusDistanceSources[i_camera->FocusSource.Get()] );

	// This parameter is only relevant if focus distance source is set to eSPECIFIC_DISTANCE.
	if( i_camera->FocusSource.Get() == FbxCamera::eFocusSpecificDistance )
	{
		DisplayCommon::DisplayDouble( "        Specific Distance: ", i_camera->FocusDistance.Get() );
	}

	DisplayCommon::DisplayDouble( "        Focus Angle: ", i_camera->FocusAngle.Get(), " degrees" );
}

void Tools::DisplayCamera::DisplayDefaultAnimationValues( FbxCamera *i_camera)
{
	DisplayCommon::DisplayString( "    Default Animation Values" );

	DisplayCommon::DisplayDouble( "        Default Field of View: ", i_camera->FieldOfView.Get() );
	DisplayCommon::DisplayDouble( "        Default Field of View X: ", i_camera->FieldOfViewX.Get() );
	DisplayCommon::DisplayDouble( "        Default Field of View Y: ", i_camera->FieldOfViewY.Get() );
	DisplayCommon::DisplayDouble( "        Default Optical Center X: ", i_camera->OpticalCenterX.Get() );
	DisplayCommon::DisplayDouble( "        Default Optical Center Y: ", i_camera->OpticalCenterY.Get() );
	DisplayCommon::DisplayDouble( "        Default Roll: ", i_camera->Roll.Get() );
}
#endif	// #ifdef DUMP_FBX_INFO