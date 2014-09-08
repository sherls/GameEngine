#include "DisplayPose.h"

#ifdef DUMP_FBX_INFO
void Tools::DisplayPose::DisplayPose( FbxScene *i_scene )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nPose\n--------------------" );

	int i, j, k, poseCount;
	FbxString name;

	poseCount = i_scene->GetPoseCount();

	for( i = 0; i < poseCount; i++ )
	{
		FbxPose *pose = i_scene->GetPose( i );

		name = pose->GetName();
		DisplayCommon::DisplayString( "Pose Name: ", name );

		DisplayCommon::DisplayBool( "    Is a bind pose: ", pose->IsBindPose() );

		DisplayCommon::DisplayInt( "    Number of items in the pose: ", pose->GetCount() );

		//DisplayCommon::DisplayString( "" , "" );

		for( j = 0; j < pose->GetCount(); j++ )
		{
			name = pose->GetNodeName(j).GetCurrentName();
			DisplayCommon::DisplayString( "    Item name: ", name );

			if( !pose->IsBindPose() )
			{
				// Rest pose can have local matrix
				DisplayCommon::DisplayBool( "    Is local space matrix: ", pose->IsLocalMatrix(j) );
			}

			DisplayCommon::DisplayString( "    Matrix value: ", "" );

			FbxString matrixValue;

			for( k = 0; k < 4; k++ )
			{
				FbxMatrix matrix = pose->GetMatrix( j );
				FbxVector4 row = matrix.GetRow( k );
				char rowValue[1024];

				FBXSDK_sprintf( rowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", row[0], row[1], row[2], row[3] );
				matrixValue += FbxString( "        " ) + FbxString( rowValue );
			}

			DisplayCommon::DisplayString( "", matrixValue.Buffer() );
		}
	}

	poseCount = i_scene->GetCharacterPoseCount();

	for( i = 0; i < poseCount; i++ )
	{
		FbxCharacterPose *pose = i_scene->GetCharacterPose( i );
		FbxCharacter *character = pose->GetCharacter();

		if( !character )
			break;

		DisplayCommon::DisplayString( "Character Pose Name: ", character->GetName() );

		FbxCharacterLink characterLink;
		FbxCharacter::ENodeId  nodeID = FbxCharacter::eHips;

		while( character->GetCharacterLink(nodeID, &characterLink) )
		{
			FbxAMatrix &globalPosition = characterLink.mNode->EvaluateGlobalTransform( FBXSDK_TIME_ZERO );

			DisplayCommon::DisplayString( "    Matrix value: ", "" );

			FbxString matrixValue;

			for( k = 0; k < 4; k++ )
			{
				FbxVector4 row = globalPosition.GetRow(k);
				char rowValue[1024];

				FBXSDK_sprintf( rowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", row[0], row[1], row[2], row[3] );
				matrixValue += FbxString( "        " ) + FbxString( rowValue );
			}

			DisplayCommon::DisplayString( "", matrixValue.Buffer() );

			nodeID = FbxCharacter::ENodeId( int(nodeID) + 1 );
		}
	}
}
#endif	// #ifdef DUMP_FBX_INFO