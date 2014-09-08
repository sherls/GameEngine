/**
 ****************************************************************************************************
	\file		Math.h
	\brief		The header of mathematical functions
 ****************************************************************************************************
*/

#ifndef _MATH_H_
#define _MATH_H_

#include "..\UtilitiesTypes.h"

namespace Utilities
{
	namespace Math
	{
		bool AreRelativelyEqual( const float &i_lhs, const float &i_rhs, const int &i_Ulps = 10 );
		bool AreWithinRange( const D3DXVECTOR3 &i_point1, const D3DXVECTOR3 &i_point2, const float &i_range );
		float MaxFloats( const float &i_float1, const float &i_float2, const float &i_float3 );
		bool RayTracing( const D3DXVECTOR3 &i_startPoint, const D3DXVECTOR3 &i_endPoint,
			const D3DXVECTOR3 &i_vertex0, const D3DXVECTOR3 &i_vertex1, const D3DXVECTOR3 &i_vertex2,
			const bool &i_bCollisionHasDetected, D3DXVECTOR3 &o_collisionPoint, float &o_collisionDistance );
	}	// namespace Math
}	// namespace GameEngine

#endif	// #ifndef _MATH_H_