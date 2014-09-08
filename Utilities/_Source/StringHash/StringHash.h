/**
 ****************************************************************************************************
 * \file		StringHash.h
 * \brief		The header of String Hash class
 ****************************************************************************************************
*/

#ifndef _STRING_HASH_H_
#define _STRING_HASH_H_

#include <string>
#include <BaseTsd.h>
//#include "../UtilitiesTypes.h"

namespace Utilities
{
	class StringHash
	{
//#ifdef _DEBUG
//		std::string _string;
//#endif	// #ifndef _DEBUG
		UINT32 _hash;

	public:
		// Constructor
		inline StringHash( void );
		inline StringHash( const char *i_string );
		inline StringHash( const StringHash &i_other );

		// Destruction
		inline ~StringHash( void );

		inline bool operator==( const StringHash &i_other );
		inline operator UINT32( void ) const;
		static UINT32 Hash( const char *i_string );
		static UINT32 Hash( void *i_bytes, UINT32 i_count );
	};
}	// namespace GameEngine

#include "StringHash.inl"

#endif	// #ifndef _STRING_HASH_H_