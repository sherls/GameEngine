/**
 ****************************************************************************************************
 * \file		EffectParser.inl
 * \brief		The inline functions implementation of effect parser
 ****************************************************************************************************
*/

#include <assert.h>

namespace Utilities
{
/**
 ****************************************************************************************************
	\fn			EffectParser( const char *i_filename )
	\brief		Constructor of EffectParser class
	\param		i_fileName name of the file where effect data is located
	\return		NONE
 ****************************************************************************************************
*/
EffectParser::EffectParser( const char *i_fileName )
{
	assert( i_fileName != NULL );
	assert( i_fileName[0] != '\0' );

	LoadEffectData( i_fileName );
}

/**
 ****************************************************************************************************
	\fn			~EffectParser( void )
	\brief		Destructor of EffectParser class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
EffectParser::~EffectParser( void )
{
	m_vertexShader.clear();
	delete m_fragmentShader;
}
}