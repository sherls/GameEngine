/**
 ****************************************************************************************************
 * \file		Matrix.cpp
 * \brief		Implementation of non-inline functions of Matrix class
 ****************************************************************************************************
*/

#include <math.h>
#include <stdio.h>

// Utilities header
#include <Debug/Debug.h>

#include "Matrix.h"
#include "../Vector3/Vector3.h"

/****************************************************************************************************
			Static class implementation
****************************************************************************************************/
#ifdef _DEBUG
/**
 ****************************************************************************************************
	\fn			void UnitTest( void )
	\brief		The unit test of Matrix class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Matrix::UnitTest( void )
{
	FUNCTION_START;

	Matrix matrix2x2( 2, 2 );

	// Check creation
	for( UINT8 i = 0; i < 2; ++i )
	{
		for( UINT8 j = 0; j < 2; ++j )
			assert( matrix2x2(i, j) == 0 );
	}

	// Check matrix identity
	matrix2x2(0, 0) = 1;
	matrix2x2(0, 1) = 2;
	matrix2x2(1, 0) = 3;
	matrix2x2(1, 1) = 4;
	Matrix identityMatrix( 2, 2 );
	identityMatrix(0, 0) = 1;
	identityMatrix(1, 1) = 1;
	Matrix m( 2, 2 );
	m = matrix2x2.Identity();
	assert( matrix2x2.Identity() == identityMatrix );

	// Check matrix transpose
	Matrix matrix2x3( 2, 3 );
	matrix2x3( 0, 0 ) = 1;
	matrix2x3( 0, 1 ) = 2;
	matrix2x3( 0, 2 ) = 3;
	matrix2x3( 1, 0 ) = 4;
	matrix2x3( 1, 1 ) = 5;
	matrix2x3( 1, 2 ) = 6;
	matrix2x3.Transpose();
	Matrix matrix3x2( 3, 2 );
	matrix3x2( 0, 0 ) = 1;
	matrix3x2( 0, 1 ) = 4;
	matrix3x2( 1, 0 ) = 2;
	matrix3x2( 1, 1 ) = 5;
	matrix3x2( 2, 0 ) = 3;
	matrix3x2( 2, 1 ) = 6;
	assert( matrix2x3 == matrix3x2 );

	// Check matrix 3x3 inverse
	Matrix matrix3x3( 3, 3 );
	matrix3x3( 0, 0 ) = 2;
	matrix3x3( 0, 1 ) = -1;
	matrix3x3( 0, 2 ) = 3;
	matrix3x3( 1, 0 ) = 1;
	matrix3x3( 1, 1 ) = 6;
	matrix3x3( 1, 2 ) = -4;
	matrix3x3( 2, 0 ) = 5;
	matrix3x3( 2, 1 ) = 0;
	matrix3x3( 2, 2 ) = 8;
	Matrix matrix3x3Inverse( 3, 3 );
	Matrix3Inverse( matrix3x3, matrix3x3Inverse );
	assert( (matrix3x3 * matrix3x3Inverse) == matrix3x3.Identity() );

	// Check matrix 4x4 inverse
	Matrix matrix4x4( 4, 4 );
	matrix4x4( 0, 0 ) = 2;
	matrix4x4( 0, 1 ) = 3;
	matrix4x4( 0, 2 ) = 4;
	matrix4x4( 0, 3 ) = 5;
	matrix4x4( 1, 0 ) = 5;
	matrix4x4( 1, 1 ) = 7;
	matrix4x4( 1, 2 ) = 9;
	matrix4x4( 1, 3 ) = 9;
	matrix4x4( 2, 0 ) = 5;
	matrix4x4( 2, 1 ) = 8;
	matrix4x4( 2, 2 ) = 7;
	matrix4x4( 2, 3 ) = 4;
	matrix4x4( 3, 0 ) = 4;
	matrix4x4( 3, 1 ) = 3;
	matrix4x4( 3, 2 ) = 3;
	matrix4x4( 3, 3 ) = 2;
	Matrix matrix4x4Inverse( 4, 4 );
	Matrix4Inverse( matrix4x4, matrix4x4Inverse );
	Matrix matrix4x4Result( 4, 4 );
	matrix4x4Result = matrix4x4 * matrix4x4Inverse;
	assert( (matrix4x4 * matrix4x4Inverse) == matrix4x4.Identity() );

	FUNCTION_FINISH;
}
#endif	// #ifdef _DEBUG

/**
 ****************************************************************************************************
	\fn			void MatrixRotationX( Matrix &o_matrix, const float &i_rotation )
	\brief		Matrix rotation around X axis creator
	\param		o_matrix output matrix
	\param		i_rotation rotation angle
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Matrix::MatrixRotationX( Matrix &o_matrix, const float &i_rotation )
{
	FUNCTION_START;

	assert( o_matrix._u32Column == 4 );
	assert( o_matrix._u32Row == 4 );

	o_matrix = o_matrix.Identity();
	o_matrix( 1, 1 ) = cos( i_rotation );
	o_matrix( 1, 2 ) = -sin( i_rotation );
	o_matrix( 1, 2 ) = sin( i_rotation );
	o_matrix( 2, 2 ) = cos( i_rotation );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void MatrixRotationY( Matrix &o_matrix, const float &i_rotation )
	\brief		Matrix rotation around Y axis creator
	\param		o_matrix output matrix
	\param		i_rotation rotation angle
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Matrix::MatrixRotationY( Matrix &o_matrix, const float &i_rotation )
{
	FUNCTION_START;

	assert( o_matrix._u32Column == 4 );
	assert( o_matrix._u32Row == 4 );

	o_matrix = o_matrix.Identity();
	o_matrix( 0, 0 ) = cos( i_rotation );
	o_matrix( 0, 2 ) = sin( i_rotation );
	o_matrix( 2, 0 ) = -sin( i_rotation );
	o_matrix( 2, 2 ) = cos( i_rotation );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void MatrixRotationZ( Matrix &o_matrix, const float &i_rotation )
	\brief		Matrix rotation around Z axis creator
	\param		o_matrix output matrix
	\param		i_rotation rotation angle
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Matrix::MatrixRotationZ( Matrix &o_matrix, const float &i_rotation )
{
	FUNCTION_START;

	assert( o_matrix._u32Column == 4 );
	assert( o_matrix._u32Row == 4 );

	o_matrix = o_matrix.Identity();
	o_matrix( 0, 0 ) = cos( i_rotation );
	o_matrix( 0, 1 ) = -sin( i_rotation );
	o_matrix( 1, 0 ) = sin( i_rotation );
	o_matrix( 1, 1 ) = cos( i_rotation );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void MatrixRotationZ( Matrix &o_matrix, const Vector3 &i_position )
	\brief		Matrix rotation around Z axis creator
	\param		o_matrix output matrix
	\param		i_rotation rotation angle
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Matrix::MatrixTranslation( Matrix &o_matrix, const Vector3 &i_position )
{
	FUNCTION_START;

	assert( o_matrix._u32Column == 4 );
	assert( o_matrix._u32Row == 4 );

	o_matrix = o_matrix.Identity();
	o_matrix( 3, 0 ) = i_position.X();
	o_matrix( 3, 1 ) = i_position.Y();
	o_matrix( 3, 2 ) = i_position.Z();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			float Matrix3Determinant( Matrix &i_matrix )
	\brief		Calculate the determinant of matrix3x3 i_matrix
	\param		i_matrix input matrix
	\return		float
	\retval		The determinant of input matrix
 ****************************************************************************************************
*/
float GameEngine::Math::Matrix::Matrix3Determinant( Matrix &i_matrix )
{
	float retVal = 0.0f;

	FUNCTION_START;
	
	assert( i_matrix._u32Column == 3 );
	assert( i_matrix._u32Row == 3 );

	retVal = i_matrix(0, 0) * (i_matrix(1, 1) * i_matrix(2, 2) - i_matrix(2, 1) * i_matrix(1, 2))
		- i_matrix(0, 1) * (i_matrix(1, 0) * i_matrix(2, 2) - i_matrix(2, 0) * i_matrix(1, 2))
		+ i_matrix(0, 2) * (i_matrix(1, 0) * i_matrix(2, 1) - i_matrix(2, 0) * i_matrix(1, 1));

	FUNCTION_FINISH;

	if( Utilities::Math::AreRelativelyEqual(retVal, 0.0f) )
		retVal = 0.0f;

	return retVal;
}

/**
 ****************************************************************************************************
	\fn			float Matrix4Determinant( Matrix &i_matrix )
	\brief		Calculate the determinant of matrix4x4 i_matrix
	\param		i_matrix input matrix
	\return		float
	\retval		The determinant of input matrix
 ****************************************************************************************************
*/
float GameEngine::Math::Matrix::Matrix4Determinant( Matrix &i_matrix )
{
	float retVal = 0.0f;
	float determinant = 0.0f;
	INT8 sign = 1;
	UINT8 n = 0;
	Matrix subMatrix( 3, 3 );

	FUNCTION_START;
	
	assert( i_matrix._u32Column == 4 );
	assert( i_matrix._u32Row == 4 );

	for( n = 0; n < 4; n++, sign *= -1 )
	{
		Matrix4SubMatrix( i_matrix, subMatrix, 0, n );
		determinant = Matrix3Determinant( subMatrix );
		retVal += (i_matrix(0, n) * determinant * sign);
	}

	FUNCTION_FINISH;

	return retVal;
}

/**
 ****************************************************************************************************
	\fn			void Matrix4SubMatrix( Matrix &i_matrix, Matrix &o_matrix, const UINT8 i_row, const UINT8 &i_column )
	\brief		Get the sub matrix of matrix4x4( i_row, i_column )
	\param		i_matrix input matrix
	\param		o_matrix output matrix
	\param		i_row row index of input matrix
	\param		i_column column index of input matrix
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Matrix::Matrix4SubMatrix( Matrix &i_matrix, Matrix &o_matrix, const UINT8 i_row, const UINT8 &i_column )
{
	UINT8 si, sj;

	FUNCTION_START;

	assert( i_matrix._u32Column == 4 );
	assert( i_matrix._u32Row == 4 );
	assert( o_matrix._u32Column == 3 );
	assert( o_matrix._u32Row == 3 );

	// Loop through 3x3 matrix
	for( UINT8 di = 0; di < 3; ++di )
	{
		for( UINT8 dj = 0; dj < 3; ++dj )
		{
			// Map 3x3 element (output) to 4x4 element (source)
			si = di + ( (di >= i_row)? 1: 0 );
			sj = dj + ( (dj >= i_column)? 1 : 0 );

			// Copy element
			o_matrix( di, dj ) = i_matrix( si, sj );
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Matrix3Inverse( Matrix &i_matrix, Matrix &o_matrix )
	\brief		Calculate the inverse of matrix3x3 i_matrix
	\param		i_matrix input matrix
	\param		o_matrix output matrix
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Matrix::Matrix3Inverse( Matrix &i_matrix, Matrix &o_matrix )
{
	float determinant = 0.0f;

	FUNCTION_START;

	assert( i_matrix._u32Column == 3 );
	assert( i_matrix._u32Row == 3 );
	assert( o_matrix._u32Column == 3 );
	assert( o_matrix._u32Row == 3 );

	determinant = Matrix3Determinant( i_matrix );

	if( Utilities::Math::AreRelativelyEqual(determinant, 0.0f) )
	{
		o_matrix = i_matrix.Identity();

		FUNCTION_FINISH;
		return;
	}

	o_matrix(0, 0) = (i_matrix(1, 1) * i_matrix(2, 2) - i_matrix(1, 2) * i_matrix(2, 1)) / determinant;
	o_matrix(0, 1) = -(i_matrix(0, 1) * i_matrix(2, 2) - i_matrix(2, 1) * i_matrix(0, 2)) / determinant;
	o_matrix(0, 2) = (i_matrix(0, 1) * i_matrix(1, 2) - i_matrix(1, 1) * i_matrix(0, 2)) / determinant;
	o_matrix(1, 0) = -(i_matrix(1, 0) * i_matrix(2, 2) - i_matrix(1, 2) * i_matrix(2, 0)) / determinant;
	o_matrix(1, 1) = (i_matrix(0, 0) * i_matrix(2, 2) - i_matrix(2, 0) * i_matrix(0, 2)) / determinant;
	o_matrix(1, 2) = -(i_matrix(0, 0) * i_matrix(1, 2) - i_matrix(1, 0) * i_matrix(0, 2)) / determinant;
	o_matrix(2, 0) = (i_matrix(1, 0) * i_matrix(2, 1) - i_matrix(2, 0) * i_matrix(1, 1)) / determinant;
	o_matrix(2, 1) = -(i_matrix(0, 0) * i_matrix(2, 1) - i_matrix(2, 0) * i_matrix(0, 1)) / determinant;
	o_matrix(2, 2) = (i_matrix(0, 0) * i_matrix(1, 1) - i_matrix(0, 1) * i_matrix(1, 0)) / determinant;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Matrix4Inverse( Matrix &i_matrix, Matrix &o_matrix )
	\brief		Calculate the inverse of matrix4x4 i_matrix
	\param		i_matrix input matrix
	\param		o_matrix output matrix
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Matrix::Matrix4Inverse( Matrix &i_matrix, Matrix &o_matrix )
{
	float determinant = 0.0f;
	Matrix subMatrix( 3, 3 );
	INT8 sign = 0;

	FUNCTION_START;

	assert( i_matrix._u32Column == 4 );
	assert( i_matrix._u32Row == 4 );
	assert( o_matrix._u32Column == 4 );
	assert( o_matrix._u32Row == 4 );
	
	determinant = Matrix4Determinant( i_matrix );

	if( Utilities::Math::AreRelativelyEqual(determinant, 0.0f, 20) )
	{
		o_matrix = i_matrix.Identity();

		FUNCTION_FINISH;
		return;
	}

	for( UINT8 i = 0; i < 4; ++i )
	{
		for( UINT8 j = 0; j < 4; ++j )
		{
			sign = 1 - ( (i + j) % 2 ) * 2;
			Matrix4SubMatrix( i_matrix, subMatrix, i, j );
			o_matrix( j, i ) = ( Matrix3Determinant(subMatrix) * sign ) / determinant;
		}
	}

	FUNCTION_FINISH;
	return;
}

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			Matrix( void )
	\brief		Default constructor of Matrix class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Math::Matrix::Matrix( void ) :
	_u32Row( 1 ),
	_u32Column( 1 )
{
	FUNCTION_START;

	assert( _u32Row != 0 );
	assert( _u32Column != 0 );

	AllocateArrays();

	for( UINT32 i = 0; i < _u32Row; ++i )
	{
		for( UINT32 j = 0; j < _u32Column; ++j )
			_value[i][j] = 0;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			Matrix( const UINT32 &i_u32Row, const UINT32 &i_u32Column )
	\brief		Matrix class constructor
	\param		i_u32Row total row of matrix class
	\param		i_u32Column total column of matrix class
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Math::Matrix::Matrix( const UINT32 &i_u32Row, const UINT32 &i_u32Column ) :
	_u32Row( i_u32Row ),
	_u32Column( i_u32Column )
{
	FUNCTION_START;

	assert( _u32Row != 0 );
	assert( _u32Column != 0 );

	AllocateArrays();

	for( UINT32 i = 0; i < _u32Row; ++i )
	{
		for( UINT32 j = 0; j < _u32Column; ++j )
			_value[i][j] = 0;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			Matrix( const Matrix &i_other )
	\brief		Copy constructor of Matrix class
	\param		i_other other Matrix
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Math::Matrix::Matrix( const Matrix &i_other ) :
	_u32Row( i_other._u32Row ),
	_u32Column( i_other._u32Column )
{
	FUNCTION_START;

	AllocateArrays();

	for( UINT32 i = 0; i < _u32Row; ++i )
	{
		for( UINT32 j = 0; j < _u32Column; ++j )
			_value[i][j] = i_other._value[i][j];
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			Matrix operator=( const Matrix &i_other )
	\brief		operator = of Matrix class
	\param		i_other other Matrix
	\return		Matrix
	\retval		new assigned Matrix
 ****************************************************************************************************
*/
GameEngine::Math::Matrix &GameEngine::Math::Matrix::operator=( const Matrix &i_other )
{
	FUNCTION_START;

	if( this == &i_other )
	{
		FUNCTION_FINISH;
		return *this;
	}
	else
	{
		if( (_u32Row != i_other._u32Row) || (_u32Column != i_other._u32Column) )
		{
			this->~Matrix();
			_u32Row = i_other._u32Row;
			_u32Column = i_other._u32Column;
			AllocateArrays();
		}

		for( UINT32 i = 0; i < _u32Row; ++i )
		{
			for( UINT32 j = 0; j < _u32Column; ++j )
				_value[i][j] = i_other._value[i][j];
		}
	}

	FUNCTION_FINISH;
	return *this;
}

/**
 ****************************************************************************************************
	\fn			Matrix operator+( const Matrix &i_other )
	\brief		operator + of Matrix class
	\param		i_other other Matrix class
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Math::Matrix GameEngine::Math::Matrix::operator+( const Matrix& i_other )
{
	FUNCTION_START;

	assert( _u32Row == i_other._u32Row );
	assert( _u32Column == i_other._u32Column );

	Matrix temp( *this );

	FUNCTION_FINISH;
	return( temp += i_other );
}

/**
 ****************************************************************************************************
	\fn			Matrix operator+=( const Matrix &i_other )
	\brief		operator += of Matrix class
	\param		i_other other Matrix class
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Math::Matrix &GameEngine::Math::Matrix::operator+=( const Matrix &i_other )
{
	FUNCTION_START;

	assert( _u32Row == i_other._u32Row );
	assert( _u32Column == i_other._u32Column );

	for( UINT32 i = 0; i < _u32Row; ++i )
	{
		for( UINT32 j = 0; j < _u32Column; ++j )
			_value[i][j] += i_other._value[i][j];
	}

	FUNCTION_FINISH;
	return *this;
}

/**
 ****************************************************************************************************
	\fn			Matrix operator-( const Matrix &i_other )
	\brief		operator - of Matrix class
	\param		i_other other Matrix class
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Math::Matrix GameEngine::Math::Matrix::operator-( const Matrix& i_other )
{
	FUNCTION_START;

	assert( _u32Row == i_other._u32Row );
	assert( _u32Column == i_other._u32Column );

	Matrix temp( *this );

	FUNCTION_FINISH;
	return( temp -= i_other );
}

/**
 ****************************************************************************************************
	\fn			Matrix operator -=( const Matrix &i_other )
	\brief		operator -= of Matrix class
	\param		i_other other Matrix class
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Math::Matrix &GameEngine::Math::Matrix::operator-=( const Matrix &i_other )
{
	FUNCTION_START;

	assert( _u32Row == i_other._u32Row );
	assert( _u32Column == i_other._u32Column );

	for( UINT32 i = 0; i < _u32Row; ++i )
	{
		for( UINT32 j = 0; j < _u32Column; ++j )
			_value[i][j] -= i_other._value[i][j];
	}

	FUNCTION_FINISH;
	return *this;
}

/**
 ****************************************************************************************************
	\fn			Matrix operator *=( const Matrix &i_other )
	\brief		operator *= of Matrix class
	\param		i_other other Matrix class
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Math::Matrix &GameEngine::Math::Matrix::operator*=( const Matrix &i_other )
{
	FUNCTION_START;

	assert( _u32Column == i_other._u32Row );
	
	Matrix result( _u32Row, i_other._u32Column );

	result = *this * i_other;
	*this = result;

	FUNCTION_FINISH;
	return *this;
}

/**
 ****************************************************************************************************
	\fn			float operator()( UINT32 i_u32Row, UINT32 i_u32Column )
	\brief		operator() of Matrix class
	\param		i_u32Row #row
	\param		i_u32Column #column
	\return		NONE
 ****************************************************************************************************
*/
float &GameEngine::Math::Matrix::operator()( UINT32 i_u32Row, UINT32 i_u32Column )
{
	FUNCTION_START;

	assert( i_u32Row < _u32Row );
	assert( i_u32Column < _u32Column );

	FUNCTION_FINISH;
	return _value[i_u32Row][i_u32Column];
}

/**
 ****************************************************************************************************
	\fn			Matrix Identity( void )
	\brief		Matrix identity
	\param		NONE
	\return		Matrix
	\retval		The identity of this Matrix class
 ****************************************************************************************************
*/
GameEngine::Math::Matrix GameEngine::Math::Matrix::Identity( void )
{
	FUNCTION_START;

	Matrix identityMatrix( _u32Row, _u32Column);

	assert( _u32Row == _u32Column );

	for( UINT32 i = 0; i < _u32Row; ++i )
		identityMatrix( i, i ) = 1;
	
	FUNCTION_FINISH;
	return identityMatrix;
}

/**
 ****************************************************************************************************
	\fn			Transpose( void )
	\brief		Transpose this matrix class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Matrix::Transpose( void )
{
	Matrix temp( _u32Column, _u32Row );

	FUNCTION_START;

	for( UINT32 i = 0; i < _u32Column; ++i )
	{
		for( UINT32 j = 0; j < _u32Row; ++j )
		{
			temp._value[i][j] = _value[j][i];
		}
	}

	*this = temp;

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			AllocateArrays( void )
	\brief		Allocate array for Matrix class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Matrix::AllocateArrays( void )
{
	FUNCTION_START;

	_value = new float * [_u32Row];
	for( UINT32 i = 0; i < _u32Row; ++i )
		_value[i] = new float[_u32Column];

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Friend function implementation
****************************************************************************************************/
namespace GameEngine
{
	namespace Math
	{
/**
 ****************************************************************************************************
	\fn			std::ostream &operator<<( std::ostream &o_out, const GameEngine::Math::Matrix &i_this )
	\brief		operator << of Matrix class
	\param		o_out out stream
	\param		i_this this Matrix
	\return		NONE
 ****************************************************************************************************
*/
std::ostream &operator<<( std::ostream &o_out, const GameEngine::Math::Matrix &i_this )
{
	FUNCTION_START;

	for( UINT32 i = 0; i < i_this._u32Row; ++i )
	{
		for( UINT32 j = 0; j < i_this._u32Column; ++j )
			o_out << i_this._value[i][j] << " ";
		o_out << std::endl;
	}

	FUNCTION_FINISH;
	return o_out;
}

/**
 ****************************************************************************************************
	\fn			Matrix operator*( const Matrix &i_lhs, const Matrix &i_rhs )
	\brief		operator * of Matrix class
	\param		i_lhs left hand side Matrix
	\param		i_rhs right hand side Matrix
	\return		Matrix
	\retval		Result matrix
 ****************************************************************************************************
*/
Matrix operator*( const Matrix &i_lhs, const Matrix &i_rhs )
{
	FUNCTION_START;

	assert( i_lhs._u32Column == i_rhs._u32Row );

	Matrix result( i_lhs._u32Row, i_rhs._u32Column );

	for( UINT32 i = 0; i < result._u32Row; ++i )
	{
		for( UINT32 j = 0; j < result._u32Column; ++j )
		{
			for( UINT32 k = 0; k < i_lhs._u32Column; ++k )
				result._value[i][j] += i_lhs._value[i][k] * i_rhs._value[k][j];
		}
	}

	FUNCTION_FINISH;
	return result;
}

/**
 ****************************************************************************************************
	\fn			Matrix operator*( const float i_value, const Matrix &i_matrix )
	\brief		operator * of Matrix class
	\param		i_value float number multiplier
	\param		i_matrix the Matrix to be multiplied
	\return		Matrix
	\retval		Result matrix
 ****************************************************************************************************
*/
Matrix operator*( const float i_value, const Matrix &i_matrix )
{
	Matrix result( i_matrix );

	FUNCTION_START;

	for( UINT32 i = 0; i < result._u32Row; ++i )
	{
		for( UINT32 j = 0; j < result._u32Column; ++j )
			result._value[i][j] = i_value * i_matrix._value[i][j];
	}

	FUNCTION_FINISH;
	return result;
}

/**
 ****************************************************************************************************
	\fn			Matrix operator*( const Matrix &i_matrix, const float i_value )
	\brief		operator * of Matrix class
	\param		i_matrix the Matrix to be multiplied
	\param		i_value float number multiplier
	\return		Matrix
	\retval		Result matrix
 ****************************************************************************************************
*/
Matrix operator*( const Matrix &i_matrix, const float i_value )
{
	FUNCTION_START;
	FUNCTION_FINISH;
	return i_value * i_matrix;
}

/**
 ****************************************************************************************************
	\fn			Matrix operator*( const Vector3 &i_vector, const Matrix &i_matrix )
	\brief		operator * of Matrix class
	\param		i_vector Vector3 multiplied
	\param		i_matrix the Matrix to be multiplied
	\return		Matrix
	\retval		Result matrix
 ****************************************************************************************************
*/
Matrix operator*( const Vector3 &i_vector, const Matrix &i_matrix )
{
	FUNCTION_START;

	assert( (i_matrix._u32Row == 1) || (i_matrix._u32Row == 3) );
	assert( (i_matrix._u32Column == 1) || (i_matrix._u32Column == 3) );

	if( i_matrix._u32Row == 1 )
	{
		Matrix vectorMatrix( 3, 1 );
		vectorMatrix(0, 0) = i_vector.X();
		vectorMatrix(1, 0) = i_vector.Y();
		vectorMatrix(2, 0) = i_vector.Z();

		FUNCTION_FINISH;
		return vectorMatrix * i_matrix;
	}
	else
	{
		Matrix vectorMatrix( 1, 3 );
		vectorMatrix( 0, 0 ) = i_vector.X();
		vectorMatrix( 0, 1 ) = i_vector.Y();
		vectorMatrix( 0, 2 ) = i_vector.Z();

		FUNCTION_FINISH;
		return vectorMatrix * i_matrix;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			Matrix operator*( const Matrix &i_matrix, const Vector &i_vector )
	\brief		operator * of Matrix class
	\param		i_matrix the Matrix to be multiplied
	\param		i_vector Vector3 multiplied
	\return		Matrix
	\retval		Result matrix
 ****************************************************************************************************
*/
Matrix operator*( const Matrix &i_matrix, const Vector3 &i_vector )
{
	FUNCTION_START;

	assert( (i_matrix._u32Row == 1) || (i_matrix._u32Row == 3) );
	assert( (i_matrix._u32Column == 1) || (i_matrix._u32Column == 3) );

	if( i_matrix._u32Row == 1 )
	{
		Matrix vectorMatrix( 3, 1 );
		vectorMatrix(0, 0) = i_vector.X();
		vectorMatrix(1, 0) = i_vector.Y();
		vectorMatrix(2, 0) = i_vector.Z();

		FUNCTION_FINISH;
		return i_matrix * vectorMatrix;
	}
	else
	{
		Matrix vectorMatrix( 1, 3 );
		vectorMatrix( 0, 0 ) = i_vector.X();
		vectorMatrix( 0, 1 ) = i_vector.Y();
		vectorMatrix( 0, 2 ) = i_vector.Z();

		FUNCTION_FINISH;
		return i_matrix * vectorMatrix;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			Matrix operator==( const Matrix &i_lhs, const Matrix &i_rhs )
	\brief		operator == of Matrix class
	\param		i_lhs left hand side Matrix
	\param		i_rhs right hand side Matrix
	\return		bool
	\retval		TRUE equal
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool operator==( const Matrix &i_lhs, const Matrix &i_rhs )
{
	FUNCTION_START;

	assert( i_lhs._u32Row == i_rhs._u32Row );
	assert( i_lhs._u32Column == i_rhs._u32Column );

	for( UINT32 i = 0; i < i_lhs._u32Row; ++i )
	{
		for( UINT32 j = 0; j < i_lhs._u32Column; ++j )
		{
			if( !Utilities::Math::AreRelativelyEqual(i_lhs._value[i][j], i_rhs._value[i][j], (const int)(10000000000)) )
			{
				FUNCTION_FINISH;
				return false;
			}
		}
	}

	FUNCTION_FINISH;
	return true;
}

	}	// namespace Math
}	// namespace GameEngine
