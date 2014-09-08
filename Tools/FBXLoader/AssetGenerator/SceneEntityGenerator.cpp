#include <vector>
#include <fstream>

#include "AssetGenerator.h"
#include <iostream>

namespace Tools
{
	namespace AssetGenerator
	{
		std::vector<FbxString> *entityFiles;
		bool GenerateEntityFile( FbxNode *i_node );
	}
}

bool Tools::AssetGenerator::GenerateSceneFile( FbxScene *i_scene, const char *i_filename )
{
	std::ofstream sceneFile;
	FbxString sceneName = FbxString( i_filename );
	FbxNode *node = i_scene->GetRootNode();

	sceneFile.open( sceneName.Buffer() );
	if( sceneFile.fail() )
		return false;

	entityFiles = new std::vector<FbxString>();

	if( node )
	{
		int totalChild = node->GetChildCount();
		for( int i = 0; i < totalChild; i++ )
		{
			if( !GenerateEntityFile(node) )
				return false;
		}
	}

	sceneFile << "<entity>" << std::endl;

	int totalEntity = entityFiles->size();
	sceneFile << totalEntity << std::endl;

	for( int i = 0; i < totalEntity; i++ )
	{
		sceneFile << "{" << std::endl;
		sceneFile << "    <file>" << std::endl;
		sceneFile << "        " << entityFiles->at(i) << std::endl;
		sceneFile << "    </file>" << std::endl;
		sceneFile << "    <position>" << std::endl;
		sceneFile << "        0, 0, 0" << std::endl;
		sceneFile << "    </position>" << std::endl;
		sceneFile << "    <orientation>" << std::endl;
		sceneFile << "        0" << std::endl;
		sceneFile << "    </orientation>" << std::endl;
		sceneFile << "}" << std::endl;
	}
	
	sceneFile << "</entity>" << std::endl;

	sceneFile.close();
	delete entityFiles;

	return true;
}

bool Tools::AssetGenerator::GenerateEntityFile( FbxNode *i_node )
{
	int totalChild = i_node->GetChildCount();

	if( i_node->GetNodeAttribute() != NULL )
	{
		FbxNodeAttribute::EType attributeType = i_node->GetNodeAttribute()->GetAttributeType();
		if( attributeType == FbxNodeAttribute::eMesh )
		{
			FbxMesh *mesh = (FbxMesh*) i_node->GetNodeAttribute();
			FbxString meshName = i_node->GetName() + FbxString( ".txt" );
			meshName.ReplaceAll( ":", "_" );
			entityFiles->push_back( meshName );
			FbxString entityName = "../../External/Data/Entities/" + meshName;
			FbxString materialName;
			std::ofstream entityFile;

			entityFile.open( entityName.Buffer() );
			if( entityFile.fail() )
			{
				//std::cout << "Cannot create entity file " << entityName.Buffer() << std::endl;
				return false;
			}

			int nodeMaterialCount = i_node->GetMaterialCount();
			int meshMaterialCount = mesh->GetElementMaterialCount();

			entityFile << "<mesh>" << std::endl;
			entityFile << "    " << meshName << std::endl;
			entityFile << "</mesh>" << std::endl;

			entityFile << std::endl;

			entityFile << "<material>" << std::endl;
			bool isAllSame = true;
			for( int i = 0; i < meshMaterialCount; i++ )
			{
				FbxGeometryElementMaterial *elementMaterial = mesh->GetElementMaterial( i );
				if( elementMaterial->GetMappingMode() == FbxGeometryElement::eByPolygon )
					isAllSame = false;

				if( elementMaterial )
				{
					if( elementMaterial->GetReferenceMode() == FbxGeometryElement::eIndex ||
						elementMaterial->GetReferenceMode() == FbxGeometryElement::eIndexToDirect )
					{
						int indexArrayCount = elementMaterial->GetIndexArray().GetCount();
						for( int i = 0; i < indexArrayCount; i++ )
						{
							int index = elementMaterial->GetIndexArray().GetAt( i );
							FbxGeometry *geometry = mesh;
							FbxNode *node = geometry->GetNode();
							FbxSurfaceMaterial *material = node->GetMaterial( index );
							materialName = material->GetName() + FbxString( ".txt" );
							materialName.ReplaceAll( ":", "_" );
							entityFile << "    " << materialName << std::endl;
						}
					}
				}
			}
			entityFile << "</material>" << std::endl;

			entityFile.close();

			GenerateMeshFile( mesh, meshName );

			if( isAllSame )
			{
				for( int i = 0; i < meshMaterialCount; i++ )
				{
					FbxGeometryElementMaterial *elementMaterial = mesh->GetElementMaterial( i );
					int materialID = elementMaterial->GetIndexArray().GetAt( 0 );
					if( (elementMaterial ->GetMappingMode() == FbxGeometryElement::eAllSame) && (materialID >= 0) )
						GenerateMaterialFile( mesh, materialName, materialID );
				}
			}
		}
	}

	for( int i = 0; i < totalChild; i++ )
		GenerateEntityFile( i_node->GetChild(i) );

	return true;
}