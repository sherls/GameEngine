/**
 ****************************************************************************************************
	\file		BitWise.cpp
	\brief		The implementation of BitWise class
 ****************************************************************************************************
*/

#include "BitWise.h"
#include <assert.h>
#include <intrin.h>

/**
 ****************************************************************************************************
	\fn			BitWise *Create( const UINT32 &i_u32NumBits )
	\brief		Create BitWise class
	\param		i_u32NumBits number of bits to be created
	\return		The pointer to the created BitWise
 ****************************************************************************************************
*/
Utilities::BitWise* Utilities::BitWise::Create( const UINT32 &i_u32NumBits )
{
	assert( i_u32NumBits );

	UINT32 u32NumElements = (i_u32NumBits + _u8BitsPerElement - 1 ) / _u8BitsPerElement;

	FUNCTION_START;

	UINT32 *pMemory = reinterpret_cast<UINT32 *>( _aligned_malloc(sizeof(UINT32) * u32NumElements, CACHE_LINE) );

	FUNCTION_FINISH;

	if( pMemory )
		return new BitWise( pMemory, i_u32NumBits );
	else
		return 0;
}

/**
 ****************************************************************************************************
	\fn			const UINT32 GetFirstClearBit( void ) const
	\brief		Get the first clear bit of the BitWise
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
const UINT32 Utilities::BitWise::GetFirstClearBit( void ) const
{
	static const UINT32 u32AllSet = 0xFFFFFFFF;
	
	UINT32 u32Element = 0;

	FUNCTION_START;

	while( (_u32pBits[u32Element] == u32AllSet) && (u32Element < _u32NumElements) )
		u32Element++;

	if( u32Element == _u32NumElements )
		return 0xFF;

	UINT8 u8Bit = 0;
	UINT32 u32BitMask = 1;
	while( _u32pBits[u32Element] & u32BitMask )
	{
		u32BitMask <<= 1;
		u8Bit++;
	}

	FUNCTION_FINISH;
	return (u32Element * _u8BitsPerElement) + u8Bit;
}

/**
 ****************************************************************************************************
	\fn			const UINT32 GetFirstSetBit( void ) const
	\brief		Get the first set bit in the BitWise
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
const UINT32 Utilities::BitWise::GetFirstSetBit( void ) const
{
	static const UINT32 u32AllClear = 0x0;
	
	UINT32 u32Element = 0;

	FUNCTION_START;

	while( (_u32pBits[u32Element] == u32AllClear) && (u32Element < _u32NumElements) )
		u32Element++;

	if( u32Element == _u32NumElements )
		return 0xFF;

	unsigned long u8Bit = 0;

	UINT8 u8BitFound = _BitScanForward( &u8Bit, _u32pBits[u32Element] );
	assert( u8BitFound != 0 );

	FUNCTION_FINISH;
	return (UINT8) ((u32Element * _u8BitsPerElement) + u8Bit);
}

/**
 ****************************************************************************************************
	\fn			void SetBit( const UINT32 &i_u32Index )
	\brief		Set the bit of the given index
	\param		i_u32Index index of the bit to be set
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::BitWise::SetBit( const UINT32 &i_u32Index )
{
	FUNCTION_START;

	assert( !IsBitSet(i_u32Index) );

	UINT32 u32Element = i_u32Index / _u8BitsPerElement;
	UINT8 u8Bit = i_u32Index % _u8BitsPerElement;

	assert( u32Element <= _u32NumElements );

	_u32pBits[u32Element] |= (1 << u8Bit);

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ClearBit( const UINT32 &i_u32Index )
	\brief		Clear the bit of the given index
	\param		i_u32Index index of the bit to be cleared
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::BitWise::ClearBit( const UINT32 &i_u32Index )
{
	FUNCTION_START;
	assert( IsBitSet(i_u32Index) );

	UINT32 u32Element = i_u32Index / _u8BitsPerElement;
	UINT8 u8Bit = i_u32Index % _u8BitsPerElement;

	assert( u32Element <= _u32NumElements );

	_u32pBits[u32Element] ^= (1 << u8Bit);

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool IsBitSet( const UINT32 &i_u32Index )
	\brief		Check whether the given index of BitWise is set
	\param		i_u32Index index of the required BitWise
	\return		NONE
 ****************************************************************************************************
*/
bool Utilities::BitWise::IsBitSet( const UINT32 &i_u32Index )
{
	UINT32 u32Element = i_u32Index / _u8BitsPerElement;
	UINT8 u8Bit = i_u32Index % _u8BitsPerElement;

	FUNCTION_START;

	assert( u32Element <= _u32NumElements );

	FUNCTION_FINISH;

	if( _u32pBits[u32Element] & (1 << u8Bit) )
		return true;
	else
		return false;
}

/**
 ****************************************************************************************************
	\fn			bool IsBitClear( const UINT32 &i_u32Index )
	\brief		Check whether the given index of BitWise is clear
	\param		i_u32Index index of the required BitWise
	\return		NONE
 ****************************************************************************************************
*/
bool Utilities::BitWise::IsBitClear( const UINT32 &i_u32Index )
{
	FUNCTION_START;
	FUNCTION_FINISH;

	return !IsBitSet( i_u32Index );
}

#ifdef _DEBUG
/**
 ****************************************************************************************************
	\fn			void UnitTest( void )
	\brief		Unit test for BitWise class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::BitWise::UnitTest( void )
{
	UINT8 i;
	UINT32 bitWiseSize = 37;
	BitWise *bitWiseTest;

	FUNCTION_START;

	bitWiseTest = BitWise::Create( bitWiseSize );

	assert( bitWiseTest->GetFirstClearBit() == 0 );
	assert( bitWiseTest->GetFirstSetBit() == 0xFF );

	for( i = 0; i < bitWiseSize; ++i )
		assert( !bitWiseTest->IsBitSet(i) );

	bitWiseTest->SetBit( 34 );
	assert( bitWiseTest->IsBitSet(34) );
	assert( bitWiseTest->GetFirstSetBit() == 34 );

	bitWiseTest->ClearBit( 34 );
	assert( bitWiseTest->IsBitClear(34) );

	bitWiseTest->Destroy();

	FUNCTION_FINISH;
}
#endif	// #ifdef _DEBUG

/****************************************************************************************************
			Private functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			BitWise( UINT32 *i_pBitMemory, const UINT32 &i_u32NumBits )
	\brief		BitWise class constructor
	\param		i_pBitMemory address of BitWise
	\param		i_u32NumBits number of bits created
	\return		NONE
 ****************************************************************************************************
*/
Utilities::BitWise::BitWise( UINT32 *i_pBitMemory, const UINT32 &i_u32NumBits ) :
	_u32pBits( i_pBitMemory ),
	_u32NumBits( i_u32NumBits )
{
	FUNCTION_START;

	assert( _u32pBits != 0 );
	assert( _u32NumBits );

	_u32NumElements = (i_u32NumBits + _u8BitsPerElement - 1 ) / _u8BitsPerElement;

	memset( _u32pBits, 0, sizeof(_u32pBits) * _u32NumElements );

	FUNCTION_FINISH;
}