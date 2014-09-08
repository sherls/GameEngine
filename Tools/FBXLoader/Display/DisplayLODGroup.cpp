#include "DisplayLODGroup.h"

#ifdef DUMP_FBX_INFO

void Tools::DisplayLODGroup::DisplayLODGroup( FbxNode *i_node )
{
	const char* displayLevels[] = { "UseLOD", "Show", "Hide" }; 

	DisplayCommon::DisplayString( "\n\n--------------------\nLOD Group\n--------------------" );
	DisplayCommon::DisplayString( "LodGroup Name: ", (char *) i_node->GetName() );

	DisplayCommon::DisplayInt( "    ", i_node->GetChildCount(), " Geometries" );
	for( int i = 0; i < i_node->GetChildCount(); i++ )
	{
		FbxNode *childNode = i_node->GetChild( i );
		DisplayCommon::DisplayString( "        ", childNode->GetName() );
	}

	FbxLODGroup *LODGroupAttr = (FbxLODGroup*)i_node->GetNodeAttribute();
	DisplayCommon::DisplayBool( "    MinMaxDistance Enabled: ", LODGroupAttr->MinMaxDistance.Get() );
	if( LODGroupAttr->MinMaxDistance.Get() )
	{
		DisplayCommon::DisplayDouble( "        Min Distance: ", LODGroupAttr->MinDistance.Get() );
		DisplayCommon::DisplayDouble( "        Max Distance: ", LODGroupAttr->MaxDistance.Get() );
	}
	DisplayCommon::DisplayBool( "    Is World Space: ", LODGroupAttr->WorldSpace.Get() );

	DisplayCommon::DisplayString( "    Thresholds " );
	for( int i = 0; i < LODGroupAttr->GetNumThresholds(); i++ )
	{
		FbxDistance threshVal;
		if( LODGroupAttr->GetThreshold(i, threshVal) )
			DisplayCommon::DisplayDouble( "        ", threshVal.value() );
	}

	DisplayCommon::DisplayString( "    DisplayLevels" );
	for( int i = 0; i < LODGroupAttr->GetNumDisplayLevels(); i++ )
	{
		FbxLODGroup::EDisplayLevel lLevel;
		if( LODGroupAttr->GetDisplayLevel(i, lLevel) )
			DisplayCommon::DisplayString( "        ", displayLevels[lLevel] );
	}
}
#endif	// #ifdef DUMP_FBX_INFO