/**
 ****************************************************************************************************
 * \file		Matrix.h
 * \brief		The header of Matrix class
 ****************************************************************************************************
*/

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>

#include "../../Utilities/GameEngineTypes.h"

namespace GameEngine
{
	//class MemoryPool;
	namespace Math
	{
		class Vector3;

		class Matrix
		{
			UINT32 _u32Row;
			UINT32 _u32Column;
			float **_value;

			void AllocateArrays( void );

			static float Matrix3Determinant( Matrix &i_matrix );
			static float Matrix4Determinant( Matrix &i_matrix );
			static void Matrix4SubMatrix( Matrix &i_matrix, Matrix &o_matrix, const UINT8 i_row, const UINT8 &i_column );

		public:
			// Constructor
			Matrix( void );
			Matrix( const UINT32 &i_u32Row, const UINT32 &i_u32Column );
			Matrix( const Matrix &i_other );

			// Destructor
			inline ~Matrix( void );

			// Operators
			Matrix &operator=( const Matrix &i_other );
			Matrix operator+( const Matrix &i_other );
			Matrix &operator+=( const Matrix &i_other );
			Matrix operator-( const Matrix &i_other );
			Matrix &operator-=( const Matrix &i_other );
			Matrix &operator*=( const Matrix &i_other );
			float &operator()( UINT32 i_u32Row, UINT32 i_u32Column );

			// Other implementation
			Matrix Identity( void );
			void Transpose( void );

			// Friends
			friend std::ostream &operator<<( std::ostream &o_out, const Matrix &i_this );
			friend std::istream &operator>>( std::istream &i_in, const Matrix &i_this );
			friend Matrix operator*( const Matrix &i_lhs, const Matrix &i_rhs );
			friend Matrix operator*( const float i_value, const Matrix &i_other );
			friend Matrix operator*( const Matrix &i_other, const float i_value );
			friend Matrix operator*( const Vector3 &i_vector, const Matrix &i_matrix );
			friend Matrix operator*( const Matrix &i_matrix, const Vector3 &i_vector );
			friend bool operator==( const Matrix &i_lhs, const Matrix &i_rhs );

			// Static
		#ifdef _DEBUG
			static void UnitTest( void );
		#endif	// #ifdef _DEBUG
			static void MatrixRotationX( Matrix &o_matrix, const float &i_rotation );
			static void MatrixRotationY( Matrix &o_matrix, const float &i_rotation );
			static void MatrixRotationZ( Matrix &o_matrix, const float &i_rotation );
			static void MatrixTranslation( Matrix &o_matrix, const Vector3 &i_vector );
			static void Matrix3Inverse( Matrix &i_matrix, Matrix &o_matrix );
			static void Matrix4Inverse( Matrix &i_matrix, Matrix &o_matrix );
		};
	}	// namespace Math
}	// namespace GameEngine

#include "Matrix.inl"
#endif	// #ifndef _MATRIX_H_