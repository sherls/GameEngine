/**
 ****************************************************************************************************
 * \file		MeshParser.h
 * \brief		Mesh parser class declaration
 ****************************************************************************************************
*/

#ifndef _MESHPARSER_H_
#define _MESHPARSER_H_

#include <string>

#include "../../UtilitiesTypes.h"

namespace Utilities
{
	class MeshParser
	{
		S_NORMAL_MAP_VERTEX_DATA *_vertex;
		uint16_t *_index;

		void LoadVertexData( const char *i_fileName );

	public:
		std::string *m_tag;
		UINT32 m_u32TotalPrimitives;
		UINT32 m_u32TotalVertices;

		// Constructor
		MeshParser() {}
		inline MeshParser( const char *i_fileName );

		// Destructor
		inline ~MeshParser( void );

		// Implementation
		const void GetVertexData( S_NORMAL_MAP_VERTEX_DATA* o_vertexData ) const;
		const void GetIndexData( uint16_t* o_indexData ) const;
	};
}

#include "MeshParser.inl"

#endif	// #ifndef _C_MESHPARSER_H_