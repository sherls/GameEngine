/**
 ****************************************************************************************************
 * \file		Vector3.cpp
 * \brief		Implementation of non-inline functions of Vector3 class
 ****************************************************************************************************
*/

#include <math.h>
#include <stdio.h>

// Utilities header
#include <Math/Math.h>
#include <MemoryPool/MemoryPool.h>

#include "Vector3.h"
#include "../Matrix/Matrix.h"
#include "../../GameEngineDefault.h"

Utilities::MemoryPool *GameEngine::Math::Vector3::_vector3Pool = NULL;

const GameEngine::Math::Vector3 GameEngine::Math::Vector3::Zero( 0.0f, 0.0f, 0.0f );
const GameEngine::Math::Vector3 GameEngine::Math::Vector3::Up( 0.0f, 1.0f, 0.0f );
const GameEngine::Math::Vector3 GameEngine::Math::Vector3::Down( 0.0f, -1.0f, 0.0f );
const GameEngine::Math::Vector3 GameEngine::Math::Vector3::Right( 1.0f, 0.0f, 0.0f );
const GameEngine::Math::Vector3 GameEngine::Math::Vector3::Left( -1.0f, 0.0f, 0.0f );
const GameEngine::Math::Vector3 GameEngine::Math::Vector3::Forward( 0.0f, 0.0f, 1.0f );
const GameEngine::Math::Vector3 GameEngine::Math::Vector3::Backward( 0.0f, 0.0f, -1.0f );

/****************************************************************************************************
			Static class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			float DotProduct( const Vector3 &i_lhs, const Vector3 &i_rhs )
	\brief		Transform 3D vector normal by the given matrix
	\param		i_lhs left hand side Vector3
	\param		i_rhs right hand side Vector3
	\return		Dot product result
 ****************************************************************************************************
*/
float GameEngine::Math::Vector3::DotProduct( const Vector3 &i_lhs, const Vector3 &i_rhs )
{
	FUNCTION_START;

	return i_lhs._x * i_rhs._x + i_lhs._y * i_rhs._y + i_lhs._z * i_rhs._z;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Vector3TransformNormal( Vector3 &o_vector, const Vector3 &i_vector, Matrix &i_matrix )
	\brief		Transform 3D vector normal by the given matrix
	\param		o_vector output vector
	\param		i_vector vector to be transformed
	\param		i_matrix matrix transformation
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Vector3::Vector3TransformNormal( Vector3 &o_vector, const Vector3 &i_vector, Matrix &i_matrix )
{
	Matrix mVector3( 1, 4 );
	Matrix mResult( 4, 1 );
	Matrix mInverse( 4, 4 );

	FUNCTION_START;

	// Create the matrix of vector
	mVector3( 0, 0 ) = i_vector.X();
	mVector3( 0, 1 ) = i_vector.Y();
	mVector3( 0, 2 ) = i_vector.Z();

	// Inverse the input matrix
	Matrix::Matrix4Inverse( i_matrix, mInverse );
	// Then transpose it
	mInverse.Transpose();

	mResult = mVector3 * mInverse;

	o_vector.X( mResult(0, 0) );
	o_vector.Y( mResult(0, 1) );
	o_vector.Z( mResult(0, 2) );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Vector3TransformCoord( Vector3 &o_vector, const Vector3 &i_vector, Matrix &i_matrix )
	\brief		Transform 3D vector by the given matrix
	\param		o_vector output vector
	\param		i_vector vector to be transformed
	\param		i_matrix matrix transformation
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Vector3::Vector3TransformCoord( Vector3 &o_vector, const Vector3 &i_vector, Matrix &i_matrix )
{
	Matrix mVector3( 1, 4 );
	Matrix mResult( 1, 4 );

	FUNCTION_START;

	// Create the matrix of vector
	mVector3( 0, 0 ) = i_vector.X();
	mVector3( 0, 1 ) = i_vector.Y();
	mVector3( 0, 2 ) = i_vector.Z();
	mVector3( 0, 3 ) = 1;

	mResult = mVector3 * i_matrix;

	o_vector.X( mResult(0, 0) );
	o_vector.Y( mResult(0, 1) );
	o_vector.Z( mResult(0, 2) );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void *operator new( UINT32 i_u32Size )
	\brief		New operator of Vector3 class
	\param		i_u32Size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
void *GameEngine::Math::Vector3::operator new( UINT32 i_u32Size )
{
	assert( i_u32Size );
	FUNCTION_START;

	if( _vector3Pool == NULL )
		_vector3Pool = Utilities::MemoryPool::Create( sizeof(Vector3), Utilities::DEFAULT_MEMORY_POOL_SIZE );

	assert( _vector3Pool != NULL );

	FUNCTION_FINISH;
	return _vector3Pool->Allocate( sizeof(Vector3) );
}

/**
 ****************************************************************************************************
	\fn			void *operator new[]( UINT32 i_u32Size )
	\brief		New[] operator of Vector3 class
	\param		i_u32Size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
void *GameEngine::Math::Vector3::operator new[]( UINT32 i_u32Size )
{
	assert( i_u32Size );
	FUNCTION_START;

	if( _vector3Pool == NULL )
		_vector3Pool = Utilities::MemoryPool::Create( sizeof(Vector3), 100 );

	assert( _vector3Pool != NULL );

	FUNCTION_FINISH;
	return _vector3Pool->Allocate( sizeof(Vector3) );
}

/**
 ****************************************************************************************************
	\fn			void operator delete( void *i_ptr )
	\brief		Delete operator of Vector3 class
	\param		*i_ptr pointer to be deleted
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::Vector3::operator delete( void *i_ptr )
{
	FUNCTION_START;

	if( i_ptr )
		_vector3Pool->Deallocate( i_ptr );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void operator delete[]( void *i_ptr )
	\brief		Delete[] operator of Vector3 class
	\param		*i_ptr pointer to be deleted
	\return		Length of Vector3
 ****************************************************************************************************
*/
void GameEngine::Math::Vector3::operator delete[]( void *i_ptr )
{
	FUNCTION_START;

	if( i_ptr )
		_vector3Pool->Deallocate( i_ptr );

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			Vector3 Reflect( Vector3 &i_input )
	\brief		Reflect the vector by the given vector
	\param		i_u32Size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
GameEngine::Math::Vector3 GameEngine::Math::Vector3::Reflect( const Vector3 &i_input )
{
	Vector3 reversedInput = i_input.Negated();
	float direction = DotProduct( reversedInput );
	Vector3 projected = direction * Normalized();

	FUNCTION_START;

	FUNCTION_FINISH;
	return reversedInput - 2.0f * (reversedInput - projected );
}

/**
 ****************************************************************************************************
	\fn			const void Negate( void ) const
	\brief		Negate Vector3
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
const void GameEngine::Math::Vector3::Negate( void )
{
	FUNCTION_START;

	_x = -_x;
	_y = -_y;
	_z = -_z;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			const Vector3 Negated( void ) const
	\brief		Get the Negated Vector3
	\param		NONE
	\return		New Vector3 which is the Negated of this Vector3
 ****************************************************************************************************
*/
const GameEngine::Math::Vector3 GameEngine::Math::Vector3::Negated( void ) const
{
	return Vector3( -_x, -_y, -_z );
}

/**
 ****************************************************************************************************
	\fn			const void Normalize( void ) const
	\brief		Normalize vector3
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
const void GameEngine::Math::Vector3::Normalize( void )
{
	const float vectorLen = Length();

	FUNCTION_START;

	if( Utilities::Math::AreRelativelyEqual(vectorLen, 0.0f) )
	{
		_x = 0.0f;
		_y = 0.0f;
		_z = 0.0f;
	}
	else
	{
		_x /= vectorLen;
		_y /= vectorLen;
		_z /= vectorLen;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			const Vector3 Normalized( void ) const
	\brief		Get the Normalized Vector3
	\param		NONE
	\return		New Vector3 which is the Normalized of this Vector3
 ****************************************************************************************************
*/
const GameEngine::Math::Vector3 GameEngine::Math::Vector3::Normalized( void ) const
{
	const float vectorLen = Length();

	if( Utilities::Math::AreRelativelyEqual(vectorLen, 0.0f) )
		return Vector3::Zero;
	else
		return Vector3( _x/vectorLen, _y/vectorLen, _z/vectorLen );
}

/**
 ****************************************************************************************************
	\fn			const float Length( void ) const
	\brief		Get the Length of Vector3
	\param		NONE
	\return		Length of Vector3
 ****************************************************************************************************
*/
const float GameEngine::Math::Vector3::Length( void ) const
{
	return sqrt( (_x*_x) + (_y*_y) + (_z*_z) );
}

/**
 ****************************************************************************************************
	\fn			const float SquaredLength( void ) const
	\brief		Square length of Vector3
	\param		NONE
	\return		Square Length of Vector3
 ****************************************************************************************************
*/
const float GameEngine::Math::Vector3::SquaredLength( void ) const
{
	return( (_x*_x) + (_y*_y) + (_z*_z) );
}

/**
 ****************************************************************************************************
	\fn			const float DotProduct( void ) const
	\brief		Dot product of Vector3
	\param		NONE
	\return		Square Length of Vector3
 ****************************************************************************************************
*/
const float GameEngine::Math::Vector3::DotProduct( const Vector3 &i_other ) const
{
	float retval;
	
	retval = _x * i_other._x + _y * i_other._y + _z * i_other._z;

	return retval;
}
