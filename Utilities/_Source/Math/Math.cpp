/**
 ***************************************************************************************************
	\file		Math.cpp
	\brief		The implementation of mathematical functions
 ****************************************************************************************************
*/

#include <math.h>
#include <assert.h>

#include "Math.h"
#include "../Debug/Debug.h"

/**
 ****************************************************************************************************
	\fn			bool AreRelativelyEqual( const float &i_lhs, const float &i_rhs, const int &i_Ulps )
	\brief		Check whether two floating number is relatively equal
	\param		i_lhs float number #1
	\param		i_rhs float number #2
	\return		BOOLEAN
	\retval		TRUE if relatively equal
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool Utilities::Math::AreRelativelyEqual( const float &i_lhs, const float &i_rhs, const int &i_Ulps )
{
	int maxUlps = i_Ulps;

	assert( (maxUlps > 0) && (maxUlps < 2147483647) );
	FUNCTION_START;

	int int_lhs = *(int *)&i_lhs;
	
	if( int_lhs < 0 )
		int_lhs = 0x80000000 - int_lhs;

	int int_rhs = *(int *)&i_rhs;
	if( int_rhs < 0 )
		int_rhs = 0x80000000 - int_rhs;

	int intDiff = abs(int_lhs - int_rhs);
	if( intDiff <= maxUlps )
	{
		FUNCTION_FINISH;
		return true;
	}

	FUNCTION_FINISH;
	return false;
}

/**
 ****************************************************************************************************
	\fn			bool AreWithinRange( const D3DXVECTOR3 &i_point1, const D3DXVECTOR3 &i_point2, const float &i_range )
	\brief		Check whether two points are within range
	\param		i_point1 point 1
	\param		i_point2 point 2
	\param		i_range the range
	\return		BOOLEAN
	\retval		TRUE if within range
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool Utilities::Math::AreWithinRange( const D3DXVECTOR3 &i_point1, const D3DXVECTOR3 &i_point2, const float &i_range )
{
	D3DXVECTOR3 distance = i_point2 - i_point1;
	FUNCTION_START;

	if( (abs(distance.x) < i_range) && (abs(distance.z) < i_range) )
		return TRUE;

	FUNCTION_FINISH;
	return FALSE;
}

/**
 ****************************************************************************************************
	\fn			float MaxFloats( const float &i_float1, const float &i_float2, const float &i_float3 )
	\brief		Find the maximum value between three float
	\param		i_float1 first float number
	\param		i_float2 second float number
	\param		i_float3 third float number
 ****************************************************************************************************
*/
float Utilities::Math::MaxFloats( const float &i_float1, const float &i_float2, const float &i_float3 )
{
	FUNCTION_START;

	if( i_float1 > i_float2 )
	{
		if( i_float1 > i_float3 )
		{
			FUNCTION_FINISH;
			return i_float1;
		}
		else
		{
			FUNCTION_FINISH;
			return i_float3;
		}
	}
	else
	{
		if( i_float2 > i_float3 )
		{
			FUNCTION_FINISH;
			return i_float2;
		}
		else
		{
			FUNCTION_FINISH;
			return i_float3;
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool RayTracing( const D3DXVECTOR3 &i_startPoint, const D3DXVECTOR3 &i_endPoint,
			const D3DXVECTOR3 &i_vertex0, const D3DXVECTOR3 &i_vertex1, const D3DXVECTOR3 &i_vertex2,
			const bool &i_bCollisionHasDetected, D3DXVECTOR3 &o_collisionPoint, float &o_collisionDistance )
	\brief		Find the intersection of a ray and a triangle
	\param		i_startPoint ray start point
	\param		i_endPoint ray end point
	\param		i_vertex0 first point of triangle
	\param		i_vertex1 second point of triangle
	\param		i_vertex2 third point of triangle
	\return		Intersection validity
	\retval		TRUE if intersection present
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool Utilities::Math::RayTracing( const D3DXVECTOR3 &i_startPoint, const D3DXVECTOR3 &i_endPoint,
	const D3DXVECTOR3 &i_vertex0, const D3DXVECTOR3 &i_vertex1, const D3DXVECTOR3 &i_vertex2,
	const bool &i_bCollisionHasDetected, D3DXVECTOR3 &o_collisionPoint, float &o_collisionDistance )
{
	float t, v, w, u;
	D3DXVECTOR3 ab;
	D3DXVECTOR3 ac;
	D3DXVECTOR3 line = i_startPoint - i_endPoint;
	D3DXVECTOR3 n;
	D3DXVECTOR3 ap;
	D3DXVECTOR3 e;
	bool collisionDetected = false;
	bool detectCollisionForThisTri = true;

	FUNCTION_START;

	ap.x = i_startPoint.x - i_vertex0.x;
	ap.y = i_startPoint.y - i_vertex0.y;
	ap.z = i_startPoint.z - i_vertex0.z;

	while( detectCollisionForThisTri )
	{
		ab.x = i_vertex1.x - i_vertex0.x;
		ab.y = i_vertex1.y - i_vertex0.y;
		ab.z = i_vertex1.z - i_vertex0.z;

		ac.x = i_vertex2.x - i_vertex0.x;
		ac.y = i_vertex2.y - i_vertex0.y;
		ac.z = i_vertex2.z - i_vertex0.z;

		D3DXVec3Cross( &n, &ab, &ac );

		float d = D3DXVec3Dot( &line, &n );
		if( d <= 0.0f )
			break;

		t = D3DXVec3Dot( &ap, &n );
		if( t < 0.0f )
			break;
		if( t > d )
			break;	// For segment, exclude this code line for a ray test

		D3DXVec3Cross( &e, &line, &ap );
		v = D3DXVec3Dot( &ac, &e );
		if( (v < 0.0f) || (v > d) )
			break;
		w = -D3DXVec3Dot( &ab, &e );
		if( (w < 0.0f) || (v + w > d) )
			break;

		float ood = 1.0f / d;
		t *= ood;
		detectCollisionForThisTri = false;
		if( i_bCollisionHasDetected )
		{
			if( t >= o_collisionDistance ) //if the floats are close enough, i don't care about it. so no need to add epsilon in. worst case it recomputes about the same collision point
				break;
		}
		else
			collisionDetected = true;

		o_collisionDistance = t;
		v *= ood;
		w *= ood;
		u = 1.0f - v - w;

		o_collisionPoint = (u * i_vertex0 ) + (v * i_vertex1) + (w * i_vertex2);
	}

	while( detectCollisionForThisTri )
	{
		ac.x = i_vertex1.x - i_vertex0.x;
		ac.y = i_vertex1.y - i_vertex0.y;
		ac.z = i_vertex1.z - i_vertex0.z;

		ab.x = i_vertex2.x - i_vertex0.x;
		ab.y = i_vertex2.y - i_vertex0.y;
		ab.z = i_vertex2.z - i_vertex0.z;

		D3DXVec3Cross( &n, &ab, &ac );

		float d = D3DXVec3Dot( &line, &n );
		if( d <= 0.0f )
			break;

		t = D3DXVec3Dot( &ap, &n );
		if( t < 0.0f )
			break;
		if( t > d )
			break;	// For segment, exclude this code line for a ray test

		D3DXVec3Cross( &e, &line, &ap );
		v = D3DXVec3Dot( &ac, &e );
		if( (v < 0.0f) || (v > d) )
			break;
		w = -D3DXVec3Dot( &ab, &e );
		if( (w < 0.0f) || (v + w > d) )
			break;

		float ood = 1.0f / d;
		t *= ood;
		detectCollisionForThisTri = false;
		if( collisionDetected )
		{
			if( t >= o_collisionDistance ) //if the floats are close enough, i don't care about it. so no need to add epsilon in. worst case it recomputes about the same collision point
				break;
		}
		else
			collisionDetected = true;

		o_collisionDistance = t;
		v *= ood;
		w *= ood;
		u = 1.0f - v - w;

		o_collisionPoint = (u * i_vertex0 ) + (v * i_vertex1) + (w * i_vertex2);
	}

	return collisionDetected;
}
