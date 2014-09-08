#include "DisplayHierarchy.h"

#ifdef DUMP_FBX_INFO

void Tools::DisplayHierarchy::DisplayHierarchy( FbxScene *i_scene )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nHierarchy\n--------------------" );

	FbxNode* rootNode = i_scene->GetRootNode();

	for( int i = 0; i < rootNode->GetChildCount(); i++ )
	{
		DisplayHierarchy( rootNode->GetChild(i), 0 );
	}
}

void Tools::DisplayHierarchy::DisplayHierarchy( FbxNode* i_node, int i_depth )
{
	FbxString string;

	for( int i = 0; i < i_depth; i++ )
	{
		string += "     ";
	}

	string += i_node->GetName();

	DisplayCommon::DisplayString( string );

	for( int i = 0; i < i_node->GetChildCount(); i++ )
	{
		DisplayHierarchy( i_node->GetChild(i), i_depth + 1 );
	}
}
#endif	// #ifdef DUMP_FBX_INFO