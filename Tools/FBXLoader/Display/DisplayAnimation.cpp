#include "DisplayAnimation.h"

#if defined( DUMP_FBX_INFO ) && defined( DISPLAY_ANIMATION_INFORMATION )
namespace Tools
{
	namespace DisplayAnimation
	{
		void DisplayAnimation( FbxAnimStack *i_animStack, FbxNode *i_node, bool isSwitcher = false );
		void DisplayAnimation( FbxAnimLayer *i_animLayer, FbxNode *i_node, bool isSwitcher = false );
		void DisplayChannels( FbxNode *i_node, FbxAnimLayer *i_animLayer, void (*DisplayCurve) (FbxAnimCurve* i_curve), void (*DisplayListCurve) (FbxAnimCurve* i_curve, FbxProperty* i_property), bool isSwitcher );
		void DisplayCurveKeys( FbxAnimCurve *i_curve );
		void DisplayListCurveKeys( FbxAnimCurve *i_curve, FbxProperty *i_property );
	}
}

void Tools::DisplayAnimation::DisplayAnimation(FbxScene* i_scene)
{
	DisplayCommon::DisplayString( "\n\n--------------------\nAnimation\n--------------------" );

	int i;
	for ( i = 0; i < i_scene->GetSrcObjectCount<FbxAnimStack>(); i++ )
	{
		FbxAnimStack *animStack = i_scene->GetSrcObject<FbxAnimStack>( i );

		FbxString string = "Animation Stack Name: ";
		string += animStack->GetName();
		string += "\n";
		FBXSDK_printf( string );
		DisplayCommon::DisplayString( string );

		DisplayAnimation( animStack, i_scene->GetRootNode(), true );
		DisplayAnimation( animStack, i_scene->GetRootNode() );
	}
}

void Tools::DisplayAnimation::DisplayAnimation( FbxAnimStack* pAnimStack, FbxNode* i_node, bool isSwitcher )
{
	int l;
	int totalAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
	FbxString string;

	string = "Animation stack contains ";
	string += totalAnimLayers;
	string += " Animation Layer(s)";
	FBXSDK_printf( string );
	DisplayCommon::DisplayString( string );

	for( l = 0; l < totalAnimLayers; l++ )
	{
		FbxAnimLayer *animLayer = pAnimStack->GetMember<FbxAnimLayer>( l );

		string = "AnimLayer ";
		string += l;
		FBXSDK_printf( string );
		DisplayCommon::DisplayString( string );

		DisplayAnimation( animLayer, i_node, isSwitcher );
	}
}

void Tools::DisplayAnimation::DisplayAnimation( FbxAnimLayer* i_animLayer, FbxNode* i_node, bool isSwitcher )
{
	int modecount;
	FbxString string;

	string = "    Node Name: ";
	string += i_node->GetName();
	string += "\n";
	FBXSDK_printf( string );
	DisplayCommon::DisplayString( string );

	DisplayChannels( i_node, i_animLayer, DisplayCurveKeys, DisplayListCurveKeys, isSwitcher );
	FBXSDK_printf ("\n");

	for( modecount = 0; modecount < i_node->GetChildCount(); modecount++ )
	{
		DisplayAnimation( i_animLayer, i_node->GetChild(modecount), isSwitcher );
	}
}

void Tools::DisplayAnimation::DisplayChannels( FbxNode* i_node, FbxAnimLayer* i_animLayer, void (*DisplayCurve) (FbxAnimCurve* i_curve), void (*DisplayListCurve) (FbxAnimCurve* i_curve, FbxProperty* i_property), bool isSwitcher )
{
	FbxAnimCurve *animCurve = NULL;

	// Display general curves.
	if (!isSwitcher)
	{
		animCurve = i_node->LclTranslation.GetCurve( i_animLayer, FBXSDK_CURVENODE_COMPONENT_X );
		if( animCurve )
		{
			FBXSDK_printf( "        TX\n" );
			DisplayCommon::DisplayString( "        TX" );
			DisplayCurve( animCurve );
		}
		animCurve = i_node->LclTranslation.GetCurve( i_animLayer, FBXSDK_CURVENODE_COMPONENT_Y );
		if( animCurve )
		{
			FBXSDK_printf( "        TY\n" );
			DisplayCommon::DisplayString( "        TY" );
			DisplayCurve( animCurve );
		}
		animCurve = i_node->LclTranslation.GetCurve( i_animLayer, FBXSDK_CURVENODE_COMPONENT_Z );
		if( animCurve )
		{
			FBXSDK_printf( "        TZ\n" );
			DisplayCommon::DisplayString( "        TZ" );
			DisplayCurve( animCurve );
		}

		animCurve = i_node->LclRotation.GetCurve( i_animLayer, FBXSDK_CURVENODE_COMPONENT_X );
		if( animCurve )
		{
			FBXSDK_printf( "        RX\n" );
			DisplayCommon::DisplayString( "        RX" );
			DisplayCurve( animCurve );
		}
		animCurve = i_node->LclRotation.GetCurve( i_animLayer, FBXSDK_CURVENODE_COMPONENT_Y );
		if( animCurve )
		{
			FBXSDK_printf( "        RY\n" );
			DisplayCommon::DisplayString( "        RY" );
			DisplayCurve( animCurve );
		}
		animCurve = i_node->LclRotation.GetCurve( i_animLayer, FBXSDK_CURVENODE_COMPONENT_Z );
		if( animCurve )
		{
			FBXSDK_printf( "        RZ\n" );
			DisplayCommon::DisplayString( "        RZ" );
			DisplayCurve( animCurve );
		}

		animCurve = i_node->LclScaling.GetCurve( i_animLayer, FBXSDK_CURVENODE_COMPONENT_X );
		if( animCurve )
		{
			FBXSDK_printf( "        SX\n" );
			DisplayCommon::DisplayString( "        SX" );
			DisplayCurve( animCurve );
		}
		animCurve = i_node->LclScaling.GetCurve( i_animLayer, FBXSDK_CURVENODE_COMPONENT_Y );
		if( animCurve )
		{
			FBXSDK_printf( "        SY\n" );
			DisplayCommon::DisplayString( "        SY" );
			DisplayCurve( animCurve );
		}
		animCurve = i_node->LclScaling.GetCurve( i_animLayer, FBXSDK_CURVENODE_COMPONENT_Z );
		if( animCurve )
		{
			FBXSDK_printf( "        SZ\n" );
			DisplayCommon::DisplayString( "        SZ" );
			DisplayCurve( animCurve );
		}
	}

	// Display curves specific to a light or marker.
	FbxNodeAttribute *nodeAttribute = i_node->GetNodeAttribute();

	if( nodeAttribute )
	{
		animCurve = nodeAttribute->Color.GetCurve( i_animLayer, FBXSDK_CURVENODE_COLOR_RED );
		if( animCurve )
		{
			FBXSDK_printf( "        Red\n" );
			DisplayCommon::DisplayString( "        Red" );
			DisplayCurve( animCurve );
		}
		animCurve = nodeAttribute->Color.GetCurve( i_animLayer, FBXSDK_CURVENODE_COLOR_GREEN );
		if( animCurve )
		{
			FBXSDK_printf( "        Green\n" );
			DisplayCommon::DisplayString( "        Green" );
			DisplayCurve( animCurve );
		}
		animCurve = nodeAttribute->Color.GetCurve( i_animLayer, FBXSDK_CURVENODE_COLOR_BLUE );
		if( animCurve )
		{
			FBXSDK_printf( "        Blue\n" );
			DisplayCommon::DisplayString( "        Blue" );
			DisplayCurve( animCurve );
		}

		// Display curves specific to a light.
		FbxLight *light = i_node->GetLight();
		if( light )
		{
			animCurve = light->Intensity.GetCurve( i_animLayer );
			if( animCurve )
			{
				FBXSDK_printf( "        Intensity\n" );
				DisplayCommon::DisplayString( "        Intensity" );
				DisplayCurve( animCurve );
			}

			animCurve = light->OuterAngle.GetCurve( i_animLayer );
			if( animCurve )
			{
				FBXSDK_printf( "        Outer Angle\n" );
				DisplayCommon::DisplayString( "        Outer Angle" );
				DisplayCurve( animCurve );
			}

			animCurve = light->Fog.GetCurve( i_animLayer );
			if( animCurve )
			{
				FBXSDK_printf( "        Fog\n" );
				DisplayCommon::DisplayString( "        Fog" );
				DisplayCurve( animCurve );
			}
		}

		// Display curves specific to a camera.
		FbxCamera *camera = i_node->GetCamera();
		if( camera )
		{
			animCurve = camera->FieldOfView.GetCurve( i_animLayer );
			if( animCurve )
			{
				FBXSDK_printf( "        Field of View\n" );
				DisplayCommon::DisplayString( "        Field of View" );
				DisplayCurve( animCurve );
			}

			animCurve = camera->FieldOfViewX.GetCurve( i_animLayer );
			if( animCurve )
			{
				FBXSDK_printf( "        Field of View X\n" );
				DisplayCommon::DisplayString( "        Field of View X" );
				DisplayCurve( animCurve );
			}

			animCurve = camera->FieldOfViewY.GetCurve( i_animLayer );
			if( animCurve )
			{
				FBXSDK_printf( "        Field of View Y\n" );
				DisplayCommon::DisplayString( "        Field of View Y" );
				DisplayCurve( animCurve );
			}

			animCurve = camera->OpticalCenterX.GetCurve( i_animLayer );
			if( animCurve )
			{
				FBXSDK_printf( "        Optical Center X\n" );
				DisplayCommon::DisplayString( "        Optical Center X" );
				DisplayCurve( animCurve );
			}

			animCurve = camera->OpticalCenterY.GetCurve( i_animLayer );
			if( animCurve )
			{
				FBXSDK_printf( "        Optical Center Y\n" );
				DisplayCommon::DisplayString( "        Optical Center Y" );
				DisplayCurve( animCurve );
			}

			animCurve = camera->Roll.GetCurve( i_animLayer );
			if( animCurve )
			{
				FBXSDK_printf( "        Roll\n" );
				DisplayCommon::DisplayString( "        Roll" );
				DisplayCurve( animCurve );
			}
		}

		// Display curves specific to a geometry.
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
			nodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
			nodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
		{
			FbxGeometry *geometry = (FbxGeometry*) nodeAttribute;

			int blendShapeDeformerCount = geometry->GetDeformerCount( FbxDeformer::eBlendShape );
			for( int blendShapeIndex = 0; blendShapeIndex<blendShapeDeformerCount; blendShapeIndex++ )
			{
				FbxBlendShape *blendShape = (FbxBlendShape*)geometry->GetDeformer( blendShapeIndex, FbxDeformer::eBlendShape );

				int blendShapeChannelCount = blendShape->GetBlendShapeChannelCount();
				for( int channelIndex = 0; channelIndex<blendShapeChannelCount; channelIndex++ )
				{
					FbxBlendShapeChannel *channel = blendShape->GetBlendShapeChannel( channelIndex );
					const char *channelName = channel->GetName();

					animCurve = geometry->GetShapeChannel( blendShapeIndex, channelIndex, i_animLayer, true );
					if( animCurve )
					{
						FBXSDK_printf( "        Shape %s\n", channelName );
						DisplayCommon::DisplayString( "        Shape ", channelName );
						DisplayCurve( animCurve );
					}
				}
			}
		}
	}

	// Display curves specific to properties
	FbxProperty lProperty = i_node->GetFirstProperty();
	while( lProperty.IsValid() )
	{
		if( lProperty.GetFlag(FbxPropertyAttr::eUserDefined) )
		{
			FbxString lFbxFCurveNodeName  = lProperty.GetName();
			FbxAnimCurveNode* curveNode = lProperty.GetCurveNode( i_animLayer );

			if( !curveNode )
			{
				lProperty = i_node->GetNextProperty( lProperty );
				continue;
			}

			FbxDataType dataType = lProperty.GetPropertyDataType();
			if( dataType.GetType() == eFbxBool || dataType.GetType() == eFbxDouble || dataType.GetType() == eFbxFloat || dataType.GetType() == eFbxInt )
			{
				FbxString message;

				message =  "        Property ";
				message += lProperty.GetName();
				if( lProperty.GetLabel().GetLen() > 0 )
				{
					message += " (Label: ";
					message += lProperty.GetLabel();
					message += ")";
				};

				DisplayCommon::DisplayString( message );

				for( int c = 0; c < curveNode->GetCurveCount(0U); c++ )
				{
					animCurve = curveNode->GetCurve( 0U, c );
					if( animCurve )
						DisplayCurve( animCurve );
				}
			}
			else if( dataType.GetType() == eFbxDouble3 || dataType.GetType() == eFbxDouble4 || dataType.Is(FbxColor3DT) || dataType.Is(FbxColor4DT) )
			{
				char* componentName1 = (dataType.Is(FbxColor3DT) ||dataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_RED : (char*)"X";
				char* componentName2 = (dataType.Is(FbxColor3DT) ||dataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_GREEN : (char*)"Y";
				char* componentName3 = (dataType.Is(FbxColor3DT) ||dataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_BLUE  : (char*)"Z";
				FbxString message;

				message =  "        Property ";
				message += lProperty.GetName();
				if( lProperty.GetLabel().GetLen() > 0 )
				{
					message += " (Label: ";
					message += lProperty.GetLabel();
					message += ")";
				}
				DisplayCommon::DisplayString( message );

				for( int c = 0; c < curveNode->GetCurveCount(0U); c++ )
				{
					animCurve = curveNode->GetCurve( 0U, c );
					if( animCurve )
					{
						DisplayCommon::DisplayString( "        Component ", componentName1 );
						DisplayCurve( animCurve );
					}
				}

				for( int c = 0; c < curveNode->GetCurveCount(1U); c++ )
				{
					animCurve = curveNode->GetCurve(1U, c);
					if( animCurve )
					{
						DisplayCommon::DisplayString( "        Component ", componentName2 );
						DisplayCurve( animCurve );
					}
				}

				for( int c = 0; c < curveNode->GetCurveCount(2U); c++ )
				{
					animCurve = curveNode->GetCurve( 2U, c );
					if( animCurve )
					{
						DisplayCommon::DisplayString( "        Component ", componentName3 );
						DisplayCurve( animCurve );
					}
				}
			}
			else if( dataType.GetType() == eFbxEnum )
			{
				FbxString message;

				message =  "        Property ";
				message += lProperty.GetName();
				if( lProperty.GetLabel().GetLen() > 0 )
				{
					message += " (Label: ";
					message += lProperty.GetLabel();
					message += ")";
				};
				DisplayCommon::DisplayString( message );

				for( int c = 0; c < curveNode->GetCurveCount(0U); c++ )
				{
					animCurve = curveNode->GetCurve( 0U, c );
					if( animCurve )
						DisplayListCurve( animCurve, &lProperty );
				}
			}
		}

		lProperty = i_node->GetNextProperty( lProperty );
	} // while
}

static int InterpolationFlagToIndex( int flags )
{
	if( (flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant ) return 1;
	if( (flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear ) return 2;
	if( (flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic ) return 3;
	return 0;
}

static int ConstantmodeFlagToIndex( int flags )
{
	if( (flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard ) return 1;
	if( (flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext ) return 2;
	return 0;
}

static int TangentmodeFlagToIndex( int flags )
{
	if( (flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto ) return 1;
	if( (flags & FbxAnimCurveDef::eTangentAutoBreak)== FbxAnimCurveDef::eTangentAutoBreak ) return 2;
	if( (flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB ) return 3;
	if( (flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser ) return 4;
	if( (flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak ) return 5;
	if( (flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak ) return 6;
	return 0;
}

static int TangentweightFlagToIndex( int flags )
{
	if( (flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone ) return 1;
	if( (flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight ) return 2;
	if( (flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft ) return 3;
	return 0;
}

static int TangentVelocityFlagToIndex( int flags )
{
	if( (flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone ) return 1;
	if( (flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight ) return 2;
	if( (flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft ) return 3;
	return 0;
}

void Tools::DisplayAnimation::DisplayCurveKeys( FbxAnimCurve* i_curve )
{
	static const char *interpolation[] = { "?", "constant", "linear", "cubic"};
	static const char *constantMode[] =  { "?", "Standard", "Next" };
	static const char *cubicMode[] =     { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
	static const char *tangentWVMode[] = { "?", "None", "Right", "Next left" };

	FbxTime keyTime;
	float keyValue;
	char timeString[256];
	FbxString string;
	int count;

	int keyCount = i_curve->KeyGetCount();

	for( count = 0; count < keyCount; count++ )
	{
		keyValue = static_cast<float>( i_curve->KeyGetValue(count) );
		keyTime  = i_curve->KeyGetTime( count );

		string = "            Key Time: ";
		string += keyTime.GetTimeString( timeString, FbxUShort(256) );
		string += ".... Key Value: ";
		string += keyValue;
		string += " [ ";
		string += interpolation[InterpolationFlagToIndex(i_curve->KeyGetInterpolation(count))];
		if( (i_curve->KeyGetInterpolation(count)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant )
		{
			string += " | ";
			string += constantMode[ ConstantmodeFlagToIndex(i_curve->KeyGetConstantMode(count)) ];
		}
		else if( (i_curve->KeyGetInterpolation(count)&FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic )
		{
			string += " | ";
			string += cubicMode[ TangentmodeFlagToIndex(i_curve->KeyGetTangentMode(count)) ];
			string += " | ";
			string += tangentWVMode[ TangentweightFlagToIndex(i_curve->KeyGet(count).GetTangentWeightMode()) ];
			string += " | ";
			string += tangentWVMode[ TangentVelocityFlagToIndex(i_curve->KeyGet(count).GetTangentVelocityMode()) ];
		}
		string += " ]";
		DisplayCommon::DisplayString( string );
		string += "\n";
		FBXSDK_printf( string );
	}
}

void Tools::DisplayAnimation::DisplayListCurveKeys( FbxAnimCurve* i_curve, FbxProperty* i_property )
{
	FbxTime keyTime;
	int keyValue;
	char timeString[256];
	FbxString listValue;
	FbxString string;
	int count;

	int keyCount = i_curve->KeyGetCount();

	for(count = 0; count < keyCount; count++)
	{
		keyValue = static_cast<int>( i_curve->KeyGetValue(count) );
		keyTime  = i_curve->KeyGetTime( count );

		string = "            Key Time: ";
		string += keyTime.GetTimeString( timeString, FbxUShort(256) );
		string += ".... Key Value: ";
		string += keyValue;
		string += " (";
		string += i_property->GetEnumValue( keyValue );
		string += ")";
		DisplayCommon::DisplayString( string );
		string += "\n";
		FBXSDK_printf ( string );
	}
}
#endif	// #if defined( DUMP_FBX_INFO ) && defined( DISPLAY_ANIMATION_INFORMATION )