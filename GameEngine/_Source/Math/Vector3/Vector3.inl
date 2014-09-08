/**
 ****************************************************************************************************
 * \file		Vector3.inl
 * \brief		The inline functions implementation of Vector3 class
 ****************************************************************************************************
*/

#include <assert.h>
#include <float.h>

// Utilities header
#include <Math/Math.h>

namespace GameEngine
{
/**
 ****************************************************************************************************
	\fn			Vector3( const float i_x, const float i_y, const float i_z )
	\brief		Construct Vector3 class and initialize the member
	\param		i_x initial _x value
	\param		i_y initial _y value
	\param		i_z initial _z value
	\return		NONE
 ****************************************************************************************************
*/
Math::Vector3::Vector3( const float i_x, const float i_y, const float i_z ) :
	_x( i_x ),
	_y( i_y ),
	_z( i_z )
{
	assert( !_isnan(i_x) );
	assert( !_isnan(i_y) );
	assert( !_isnan(i_z) );
}

/**
 ****************************************************************************************************
	\fn			Vector3( const Vector3 &i_other )
	\brief		Vector3 class copy constructor
	\param		&i_rhs Vector3 class source
	\return		NONE
 ****************************************************************************************************
*/
Math::Vector3::Vector3( const Vector3 &i_other ) :
	_x( i_other._x ),
	_y( i_other._y ),
	_z( i_other._z )
{
	assert( !_isnan(i_other._x) );
	assert( !_isnan(i_other._y) );
	assert( !_isnan(i_other._z) );
}

/**
 ****************************************************************************************************
	\fn			Vector3( const D3DXVECTOR3 &i_other )
	\brief		Vector3 class copy constructor
	\param		&i_rhs Vector3 class source
	\return		NONE
 ****************************************************************************************************
*/
Math::Vector3::Vector3( const D3DXVECTOR3 &i_other ) :
	_x( i_other.x ),
	_y( i_other.y ),
	_z( i_other.z )
{
	assert( !_isnan(i_other.x) );
	assert( !_isnan(i_other.y) );
	assert( !_isnan(i_other.z) );
}

/**
 ****************************************************************************************************
	\fn			void X( const float i_x )
	\brief		Set the value of x member of Vector3 class
	\param		i_x new _x value
	\return		NONE
 ****************************************************************************************************
*/
void Math::Vector3::X( const float &i_x )
{
	assert( !_isnan(i_x) );
	_x = i_x;
}

/**
 ****************************************************************************************************
	\fn			void Y( const float i_y )
	\brief		Set the value of y member of Vector3 class
	\param		i_y new _y value
	\return		NONE
 ****************************************************************************************************
*/
void Math::Vector3::Y( const float &i_y )
{
	assert( !_isnan(i_y) );
	_y = i_y;
}

/**
 ****************************************************************************************************
	\fn			void Z( const float i_z )
	\brief		Set the value of z member of Vector3 class
	\param		i_z new _z value
	\return		NONE
 ****************************************************************************************************
*/
void Math::Vector3::Z( const float &i_z )
{
	assert( !_isnan(i_z) );
	_z = i_z;
}

/**
 ****************************************************************************************************
	\fn			const float X( void ) const
	\brief		Get the value of x member of Vector3 class
	\param		NONE
	\return		_x the value of _x member of Vector3 class
 ****************************************************************************************************
*/
const float Math::Vector3::X( void ) const
{
	return _x;
}

/**
 ****************************************************************************************************
	\fn			const float Y( void ) const
	\brief		Get the value of y member of Vector3 class
	\param		NONE
	\return		_y the value of _y member of Vector3 class
 ****************************************************************************************************
*/
const float Math::Vector3::Y( void ) const
{
	return _y;
}

/**
 ****************************************************************************************************
	\fn			const float Z( void ) const
	\brief		Get the value of z member of Vector3 class
	\param		NONE
	\return		_z the value of _z member of Vector3 class
 ****************************************************************************************************
*/
const float Math::Vector3::Z( void ) const
{
	return _z;
}

/**
 ****************************************************************************************************
	\fn			void operator=( const Vector3 &i_rhs )
	\brief		= operator of Vector3 class
	\param		&i_rhs Vector3 class source
	\return		NONE
 ****************************************************************************************************
*/
void Math::Vector3::operator=( const Vector3 &i_rhs )
{
	assert( !_isnan(i_rhs._x) );
	assert( !_isnan(i_rhs._y) );
	assert( !_isnan(i_rhs._z) );

	_x = i_rhs._x;
	_y = i_rhs._y;
	_z = i_rhs._z;
}

/**
 ****************************************************************************************************
	\fn			void operator=( const D3DXVEXTOR3 &i_rhs )
	\brief		= operator of Vector3 class
	\param		&i_rhs D3DXVECTOR3 source
	\return		NONE
 ****************************************************************************************************
*/
void Math::Vector3::operator=( const D3DXVECTOR3 &i_rhs )
{
	assert( !_isnan(i_rhs.x) );
	assert( !_isnan(i_rhs.y) );
	assert( !_isnan(i_rhs.z) );

	_x = i_rhs.x;
	_y = i_rhs.y;
	_z = i_rhs.z;
}

/**
 ****************************************************************************************************
	\fn			void operator+=( const Vector3 &i_rhs )
	\brief		+= operator of Vector3 class
	\param		&i_rhs right hand site of operator -
	\return		NONE
 ****************************************************************************************************
*/
void Math::Vector3::operator+=( const Vector3 &i_rhs )
{
	_x += i_rhs.X();
	_y += i_rhs.Y();
	_z += i_rhs.Z();
}

/**
 ****************************************************************************************************
	\fn			void &operator-=( const Vector3 &i_rhs )
	\brief		-= operator of Vector3 class
	\param		&i_rhs right hand site of operator -
	\return		NONE
 ****************************************************************************************************
*/
void Math::Vector3::operator-=( const Vector3 &i_rhs )
{
	_x -= i_rhs.X();
	_y -= i_rhs.Y();
	_z -= i_rhs.Z();
}

/**
 ****************************************************************************************************
	\fn			void operator*=( const floats &i_rhs )
	\brief		*= operator of Vector3 class
	\param		&i_rhs right hand site of operator *=
	\return		NONE
 ****************************************************************************************************
*/
void Math::Vector3::operator*=( const float &i_rhs )
{
	_x *= i_rhs;
	_y *= i_rhs;
	_z *= i_rhs;
}

/****************************************************************************************************
			OUTSIDE CLASS MEMBER
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			const Vector3 operator+( const Vector3 &i_lhs, const Vector3 &i_rhs )
	\brief		Override + operator of Vector3 class
	\param		&i_lhs left hand side of operator +
	\param		&i_rhs right hand side of operator +
	\return		Result of i_lhs + i_rhs
 ****************************************************************************************************
*/
Math::Vector3 Math::operator+( const Vector3 &i_lhs, const Vector3 &i_rhs )
{
	return Vector3( i_lhs.X() + i_rhs.X(), i_lhs.Y() + i_rhs.Y(), i_lhs.Z() + i_rhs.Z() );
}

/**
 ****************************************************************************************************
	\fn			const Vector3 operator-( const Vector3 &i_lhs, const Vector3 &i_rhs )
	\brief		Override - operator of Vector3 class
	\param		&i_lhs left hand side of operator -
	\param		&i_rhs right hand side of operator -
	\return		Result of i_lhs - i_rhs
 ****************************************************************************************************
*/
Math::Vector3 Math::operator-( const Vector3 &i_lhs, const Vector3 &i_rhs )
{
	return Vector3( i_lhs.X() - i_rhs.X(), i_lhs.Y() - i_rhs.Y(), i_lhs.Z() - i_rhs.Z() );
}

/**
 ****************************************************************************************************
	\fn			bool operator>( const Vector3 &i_lhs, const Vector3 &i_rhs )
	\brief		> operator of Vector3 class
	\param		&i_lhs left hand side of operator >
	\param		&i_rhs right hand side of operator >
	\return		Constant result of i_rhs > this
 ****************************************************************************************************
*/
bool Math::operator>( const Vector3 &i_lhs, const Vector3 &i_rhs )
{
	if( i_lhs.SquaredLength() > i_rhs.SquaredLength() )
		return true;
	else
		return false;
}

/**
 ****************************************************************************************************
	\fn			bool operator==( const Vector3 &i_lhs, const Vector3 &i_rhs ) const
	\brief		== operator of Vector3 class
	\param		&i_lhs left hand side of operator ==
	\param		&i_rhs right hand side of operator ==
	\return		Constant result of i_rhs == this
 ****************************************************************************************************
*/
bool Math::operator==( const Vector3 &i_lhs, const Vector3 &i_rhs )
{
	if( Utilities::Math::AreRelativelyEqual(i_lhs.X(), i_rhs.X(), 10)
		&& Utilities::Math::AreRelativelyEqual(i_lhs.Y(), i_rhs.Y(), 10)
		&& Utilities::Math::AreRelativelyEqual(i_lhs.Z(), i_rhs.Z(), 10) )
		return true;
	else
		return false;
}

/**
 ****************************************************************************************************
	\fn			bool operator!=( const Vector3 &i_lhs, const Vector3 &i_rhs ) const
	\brief		!= operator of Vector3 class
	\param		&i_lhs left hand side of operator !=
	\param		&i_rhs right hand side of operator !=
	\return		Constant result of i_rhs != this
 ****************************************************************************************************
*/
bool Math::operator!=( const Vector3 &i_lhs, const Vector3 &i_rhs )
{
	return !(i_lhs == i_rhs);
}

/**
 ****************************************************************************************************
	\fn			const Vector3 operator*( float i_lhs, const Vector3 &i_rhs )
	\brief		* operator of Vector3 class
	\param		i_lhs float number multiplier
	\param		&i_rhs Vector3 value to be multiplied
	\return		Constant result of multiplication
 ****************************************************************************************************
*/
const Math::Vector3 Math::operator*( float i_lhs, const Vector3 &i_rhs )
{
	return Vector3( i_rhs.X() * i_lhs, i_rhs.Y() * i_lhs, i_rhs.Z() * i_lhs );
}

/**
 ****************************************************************************************************
	\fn			const Vector3 operator*( const Vector3 &i_lhs, float i_rhs )
	\brief		* operator of Vector3 class
	\param		i_rhs float number multiplier
	\param		&i_lhs Vector3 value to be multiplied
	\return		Constant result of multiplication
 ****************************************************************************************************
*/
const Math::Vector3 Math::operator*( const Vector3 &i_lhs, float i_rhs )
{
	return Vector3( i_lhs.X() * i_rhs, i_lhs.Y() * i_rhs, i_lhs.Z() * i_rhs );
}
}	// namespace GameEngine