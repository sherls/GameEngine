/**
 ****************************************************************************************************
 * \file		MeshParser.inl
 * \brief		The inline functions implementation of mesh parser
 ****************************************************************************************************
*/

#include <assert.h>

namespace Utilities
{
/**
 ****************************************************************************************************
	\fn			MeshParser( const char *i_fileName )
	\brief		Constructor of MeshParser class
	\param		i_fileName name of the file where mesh data is located
	\return		NONE
 ****************************************************************************************************
*/
MeshParser::MeshParser( const char *i_fileName ) :
	m_tag( NULL ),
	m_u32TotalVertices( 0 ),
	m_u32TotalPrimitives( 0 )
{
	//LogMessage( "Mesh parser created" );
	assert( i_fileName != NULL );
	assert( i_fileName[0] != '\0' );

	LoadVertexData( i_fileName );
}

/**
 ****************************************************************************************************
	\fn			~MeshParser( void )
	\brief		Destructor of MeshParser class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
MeshParser::~MeshParser( void )
{
	if( m_tag )
	{
		delete m_tag;
		m_tag = NULL;
	}

	if( _vertex )
	{
		delete []_vertex;
		_vertex = NULL;
	}

	if( _index )
	{
		delete []_index;
		_index = NULL;
	}
}
}