/**
 ****************************************************************************************************
 * \file		MemoryPool.cpp
 * \brief		The implementation of MemoryPool class
 ****************************************************************************************************
*/

#include "MemoryPool.h"
#include "../BitWise/BitWise.h"
#include "../UtilitiesDefault.h"

/****************************************************************************************************
			PUBLIC FUNCTIONS IMPLEMENTATION
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			MemoryPool *Create( const UINT32 &i_u32Size, const UINT32 &i_u32NumElements )
	\brief		Create MemoryPool
	\param		i_u32Size size of each element in Memory Pool
	\param		i_u3NumElements total number of elements to be created
	\return		Pointer to the created memory pool
 ****************************************************************************************************
*/
Utilities::MemoryPool *Utilities::MemoryPool::Create( const UINT32 &i_u32Size, const UINT32 &i_u32NumElements )
{
	assert( i_u32Size );
	assert( i_u32NumElements );

	FUNCTION_START;

	UINT8 *memory = new UINT8[i_u32NumElements*i_u32Size];

	FUNCTION_FINISH;

	if( memory )
		return new MemoryPool( memory, i_u32Size, i_u32NumElements );
	else
		return 0;
}

/**
 ****************************************************************************************************
	\fn			~MemoryPool( void )
	\brief		Destroy MemoryPool class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
Utilities::MemoryPool::~MemoryPool( void )
{
	FUNCTION_START;

	_availibility->Destroy();
	delete [] _memory;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void *Allocate( const UINT32 &i_u32Size )
	\brief		Create MemoryPool
	\param		i_u8Size size of each element in Memory Pool
	\param		i_u3Elements total number of elements to be created
	\return		The address of the available memory pool
 ****************************************************************************************************
*/
void* Utilities::MemoryPool::Allocate( const UINT32 &i_u32Size )
{
	assert( _u32Size == i_u32Size );

	if( IsFull() )
		return 0;

	UINT32 u32RetIndex = _availibility->GetFirstClearBit();
	_availibility->SetBit( u32RetIndex );

	return &_memory[u32RetIndex * _u32Size];
}

/**
 ****************************************************************************************************
	\fn			void Deallocate( void *i_ptr )
	\brief		Deallocate given memory pool
	\param		i_ptr pointer to the memory pool
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::MemoryPool::Deallocate( void *i_ptr )
{
	// Make sure address is greater than or equal to lower bounds 
	assert( reinterpret_cast<unsigned int>( i_ptr ) >= reinterpret_cast<unsigned int>( &_memory[0] ) );

	// Make sure address is less than or equal to upper bounds
	assert( reinterpret_cast<unsigned int>( i_ptr ) <= reinterpret_cast<unsigned int>( &_memory[_u32NumElements * _u32Size] ) );
	
	// Make sure address offset is a multiple of sizeof( T )
	assert( ( (reinterpret_cast<unsigned int>( i_ptr ) - reinterpret_cast<unsigned int>( &_memory[0] ) ) % _u32Size ) == 0 );
	
	UINT32 u8Index = (reinterpret_cast<unsigned int>( i_ptr ) - reinterpret_cast<unsigned int>( &_memory[0] ) ) / _u32Size;
	
	// Make sure it was actually allocated
	assert( _availibility->IsBitSet(u8Index) );
	
	_availibility->ClearBit( u8Index );
}

/**
 ****************************************************************************************************
	\fn			bool IsFull( void )
	\brief		Check whether memory pool is full
	\param		NONE
	\return		boolean
 ****************************************************************************************************
*/
bool Utilities::MemoryPool::IsFull( void ) const
{
	if( _availibility->GetFirstClearBit() > _u32NumElements )
		return true;
	else
		return false;
}

/**
 ****************************************************************************************************
	\fn			bool IsEmpty( void )
	\brief		Check whether memory pool is empty
	\param		NONE
	\return		boolean
 ****************************************************************************************************
*/
bool Utilities::MemoryPool::IsEmpty( void ) const
{
	if( _availibility->GetFirstSetBit() == 0xFF )
		return true;
	else
		return false;
}

#ifdef _DEBUG
/**
 ****************************************************************************************************
	\fn			void UnitTest( void )
	\brief		Unit test for MemoryPool class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::MemoryPool::UnitTest( void )
{
	MemoryPool *pMemoryPoolTest;

	FUNCTION_START;

	pMemoryPoolTest = MemoryPool::Create( sizeof(UINT32), 10 );

	assert( !pMemoryPoolTest->IsFull() );
	assert( pMemoryPoolTest->Size() == sizeof(UINT32) );
	assert( pMemoryPoolTest->Count() == 10 );

	UINT32 *u32A = reinterpret_cast<UINT32*>( pMemoryPoolTest->Allocate(sizeof(UINT32)) );

	delete pMemoryPoolTest;

	FUNCTION_FINISH;
}
#endif	// #ifdef _DEBUG

/****************************************************************************************************
			PRIVATE FUNCTIONS IMPLEMENTATION
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			MemoryPool( UINT8 *i_pMemory, const UINT32 &i_u32Size, const UINT32 &i_u32NumElements )
	\brief		MemoryPool constructor
	\param		i_pMemory pointer to the memory pool
	\param		i_u32Size size of each element in Memory Pool
	\param		i_u3NumElements total number of elements to be created
	\return		NONE
 ****************************************************************************************************
*/
Utilities::MemoryPool::MemoryPool( UINT8 *i_pMemory, const UINT32 &i_u32Size, const UINT32 &i_u32NumElements ) :
	_memory( i_pMemory ),
	_u32Size( i_u32Size ),
	_u32NumElements( i_u32NumElements )
{
	assert( _memory != 0 );
	assert( _u32Size );
	assert( _u32NumElements );

	_availibility = BitWise::Create( _u32NumElements );
}