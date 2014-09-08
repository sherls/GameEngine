#include <vector>
#include <string>
#include <fstream>

// Utility header
#include <Utilities.h>
#include <Math/Math.h>
#include <UtilitiesTypes.h>

#include "AssetGenerator.h"
#include "../OctreeGenerator/OctreeGenerator.h"

//#define GENERATE_BINARY_DUMP_FILE

namespace Tools
{
	namespace AssetGenerator
	{
		FbxColor GetColour( FbxMesh *i_mesh, int i_controlPointIdx );
		FbxColor GetColour( FbxMesh *i_mesh, int i_controlPointIdx, int i_indexIDx );
		FbxVector2 GetUV( FbxMesh *i_mesh, int i_controlPointIdx );
		FbxVector2 GetUV( FbxMesh *i_mesh, int i_controlPointIdx, int i_indexIDx, int i_polygonCtr, int i_polygonSizeCtr );
		FbxVector4 GetNormal( FbxMesh *i_mesh, int i_indexIDx );
		FbxVector4 GetNormalByControlPoint( FbxMesh *i_mesh, int i_controlPointIdx );
		FbxVector4 GetTangent( FbxMesh *i_mesh, int i_indexIDx );
		FbxVector4 GetTangentByControlPoint( FbxMesh *i_mesh, int i_controlPointIdx );
		FbxVector4 GetBinormal( FbxMesh *i_mesh, int i_indexIDx );
		FbxVector4 GetBinormalByControlPoint( FbxMesh *i_mesh, int i_controlPointIdx );
	}
}

bool Tools::AssetGenerator::GenerateMeshFile( FbxMesh *i_mesh, const char *i_filename )
{
	FbxString meshName = "../../External/Data/Meshes/" + FbxString( i_filename );
	std::ofstream meshFile;
	Utilities::StringHash emptyHash = Utilities::StringHash( "" );

	meshFile.open( meshName.Buffer(), std::ios::binary );
	if( meshFile.fail() )
		return false;

#ifdef GENERATE_BINARY_DUMP_FILE
	std::ofstream binaryDump;
	meshName += "b";
	binaryDump.open( meshName.Buffer() );
	if( binaryDump.fail() )
		return false;
#endif	// #if GENERATE_BINARY_DUMP_FILE

	UINT32 totalVertices = i_mesh->GetControlPointsCount();
	FbxVector4 *vertices = i_mesh->GetControlPoints();
	UINT32 polygonCount = i_mesh->GetPolygonCount();
	int polygonSize = i_mesh->GetPolygonSize( 0 );
	int totalIndices = i_mesh->GetPolygonCount() *i_mesh->GetPolygonSize( 0 );

	Utilities::S_NORMAL_MAP_VERTEX_DATA vertexData = {
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR2(0, 0),
		D3DCOLOR_RGBA( 0, 0, 0, 0)
	};
	std::vector<uint16_t> index;
	std::vector<Utilities::S_NORMAL_MAP_VERTEX_DATA> vertex;
	std::vector<bool> onlyVertexPosition;

	for( UINT32 i = 0; i < totalVertices; ++i )
	{
		FbxVector4 position = vertices[i];
		vertexData.position = D3DXVECTOR3( static_cast<float>(position[0]), static_cast<float>(position[1]), static_cast<float>(position[2]) );

		OctreeGenerator::SetMaxSize( static_cast<float>(position[0]), static_cast<float>(position[1]), static_cast<float>(position[2]) );

		onlyVertexPosition.push_back( true );
		vertex.push_back( vertexData );
	}

	UINT32 indexID = 0;
	for( UINT32 i = 0; i < polygonCount; ++i )
	{
		D3DXVECTOR3 a, b, c;
		polygonSize = i_mesh->GetPolygonSize( i );
		for( int j = 0; j < polygonSize; ++j )
		{
			uint16_t controlPointIndex = i_mesh->GetPolygonVertex(i, j);

			// Store the triangle
			switch( j )
			{
			case 0: a = vertex[controlPointIndex].position; break;
			case 1: b = vertex[controlPointIndex].position; break;
			case 2: c = vertex[controlPointIndex].position; break;
			}

			FbxColor colour = GetColour( i_mesh, controlPointIndex, indexID );
			int red = static_cast<int>(colour.mRed * 255);
			int green = static_cast<int>(colour.mGreen * 255);
			int blue = static_cast<int>(colour.mBlue * 255);
			int alpha = static_cast<int>(colour.mAlpha * 255);
			D3DCOLOR newColour = D3DCOLOR_RGBA( red, green, blue, alpha );

			FbxVector4 normal = GetNormal( i_mesh, indexID );
			D3DXVECTOR3 newNormal = D3DXVECTOR3( static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2]) );

			FbxVector4 tangent = GetTangent( i_mesh, indexID );
			D3DXVECTOR3 newTangent = D3DXVECTOR3( static_cast<float>(tangent[0]), static_cast<float>(tangent[1]), static_cast<float>(tangent[2]) );

			FbxVector4 binormal = GetBinormal( i_mesh, indexID );
			D3DXVECTOR3 newBinormal = D3DXVECTOR3( static_cast<float>(binormal[0]), static_cast<float>(binormal[1]), static_cast<float>(binormal[2]) );

			FbxVector2 UV = GetUV( i_mesh, controlPointIndex, indexID, i, j );
			D3DXVECTOR2 newUV = D3DXVECTOR2( static_cast<float>(UV[0]), static_cast<float>(UV[1]) );

			if( onlyVertexPosition[controlPointIndex] )
			{
				vertex[controlPointIndex].colour = newColour;
				vertex[controlPointIndex].texcoord = newUV;
				vertex[controlPointIndex].normal = newNormal;
				vertex[controlPointIndex].biNormal = newBinormal;
				vertex[controlPointIndex].tangent = newTangent;
				onlyVertexPosition[controlPointIndex] = false;
			}
			else
			{
				vertexData = vertex.at( controlPointIndex );
				bool bDifferencesFound = false;

				if( vertexData.colour != newColour )
					bDifferencesFound = true;

				if( !Utilities::Math::AreRelativelyEqual(vertexData.texcoord.x, newUV.x) || !Utilities::Math::AreRelativelyEqual(vertexData.texcoord.y, newUV.y) )
					bDifferencesFound = true;

				if( !Utilities::Math::AreRelativelyEqual(vertexData.normal.x, newNormal.x) || !Utilities::Math::AreRelativelyEqual(vertexData.normal.y, newNormal.y)
					|| !Utilities::Math::AreRelativelyEqual(vertexData.normal.z, newNormal.z) )
					bDifferencesFound = true;

				if( !Utilities::Math::AreRelativelyEqual(vertexData.biNormal.x, newBinormal.x) || !Utilities::Math::AreRelativelyEqual(vertexData.biNormal.y, newBinormal.y)
					|| !Utilities::Math::AreRelativelyEqual(vertexData.biNormal.z, newBinormal.z) )
					bDifferencesFound = true;

				if( !Utilities::Math::AreRelativelyEqual(vertexData.tangent.x, newTangent.x) || !Utilities::Math::AreRelativelyEqual(vertexData.tangent.y, newTangent.y)
					|| !Utilities::Math::AreRelativelyEqual(vertexData.tangent.z, newTangent.z) )
					bDifferencesFound = true;

				if( bDifferencesFound )
				{
					vertexData.colour = newColour;
					vertexData.texcoord = newUV;
					vertexData.normal = newNormal;
					vertexData.biNormal = newBinormal;
					vertexData.tangent = newTangent;
					controlPointIndex = vertex.size();
					vertex.push_back( vertexData );
					totalVertices++;
				}
			}

			index.push_back( controlPointIndex );
			indexID++;
		}

		OctreeGenerator::AddTriangle( a, b , c, emptyHash );
	}

	meshFile.write( reinterpret_cast <const char*> (&totalVertices), sizeof(totalVertices) );
	meshFile.write( reinterpret_cast <const char*> (&polygonCount), sizeof(polygonCount) );
#ifdef GENERATE_BINARY_DUMP_FILE
	binaryDump << "<vertex>" << std::endl;
	binaryDump << totalVertices << std::endl;
#endif	// #ifdef GENERATE_BINARY_DUMP_FILE
	for( unsigned int i = 0; i < totalVertices; ++i )
	{
		meshFile.write( reinterpret_cast <const char*> (&vertex[i]), sizeof(Utilities::S_NORMAL_MAP_VERTEX_DATA) );

#ifdef GENERATE_BINARY_DUMP_FILE
		binaryDump << "{" << std::endl;

		binaryDump << "    <position>" << std::endl;
		binaryDump << "        " << vertex[i].position.x << ", " << vertex[i].position.y << ", " << vertex[i].position.z << std::endl;
		binaryDump << "    </position>" << std::endl;

		binaryDump << "    <colour>" << std::endl;
		binaryDump << "        " << vertex[i].colour << std::endl;
		binaryDump << "    </colour>" << std::endl;

		binaryDump << "    <normal>" << std::endl;
		binaryDump << "        " << vertex[i].normal.x << ", " << vertex[i].normal.y << ", " << vertex[i].normal.z << std::endl;
		binaryDump << "    </normal>" << std::endl;

		binaryDump << "    <tangent>" << std::endl;
		binaryDump << "        " << vertex[i].tangent.x << ", " << vertex[i].tangent.y << ", " << vertex[i].tangent.z << std::endl;
		binaryDump << "    </tangent>" << std::endl;

		binaryDump << "    <biNormal>" << std::endl;
		binaryDump << "        " << vertex[i].biNormal.x << ", " << vertex[i].biNormal.y << ", " << vertex[i].biNormal.z << std::endl;
		binaryDump << "    </biNormal>" << std::endl;

		binaryDump << "    <uv>" << std::endl;
		binaryDump << "        " << vertex[i].texcoord.x << ", " << vertex[i].texcoord.y << std::endl;
		binaryDump << "    </uv>" << std::endl;

		binaryDump << "}" << std::endl;
#endif	// #ifdef GENERATE_BINARY_DUMP_FILE
	}
#ifdef GENERATE_BINARY_DUMP_FILE
	binaryDump << "</vertex>" << std::endl;
	binaryDump << std::endl;
	binaryDump << "<index>" << std::endl;
	binaryDump << totalIndices << std::endl;
#endif	// #ifdef GENERATE_BINARY_DUMP_FILE

	for( unsigned int i = 0; i < index.size(); ++i )
	{
		meshFile.write( reinterpret_cast <const char*>(&index[i]), sizeof(uint16_t) );
#ifdef GENERATE_BINARY_DUMP_FILE
		binaryDump << "    " << index.at(i) << std::endl;
#endif	// #ifdef GENERATE_BINARY_DUMP_FILE
	}
#ifdef GENERATE_BINARY_DUMP_FILE
	binaryDump << "</index>" << std::endl;
	binaryDump.close();
#endif	// #ifdef GENERATE_BINARY_DUMP_FILE

	meshFile.close();
	return true;
}

FbxColor Tools::AssetGenerator::GetColour( FbxMesh *i_mesh, int i_controlPointIdx )
{
	int totalVertexColour = i_mesh->GetElementVertexColorCount();

	for( int i = 0; i < totalVertexColour; ++i )
	{
		FbxGeometryElementVertexColor *elementVectorColor = i_mesh->GetElementVertexColor( i );

		if( elementVectorColor->GetMappingMode() == FbxGeometryElement::eByControlPoint )
		{
			switch( elementVectorColor->GetReferenceMode() )
			{
				case FbxGeometryElement::eDirect:
					return elementVectorColor->GetDirectArray().GetAt( i_controlPointIdx );
					break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int ID = elementVectorColor->GetIndexArray().GetAt( i_controlPointIdx );
					return elementVectorColor->GetDirectArray().GetAt(ID);
					break;
				}

				default:
					break; // other reference modes not shown here!
			}
		}
	}

	return FbxColor( 0, 0, 0 );
}

FbxColor Tools::AssetGenerator::GetColour( FbxMesh *i_mesh, int i_controlPointIdx, int i_indexIDx )
{
	int totalVertexColour = i_mesh->GetElementVertexColorCount();

	for( int i = 0; i < totalVertexColour; ++i )
	{
		FbxGeometryElementVertexColor *elementVectorColor = i_mesh->GetElementVertexColor( i );

		switch( elementVectorColor->GetMappingMode() )
		{
			case FbxGeometryElement::eByControlPoint:
			{
				switch( elementVectorColor->GetReferenceMode() )
				{
					case FbxGeometryElement::eDirect:
						return elementVectorColor->GetDirectArray().GetAt( i_controlPointIdx );
						break;

					case FbxGeometryElement::eIndexToDirect:
					{
						int ID = elementVectorColor->GetIndexArray().GetAt( i_controlPointIdx );
						return elementVectorColor->GetDirectArray().GetAt(ID);
						break;
					}

					default:
						break; // other reference modes not shown here!
				}
				break;
			}

			case FbxGeometryElement::eByPolygonVertex:
			{
				switch( elementVectorColor->GetReferenceMode() )
				{
					case FbxGeometryElement::eDirect:
						return elementVectorColor->GetDirectArray().GetAt( i_indexIDx );
						break;

					case FbxGeometryElement::eIndexToDirect:
					{
						int ID = elementVectorColor->GetIndexArray().GetAt( i_indexIDx );
						return elementVectorColor->GetDirectArray().GetAt( ID );
						break;
					}

					default:
						break; // other reference modes not shown here!
				}
				break;
			}

			default:
			case FbxGeometryElement::eByPolygon:	// doesn't make much sense for UVs
			case FbxGeometryElement::eAllSame:		// doesn't make much sense for UVs
			case FbxGeometryElement::eNone:			// doesn't make much sense for UVs
				break;
		}
	}

	return FbxColor( 0, 0, 0 );
}

FbxVector2 Tools::AssetGenerator::GetUV( FbxMesh *i_mesh, int i_controlPointIdx )
{
	int totalUV = i_mesh->GetElementUVCount();

	for( int i = 0; i < totalUV; ++i )
	{
		FbxGeometryElementUV *elementUV = i_mesh->GetElementUV( i );

		if( elementUV->GetMappingMode() == FbxGeometryElement::eByControlPoint )
		{
			switch (elementUV->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
					return elementUV->GetDirectArray().GetAt( i_controlPointIdx );
					break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int ID = elementUV->GetIndexArray().GetAt( i_controlPointIdx );
					return elementUV->GetDirectArray().GetAt( ID );
					break;
				}

				default:
					break;	// other reference modes not shown here!
			}
		}
	}

	return FbxVector2( 0, 0 );
}

FbxVector2 Tools::AssetGenerator::GetUV( FbxMesh *i_mesh, int i_controlPointIdx, int i_indexIDx, int i_polygonCtr, int i_polygonSizeCtr )
{
	int totalUV = i_mesh->GetElementUVCount();

	for( int i = 0; i < totalUV; ++i )
	{
		FbxGeometryElementUV *elementUV = i_mesh->GetElementUV( i );

		switch( elementUV->GetMappingMode() )
		{
			case FbxGeometryElement::eByControlPoint:
			{
				switch (elementUV->GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
						return elementUV->GetDirectArray().GetAt( i_controlPointIdx );
						break;

					case FbxGeometryElement::eIndexToDirect:
					{
						int ID = elementUV->GetIndexArray().GetAt( i_controlPointIdx );
						return elementUV->GetDirectArray().GetAt( ID );
						break;
					}

					default:
						break;	// other reference modes not shown here!
				}
				break;
			}

			case FbxGeometryElement::eByPolygonVertex:
			{
				int textureUVIdx = i_mesh->GetTextureUVIndex( i_polygonCtr, i_polygonSizeCtr );
				switch( elementUV->GetReferenceMode() )
				{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
						return elementUV->GetDirectArray().GetAt( textureUVIdx );
						break;

					default:
						break;	// other reference modes not shown here!
				}
				break;
			}

			default:
			case FbxGeometryElement::eByPolygon:	// doesn't make much sense for UVs
			case FbxGeometryElement::eAllSame:		// doesn't make much sense for UVs
			case FbxGeometryElement::eNone:			// doesn't make much sense for UVs
				break;
		}
	}

	return FbxVector2( 0, 0 );
}

FbxVector4 Tools::AssetGenerator::GetNormal( FbxMesh *i_mesh, int i_indexIDx )
{
	int totalNormal = i_mesh->GetElementNormalCount();

	for( int i = 0; i < totalNormal; ++i )
	{
		FbxGeometryElementNormal *elementNormal = i_mesh->GetElementNormal( i );

		if( elementNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
		{
			switch( elementNormal->GetReferenceMode() )
			{
				case FbxGeometryElement::eDirect:
					return elementNormal->GetDirectArray().GetAt( i_indexIDx );
					break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int ID = elementNormal->GetIndexArray().GetAt( i_indexIDx );
					return elementNormal->GetDirectArray().GetAt( ID );
					break;
				}

				default:
					break; // other reference modes not shown here!
			}
		}
	}

	return FbxVector4( 0, 0, 0, 0 );
}

FbxVector4 Tools::AssetGenerator::GetNormalByControlPoint( FbxMesh *i_mesh, int i_controlPointIdx )
{
	int totalNormal = i_mesh->GetElementNormalCount();

	for( int i = 0; i < totalNormal; ++i )
	{
		FbxGeometryElementNormal *elementNormal = i_mesh->GetElementNormal( i );

		if( elementNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint )
		{
			switch( elementNormal->GetReferenceMode() )
			{
				case FbxGeometryElement::eDirect:
					return elementNormal->GetDirectArray().GetAt( i_controlPointIdx );
					break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int ID = elementNormal->GetIndexArray().GetAt( i_controlPointIdx );
					return elementNormal->GetDirectArray().GetAt( ID );
					break;
				}

				default:
					break; // other reference modes not shown here!
			}
		}
	}

	return FbxVector4( 0, 0, 0, 0 );
}

FbxVector4 Tools::AssetGenerator::GetTangent( FbxMesh *i_mesh, int i_indexIDx )
{
	int totalTangent = i_mesh->GetElementTangentCount();

	for( int i = 0; i < totalTangent; ++i )
	{
		FbxGeometryElementTangent *elementTangent = i_mesh->GetElementTangent( i );

		if( elementTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
		{
			switch( elementTangent->GetReferenceMode() )
			{
				case FbxGeometryElement::eDirect:
					return elementTangent->GetDirectArray().GetAt( i_indexIDx );
					break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int ID = elementTangent->GetIndexArray().GetAt( i_indexIDx );
					return elementTangent->GetDirectArray().GetAt( ID );
					break;
				}

				default:
					break; // other reference modes not shown here!
			}
		}
	}

	return FbxVector4( 0, 0, 0, 0 );
}

FbxVector4 Tools::AssetGenerator::GetTangentByControlPoint( FbxMesh *i_mesh, int i_controlPointIdx )
{
	int totalTangent = i_mesh->GetElementTangentCount();

	for( int i = 0; i < totalTangent; ++i )
	{
		FbxGeometryElementTangent *elementTangent = i_mesh->GetElementTangent( i );

		if( elementTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint )
		{
			switch( elementTangent->GetReferenceMode() )
			{
				case FbxGeometryElement::eDirect:
					return elementTangent->GetDirectArray().GetAt( i_controlPointIdx );
					break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int ID = elementTangent->GetIndexArray().GetAt( i_controlPointIdx );
					return elementTangent->GetDirectArray().GetAt( ID );
					break;
				}

				default:
					break; // other reference modes not shown here!
			}
		}
	}

	return FbxVector4( 0, 0, 0, 0 );
}

FbxVector4 Tools::AssetGenerator::GetBinormal( FbxMesh *i_mesh, int i_indexIDx )
{
	int totalBinormal = i_mesh->GetElementBinormalCount();

	for( int i = 0; i < totalBinormal; ++i )
	{
		FbxGeometryElementBinormal *elementBinormal = i_mesh->GetElementBinormal( i );

		if( elementBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
		{
			switch ( elementBinormal->GetReferenceMode() )
			{
				case FbxGeometryElement::eDirect:
					return elementBinormal->GetDirectArray().GetAt( i_indexIDx );
					break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int ID = elementBinormal->GetIndexArray().GetAt( i_indexIDx );
					return elementBinormal->GetDirectArray().GetAt( ID );
					break;
				}

				default:
					break; // other reference modes not shown here!
			}
		}
	}

	return FbxVector4( 0, 0, 0, 0 );
}

FbxVector4 Tools::AssetGenerator::GetBinormalByControlPoint( FbxMesh *i_mesh, int i_controlPointIdx )
{
	int totalBinormal = i_mesh->GetElementBinormalCount();

	for( int i = 0; i < totalBinormal; ++i )
	{
		FbxGeometryElementBinormal *elementBinormal = i_mesh->GetElementBinormal( i );

		if( elementBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint )
		{
			switch ( elementBinormal->GetReferenceMode() )
			{
				case FbxGeometryElement::eDirect:
					return elementBinormal->GetDirectArray().GetAt( i_controlPointIdx );
					break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int ID = elementBinormal->GetIndexArray().GetAt( i_controlPointIdx );
					return elementBinormal->GetDirectArray().GetAt( ID );
					break;
				}

				default:
					break; // other reference modes not shown here!
			}
		}
	}

	return FbxVector4( 0, 0, 0, 0 );
}
