/**
 ****************************************************************************************************
 * \file		MemoryPool.inl
 * \brief		The inline functions implementation of MemoryPool class
 ****************************************************************************************************
*/

#include <assert.h>
#include "../UtilitiesTypes.h"

/**
 ****************************************************************************************************
	\fn			const UINT32 Size( void ) const
	\brief		Get size of each memory pool element
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
const UINT32 Utilities::MemoryPool::Size( void ) const
{
	FUNCTION_START;
	FUNCTION_FINISH;

	return _u32Size;
}

/**
 ****************************************************************************************************
	\fn			const UINT8 Count( void ) const
	\brief		Get total element in memory pool
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
const UINT32 Utilities::MemoryPool::Count( void ) const
{
	FUNCTION_START;
	FUNCTION_FINISH;

	return _u32NumElements;
}