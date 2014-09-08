#include <fstream>

#include "AssetGenerator.h"

namespace Tools
{
	namespace AssetGenerator
	{
		void GetTextureName( FbxProperty &i_property, FbxString &o_filename );
	}
}

bool Tools::AssetGenerator::GenerateMaterialFile( FbxGeometry *i_geometry, const char *i_filename, int &i_materialID )
{
	std::ofstream materialFile;
	FbxString materialName = "../../External/Data/Materials/" + FbxString( i_filename );
	bool bNormalMapExist = false;
	bool bNoTextureFound = true;

	if( i_geometry == NULL )
		return false;

	materialFile.open( materialName.Buffer() );
	if( materialFile.fail() )
		return false;

	FbxNode *node = i_geometry->GetNode();
	int materialCount = 0;
	if( node )
		materialCount = node->GetSrcObjectCount<FbxSurfaceMaterial>();

	for( int ctr = 0; ctr < materialCount; ctr++ )
	{
		FbxSurfaceMaterial *material = node->GetSrcObject<FbxSurfaceMaterial>( ctr );
		double transparency = 0.0;
		double shininess = 0.0;
		double reflection = 0.0;

		if( material->GetClassId().Is(FbxSurfacePhong::ClassId) )
		{
			transparency = ((FbxSurfacePhong *) material)->TransparencyFactor;
			shininess = ((FbxSurfacePhong *) material)->Shininess;
			reflection = ((FbxSurfacePhong *) material)->ReflectionFactor;
		}
		else if( material->GetClassId().Is(FbxSurfaceLambert::ClassId) )
		{
			transparency = ((FbxSurfaceLambert *)material)->TransparencyFactor;
		}

		for( int i = 0; i < FbxLayerElement::sTypeTextureCount; i++ )
		{
			FbxProperty textureProperty = material->FindProperty( FbxLayerElement::sTextureChannelNames[i] );
			if( textureProperty.IsValid() )
			{
				int totalTexture = textureProperty.GetSrcObjectCount<FbxTexture>();
				for( int j = 0; j < totalTexture; j++ )
				{
					FbxLayeredTexture *layeredTexture = textureProperty.GetSrcObject<FbxLayeredTexture>( j );
					if( layeredTexture )
					{
						FbxLayeredTexture *layeredTexture = textureProperty.GetSrcObject<FbxLayeredTexture>( j );
						int totalLayeredTexture = layeredTexture->GetSrcObjectCount<FbxTexture>();
						for( int k = 0; k < totalLayeredTexture; k++ )
						{
							FbxTexture *texture = layeredTexture->GetSrcObject<FbxTexture>( k );
							if( texture )
							{
								FbxFileTexture *fileTexture = FbxCast<FbxFileTexture>( texture );
								std::string textureName = fileTexture->GetFileName();
								if( (textureProperty.GetName() == FbxString("NormalMap"))
									&& (textureProperty.GetName().GetLen() == FbxString("NormalMap").GetLen()) )
									bNormalMapExist = true;
								int start = textureName.find_last_of( "\\/" );
								int last = textureName.size();
								textureName = textureName.substr( start + 1, last - start );
								if( textureName.size() > 0 )
									bNoTextureFound = false;
								materialFile << "<" << textureProperty.GetName() << ">" << std::endl;
								materialFile << "    " << textureName.c_str() << std::endl;
								materialFile << "</" << textureProperty.GetName() << ">" << std::endl;
								materialFile << std::endl;
							}
						}
					}
					else
					{
						FbxTexture *texture = textureProperty.GetSrcObject<FbxTexture>( j );
						if( texture )
						{
							FbxFileTexture *fileTexture = FbxCast<FbxFileTexture>( texture );
							std::string textureName = fileTexture->GetFileName();
							if( (textureProperty.GetName() == FbxString("NormalMap"))
								&& (textureProperty.GetName().GetLen() == FbxString("NormalMap").GetLen()) )
								bNormalMapExist = true;
							int start = textureName.find_last_of( "\\/" );
							int last = textureName.size();
							textureName = textureName.substr( start + 1, last - start );
							if( textureName.size() > 0 )
								bNoTextureFound = false;
							materialFile << "<" << textureProperty.GetName() << ">" << std::endl;
							materialFile << "    " << textureName.c_str() << std::endl;
							materialFile << "</" << textureProperty.GetName() << ">" << std::endl;
							materialFile << std::endl;
						}
					}
				}
			}
		}
		materialFile << "<transparency>" << std::endl;
		materialFile << "    " << transparency << std::endl;
		materialFile << "</transparency>" << std::endl;
		materialFile << std::endl;
		materialFile << "<reflectance>" << std::endl;
		materialFile << "    " << reflection << std::endl;
		materialFile << "</reflectance>" << std::endl;
		materialFile << std::endl;
		materialFile << "<shininess>" << std::endl;
		materialFile << "    " << shininess << std::endl;
		materialFile << "</shininess>" << std::endl;
		materialFile << std::endl;
		materialFile << "<effect>" << std::endl;
		if( bNoTextureFound )
		{
			materialFile << "    original.txt" << std::endl;
		}
		else
		{
			materialFile << "    diffuseEffect.txt" << std::endl;
			/*if( bNormalMapExist )
			{
				if( transparency < 1.0 )
					materialFile << "    alphaBlendNormalEffect.txt" << std::endl;
				else if( shininess > 0.0 )
					materialFile << "    diffuseSpecularNormalEffect.txt" << std::endl;
				else
					materialFile << "    alphaBinaryNormalEffect.txt" << std::endl;
			}
			else
			{
				if( transparency < 1.0 )
					materialFile << "    alphaBlendEffect.txt" << std::endl;
				else if( shininess > 0.0 )
					materialFile << "    diffuseSpecularEffect.txt" << std::endl;
				else
					materialFile << "    alphaBinaryEffect.txt" << std::endl;
			}*/
		}
		materialFile << "</effect>" << std::endl;
		materialFile << std::endl;
	}

	materialFile.close();

	return true;
}

void Tools::AssetGenerator::GetTextureName( FbxProperty &i_property, FbxString &o_filename )
{
	int totalLayeredTexture = i_property.GetSrcObjectCount<FbxLayeredTexture>();

	if( totalLayeredTexture > 0 )
	{
		for( int i = 0; i < totalLayeredTexture; i++ )
		{
			FbxLayeredTexture *layeredTexture = i_property.GetSrcObject<FbxLayeredTexture>( i );
			int totalTextures = layeredTexture->GetSrcObjectCount<FbxTexture>();
			for( int j = 0; j < totalTextures; j++ )
			{
				o_filename += (char*)layeredTexture->GetName();
				o_filename += "\n";
			}
		}
	}
	else
	{
		int totalTexture = i_property.GetSrcObjectCount<FbxTexture>();
		for( int i = 0; i < totalTexture; i++ )
		{
			FbxTexture *texture = i_property.GetSrcObject<FbxTexture>( i );
			if( texture )
			{
				o_filename += (char*)texture->GetName();
				o_filename += "\n";
			}
		}
	}
}