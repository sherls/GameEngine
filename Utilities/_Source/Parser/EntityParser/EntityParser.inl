/**
 ****************************************************************************************************
 * \file		EntityParser.inl
 * \brief		The inline functions implementation of entity parser
 ****************************************************************************************************
*/

#include <assert.h>

namespace Utilities
{
/**
 ****************************************************************************************************
	\fn			EntityParser( const char *i_fileName )
	\brief		Constructor of EntityParser class
	\param		i_fileName name of the file where entity data is located
	\return		NONE
 ****************************************************************************************************
*/
EntityParser::EntityParser( const char *i_fileName )
{
	assert( i_fileName != NULL );
	assert( i_fileName[0] != '\0' );

	LoadEntityData( i_fileName );
}

/**
 ****************************************************************************************************
	\fn			~EntityParser( void )
	\brief		Destructor of EntityParser class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
EntityParser::~EntityParser( void )
{
	m_meshData.clear();
	m_materialData.clear();
}
}