#ifndef _DISPLAY_COMMON_H_
#define _DISPLAY_COMMON_H_

#include <string>
#include <fbxsdk.h>

//#define DUMP_FBX_INFO
#ifdef DUMP_FBX_INFO
	//#define DISPLAY_METADATA_INFORMATION
	//#define DISPLAY_HIERARCHY_INFORMATION
	//#define DISPLAY_GLOBAL_TIME_SETTING_INFORMATION
	//#define DISPLAY_GLOBAL_LIGHT_SETTING_INFORMATION
	//#define DISPLAY_GLOBAL_CAMERA_SETTING_INFORMATION
	//#define DISPLAY_HARDWARE_SHADER_INFORMATION
	//#define DISPLAY_PROPERTY_INFORMATION
	//#define DISPLAY_ANIMATION_INFORMATION
	//#define DISPLAY_GENERIC_INFORMATION
	#ifdef DISPLAY_ANIMATION_INFORMATION
		#define DISPLAY_PIVOT_INFORMATION
	#endif	// #ifdef DISPLAY_ANIMATION

namespace Tools
{
	namespace DisplayCommon
	{
		void Initialize( const std::string &i_dumpInfoFilename );
		void ShutDown( void );

		void DisplayMetaDataConnections( FbxObject* i_node );
		void DisplayString( const char* i_header, const char* i_value  = "", const char* i_suffix  = "" );
		void DisplayBool( const char* i_header, bool i_value, const char* i_suffix  = "" );
		void DisplayInt( const char* i_header, int i_value, const char* i_suffix  = "" );
		void DisplayDouble( const char* i_header, double i_value, const char* i_suffix  = "" );
		void Display2DVector( const char* i_header, FbxVector2 i_value, const char* i_suffix  = "" );
		void Display3DVector( const char* i_header, FbxVector4 i_value, const char* i_suffix  = "" );
		void DisplayColor( const char* i_header, FbxColor i_value, const char* i_suffix  = "" );
		void Display4DVector( const char* i_header, FbxVector4 i_value, const char* i_suffix  = "" );
	}
}
#else
namespace Tools
{
	namespace DisplayCommon
	{
		inline void Initialize( const std::string &i_dumpInfoFilename ){ }
		inline void ShutDown( void ){ }

		inline void DisplayMetaDataConnections( FbxObject* i_node ){ }
		inline void DisplayString( const char* i_header, const char* i_value  = "", const char* i_suffix  = "" ){ }
		inline void DisplayBool( const char* i_header, bool i_value, const char* i_suffix  = "" ){ }
		inline void DisplayInt( const char* i_header, int i_value, const char* i_suffix  = "" ){ }
		inline void DisplayDouble( const char* i_header, double i_value, const char* i_suffix  = "" ){ }
		inline void Display2DVector( const char* i_header, FbxVector2 i_value, const char* i_suffix  = "" ){ }
		inline void Display3DVector( const char* i_header, FbxVector4 i_value, const char* i_suffix  = "" ){ }
		inline void DisplayColor( const char* i_header, FbxColor i_value, const char* i_suffix  = "" ){ }
		inline void Display4DVector( const char* i_header, FbxVector4 i_value, const char* i_suffix  = "" ){ }
	}
}
#endif	// #ifdef DUMP_FBX_INFO

#endif	// #ifndef _DISPLAY_COMMON_H_