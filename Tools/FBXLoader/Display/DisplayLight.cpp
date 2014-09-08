#include "DisplayLight.h"

#ifdef DUMP_FBX_INFO

namespace Tools
{
	namespace DisplayLight
	{
		void DisplayDefaultAnimationValues( FbxLight *i_light );
	}
}

void Tools::DisplayLight::DisplayLight( FbxNode* i_node )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nLight\n--------------------" );

	FbxLight *light = (FbxLight*) i_node->GetNodeAttribute();

	DisplayCommon::DisplayString( "Light Name: ", (char *) i_node->GetName() );
	DisplayCommon::DisplayMetaDataConnections(light);

	const char *lightTypes[] = { "Point", "Directional", "Spot" };

	DisplayCommon::DisplayString( "    Type: ", lightTypes[light->LightType.Get()] );
	DisplayCommon::DisplayBool( "    Cast Light: ", light->CastLight.Get() );

	if( !(light->FileName.Get().IsEmpty()) )
	{
		DisplayCommon::DisplayString( "    Gobo" );

		DisplayCommon::DisplayString( "        File Name: \"", light->FileName.Get().Buffer(), "\"" );
		DisplayCommon::DisplayBool( "        Ground Projection: ", light->DrawGroundProjection.Get() );
		DisplayCommon::DisplayBool( "        Volumetric Projection: ", light->DrawVolumetricLight.Get() );
		DisplayCommon::DisplayBool( "        Front Volumetric Projection: ", light->DrawFrontFacingVolumetricLight.Get() );
	}

	DisplayDefaultAnimationValues( light );
}

void Tools::DisplayLight::DisplayDefaultAnimationValues( FbxLight *i_light )
{
	DisplayCommon::DisplayString( "    Default Animation Values" );

	FbxDouble3 c = i_light->Color.Get();
	FbxColor lColor( c[0], c[1], c[2] );
	DisplayCommon::DisplayColor( "        Default Color: ", lColor );
	DisplayCommon::DisplayDouble( "        Default Intensity: ", i_light->Intensity.Get() );
	DisplayCommon::DisplayDouble( "        Default Outer Angle: ", i_light->OuterAngle.Get() );
	DisplayCommon::DisplayDouble( "        Default Fog: ", i_light->Fog.Get() );
}
#endif	// #ifdef DUMP_FBX_INFO