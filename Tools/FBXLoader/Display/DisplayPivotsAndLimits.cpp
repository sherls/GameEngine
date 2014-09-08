#include "DisplayPivotsAndLimits.h"

#if defined( DUMP_FBX_INFO ) && defined( DISPLAY_PIVOT_INFORMATION )
void Tools::DisplayPivotAndLimits::DisplayPivotsAndLimits( FbxNode *i_node )
{
	FbxVector4 tempVector;
	char buffer[64];

	//
	// Pivots
	//
	DisplayCommon::DisplayString( "\n\n--------------------\nPivot Information\n--------------------" );

	FbxNode::EPivotState pivotState;
	i_node->GetPivotState( FbxNode::eSourcePivot, pivotState );
	if( pivotState == FbxNode::ePivotActive )
		DisplayCommon::DisplayString( "        Pivot State: Active" );
	else
		DisplayCommon::DisplayString( "        Pivot State: Reference" );

	tempVector = i_node->GetPreRotation( FbxNode::eSourcePivot );
	FBXSDK_sprintf( buffer, 64, "        Pre-Rotation: %f %f %f", tempVector[0], tempVector[1], tempVector[2] );
	DisplayCommon::DisplayString( buffer );

	tempVector = i_node->GetPostRotation( FbxNode::eSourcePivot );
	FBXSDK_sprintf( buffer, 64, "        Post-Rotation: %f %f %f", tempVector[0], tempVector[1], tempVector[2] );
	DisplayCommon::DisplayString( buffer );

	tempVector = i_node->GetRotationPivot( FbxNode::eSourcePivot );
	FBXSDK_sprintf( buffer, 64, "        Rotation Pivot: %f %f %f", tempVector[0], tempVector[1], tempVector[2] );
	DisplayCommon::DisplayString( buffer );

	tempVector = i_node->GetRotationOffset( FbxNode::eSourcePivot );
	FBXSDK_sprintf( buffer, 64, "        Rotation Offset: %f %f %f", tempVector[0], tempVector[1], tempVector[2] );
	DisplayCommon::DisplayString( buffer );

	tempVector = i_node->GetScalingPivot( FbxNode::eSourcePivot );
	FBXSDK_sprintf( buffer, 64, "        Scaling Pivot: %f %f %f", tempVector[0], tempVector[1], tempVector[2] );
	DisplayCommon::DisplayString( buffer );

	tempVector = i_node->GetScalingOffset( FbxNode::eSourcePivot );
	FBXSDK_sprintf( buffer, 64, "        Scaling Offset: %f %f %f", tempVector[0], tempVector[1], tempVector[2] );
	DisplayCommon::DisplayString( buffer );

	//
	// Limits
	//
	bool isActive, isMinXActive, isMinYActive, isMinZActive;
	bool isMaxXActive, isMaxYActive, isMaxZActive;
	FbxDouble3	minValues, maxValues;

	DisplayCommon::DisplayString( "\n\n--------------------\nLimit Information\n--------------------" );

	isActive = i_node->TranslationActive;
	isMinXActive = i_node->TranslationMinX;
	isMinYActive = i_node->TranslationMinY;
	isMinZActive = i_node->TranslationMinZ;
	isMaxXActive = i_node->TranslationMaxX;
	isMaxYActive = i_node->TranslationMaxY;
	isMaxZActive = i_node->TranslationMaxZ;
	minValues = i_node->TranslationMin;
	maxValues = i_node->TranslationMax;

	FBXSDK_sprintf( buffer, 64, "        Translation limits: %s", isActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayString( "            X" );
	FBXSDK_sprintf( buffer, 64, "                Min Limit: %s", isMinXActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Min Limit Value: ", minValues[0] );
	FBXSDK_sprintf( buffer, 64, "                Max Limit: %s", isMaxXActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Max Limit Value: ", maxValues[0] );
	DisplayCommon::DisplayString( "            Y" );
	FBXSDK_sprintf( buffer, 64, "                Min Limit: %s", isMinYActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Min Limit Value: ", minValues[1] );
	FBXSDK_sprintf( buffer, 64, "                Max Limit: %s", isMaxYActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Max Limit Value: ", maxValues[1] );
	DisplayCommon::DisplayString( "            Z" );
	FBXSDK_sprintf( buffer, 64, "                Min Limit: %s", isMinZActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Min Limit Value: ", minValues[2] );
	FBXSDK_sprintf( buffer, 64, "                Max Limit: %s", isMaxZActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Max Limit Value: ", maxValues[2] );

	isActive = i_node->RotationActive;
	isMinXActive = i_node->RotationMinX;
	isMinYActive = i_node->RotationMinY;
	isMinZActive = i_node->RotationMinZ;
	isMaxXActive = i_node->RotationMaxX;
	isMaxYActive = i_node->RotationMaxY;
	isMaxZActive = i_node->RotationMaxZ;
	minValues = i_node->RotationMin;
	maxValues = i_node->RotationMax;

	FBXSDK_sprintf( buffer, 64, "        Rotation limits: %s", isActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayString( "            X" );
	FBXSDK_sprintf( buffer, 64, "                Min Limit: %s", isMinXActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Min Limit Value: ", minValues[0] );
	FBXSDK_sprintf( buffer, 64, "                Max Limit: %s", isMaxXActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Max Limit Value: ", maxValues[0] );
	DisplayCommon::DisplayString( "            Y" );
	FBXSDK_sprintf( buffer, "                Min Limit: %s", isMinYActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Min Limit Value: ", minValues[1] );
	FBXSDK_sprintf( buffer, 64, "                Max Limit: %s", isMaxYActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Max Limit Value: ", maxValues[1] );
	DisplayCommon::DisplayString( "            Z" );
	FBXSDK_sprintf( buffer, 64, "                Min Limit: %s", isMinZActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Min Limit Value: ", minValues[2] );
	FBXSDK_sprintf( buffer, 64, "                Max Limit: %s", isMaxZActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Max Limit Value: ", maxValues[2] );

	isActive = i_node->ScalingActive;
	isMinXActive = i_node->ScalingMinX;
	isMinYActive = i_node->ScalingMinY;
	isMinZActive = i_node->ScalingMinZ;
	isMaxXActive = i_node->ScalingMaxX;
	isMaxYActive = i_node->ScalingMaxY;
	isMaxZActive = i_node->ScalingMaxZ;
	minValues = i_node->ScalingMin;
	maxValues = i_node->ScalingMax;

	FBXSDK_sprintf( buffer, 64, "        Scaling limits: %s", isActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayString( "            X" );
	FBXSDK_sprintf( buffer, 64, "                Min Limit: %s", isMinXActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Min Limit Value: ", minValues[0] );
	FBXSDK_sprintf( buffer, 64, "                Max Limit: %s", isMaxXActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Max Limit Value: ", maxValues[0] );
	DisplayCommon::DisplayString( "            Y" );
	FBXSDK_sprintf( buffer, 64, "                Min Limit: %s", isMinYActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Min Limit Value: ", minValues[1] );
	FBXSDK_sprintf( buffer, 64, "                Max Limit: %s", isMaxYActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Max Limit Value: ", maxValues[1] );
	DisplayCommon::DisplayString( "            Z" );
	FBXSDK_sprintf( buffer, 64, "                Min Limit: %s", isMinZActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayDouble( "                Min Limit Value: ", minValues[2] );
	FBXSDK_sprintf( buffer, 64, "                Max Limit: %s", isMaxZActive ? "Active" : "Inactive" );
	DisplayCommon::DisplayString( buffer );
	DisplayCommon::DisplayDouble( "                Max Limit Value: ", maxValues[2] );
}
#endif	// #if defined( DUMP_FBX_INFO ) && defined( DISPLAY_PIVOT_INFORMATION )