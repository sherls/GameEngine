/**
 ****************************************************************************************************
 * \file		Vector3.h
 * \brief		The header of Vector3 class
 ****************************************************************************************************
*/

#ifndef _VECTOR3_H_
#define _VECTOR3_H_

// Utilities header
#include <MemoryPool/MemoryPool.h>

#include "../../Utilities/GameEngineTypes.h"

namespace GameEngine
{
	class MemoryPool;

	namespace Math
	{
		class Matrix;

		class Vector3
		{
			float _x, _y, _z;
			static Utilities::MemoryPool *_vector3Pool;

		public:
			static const Vector3 Zero;
			static const Vector3 Up;
			static const Vector3 Down;
			static const Vector3 Left;
			static const Vector3 Right;
			static const Vector3 Forward;
			static const Vector3 Backward;

			// Default constructor
			Vector3( void ){ };

			// Standard constructor
			inline Vector3( const float i_x, const float i_y, const float i_z );
			inline Vector3( const Vector3 &i_other );
			inline Vector3( const D3DXVECTOR3 &i_other );

			// Destructor

			// Set individual element
			inline void X( const float &i_x );
			inline void Y( const float &i_y );
			inline void Z( const float &i_z );

			// Element accesss
			inline const float X( void ) const;
			inline const float Y( void ) const;
			inline const float Z( void ) const;

			// Operators
			inline void operator=( const Vector3 &i_rhs );
			inline void operator=( const D3DXVECTOR3 &i_rhs );
			inline void operator+=( const Vector3 &i_rhs );
			inline void operator-=( const Vector3 &i_rhs );
			inline void operator*=( const float &i_rhs );
			void *operator new( UINT32 i_u32Size );
			void *operator new[]( UINT32 i_u32Size );
			void operator delete( void *i_ptr );
			void operator delete[]( void *i_ptr );

			// Other operation
			const void Negate( void );
			const Vector3 Negated( void ) const;
			const void Normalize( void );
			const Vector3 Normalized( void ) const;
			const float Length( void ) const;
			const float SquaredLength( void ) const;
			const float DotProduct( const Vector3 &i_other ) const;
			Vector3 Reflect( const Vector3 &i_vector );

			// Static methods
			static float DotProduct( const Vector3 &i_lhs, const Vector3 &i_rhs );
			static void Vector3TransformNormal( Vector3 &o_vector, const Vector3 &i_vector, Matrix &i_matrix );
			static void Vector3TransformCoord( Vector3 &o_vector, const Vector3 &i_vector, Matrix &i_matrix );
		};

		inline Vector3 operator+( const Vector3 &i_lhs, const Vector3 &i_rhs );
		inline Vector3 operator-( const Vector3 &i_lhs, const Vector3 &i_rhs );
		inline bool operator>( const Vector3 &i_lhs, const Vector3 &i_rhs );
		inline bool operator==( const Vector3 &i_lhs, const Vector3 &i_rhs );
		inline bool operator!=( const Vector3 &i_lhs, const Vector3 &i_rhs );
		inline const Vector3 operator*( float i_lhs, const Vector3 &i_rhs );
		inline const Vector3 operator*( const Vector3 &i_lhs, float i_rhs );
	}	// namespace Math
}	// namespace GameEngine

#include "Vector3.inl"

#endif	// #ifndef _VECTOR3_H_