#include "DisplayPatch.h"

#ifdef DUMP_FBX_INFO
#include "DisplayLink.h"
#include "DisplayShape.h"
#include "DisplayTexture.h"
#include "DisplayMaterial.h"

void Tools::DisplayPatch::DisplayPatch( FbxNode *i_node )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nPatch\n--------------------" );
	FbxPatch *patch = (FbxPatch*) i_node->GetNodeAttribute();

	DisplayCommon::DisplayString( "Patch Name: ", (char *) i_node->GetName() );
	DisplayCommon::DisplayMetaDataConnections( patch );

	const char* surfaceModes[] = { "Raw", "Low No Normals", "Low", "High No Normals", "High" };

	DisplayCommon::DisplayString( "    Surface Mode: ", surfaceModes[patch->GetSurfaceMode()] );

	int i, controlPointsCount = patch->GetControlPointsCount();
	FbxVector4 *controlPoints = patch->GetControlPoints();

	for( i = 0; i < controlPointsCount; i++ )
	{
		DisplayCommon::DisplayInt( "    Control Point ", i );
		DisplayCommon::Display3DVector( "        Coordinates: ", controlPoints[i] );
		DisplayCommon::DisplayDouble( "        Weight: ", controlPoints[i][3] );
	}

	const char* patchTypes[] = { "Bezier", "Bezier Quadric", "Cardinal", "B-Spline", "Linear" };

	DisplayCommon::DisplayString( "    Patch U Type: ", patchTypes[patch->GetPatchUType()] );
	DisplayCommon::DisplayInt( "    U Count: ", patch->GetUCount() );
	DisplayCommon::DisplayString( "    Patch V Type: ", patchTypes[patch->GetPatchVType()] );
	DisplayCommon::DisplayInt( "    V Count: ", patch->GetVCount() );
	DisplayCommon::DisplayInt( "    U Step: ", patch->GetUStep() );
	DisplayCommon::DisplayInt( "    V Step: ", patch->GetVStep() );
	DisplayCommon::DisplayBool( "    U Closed: ", patch->GetUClosed() );
	DisplayCommon::DisplayBool( "    V Closed: ", patch->GetVClosed() );
	DisplayCommon::DisplayBool( "    U Capped Top: ", patch->GetUCappedTop() );
	DisplayCommon::DisplayBool( "    U Capped Bottom: ", patch->GetUCappedBottom() );
	DisplayCommon::DisplayBool( "    V Capped Top: ", patch->GetVCappedTop() );
	DisplayCommon::DisplayBool( "    V Capped Bottom: ", patch->GetVCappedBottom() );

	DisplayCommon::DisplayString( "" );

	DisplayTexture::DisplayTexture( patch );
	DisplayMaterial::DisplayMaterial( patch );
	DisplayLink::DisplayLink( patch );
	DisplayShape::DisplayShape( patch );
}
#endif	// #ifdef DUMP_FBX_INFO