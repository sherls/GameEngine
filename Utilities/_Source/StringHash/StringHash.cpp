/**
 ****************************************************************************************************
 * \file		StringHash.cpp
 * \brief		Implementation of String Hash class
 ****************************************************************************************************
*/

#include "StringHash.h"

UINT32 Utilities::StringHash::Hash( const char *i_string )
{
	assert( i_string );

	FUNCTION_START;

	return Hash( reinterpret_cast<void*>( const_cast<char*>(i_string)), strlen(i_string) );

	FUNCTION_FINISH;
}

UINT32 Utilities::StringHash::Hash( void *i_bytes, UINT32 i_count )
{
	// FNV hash, http://isthe.com/chongo/tech/comp/fnv/
	FUNCTION_START;

	register const unsigned char * p = static_cast<const unsigned char *>( i_bytes );
	UINT32 hash = 2166136261;

	for( UINT32 i = 0; i < i_count; ++i )
		hash = 16777619 * (hash ^ p[i]);

	FUNCTION_FINISH;
	return hash ^ (hash >> 16);
}