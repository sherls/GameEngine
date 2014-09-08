#include "DisplayGlobalSettings.h"

#ifdef DUMP_FBX_INFO

#ifdef DISPLAY_GLOBAL_LIGHT_SETTING_INFORMATION
void Tools::DisplayGlobalSettings::DisplayGlobalLightSettings( FbxGlobalSettings *i_globalSettings )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nGlobal light\n--------------------" );
	DisplayCommon::DisplayColor( "Ambient Color: ", i_globalSettings->GetAmbientColor() );
}
#endif	// #ifdef DISPLAY_GLOBAL_LIGHT_SETTING_INFORMATION

#ifdef DISPLAY_GLOBAL_CAMERA_SETTING_INFORMATION
void Tools::DisplayGlobalSettings::DisplayGlobalCameraSettings( FbxGlobalSettings *i_globalSettings )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nGlobal Camera\n--------------------" );
	DisplayCommon::DisplayString( "Default Camera: ", i_globalSettings->GetDefaultCamera() );
}
#endif	// #ifdef DISPLAY_GLOBAL_CAMERA_SETTING_INFORMATION

#ifdef DISPLAY_GLOBAL_TIME_SETTING_INFORMATION
void Tools::DisplayGlobalSettings::DisplayGlobalTimeSettings( FbxGlobalSettings *i_globalSettings )
{
	char timeString[256];

	DisplayCommon::DisplayString( "\n\n--------------------\nGlobal Time\n--------------------" );
	DisplayCommon::DisplayString( "Time Mode : ", FbxGetTimeModeName(i_globalSettings->GetTimeMode()) );

	FbxTimeSpan lTs;
	FbxTime lStart, lEnd;
	i_globalSettings->GetTimelineDefaultTimeSpan(lTs);
	lStart = lTs.GetStart();
	lEnd = lTs.GetStop();
	DisplayCommon::DisplayString( "Timeline default timespan: " );
	DisplayCommon::DisplayString( "     Start: ", lStart.GetTimeString(timeString, FbxUShort(256)) );
	DisplayCommon::DisplayString( "     Stop : ", lEnd.GetTimeString(timeString, FbxUShort(256))) ;
}
#endif	// #ifdef DISPLAY_GLOBAL_TIME_SETTING_INFORMATION
#endif	// #ifdef DUMP_FBX_INFO