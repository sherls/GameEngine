#ifndef _DEFINE_GLOBAL_SETTINGS_H_
#define _DEFINE_GLOBAL_SETTINGS_H_

#include "DisplayCommon.h"

#ifdef DUMP_FBX_INFO
namespace Tools
{
	namespace DisplayGlobalSettings
	{
#ifdef DISPLAY_GLOBAL_LIGHT_SETTING_INFORMATION
		void DisplayGlobalLightSettings( FbxGlobalSettings* i_globalSettings );
#endif	// #ifdef DISPLAY_GLOBAL_LIGHT_SETTING_INFORMATION
#ifdef DISPLAY_GLOBAL_CAMERA_SETTING_INFORMATION
		void DisplayGlobalCameraSettings( FbxGlobalSettings* i_globalSettings);
#endif	// #ifdef DISPLAY_GLOBAL_CAMERA_SETTING_INFORMATION
#ifdef DISPLAY_GLOBAL_TIME_SETTING_INFORMATION
		void DisplayGlobalTimeSettings( FbxGlobalSettings* i_globalSettings );
#endif	// #ifdef DISPLAY_GLOBAL_TIME_SETTING_INFORMATION
	}
}
#endif	// #ifdef DUMP_FBX_INFO

#endif	// #ifndef _DEFINE_GLOBALSETTINGS_H_