#include "DisplayGenericInfo.h"

#if defined( DUMP_FBX_INFO ) && defined( DISPLAY_GENERIC_INFORMATION )
namespace Tools
{
	namespace DisplayGenericInfo
	{
#ifdef DISPLAY_PROPERTY_INFORMATION
		void DisplayProperties( FbxObject *i_object );
#endif	// #ifdef DISPLAY_PROPERTY_INFORMATION
		void DisplayGenericInfo( FbxNode *i_node, int i_depth );
	}
}

void Tools::DisplayGenericInfo::DisplayGenericInfo( FbxScene *i_scene )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nGeneric Info\n--------------------" );

	int i;
	FbxNode* lRootNode = i_scene->GetRootNode();

	for( i = 0; i < lRootNode->GetChildCount(); i++ )
	{
		DisplayGenericInfo( lRootNode->GetChild(i), 0 );
	}

#ifdef DISPLAY_PROPERTY_INFORMATION
	//Other objects directly connected onto the scene
	for( i = 0; i < i_scene->GetSrcObjectCount(); i++ )
	{
		DisplayProperties( i_scene->GetSrcObject(i) );
	}
#endif	// #ifdef DISPLAY_PROPERTY_INFORMATION
}

void Tools::DisplayGenericInfo::DisplayGenericInfo( FbxNode *i_node, int i_depth )
{
	FbxString string;
	int i;

	for( i = 0; i < i_depth; i++ )
	{
		string += "     ";
	}

	string += i_node->GetName();
	DisplayCommon::DisplayString( string );

	//Display generic info about that Node
#ifdef DISPLAY_PROPERTY_INFORMATION
	DisplayProperties( i_node );
#endif	// #ifdef DISPLAY_PROPERTY_INFORMATION
	DisplayCommon::DisplayString( "" );
	for( i = 0; i < i_node->GetChildCount(); i++ )
	{
		DisplayGenericInfo( i_node->GetChild(i), i_depth + 1 );
	}
}

#ifdef DISPLAY_PROPERTY_INFORMATION
void Tools::DisplayGenericInfo::DisplayProperties( FbxObject *i_object )
{
	DisplayCommon::DisplayString( "Name: ", (char *)i_object->GetName() );

	// Display all the properties
	int i, count = 0;
	FbxProperty lProperty = i_object->GetFirstProperty();
	while( lProperty.IsValid() )
	{
		count++;
		lProperty = i_object->GetNextProperty( lProperty );
	}

	FbxString lTitleStr = "    Property Count: ";

	if( count == 0 )
		return; // there are no properties to display

	DisplayCommon::DisplayInt( lTitleStr.Buffer(), count );

	i = 0;
	lProperty = i_object->GetFirstProperty();
	while( lProperty.IsValid() )
	{
		// exclude user properties
		FbxString string;
		DisplayCommon::DisplayInt( "        Property ", i );
		string = lProperty.GetLabel();
		DisplayCommon::DisplayString( "            Display Name: ", string );
		string = lProperty.GetName();
		DisplayCommon::DisplayString( "            Internal Name: ", string.Buffer() );
		string = lProperty.GetPropertyDataType().GetName();
		DisplayCommon::DisplayString( "            Type: ",string.Buffer() );
		if (lProperty.HasMinLimit())
			DisplayCommon::DisplayDouble( "            Min Limit: ", lProperty.GetMinLimit() );
		if (lProperty.HasMaxLimit())
			DisplayCommon::DisplayDouble( "            Max Limit: ", lProperty.GetMaxLimit() );
		DisplayCommon::DisplayBool( "            Is Animatable: ", lProperty.GetFlag(FbxPropertyAttr::eAnimatable) );

		switch( lProperty.GetPropertyDataType().GetType() )
		{
		case eFbxBool:
			DisplayCommon::DisplayBool( "            Default Value: ", lProperty.Get<FbxBool>() );
			break;

		case eFbxDouble:
			DisplayCommon::DisplayDouble( "            Default Value: ", lProperty.Get<FbxDouble>() );
			break;

		case eFbxDouble4:
			{
				FbxColor lDefault;
				char buffer[64];

				lDefault = lProperty.Get<FbxColor>();
				FBXSDK_sprintf( buffer, 64, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha );
				DisplayCommon::DisplayString( "            Default Value: ", buffer );
			}
			break;

		case eFbxInt:
			DisplayCommon::DisplayInt( "            Default Value: ", lProperty.Get<FbxInt>() );
			break;

		case eFbxDouble3:
			{
				FbxDouble3 lDefault;
				char buffer[64];

				lDefault = lProperty.Get<FbxDouble3>();
				FBXSDK_sprintf( buffer, 64, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2] );
				DisplayCommon::DisplayString( "            Default Value: ", buffer );
			}
			break;

			//case FbxEnumDT:
			//    DisplayInt("            Default Value: ", lProperty.Get<FbxEnum>());
			//    break;

		case eFbxFloat:
			DisplayCommon::DisplayDouble( "            Default Value: ", lProperty.Get<FbxFloat>() );
			break;

		case eFbxString:
			string = lProperty.Get<FbxString>();
			DisplayCommon::DisplayString( "            Default Value: ", string.Buffer() );
			break;

		default:
			DisplayCommon::DisplayString( "            Default Value: UNIDENTIFIED" );
			break;
		}
		i++;
		lProperty = i_object->GetNextProperty( lProperty );
	}
}
#endif	// #ifdef DISPLAY_PROPERTY_INFORMATION
#endif	// #if defined( DUMP_FBX_INFO ) && defined( DISPLAY_GENERIC_INFORMATION )