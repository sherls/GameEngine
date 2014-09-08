#include "DisplayMarker.h"

#ifdef DUMP_FBX_INFO

void Tools::DisplayMarker::DisplayMarker( FbxNode* i_node )
{
	FbxMarker *marker = (FbxMarker*)i_node->GetNodeAttribute();
	FbxString string;

	DisplayCommon::DisplayString( "\n\n--------------------\nMarker\n--------------------" );
	DisplayCommon::DisplayString( "Marker Name: ", (char *) i_node->GetName() );
	DisplayCommon::DisplayMetaDataConnections( marker );

	// Type
	string = "    Marker Type: ";
	switch( marker->GetType() )
	{
	case FbxMarker::eStandard:
		string += "Standard";					break;
	case FbxMarker::eOptical:
		string += "Optical";					break;
	case FbxMarker::eEffectorIK:
		string += "IK Effector";			break;
	case FbxMarker::eEffectorFK:
		string += "FK Effector";			break;
	}
	DisplayCommon::DisplayString( string );

	// Look
	string = "    Marker Look: ";
	switch( marker->Look.Get() )
	{
	case FbxMarker::eCube:
		string += "Cube";							break;
	case FbxMarker::eHardCross:
		string += "Hard Cross";				break;
	case FbxMarker::eLightCross:
		string += "Light Cross";			break;
	case FbxMarker::eSphere:
		string += "Sphere";						break;
	}
	DisplayCommon::DisplayString( string );

	// Size
	string = FbxString( "    Size: " ) + FbxString( marker->Size.Get() );
	DisplayCommon::DisplayString( string );

	// Color
	FbxDouble3 c = marker->Color.Get();
	FbxColor color( c[0], c[1], c[2] );
	DisplayCommon::DisplayColor( "    Color: ", color );

	// IKPivot
	DisplayCommon::Display3DVector( "    IKPivot: ", marker->IKPivot.Get() );
}
#endif	// #ifdef DUMP_FBX_INFO