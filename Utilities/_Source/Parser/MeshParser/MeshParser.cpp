/**
 ****************************************************************************************************
 * \file		MeshParser.cpp
 * \brief		Mesh parser class implementation
 ****************************************************************************************************
*/

#include <fstream>

#include "../TagList.h"
#include "../ParserHelper.h"
#include "../../Debug/Debug.h"

#include "MeshParser.h"

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void LoadVertexData( const char *i_fileName )
	\brief		Load vertex data from input file
	\param		i_fileName name of the file where vertex data is located
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::MeshParser::LoadVertexData( const char *i_fileName )
{
	std::ifstream in;
	char fileName[MAX_FILENAME_INPUT];
	strcpy_s( fileName, MAX_FILENAME_INPUT, "../../External/Data/Meshes/" );
	strcat_s( fileName, MAX_FILENAME_INPUT, i_fileName);

	in.open( fileName, std::ios::binary );

	if( in.fail() )
	{
		DEBUG_MSG( DBG_RENDERER, D_ERR, "Couldn't open meshes data %s", i_fileName );
		return;
	}

	in.seekg( 0, std::ios::beg );
	in.read( (char*)&m_u32TotalVertices, sizeof(m_u32TotalVertices) );

	in.read( (char*)&m_u32TotalPrimitives, sizeof(m_u32TotalPrimitives) );

	char* tempVertex = new char[m_u32TotalVertices*sizeof(S_NORMAL_MAP_VERTEX_DATA)];
	in.read( tempVertex, m_u32TotalVertices*sizeof(S_NORMAL_MAP_VERTEX_DATA) );
	_vertex = new S_NORMAL_MAP_VERTEX_DATA[m_u32TotalVertices];
	memcpy( _vertex, tempVertex, m_u32TotalVertices * sizeof(S_NORMAL_MAP_VERTEX_DATA) );

	char* tempIndex = new char[m_u32TotalPrimitives*3*sizeof(uint16_t)];
	in.read( tempIndex, m_u32TotalPrimitives*3*sizeof(uint16_t) );
	_index = new uint16_t[m_u32TotalPrimitives*3];
	memcpy( _index, tempIndex, m_u32TotalPrimitives*3*sizeof(uint16_t) );

	if( !in.eof() )
	{
		UINT32 tagNameLength = 0;
		in.read( (char*)&tagNameLength, sizeof(UINT32) );
		if( tagNameLength > 0 )
		{
			char *tempName = new char[tagNameLength+1];
			in.read( tempName, sizeof(char) * tagNameLength );
			tempName[tagNameLength] = '\0';
			m_tag = new std::string( tempName );
			delete [] tempName;
		}
	}

	delete []tempVertex;
	delete []tempIndex;

	in.close();

	return;
}

/**
 ****************************************************************************************************
	\fn			const void GetVertexData( void* o_vertexData ) const
	\brief		Get vertex data
	\param		*o_vertexData pointer to returned vertex data
	\return		NONE
 ****************************************************************************************************
*/
const void Utilities::MeshParser::GetVertexData( S_NORMAL_MAP_VERTEX_DATA* o_vertexData ) const
{
	memcpy( o_vertexData, _vertex, m_u32TotalVertices * sizeof(S_NORMAL_MAP_VERTEX_DATA) );

	return;
}

/**
 ****************************************************************************************************
	\fn			const void GetIndexData( void* o_indexData ) const
	\brief		Get index data
	\param		*o_indexData pointer to returned vertex data
	\return		NONE
 ****************************************************************************************************
*/
const void Utilities::MeshParser::GetIndexData( uint16_t* o_indexData ) const
{
	memcpy( o_indexData, _index, m_u32TotalPrimitives * 3 * sizeof(uint16_t) );

	return;
}
