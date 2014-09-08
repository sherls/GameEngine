#include "DisplayMesh.h"

#ifdef DUMP_FBX_INFO
#include "DisplayLink.h"
#include "DisplayShape.h"
#include "DisplayTexture.h"
#include "DisplayMaterial.h"

#define MAT_HEADER_LENGTH 256

namespace Tools
{
	namespace DisplayMesh
	{
		void DisplayControlsPoints( FbxMesh *i_mesh );
		void DisplayPolygons( FbxMesh *i_mesh );
		void DisplayMaterialMapping( FbxMesh *i_mesh );
		void DisplayTextureMapping( FbxMesh *i_mesh );
		void DisplayTextureNames( FbxProperty &i_property, FbxString& i_connectionString );
		void DisplayMaterialConnections( FbxMesh *i_mesh );
		void DisplayMaterialTextureConnections( FbxSurfaceMaterial* i_material, char *i_header, int i_matID, int l );
	}
}

void Tools::DisplayMesh::DisplayMesh( FbxNode* i_node )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nMesh\n--------------------" );
	FbxMesh *mesh = (FbxMesh*) i_node->GetNodeAttribute();

	DisplayCommon::DisplayString( "Mesh Name: ", (char *) i_node->GetName() );
	DisplayCommon::DisplayMetaDataConnections( mesh );
	DisplayControlsPoints( mesh );
	DisplayPolygons( mesh );
	DisplayMaterialMapping( mesh );
	DisplayMaterial::DisplayMaterial( mesh );
	DisplayTexture::DisplayTexture( mesh );
	DisplayMaterialConnections( mesh );
	DisplayLink::DisplayLink( mesh );
	DisplayShape::DisplayShape( mesh );
}

void Tools::DisplayMesh::DisplayControlsPoints( FbxMesh *i_mesh )
{
	int i, controlPointsCount = i_mesh->GetControlPointsCount();
	FbxVector4 *controlPoints = i_mesh->GetControlPoints();

	DisplayCommon::DisplayString( "    Control Points" );
	DisplayCommon::DisplayInt( "    Total vertices: ", controlPointsCount );
	for( i = 0; i < controlPointsCount; i++ )
	{
		DisplayCommon::DisplayInt( "        Control Point ", i );
		DisplayCommon::Display3DVector( "            Coordinates: ", controlPoints[i] );

		for( int j = 0; j < i_mesh->GetElementNormalCount(); j++ )
		{
			FbxGeometryElementNormal* elementNormals = i_mesh->GetElementNormal( j );
			if( elementNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint )
			{
				char header[100];
				FBXSDK_sprintf( header, 100, "            Normal Vector: " ); 
				if( elementNormals->GetReferenceMode() == FbxGeometryElement::eDirect )
					DisplayCommon::Display3DVector( header, elementNormals->GetDirectArray().GetAt(i) );
			}
		}
	}
	DisplayCommon::DisplayString("");
}

void Tools::DisplayMesh::DisplayPolygons( FbxMesh *i_mesh )
{
	int i, j, polygonCount = i_mesh->GetPolygonCount();
	FbxVector4 *controlPoints = i_mesh->GetControlPoints(); 
	char header[MAT_HEADER_LENGTH];

	DisplayCommon::DisplayString( "    Polygons" );

	int vertexID = 0;
	DisplayCommon::DisplayInt( "    Total indices: ", polygonCount * i_mesh->GetPolygonSize(0) );
	for( i = 0; i < polygonCount; i++ )
	{
		DisplayCommon::DisplayInt( "        Polygon ", i );
		int l;

		for( l = 0; l < i_mesh->GetElementPolygonGroupCount(); l++ )
		{
			FbxGeometryElementPolygonGroup *elementPolygonGroup = i_mesh->GetElementPolygonGroup(l);
			switch( elementPolygonGroup->GetMappingMode() )
			{
			case FbxGeometryElement::eByPolygon:
				if( elementPolygonGroup->GetReferenceMode() == FbxGeometryElement::eIndex )
				{
					FBXSDK_sprintf( header, MAT_HEADER_LENGTH, "        Assigned to group: " );
					int polyGroupID = elementPolygonGroup->GetIndexArray().GetAt( i );
					DisplayCommon::DisplayInt( header, polyGroupID );
					break;
				}
			default:
				// any other mapping modes don't make sense
				DisplayCommon::DisplayString( "        \"unsupported group assignment\"" );
				break;
			}
		}

		int polygonSize = i_mesh->GetPolygonSize( i );

		for( j = 0; j < polygonSize; j++ )
		{
			int controlPointIndex = i_mesh->GetPolygonVertex( i, j );

			DisplayCommon::DisplayInt( "        Vertices (control point): ", controlPointIndex );
			DisplayCommon::Display3DVector( "            Coordinates: ", controlPoints[controlPointIndex] );

			DisplayCommon::DisplayInt( "            Total vertex colour: ", i_mesh->GetElementVertexColorCount() );
			for( l = 0; l < i_mesh->GetElementVertexColorCount(); l++ )
			{
				FbxGeometryElementVertexColor *elementVectorColor = i_mesh->GetElementVertexColor( l );
				FBXSDK_sprintf( header, MAT_HEADER_LENGTH, "            Color vertex: " ); 

				switch( elementVectorColor->GetMappingMode() )
				{
				case FbxGeometryElement::eByControlPoint:
					DisplayCommon::DisplayString( "            =====>By control point" );
					switch( elementVectorColor->GetReferenceMode() )
					{
					case FbxGeometryElement::eDirect:
						DisplayCommon::DisplayColor( header, elementVectorColor->GetDirectArray().GetAt(controlPointIndex) );
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = elementVectorColor->GetIndexArray().GetAt( controlPointIndex );
							DisplayCommon::DisplayColor( header, elementVectorColor->GetDirectArray().GetAt(id) );
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{
						//DisplayCommon::DisplayString( "By polygon vertex" );
						switch( elementVectorColor->GetReferenceMode() )
						{
						case FbxGeometryElement::eDirect:
							DisplayCommon::DisplayColor( header, elementVectorColor->GetDirectArray().GetAt(vertexID) );
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = elementVectorColor->GetIndexArray().GetAt( vertexID );
								DisplayCommon::DisplayColor( header, elementVectorColor->GetDirectArray().GetAt(id) );
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}

			DisplayCommon::DisplayInt( "            Total UV: ", i_mesh->GetElementUVCount() );
			for( l = 0; l < i_mesh->GetElementUVCount(); l++ )
			{
				FbxGeometryElementUV *elementUV = i_mesh->GetElementUV( l );
				FBXSDK_sprintf( header, MAT_HEADER_LENGTH, "            Texture UV: " ); 

				switch( elementUV->GetMappingMode() )
				{
				case FbxGeometryElement::eByControlPoint:
					DisplayCommon::DisplayString( "            =====>By control point" );
					switch (elementUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						DisplayCommon::Display2DVector( header, elementUV->GetDirectArray().GetAt(controlPointIndex) );
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = elementUV->GetIndexArray().GetAt( controlPointIndex );
							DisplayCommon::Display2DVector( header, elementUV->GetDirectArray().GetAt(id) );
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{
						DisplayCommon::DisplayString( "            =====>By polygon vertex" );
						int lTextureUVIndex = i_mesh->GetTextureUVIndex( i, j );
						switch( elementUV->GetReferenceMode() )
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
							{
								DisplayCommon::Display2DVector( header, elementUV->GetDirectArray().GetAt(lTextureUVIndex) );
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}

			DisplayCommon::DisplayInt( "            Total normal: ", i_mesh->GetElementNormalCount() );
			for( l = 0; l < i_mesh->GetElementNormalCount(); l++ )
			{
				FbxGeometryElementNormal *elementNormal = i_mesh->GetElementNormal( l );
				FBXSDK_sprintf( header, MAT_HEADER_LENGTH, "            Normal: "); 

				if( elementNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
				{
					switch( elementNormal->GetReferenceMode() )
					{
					case FbxGeometryElement::eDirect:
						DisplayCommon::Display3DVector( header, elementNormal->GetDirectArray().GetAt(vertexID) );
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = elementNormal->GetIndexArray().GetAt( vertexID );
							DisplayCommon::Display3DVector( header, elementNormal->GetDirectArray().GetAt(id) );
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
				else
					DisplayCommon::DisplayString( "            =====>By control point" );
			}

			DisplayCommon::DisplayInt( "            Total tangent: ", i_mesh->GetElementTangentCount() );
			for( l = 0; l < i_mesh->GetElementTangentCount(); l++ )
			{
				FbxGeometryElementTangent *elementTangent = i_mesh->GetElementTangent( l);
				FBXSDK_sprintf( header, MAT_HEADER_LENGTH, "            Tangent: " );

				if( elementTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
				{
					switch( elementTangent->GetReferenceMode() )
					{
					case FbxGeometryElement::eDirect:
						DisplayCommon::Display3DVector( header, elementTangent->GetDirectArray().GetAt(vertexID) );
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = elementTangent->GetIndexArray().GetAt( vertexID );
							DisplayCommon::Display3DVector( header, elementTangent->GetDirectArray().GetAt(id) );
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
				else
					DisplayCommon::DisplayString( "            =====>By control point" );
			}

			DisplayCommon::DisplayInt( "            Total binormal: ", i_mesh->GetElementBinormalCount() );
			for( l = 0; l < i_mesh->GetElementBinormalCount(); l++ )
			{
				FbxGeometryElementBinormal *elementBinormal = i_mesh->GetElementBinormal( l );

				FBXSDK_sprintf( header, MAT_HEADER_LENGTH, "            Binormal: " );
				if( elementBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
				{
					switch ( elementBinormal->GetReferenceMode() )
					{
					case FbxGeometryElement::eDirect:
						DisplayCommon::Display3DVector( header, elementBinormal->GetDirectArray().GetAt(vertexID) );
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = elementBinormal->GetIndexArray().GetAt( vertexID );
							DisplayCommon::Display3DVector( header, elementBinormal->GetDirectArray().GetAt(id) );
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
				else
					DisplayCommon::DisplayString( "            =====>By control point" );
			}
			vertexID++;
		} // for polygonSize
	} // for polygonCount

	//check visibility for the edges of the mesh
	DisplayCommon::DisplayInt( "            Total element visibility: ", i_mesh->GetElementVisibilityCount() );
	for( int l = 0; l < i_mesh->GetElementVisibilityCount(); l++ )
	{
		FbxGeometryElementVisibility *elementVisibility = i_mesh->GetElementVisibility( l );
		FBXSDK_sprintf( header, MAT_HEADER_LENGTH, "    Edge Visibility : " );
		DisplayCommon::DisplayString( header );
		switch( elementVisibility->GetMappingMode() )
		{
			//should be eByEdge
		case FbxGeometryElement::eByEdge:
			//should be eDirect
			for( int j = 0; j != i_mesh->GetMeshEdgeCount(); j++ )
			{
				DisplayCommon::DisplayInt( "        Edge ", j );
				DisplayCommon::DisplayBool( "              Edge visibility: ", elementVisibility->GetDirectArray().GetAt(j) );
			}
			break;
		}
	}
	DisplayCommon::DisplayString( "" );
}

void Tools::DisplayMesh::DisplayTextureNames( FbxProperty &i_property, FbxString& i_connectionString )
{
	int layeredTextureCount = i_property.GetSrcObjectCount<FbxLayeredTexture>();
	if( layeredTextureCount > 0 )
	{
		for( int j = 0; j < layeredTextureCount; j++ )
		{
			FbxLayeredTexture *layeredTexture = i_property.GetSrcObject<FbxLayeredTexture>( j );
			int totalTextures = layeredTexture->GetSrcObjectCount<FbxTexture>();
			i_connectionString += " Texture ";

			for( int k = 0; k < totalTextures; k++ )
			{
				//lConnectionString += k;
				i_connectionString += "\"";
				i_connectionString += (char*)layeredTexture->GetName();
				i_connectionString += "\"";
				i_connectionString += " ";
			}
			i_connectionString += "of ";
			i_connectionString += i_property.GetName();
			i_connectionString += " on layer ";
			i_connectionString += j;
		}
		i_connectionString += " |";
	}
	else
	{
		//no layered texture simply get on the property
		int totalTextures = i_property.GetSrcObjectCount<FbxTexture>();

		if( totalTextures > 0 )
		{
			i_connectionString += " Texture ";
			i_connectionString += " ";

			for( int j = 0; j < totalTextures; j++ )
			{
				FbxTexture *texture = i_property.GetSrcObject<FbxTexture>( j );
				if( texture )
				{
					i_connectionString += "\"";
					i_connectionString += (char*)texture->GetName();
					i_connectionString += "\"";
					i_connectionString += " ";
				}
			}
			i_connectionString += "of ";
			i_connectionString += i_property.GetName();
			i_connectionString += " |";
		}
	}
}

void Tools::DisplayMesh::DisplayMaterialTextureConnections( FbxSurfaceMaterial *i_material, char *i_header, int i_materialID, int l )
{
	if( !i_material )
		return;

	FbxString connectionString = "            Material -- " + FbxString( i_materialID );
	//Show all the textures

	FbxProperty lProperty;
	//Diffuse Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sDiffuse );
	DisplayTextureNames( lProperty, connectionString );

	//DiffuseFactor Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sDiffuseFactor );
	DisplayTextureNames( lProperty, connectionString );

	//Emissive Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sEmissive );
	DisplayTextureNames( lProperty, connectionString );

	//EmissiveFactor Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sEmissiveFactor );
	DisplayTextureNames( lProperty, connectionString );

	//Ambient Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sAmbient );
	DisplayTextureNames( lProperty, connectionString );

	//AmbientFactor Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sAmbientFactor );
	DisplayTextureNames( lProperty, connectionString );

	//Specular Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sSpecular );
	DisplayTextureNames( lProperty, connectionString );

	//SpecularFactor Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sSpecularFactor );
	DisplayTextureNames( lProperty, connectionString );

	//Shininess Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sShininess );
	DisplayTextureNames( lProperty, connectionString );

	//Bump Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sBump );
	DisplayTextureNames( lProperty, connectionString );

	//Normal Map Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sNormalMap );
	DisplayTextureNames( lProperty, connectionString );

	//Transparent Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sTransparentColor );
	DisplayTextureNames( lProperty, connectionString );

	//TransparencyFactor Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sTransparencyFactor );
	DisplayTextureNames( lProperty, connectionString );

	//Reflection Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sReflection );
	DisplayTextureNames( lProperty, connectionString );

	//ReflectionFactor Textures
	lProperty = i_material->FindProperty( FbxSurfaceMaterial::sReflectionFactor );
	DisplayTextureNames( lProperty, connectionString );

	//Update header with material info
	bool stringOverflow = ( connectionString.GetLen() + 10 >= MAT_HEADER_LENGTH ); // allow for string length and some padding for "%d"
	if( stringOverflow )
	{
		// Truncate string!
		connectionString = connectionString.Left( MAT_HEADER_LENGTH - 10 );
		connectionString = connectionString + "...";
	}
	//FBXSDK_sprintf( i_header, sizeof(i_header), connectionString.Buffer(), i_materialID, l );
	connectionString += l;
	DisplayCommon::DisplayString( connectionString );
}

void Tools::DisplayMesh::DisplayMaterialConnections( FbxMesh *i_mesh )
{
	int i, l, polygonCount = i_mesh->GetPolygonCount();

	char header[MAT_HEADER_LENGTH];

	DisplayCommon::DisplayString( "    Polygons Material Connections" );

	//check whether the material maps with only one mesh
	bool isAllSame = true;
	for( l = 0; l < i_mesh->GetElementMaterialCount(); l++ )
	{
		FbxGeometryElementMaterial* materialElement = i_mesh->GetElementMaterial( l );
		if( materialElement->GetMappingMode() == FbxGeometryElement::eByPolygon )
		{
			isAllSame = false;
			break;
		}
	}

	//For eAllSame mapping type, just out the material and texture mapping info once
	if( isAllSame )
	{
		DisplayCommon::DisplayString( "All the same " );
		for( l = 0; l < i_mesh->GetElementMaterialCount(); l++ )
		{
			FbxGeometryElementMaterial* materialElement = i_mesh->GetElementMaterial( l );
			if( materialElement->GetMappingMode() == FbxGeometryElement::eAllSame )
			{
				FbxSurfaceMaterial* material = i_mesh->GetNode()->GetMaterial( materialElement->GetIndexArray().GetAt(0) );
				int materialID = materialElement->GetIndexArray().GetAt( 0 );
				if( materialID >= 0 )
				{
					DisplayCommon::DisplayInt( "        All polygons share the same material in mesh ", l );
					DisplayMaterialTextureConnections( material, header, materialID, l );
				}
			}
		}

		//no material
		if( l == 0 )
			DisplayCommon::DisplayString( "        no material applied" );
	}
	//For eByPolygon mapping type, just out the material and texture mapping info once
	else
	{
		for( i = 0; i < polygonCount; i++ )
		{
			DisplayCommon::DisplayInt( "        Polygon ", i );

			for( l = 0; l < i_mesh->GetElementMaterialCount(); l++ )
			{
				FbxGeometryElementMaterial* materialElement = i_mesh->GetElementMaterial( l );
				FbxSurfaceMaterial* material = NULL;
				int materialID = -1;
				material = i_mesh->GetNode()->GetMaterial( materialElement->GetIndexArray().GetAt(i) );
				materialID = materialElement->GetIndexArray().GetAt( i );

				if( materialID >= 0 )
				{
					DisplayMaterialTextureConnections( material, header, materialID, l );
				}
			}
		}
	}
}

void Tools::DisplayMesh::DisplayMaterialMapping( FbxMesh *i_mesh )
{
	const char* mappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
	const char* referenceMode[] = { "Direct", "Index", "Index to Direct" };

	int materialCount = 0;
	FbxNode *node = NULL;
	if( i_mesh )
	{
		node = i_mesh->GetNode();
		if( node )
			materialCount = node->GetMaterialCount();
	}

	DisplayCommon::DisplayInt( "Total material: ", materialCount );

	for( int l = 0; l < i_mesh->GetElementMaterialCount(); l++ )
	{
		FbxGeometryElementMaterial *elementMaterial = i_mesh->GetElementMaterial( l );
		if( elementMaterial )
		{
			char header[100];
			FBXSDK_sprintf( header, 100, "    Material Element %d: ", l );
			DisplayCommon::DisplayString( header );
			DisplayCommon::DisplayString( "           Mapping: ", mappingTypes[elementMaterial->GetMappingMode()] );
			DisplayCommon::DisplayString( "           ReferenceMode: ", referenceMode[elementMaterial->GetReferenceMode()] );

			int materialCount = 0;
			FbxString string;

			if( elementMaterial->GetReferenceMode() == FbxGeometryElement::eDirect ||
				elementMaterial->GetReferenceMode() == FbxGeometryElement::eIndexToDirect )
			{
				materialCount = materialCount;
			}

			if( elementMaterial->GetReferenceMode() == FbxGeometryElement::eIndex ||
				elementMaterial->GetReferenceMode() == FbxGeometryElement::eIndexToDirect )
			{
				int i;

				string = "           Indices: ";

				int indexArrayCount = elementMaterial->GetIndexArray().GetCount();
				for( i = 0; i < indexArrayCount; i++ )
				{
					int index = elementMaterial->GetIndexArray().GetAt( i );
					string += elementMaterial->GetIndexArray().GetAt( i );

					FbxGeometry *geometry = i_mesh;
					FbxNode *node = geometry->GetNode();
					FbxSurfaceMaterial *material = node->GetMaterial( index );
					DisplayCommon::DisplayString( "           Material name: ", material->GetName() );

					if( node )
						materialCount = node->GetMaterialCount();

					if( i < indexArrayCount - 1 )
					{
						string += ", ";
					}
				}

				DisplayCommon::DisplayString( string );
				//string += "\n";
				//FBXSDK_printf( string );
			}
		}
	}

	//DisplayString("");
}
#endif	// #ifdef DUMP_FBX_INFO