/**
 ****************************************************************************************************
 * \file		FastVector3.h
 * \brief		The header of FastVector3 class
 ****************************************************************************************************
*/

#ifndef _FAST_VECTOR3_H_
#define _FAST_VECTOR3_H_

#include <malloc.h>

// Utilities header
#include <Target/Target.h>

#include "Vector3.h"

namespace GameEngine
{
	namespace Math
	{
		CACHE_ALIGN class FastVector3: public Vector3
		{
			float _x, _y, _z;

		public:
			// Default constructor
			FastVector3( void ) {}

			// Standard constructor
			inline FastVector3( const float i_x, const float i_y, const float i_z );

			// Operators
			void *operator new( size_t i_size )
			{
				return _aligned_malloc( sizeof(FastVector3), CACHE_LINE );
			}

			void *operator new[]( size_t i_size )
			{
				return _aligned_malloc( sizeof(FastVector3), CACHE_LINE );
			}

			void operator delete( void *i_ptr )
			{
				if( i_ptr )
					_aligned_free( i_ptr );
			}

			void operator delete[]( void *i_ptr )
			{
				if( i_ptr )
					_aligned_free( i_ptr );
			}

			// Others
		#ifdef _DEBUG
			static void UnitTest( void );
		#endif	// #ifdef _DEBUG
		};
	}
}	// namespace GameEngine

#include "FastVector3.inl"

#endif	// #ifndef _FAST_VECTOR3_H_