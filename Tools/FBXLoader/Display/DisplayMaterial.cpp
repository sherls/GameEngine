#include "DisplayMaterial.h"

#ifdef DUMP_FBX_INFO

void Tools::DisplayMaterial::DisplayMaterial( FbxGeometry *i_geometry )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nMaterial\n--------------------" );
	int materialCount = 0;
	FbxNode *node = NULL;

	if( i_geometry )
	{
		node = i_geometry->GetNode();
		if( node )
			materialCount = node->GetMaterialCount();
	}

	if( materialCount > 0 )
	{
		FbxPropertyT<FbxDouble3> double3;
		FbxPropertyT<FbxDouble> double1;
		FbxColor theColor;

		for( int ctr = 0; ctr < materialCount; ctr++ )
		{
			DisplayCommon::DisplayInt( "        Material ", ctr );

			FbxSurfaceMaterial *material = node->GetMaterial( ctr );

			DisplayCommon::DisplayString( "            Name: \"", (char *) material->GetName(), "\"" ); 

#ifdef DISPLAY_HARDWARE_SHADER_INFORMATION
			//Get the implementation to see if it's a hardware shader.
			// Note:: this cause memory leak
			const FbxImplementation* implementation = GetImplementation( material, FBXSDK_IMPLEMENTATION_HLSL );
			FbxString implementationType = "HLSL";
			if( !implementation )
			{
				implementation = GetImplementation( material, FBXSDK_IMPLEMENTATION_CGFX );
				implementationType = "CGFX";
			}

			if( implementation )
			{
				//Now we have a hardware shader, let's read it
				FBXSDK_printf( "            Hardware Shader Type: %s\n", implemenationType.Buffer() );
				DisplayCommon::DisplayString( "            Hardware Shader Type: ", implemenationType );

				const FbxBindingTable* rootTable = implementation->GetRootTable();
				FbxString fileName = rootTable->DescAbsoluteURL.Get();
				FbxString techniqueName = rootTable->DescTAG.Get(); 

				const FbxBindingTable* table = implementation->GetRootTable();
				size_t entryNum = table->GetEntryCount();

				for( int i = 0; i < (int)entryNum; i++ )
				{
					const FbxBindingTableEntry& entry = table->GetEntry( i );
					const char *entrySrcType = entry.GetEntryType( true ); 
					FbxProperty fbxProp;

					FbxString test = entry.GetSource();
					FBXSDK_printf( "            Entry: %s\n", test.Buffer() );
					DisplayCommon::DisplayString( "            Entry: %s\n", test );

					if ( strcmp( FbxPropertyEntryView::sEntryType, entrySrcType ) == 0 )
					{
						fbxProp = material->FindPropertyHierarchical(entry.GetSource()); 
						if( !fbxProp.IsValid() )
						{
							fbxProp = material->RootProperty.FindHierarchical( entry.GetSource() );
						}
					}
					else if( strcmp( FbxConstantEntryView::sEntryType, entrySrcType ) == 0 )
					{
						fbxProp = implementation->GetConstants().FindHierarchical( entry.GetSource() );
					}
					if( fbxProp.IsValid() )
					{
						if( fbxProp.GetSrcObjectCount<FbxTexture>() > 0 )
						{
							//do what you want with the textures
							for( int j = 0; j < fbxProp.GetSrcObjectCount<FbxFileTexture>(); j++ )
							{
								FbxFileTexture *tex = fbxProp.GetSrcObject<FbxFileTexture>( j );
								FBXSDK_printf( "           File Texture: %s\n", tex->GetFileName() );
								DisplayCommon::DisplayString( "           File Texture: %s\n", tex->GetFileName() );
							}
							for( int j = 0; j < fbxProp.GetSrcObjectCount<FbxLayeredTexture>(); j++ )
							{
								FbxLayeredTexture *tex = fbxProp.GetSrcObject<FbxLayeredTexture>( j );
								FBXSDK_printf( "        Layered Texture: %s\n", tex->GetName() );
								DisplayCommon::DisplayString( "        Layered Texture: %s\n", tex->GetName() );
							}
							for( int j = 0; j < fbxProp.GetSrcObjectCount<FbxProceduralTexture>(); j++ )
							{
								FbxProceduralTexture *tex = fbxProp.GetSrcObject<FbxProceduralTexture>( j );
								FBXSDK_printf( "     Procedural Texture: %s\n", tex->GetName() );
								DisplayCommon::DisplayString( "     Procedural Texture: %s\n", tex->GetName() );
							}
						}
						else
						{
							FbxDataType fbxType = fbxProp.GetPropertyDataType();
							FbxString fbxName = fbxType.GetName();
							if( FbxBoolDT == fbxType )
							{
								DisplayCommon::DisplayBool( "                Bool: ", fbxProp.Get<FbxBool>() );
							}
							else if( FbxIntDT == fbxType ||  FbxEnumDT  == fbxType )
							{
								DisplayCommon::DisplayInt( "                Int: ", fbxProp.Get<FbxInt>() );
							}
							else if( FbxFloatDT == fbxType )
							{
								DisplayCommon::DisplayDouble( "                Float: ", fbxProp.Get<FbxFloat>() );
							}
							else if( FbxDoubleDT == fbxType )
							{
								DisplayCommon::DisplayDouble( "                Double: ", fbxProp.Get<FbxDouble>() );
							}
							else if( FbxStringDT == fbxType || FbxUrlDT  == fbxType || FbxXRefUrlDT  == fbxType )
							{
								DisplayCommon::DisplayString( "                String: ", fbxProp.Get<FbxString>().Buffer() );
							}
							else if( FbxDouble2DT == fbxType )
							{
								FbxDouble2 double2 = fbxProp.Get<FbxDouble2>();
								FbxVector2 vector;
								vector[0] = double2[0];
								vector[1] = double2[1];
								DisplayCommon::Display2DVector( "                2D vector: ", vector );
							}
							else if( (FbxDouble3DT == fbxType) || (FbxColor3DT == fbxType) )
							{
								FbxDouble3 double3 = fbxProp.Get<FbxDouble3>();
								FbxVector4 vector;
								vector[0] = double3[0];
								vector[1] = double3[1];
								vector[2] = double3[2];
								DisplayCommon::Display3DVector( "                3D vector: ", vector );
							}
							else if( (FbxDouble4DT == fbxType) || (FbxColor4DT == fbxType) )
							{
								FbxDouble4 double4 = fbxProp.Get<FbxDouble4>();
								FbxVector4 vector;
								vector[0] = double4[0];
								vector[1] = double4[1];
								vector[2] = double4[2];
								vector[3] = double4[3];
								DisplayCommon::Display4DVector( "                4D vector: ", vector );
							}
							else if( FbxDouble4x4DT == fbxType )
							{
								FbxDouble4x4 double44 = fbxProp.Get<FbxDouble4x4>();
								for( int j = 0; j < 4; j++ )
								{
									FbxVector4 vector;
									vector[0] = double44[j][0];
									vector[1] = double44[j][1];
									vector[2] = double44[j][2];
									vector[3] = double44[j][3];
									DisplayCommon::Display4DVector( "                4x4D vector: ", vector );
								}
							}
						}
					}
				}
			}
			else
#endif	// #ifdef DISPLAY_HARDWARE_SHADER_INFORMATION
			if( material->GetClassId().Is(FbxSurfacePhong::ClassId) )
			{
				// We found a Phong material.  Display its properties.

				// Display the Ambient Color
				double3 = ((FbxSurfacePhong *) material)->Ambient;
				theColor.Set( double3.Get()[0], double3.Get()[1], double3.Get()[2] );
				DisplayCommon::DisplayColor( "            Ambient: ", theColor );

				// Display the Diffuse Color
				double3 = ((FbxSurfacePhong *) material)->Diffuse;
				theColor.Set( double3.Get()[0], double3.Get()[1], double3.Get()[2] );
				DisplayCommon::DisplayColor( "            Diffuse: ", theColor );

				// Display the Specular Color (unique to Phong materials)
				double3 = ((FbxSurfacePhong *) material)->Specular;
				theColor.Set( double3.Get()[0], double3.Get()[1], double3.Get()[2] );
				DisplayCommon::DisplayColor( "            Specular: ", theColor );

				// Display the Emissive Color
				double3 = ((FbxSurfacePhong *) material)->Emissive;
				theColor.Set( double3.Get()[0], double3.Get()[1], double3.Get()[2] );
				DisplayCommon::DisplayColor( "            Emissive: ", theColor );

				//Opacity is Transparency factor now
				double1 = ((FbxSurfacePhong *) material)->TransparencyFactor;
				DisplayCommon::DisplayDouble( "            Opacity: ", 1.0-double1.Get() );

				// Display the Shininess
				double1 = ((FbxSurfacePhong *) material)->Shininess;
				DisplayCommon::DisplayDouble( "            Shininess: ", double1.Get() );

				// Display the Reflectivity
				double1 = ((FbxSurfacePhong *) material)->ReflectionFactor;
				DisplayCommon::DisplayDouble( "            Reflectivity: ", double1.Get() );
			}
			else if( material->GetClassId().Is(FbxSurfaceLambert::ClassId) )
			{
				// We found a Lambert material. Display its properties.
				// Display the Ambient Color
				double3 = ((FbxSurfaceLambert *)material)->Ambient;
				theColor.Set( double3.Get()[0], double3.Get()[1], double3.Get()[2] );
				DisplayCommon::DisplayColor( "            Ambient: ", theColor );

				// Display the Diffuse Color
				double3 = ((FbxSurfaceLambert *)material)->Diffuse;
				theColor.Set( double3.Get()[0], double3.Get()[1], double3.Get()[2] );
				DisplayCommon::DisplayColor( "            Diffuse: ", theColor );

				// Display the Emissive
				double3 = ((FbxSurfaceLambert *)material)->Emissive;
				theColor.Set( double3.Get()[0], double3.Get()[1], double3.Get()[2] );
				DisplayCommon::DisplayColor( "            Emissive: ", theColor );

				// Display the Opacity
				double1 = ((FbxSurfaceLambert *)material)->TransparencyFactor;
				DisplayCommon::DisplayDouble( "            Opacity: ", 1.0-double1.Get() );
			}
			else
				DisplayCommon::DisplayString( "Unknown type of Material" );

			FbxPropertyT<FbxString> string;
			string = material->ShadingModel;
			DisplayCommon::DisplayString( "            Shading Model: ", string.Get() );
		}
	}
}
#endif	// #ifdef DUMP_FBX_INFO