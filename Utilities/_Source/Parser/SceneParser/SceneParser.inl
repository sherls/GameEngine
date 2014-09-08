/**
 ****************************************************************************************************
 * \file		SceneParser.inl
 * \brief		The inline functions implementation of scene parser
 ****************************************************************************************************
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>

/**
 ****************************************************************************************************
	\fn			SceneParser( const char* i_fileName )
	\brief		Constructor of SceneParser class
	\param		i_fileName name of the file where scene data is located
	\return		NONE
 ****************************************************************************************************
*/
Utilities::SceneParser::SceneParser( const char* i_fileName )
{
	assert( strlen(i_fileName) != 0 );

	LoadSceneData( i_fileName );
}

/**
 ****************************************************************************************************
	\fn			~SceneParser( void )
	\brief		Destructor of SceneParser class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
Utilities::SceneParser::~SceneParser( void )
{
	if( m_entity )
	{
		for( UINT32 i = 0; i < m_u32TotalEntity; ++i )
		{
			m_entity[i].file.clear();
		}
		delete []m_entity;
	}

	m_environmentMap.clear();
}