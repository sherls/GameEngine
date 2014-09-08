/**
 ****************************************************************************************************
 * \file		BoundingBox.cpp
 * \brief		The header of BoundingBox class
 ****************************************************************************************************
*/

#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include "..\Math\Vector3\Vector3.h"

namespace GameEngine
{
	namespace TriggerBox
	{
		class BoundingBox
		{
			Math::Vector3 _origin;

		public:
			typedef struct _s_extends_
			{
				float minX;
				float minY;
				float maxX;
				float maxY;
				float minZ;
				float maxZ;

				_s_extends_( float i_minX, float i_maxX, float i_minY, float i_maxY, float i_minZ, float i_maxZ ) :
					minX( i_minX ),
					maxX( i_maxX ),
					minY( i_minY ),
					maxY( i_maxY ),
					minZ( i_minZ ),
					maxZ( i_maxZ )
				{
				}
			} S_EXTENDS;
			float	m_halfW;
			float	m_halfH;
			float	m_halfD;

			inline BoundingBox( void );
			inline BoundingBox( const BoundingBox &i_other );
			inline BoundingBox( const Math::Vector3 &i_origin, const Utilities::S_SIZE &i_size );
			inline BoundingBox( const Math::Vector3 &i_origin, const float &i_halfW, const float &i_halfH, const float &i_halfD );

			inline const Math::Vector3 &Center( void ) const;
			inline const BoundingBox::S_EXTENDS GetExtends( void ) const;

			inline void Move( const Math::Vector3 &i_move );
			inline const BoundingBox Moved( const Math::Vector3 &i_move ) const;
		};
	}	// namespace Collision
}	// namespace GameEngine

#include "BoundingBox.inl"

#endif	// #ifndef _AABB_H_