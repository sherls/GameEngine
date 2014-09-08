/**
 ****************************************************************************************************
 * \file		FastVector3.inl
 * \brief		The inline functions implementation of FastVector3.h
 ****************************************************************************************************
*/

#include <assert.h>
#include <math.h>

/**
 ****************************************************************************************************
	\fn			FastVector3( const float i_x, const float i_y, const float i_z )
	\brief		Construct FastVector3 class and initialize the member
	\param		i_x initial _x value
	\param		i_y initial _y value
	\param		i_z initial _z value
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Math::FastVector3::FastVector3( const float i_x, const float i_y, const float i_z ) :
	_x( i_x ),
	_y( i_y ),
	_z( i_z )
{
	assert( !_isnan(i_x) );
	assert( !_isnan(i_y) );
	assert( !_isnan(i_z) );
}
