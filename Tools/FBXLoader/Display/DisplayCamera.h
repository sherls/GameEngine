#ifndef _DISPLAY_CAMERA_H_
#define _DISPLAY_CAMERA_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayCamera
	{
#ifdef DUMP_FBX_INFO
		void DisplayCamera( FbxNode* i_node );
		void DisplayCamera( FbxCamera* i_camera, char* i_name, FbxNode* i_targetNode = NULL, FbxNode* i_targetUI_node = NULL );
		void DisplayDefaultAnimationValues( FbxCamera* i_camera );
		void DisplayRenderOptions( FbxCamera* i_camera );
		void DisplayCameraViewOptions( FbxCamera* i_camera );
		void DisplayBackgroundProperties( FbxCamera* i_camera );
		void DisplayApertureAndFilmControls( FbxCamera* i_camera );
		void DisplayViewingAreaControls( FbxCamera* i_camera );
		void DisplayCameraPositionAndOrientation( FbxCamera* i_camera, FbxNode* i_targetNode, FbxNode* i_UI_targetNode );
#else
		inline void DisplayCamera( FbxNode* i_node ){ }
		inline void DisplayCamera( FbxCamera* i_camera, char* i_name, FbxNode* i_targetNode = NULL, FbxNode* i_targetUI_node = NULL ){ }
		inline void DisplayDefaultAnimationValues( FbxCamera* i_camera ){ }
		inline void DisplayRenderOptions( FbxCamera* i_camera ){ }
		inline void DisplayCameraViewOptions( FbxCamera* i_camera ){ }
		inline void DisplayBackgroundProperties( FbxCamera* i_camera ){ }
		inline void DisplayApertureAndFilmControls( FbxCamera* i_camera ){ }
		inline void DisplayViewingAreaControls( FbxCamera* i_camera ){ }
		inline void DisplayCameraPositionAndOrientation( FbxCamera* i_camera, FbxNode* i_targetNode, FbxNode* i_UI_targetNode ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif	// #ifndef _DISPLAY_CAMERA_H_