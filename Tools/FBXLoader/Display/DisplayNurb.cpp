#include "DisplayNURB.h"

#ifdef DUMP_FBX_INFO
#include "DisplayLink.h"
#include "DisplayShape.h"
#include "DisplayTexture.h"
#include "DisplayMaterial.h"

void Tools::DisplayNURB::DisplayNURB( FbxNode *i_node )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nNURB\n--------------------" );

	FbxNurbs* NURBs = (FbxNurbs*) i_node->GetNodeAttribute ();
	int i;

	DisplayCommon::DisplayString( "Nurb Name: ", (char *) i_node->GetName() );
	DisplayCommon::DisplayMetaDataConnections( NURBs );

	const char* surfaceModes[] = { "Raw", "Low No Normals", "Low", "High No Normals", "High" };

	DisplayCommon::DisplayString( "    Surface Mode: ", surfaceModes[NURBs->GetSurfaceMode()] );

	int controlPointsCount = NURBs->GetControlPointsCount();
	FbxVector4 *controlPoints = NURBs->GetControlPoints();

	for( i = 0; i < controlPointsCount; i++ )
	{
		DisplayCommon::DisplayInt( "    Control Point ", i );
		DisplayCommon::Display3DVector( "        Coordinates: ", controlPoints[i] );
		DisplayCommon::DisplayDouble( "        Weight: ", controlPoints[i][3] );
	}

	const char* NURBTypes[] = { "Periodic", "Closed", "Open" };

	DisplayCommon::DisplayString( "    Nurb U Type: ", NURBTypes[NURBs->GetNurbsUType()] );
	DisplayCommon::DisplayInt( "    U Count: ", NURBs->GetUCount() );
	DisplayCommon::DisplayString( "    Nurb V Type: ", NURBTypes[NURBs->GetNurbsVType()] );
	DisplayCommon::DisplayInt( "    V Count: ", NURBs->GetVCount() );
	DisplayCommon::DisplayInt( "    U Order: ", NURBs->GetUOrder() );
	DisplayCommon::DisplayInt( "    V Order: ", NURBs->GetVOrder() );
	DisplayCommon::DisplayInt( "    U Step: ", NURBs->GetUStep() );
	DisplayCommon::DisplayInt( "    V Step: ", NURBs->GetVStep() );

	FbxString string;
	int UKnotCount = NURBs->GetUKnotCount();
	int VKnotCount = NURBs->GetVKnotCount();
	int UMultiplicityCount = NURBs->GetUCount();
	int VMultiplicityCount = NURBs->GetVCount();
	double* UKnotVector = NURBs->GetUKnotVector();
	double* VKnotVector = NURBs->GetVKnotVector();
	int* UMultiplicityVector = NURBs->GetUMultiplicityVector();
	int* VMultiplicityVector = NURBs->GetVMultiplicityVector();

	string = "    U Knot Vector: ";

	for( i = 0; i < UKnotCount; i++ )
	{
		string += (float) UKnotVector[i];

		if(i < UKnotCount - 1)
		{
			string += ", ";
		}
	}

	DisplayCommon::DisplayString( string );
	string += "\n";
	FBXSDK_printf( string );

	string = "    V Knot Vector: ";

	for( i = 0; i < VKnotCount; i++ )
	{
		string += (float) VKnotVector[i];

		if( i < VKnotCount - 1 )
		{
			string += ", ";
		}
	}

	DisplayCommon::DisplayString( string );
	string += "\n";
	FBXSDK_printf(string);

	string = "    U Multiplicity Vector: ";

	for( i = 0; i < UMultiplicityCount; i++ )
	{
		string += UMultiplicityVector[i];

		if( i < UMultiplicityCount - 1 )
		{
			string += ", ";
		}
	}

	DisplayCommon::DisplayString( string );
	string += "\n";
	FBXSDK_printf(string);

	string = "    V Multiplicity Vector: ";

	for( i = 0; i < VMultiplicityCount; i++ )
	{
		string += VMultiplicityVector[i];

		if( i < VMultiplicityCount - 1 )
		{
			string += ", ";
		}
	}

	DisplayCommon::DisplayString( string );
	string += "\n";
	FBXSDK_printf( string );

	//DisplayString( "" );

	DisplayTexture::DisplayTexture( NURBs );
	DisplayMaterial::DisplayMaterial( NURBs );
	DisplayLink::DisplayLink( NURBs );
	DisplayShape::DisplayShape( NURBs );
}
#endif	// #ifdef DUMP_FBX_INFO