/**
 ****************************************************************************************************
 * \file		SmartPtr.h
 * \brief		The header of Smart Pointer class
 ****************************************************************************************************
*/

#ifndef _SHARED_PTR_H_
#define _SHARED_PTR_H_

#include <assert.h>
#include <new>

#include "../UtilitiesTypes.h"
#include "../MemoryPool/MemoryPool.h"

namespace Utilities
{
	class MemoryPool;

	namespace Pointer
	{
		extern MemoryPool *_smartPtrPool;

		bool Initialize( void );
		void ShutDown( void );
		void *AllocateCount( void );
		void FreeCount( void *i_count );

		template< class T >
		class SmartPtr
		{
			T *_ptr;
			UINT32 *_count;

			void Release( void );

		public:
			SmartPtr( void );
			SmartPtr( T *i_ptr );
			SmartPtr( const SmartPtr &i_other );

			~SmartPtr( void );

			bool operator==( const T *i_other ) const;
			bool operator==( const SmartPtr &i_other ) const;
// 			bool operator!=( const T *i_other ) const;
// 			bool operator!=( const SmartPtr &i_other ) const;
			SmartPtr &operator=( T *i_other );
			SmartPtr &operator=( const SmartPtr &i_other );
			T *operator->( void );
			T &operator*( void );
			const T *operator->( void ) const;
			const T &operator*( void ) const;
			operator bool() const;
		};
	}	// namespace Pointer
}	// namespace GameEngine

#include "SmartPtr.inl"

#endif	// #ifndef _SHARED_PTR_H_