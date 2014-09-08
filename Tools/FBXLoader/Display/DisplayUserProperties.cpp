#include "DisplayUserProperties.h"

#if defined( DUMP_FBX_INFO ) && defined( DISPLAY_PROPERTY_INFORMATION )
void Tools::DisplayUserProperties::DisplayUserProperties( FbxObject *i_object )
{
	int count = 0;
	FbxString titleStr = "    Property Count: ";

	FbxProperty lProperty = i_object->GetFirstProperty();
	while( lProperty.IsValid() )
	{
		if( lProperty.GetFlag(FbxPropertyAttr::eUserDefined) )
			count++;

		lProperty = i_object->GetNextProperty( lProperty );
	}

	if( count == 0 )
		return; // there are no user properties to display

	DisplayCommon::DisplayInt( titleStr, count );

	lProperty = i_object->GetFirstProperty();
	int i = 0;
	while( lProperty.IsValid() )
	{
		if( lProperty.GetFlag(FbxPropertyAttr::eUserDefined) )
		{
			DisplayCommon::DisplayInt( "        Property ", i );
			FbxString lString = lProperty.GetLabel();
			DisplayCommon::DisplayString("            Display Name: ", lString.Buffer());
			lString = lProperty.GetName();
			DisplayCommon::DisplayString("            Internal Name: ", lString.Buffer());
			DisplayCommon::DisplayString("            Type: ", lProperty.GetPropertyDataType().GetName());
			if( lProperty.HasMinLimit() )
				DisplayCommon::DisplayDouble("            Min Limit: ", lProperty.GetMinLimit());
			if( lProperty.HasMaxLimit() )
				DisplayCommon::DisplayDouble("            Max Limit: ", lProperty.GetMaxLimit());
			DisplayCommon::DisplayBool("            Is Animatable: ", lProperty.GetFlag(FbxPropertyAttr::eAnimatable));

			FbxDataType propertyDataType=lProperty.GetPropertyDataType();

			// BOOL
			if( propertyDataType.GetType() == eFbxBool )
			{
				DisplayCommon::DisplayBool( "            Default Value: ", lProperty.Get<FbxBool>() );
			}
			// REAL
			else if( (propertyDataType.GetType() == eFbxDouble) || (propertyDataType.GetType() == eFbxFloat) )
			{
				DisplayCommon::DisplayDouble( "            Default Value: ", lProperty.Get<FbxDouble>() );
			}
			// COLOR
			else if( propertyDataType.Is(FbxColor3DT) || propertyDataType.Is(FbxColor4DT) )
			{
				FbxColor lDefault;
				char buffer[64];

				lDefault = lProperty.Get<FbxColor>();
				FBXSDK_sprintf( buffer, 64, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha );
				DisplayCommon::DisplayString("            Default Value: ", buffer );
			}
			// INTEGER
			else if( propertyDataType.GetType() == eFbxInt )
			{
				DisplayCommon::DisplayInt( "            Default Value: ", lProperty.Get<FbxInt>() );
			}
			// VECTOR
			else if( (propertyDataType.GetType() == eFbxDouble3) || (propertyDataType.GetType() == eFbxDouble4) )
			{
				FbxDouble3 lDefault;
				char buffer[64];

				lDefault = lProperty.Get<FbxDouble3>();
				FBXSDK_sprintf( buffer, 64, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2] );
				DisplayCommon::DisplayString( "            Default Value: ", buffer );
			}
			// LIST
			else if( propertyDataType.GetType() == eFbxEnum )
			{
				DisplayCommon::DisplayInt( "            Default Value: ", lProperty.Get<FbxEnum>() );
			}
			// UNIDENTIFIED
			else
			{
				DisplayCommon::DisplayString( "            Default Value: UNIDENTIFIED" );
			}
			i++;
		}

		lProperty = i_object->GetNextProperty( lProperty );
	}
}
#endif	// #if defined( DUMP_FBX_INFO ) && defined( DISPLAY_PROPERTY_INFORMATION )