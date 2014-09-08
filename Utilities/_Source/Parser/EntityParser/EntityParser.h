/**
 ****************************************************************************************************
 * \file		EntityParser.h
 * \brief		Entity parser class declaration
 ****************************************************************************************************
*/

#ifndef _ENTITYPARSER_H_
#define _ENTITYPARSER_H_

namespace Utilities
{
	class EntityParser
	{
		// Constructor
		EntityParser( void );

		void LoadEntityData( const char *i_fileName );

	public:
		std::string m_meshData;
		std::string m_materialData;

		// Constructor
		inline EntityParser( const char *i_fileName );

		// Destructor
		inline ~EntityParser( void );
	};
}

#include "EntityParser.inl"
#endif	// #ifndef _ENTITYPARSER_H_