/**
 ****************************************************************************************************
 * \file		StringHash.inl
 * \brief		The inline functions implementation of StringHash.h
 ****************************************************************************************************
*/

#include <assert.h>

#include "../Debug/Debug.h"

namespace Utilities
{
/****************************************************************************************************
			Public functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			StringHash( void )
	\brief		StringHash class default constructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
StringHash::StringHash( void ) :
	_hash( Hash("") )
{
}

/**
 ****************************************************************************************************
	\fn			StringHash( const char *i_string )
	\brief		StringHash class copy constructor
	\param		*i_string pointer to be copied
	\return		NONE
 ****************************************************************************************************
*/
StringHash::StringHash( const char *i_string ) :
	_hash( Hash(i_string) )
//#ifdef _DEBUG
//	, _string( i_string )
//#endif	// #ifdef _DEBUG
{
}

/**
 ****************************************************************************************************
	\fn			StringHash( const StringHash &i_other )
	\brief		StringHash class copy constructor
	\param		*i_other StringHash to be copied
	\return		NONE
 ****************************************************************************************************
*/
StringHash::StringHash( const StringHash &i_other ) :
	_hash( i_other._hash )
//#ifdef _DEBUG
//	, _string( i_other._string )
//#endif	// #ifdef _DEBUG
{
}

/**
 ****************************************************************************************************
	\fn			~StringHash( void )
	\brief		Standard destructor of StringHash class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
StringHash::~StringHash( void )
{
}

/**
 ****************************************************************************************************
	\fn			bool operator==( const StringHash &i_other )
	\brief		== operator of StringHash class
	\param		*i_ptr pointer to be compared
	\return		boolean
	\retval		TRUE equal
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool StringHash::operator==( const StringHash &i_other )
{
	return _hash == i_other._hash;
}

/**
 ****************************************************************************************************
	\fn			operator bool( void ) const
	\brief		Operator bool of StringHash class
	\param		NONE
	\return		boolean
	\retval		TRUE pointer is not NULL
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
StringHash::operator UINT32( void ) const
{
	return _hash;
}

}	// namespace GameEngine