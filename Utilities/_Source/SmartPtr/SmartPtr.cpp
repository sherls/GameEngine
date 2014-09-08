/**
 ****************************************************************************************************
 * \file		SmartPtr.cpp
 * \brief		Implementation of non-inline functions of SmartPtr class
 ****************************************************************************************************
*/

#include "SmartPtr.h"
#include "../UtilitiesDefault.h"

Utilities::MemoryPool * Utilities::Pointer::_smartPtrPool = 0;

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize smart pointer
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool Utilities::Pointer::Initialize( void )
{
	FUNCTION_START;

	if( _smartPtrPool )
		return SUCCESS;
	//assert( _smartPtrPool == NULL );

	_smartPtrPool = MemoryPool::Create( sizeof(UINT32), DEFAULT_MEMORY_POOL_SIZE );

	assert( _smartPtrPool );

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shutdown collision
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::Pointer::ShutDown( void )
{
	FUNCTION_START;

	if( _smartPtrPool )
	{
		delete _smartPtrPool;
		_smartPtrPool = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void AllocateCount( void )
	\brief		Allocate count for Smart Pointer
	\param		NONE
	\return		The pointer to allocated count
 ****************************************************************************************************
*/
void *Utilities::Pointer::AllocateCount( void )
{
	FUNCTION_START;

	assert( _smartPtrPool );
	
	FUNCTION_FINISH;
	return _smartPtrPool->Allocate( sizeof(UINT32) );
}

/**
 ****************************************************************************************************
	\fn			void FreeCount( void *i_count )
	\brief		Free count for Smart Pointer
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::Pointer::FreeCount( void *i_count )
{
	FUNCTION_START;

	assert( _smartPtrPool );

	_smartPtrPool->Deallocate( i_count );

	FUNCTION_FINISH;
}
