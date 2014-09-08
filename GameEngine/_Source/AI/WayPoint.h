/**
 ****************************************************************************************************
 * \file		WayPoint.h
 * \brief		Way point structures
 ****************************************************************************************************
*/

#ifndef _WAY_POINT_H_
#define _WAY_POINT_H_

namespace GameEngine
{
	namespace AI
	{
		typedef struct _s_way_point_
		{
			D3DXVECTOR3 centre;
			float radius;
		} S_WAY_POINT;

		typedef struct _s_way_point_link_
		{
			UINT32 u32From;
			UINT32 u32To;
			//float weight;
		} S_WAY_POINT_LINK;

		typedef struct _s_way_point_compare_
		{
			bool operator()( const S_WAY_POINT_LINK &i_lhs, const S_WAY_POINT_LINK &i_rhs )
			{
				if( i_lhs.u32From < i_rhs.u32From )
					return true;
				else if( i_lhs.u32From > i_rhs.u32From )
					return false;
				// if i_lhs.from = i_rhs.from
				else if( i_lhs.u32To < i_rhs.u32To )
					return true;
				else
					return false;
			}
		} S_WAY_POINT_COMPARE;
	}
}

#endif	// #ifndef _WAY_POINT_H_