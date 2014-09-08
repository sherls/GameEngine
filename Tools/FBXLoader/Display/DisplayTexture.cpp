#include "DisplayTexture.h"

#ifdef DUMP_FBX_INFO
void Tools::DisplayTexture::DisplayTextureInfo( FbxTexture* i_texture, int i_blendMode )
{
	FbxFileTexture *fileTexture = FbxCast<FbxFileTexture>( i_texture );
	FbxProceduralTexture *proceduralTexture = FbxCast<FbxProceduralTexture>( i_texture );

	DisplayCommon::DisplayString( "            Name: \"", (char *) i_texture->GetName(), "\"" );
	if( fileTexture )
	{
		DisplayCommon::DisplayString( "            Type: File Texture" );
		DisplayCommon::DisplayString( "            File Name: \"", (char *) fileTexture->GetFileName(), "\"" );
	}
	else if( proceduralTexture )
	{
		DisplayCommon::DisplayString( "            Type: Procedural Texture" );
	}
	DisplayCommon::DisplayDouble( "            Scale U: ", i_texture->GetScaleU() );
	DisplayCommon::DisplayDouble( "            Scale V: ", i_texture->GetScaleV() );
	DisplayCommon::DisplayDouble( "            Translation U: ", i_texture->GetTranslationU() );
	DisplayCommon::DisplayDouble( "            Translation V: ", i_texture->GetTranslationV() );
	DisplayCommon::DisplayBool( "            Swap UV: ", i_texture->GetSwapUV() );
	DisplayCommon::DisplayDouble( "            Rotation U: ", i_texture->GetRotationU() );
	DisplayCommon::DisplayDouble( "            Rotation V: ", i_texture->GetRotationV() );
	DisplayCommon::DisplayDouble( "            Rotation W: ", i_texture->GetRotationW() );

	const char* lAlphaSources[] = { "None", "RGB Intensity", "Black" };

	DisplayCommon::DisplayString( "            Alpha Source: ", lAlphaSources[i_texture->GetAlphaSource()] );
	DisplayCommon::DisplayDouble( "            Cropping Left: ", i_texture->GetCroppingLeft() );
	DisplayCommon::DisplayDouble( "            Cropping Top: ", i_texture->GetCroppingTop() );
	DisplayCommon::DisplayDouble( "            Cropping Right: ", i_texture->GetCroppingRight() );
	DisplayCommon::DisplayDouble( "            Cropping Bottom: ", i_texture->GetCroppingBottom() );

	const char* mappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical", 
		"Box", "Face", "UV", "Environment" };

	DisplayCommon::DisplayString( "            Mapping Type: ", mappingTypes[i_texture->GetMappingType()] );

	if( i_texture->GetMappingType() == FbxTexture::ePlanar )
	{
		const char* planarMappingNormals[] = { "X", "Y", "Z" };

		DisplayCommon::DisplayString( "            Planar Mapping Normal: ", planarMappingNormals[i_texture->GetPlanarMappingNormal()] );
	}

	const char* blendModes[] = { "Translucent", "Add", "Modulate", "Modulate2" };
	if( i_blendMode >= 0 )
		DisplayCommon::DisplayString( "            Blend Mode: ", blendModes[i_blendMode] );
	DisplayCommon::DisplayDouble( "            Alpha: ", i_texture->GetDefaultAlpha() );

	if( fileTexture )
	{
		const char* lMaterialUses[] = { "Model Material", "Default Material" };
		DisplayCommon::DisplayString( "            Material Use: ", lMaterialUses[fileTexture->GetMaterialUse()] );
	}

	const char* textureUses[] = { "Standard", "Shadow Map", "Light Map", 
		"Spherical Reflexion Map", "Sphere Reflexion Map", "Bump Normal Map" };

	DisplayCommon::DisplayString( "            Texture Use: ", textureUses[i_texture->GetTextureUse()] );
	DisplayCommon::DisplayString( "" );
}

void Tools::DisplayTexture::FindAndDisplayTextureInfoByProperty( FbxProperty i_property, bool& i_displayHeader, int i_materialIndex )
{
	if( i_property.IsValid() )
	{
		int textureCount = i_property.GetSrcObjectCount<FbxTexture>();

		for( int j = 0; j < textureCount; j++ )
		{
			//Here we have to check if it's layered textures, or just textures:
			FbxLayeredTexture *layeredTexture = i_property.GetSrcObject<FbxLayeredTexture>( j );
			if( layeredTexture )
			{
				DisplayCommon::DisplayInt( "    Layered Texture: ", j );
				FbxLayeredTexture *layeredTexture = i_property.GetSrcObject<FbxLayeredTexture>( j );
				int totalTextures = layeredTexture->GetSrcObjectCount<FbxTexture>();
				for( int k = 0; k < totalTextures; k++ )
				{
					FbxTexture *texture = layeredTexture->GetSrcObject<FbxTexture>(k);
					if( texture )
					{
						if(i_displayHeader)
						{
							DisplayCommon::DisplayInt( "    Textures connected to Material ", i_materialIndex );
							i_displayHeader = false;
						}

						//NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
						//Why is that?  because one texture can be shared on different layered textures and might
						//have different blend modes.
						FbxLayeredTexture::EBlendMode blendMode;
						layeredTexture->GetTextureBlendMode( k, blendMode );
						DisplayCommon::DisplayString( "    Textures for ", i_property.GetName() );
						DisplayCommon::DisplayInt( "        Texture ", k );
						DisplayTextureInfo( texture, (int) blendMode );
					}
				}
			}
			else
			{
				//no layered texture simply get on the property
				FbxTexture *texture = i_property.GetSrcObject<FbxTexture>( j );
				if( texture )
				{
					//display connected Material header only at the first time
					if( i_displayHeader )
					{
						DisplayCommon::DisplayInt( "    Textures connected to Material ", i_materialIndex );
						i_displayHeader = false;
					}
					DisplayCommon::DisplayString( "    Textures for ", i_property.GetName() );
					DisplayCommon::DisplayInt( "        Texture ", j );
					DisplayTextureInfo( texture, -1 );
				}
			}
		}
	}//end if pProperty
}

void Tools::DisplayTexture::DisplayTexture( FbxGeometry *i_geometry )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nTexture\n--------------------" );

	int materialIndex;
	FbxProperty lProperty;
	if( i_geometry->GetNode() == NULL )
		return;

	int lNbMat = i_geometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
	for( materialIndex = 0; materialIndex < lNbMat; materialIndex++ )
	{
		FbxSurfaceMaterial *material = i_geometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>( materialIndex );
		bool lDisplayHeader = true;

		//go through all the possible textures
		if( material )
		{
			int textureIndex;
			FBXSDK_FOR_EACH_TEXTURE( textureIndex )
			{
				lProperty = material->FindProperty( FbxLayerElement::sTextureChannelNames[textureIndex] );
				FindAndDisplayTextureInfoByProperty( lProperty, lDisplayHeader, materialIndex ); 
			}
		}//end if(lMaterial)
	}// end for lMaterialIndex
}
#endif	// #ifdef DUMP_FBX_INFO