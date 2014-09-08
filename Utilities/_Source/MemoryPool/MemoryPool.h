/**
 ****************************************************************************************************
 * \file		MemoryPool.h
 * \brief		The header of Memory Pool class
 ****************************************************************************************************
*/

#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include <assert.h>
//#include "../../Target/Target.h"
#include "../UtilitiesTypes.h"
#include "../Debug/Debug.h"

namespace Utilities
{
	class BitWise;

	class MemoryPool
	{
		UINT32 _u32NumElements;
		UINT32 _u32Size;
		BitWise *_availibility;
		UINT8 *_memory;

		MemoryPool( UINT8 *pMemory, const UINT32 &i_u32Size, const UINT32 &i_u32NumElements );
	
		// Prohibit duplication and assignment
		MemoryPool( const MemoryPool &i_other );
		const MemoryPool &operator=( const MemoryPool & i_other );

	public:
		// Constructor
		MemoryPool( void );
		MemoryPool( const UINT32 &i_u32Size );

		static MemoryPool *Create( const UINT32 &i_u32Size, const UINT32 &i_u32Elements );

		// Destructor
		~MemoryPool( void );

		// Operations
		void *Allocate( const UINT32 &i_u32Size );
		void Deallocate( void *i_ptr );
		inline const UINT32 Size( void ) const;
		inline const UINT32 Count( void ) const;
		bool IsEmpty( void ) const;
		bool IsFull( void ) const;

	#ifdef _DEBUG
		static void UnitTest( void );
	#endif	// #ifdef _DEBUG
	};
}	// namespace GameEngine

#include "MemoryPool.inl"

#endif	// #ifndef _MEMORY_POOL_H_