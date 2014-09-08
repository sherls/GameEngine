#include "DisplaySkeleton.h"

#if defined( DUMP_FBX_INFO ) && defined( DISPLAY_ANIMATION_INFORMATION )
void Tools::DisplaySkeleton::DisplaySkeleton( FbxNode* pNode )
{ 
	FbxSkeleton* skeleton = (FbxSkeleton*) pNode->GetNodeAttribute();

	DisplayCommon::DisplayString( "\n\n--------------------\nSkeleton\n--------------------" );

	DisplayCommon::DisplayString( "Skeleton Name: ", (char *) pNode->GetName() );
	DisplayCommon::DisplayMetaDataConnections( skeleton );

	const char* skeletonTypes[] = { "Root", "Limb", "Limb Node", "Effector" };

	DisplayCommon::DisplayString( "    Type: ", skeletonTypes[skeleton->GetSkeletonType()] );

	if( skeleton->GetSkeletonType() == FbxSkeleton::eLimb )
	{
		DisplayCommon::DisplayDouble( "    Limb Length: ", skeleton->LimbLength.Get() );
	}
	else if( skeleton->GetSkeletonType() == FbxSkeleton::eLimbNode )
	{
		DisplayCommon::DisplayDouble( "    Limb Node Size: ", skeleton->Size.Get() );
	}
	else if( skeleton->GetSkeletonType() == FbxSkeleton::eRoot )
	{
		DisplayCommon::DisplayDouble( "    Limb Root Size: ", skeleton->Size.Get() );
	}

	DisplayCommon::DisplayColor( "    Color: ", skeleton->GetLimbNodeColor() );
}
#endif	// #if defined( DUMP_FBX_INFO ) && defined( DISPLAY_ANIMATION_INFORMATION )