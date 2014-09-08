/**
 ****************************************************************************************************
 * \file		PointLight.h
 * \brief		Point light class declaration
 ****************************************************************************************************
*/

#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include <d3d9.h>

namespace GameEngine
{
	class PointLight
	{
	public:
		D3DXVECTOR3 m_position;
		D3DCOLOR m_colour;
		D3DCOLOR m_ambient;
		float m_intensity;
		float m_attentuator;
		float m_radius;

		// Constructor
		inline PointLight( void );
		inline PointLight( const float i_x, const float i_y, const float i_z );
		inline PointLight( const D3DXVECTOR3 &i_position );
		inline PointLight( const D3DXVECTOR3 &i_position, const D3DCOLOR &i_colour, const D3DCOLOR &i_ambient,
			const float &i_intensity, const float &i_attentuator, const float &i_radius );

		// Destructor
		inline ~PointLight( void );
		
		void Update( void );
	};
}

#include "PointLight.inl"

#endif	// #ifndef _POINT_LIGHT_H_