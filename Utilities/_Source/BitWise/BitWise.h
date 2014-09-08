/**
 ****************************************************************************************************
 * \file		BitWise.h
 * \brief		The header of Bit Wise class
 ****************************************************************************************************
*/

#include <assert.h>
#include <malloc.h>

#include "../UtilitiesTypes.h"
#include "../Debug/Debug.h"
#include "../Target/Target.h"

namespace Utilities
{
class BitWise
{
	UINT32 _u32NumElements;
	UINT32 _u32NumBits;
	UINT32 *_u32pBits;
	static const UINT8 _u8BitsPerElement = sizeof( UINT32 ) * 8;

	BitWise( void );
	~BitWise( void ){ };
	BitWise( const UINT32 &i_u32NumBits );
	void operator=( const BitWise & i_other );

	BitWise( UINT32 *i_pBitMemory, const UINT32 &i_u32NumBits );

public:
	static BitWise* Create( const UINT32 &i_u32NumBits );

	inline void Destroy( void );

	const UINT32 GetFirstClearBit( void ) const;
	const UINT32 GetFirstSetBit( void ) const;

	void SetBit( const UINT32 &i_u32Index );
	void ClearBit( const UINT32 &i_u32Index );

	bool IsBitSet( const UINT32 &i_u32Index );
	bool IsBitClear( const UINT32 &i_u32Index );

#ifdef _DEBUG
	static void UnitTest( void );
#endif	// #ifdef _DEBUG
};
}

#include "BitWise.inl"