/**
 ****************************************************************************************************
 * \file		MaterialParser.inl
 * \brief		The inline functions implementation of material parser
 ****************************************************************************************************
*/

#include <assert.h>

namespace Utilities
{
/**
 ****************************************************************************************************
	\fn			MaterialParser( const char *i_filename )
	\brief		Constructor of MaterialParser class
	\param		i_fileName name of the file where material data is located
	\return		NONE
 ****************************************************************************************************
*/
MaterialParser::MaterialParser( const char *i_fileName ) :
	m_shininess( 0.0f ),
	m_transparency( 1.0f )
{
	assert( i_fileName != NULL );
	assert( i_fileName[0] != '\0' );

	LoadMaterialData( i_fileName );
}

/**
 ****************************************************************************************************
	\fn			~MaterialParser( void )
	\brief		Destructor of MaterialParser class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
MaterialParser::~MaterialParser( void )
{
	m_effectData.clear();
	m_diffuseColorTexture.clear();
	m_normalMapTexture.clear();
	m_transparentColourTexture.clear();
	//m_textureData->clear();
	//delete m_textureData;
}
}