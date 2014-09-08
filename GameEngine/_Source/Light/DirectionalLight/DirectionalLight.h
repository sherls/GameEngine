/**
 ****************************************************************************************************
 * \file		DirectionalLight.h
 * \brief		Directional light class declaration
 ****************************************************************************************************
*/

#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include <d3d9.h>

#include <UtilitiesTypes.h>

namespace GameEngine
{
	class DirectionalLight
	{
		D3DXVECTOR3 _lookAt;
		float _nearView;
		float _width;
		float _height;

	public:
		D3DXVECTOR3 m_direction;
		D3DCOLOR m_colour;
		float m_intensity;
		float m_farView;

		// Constructor
		inline DirectionalLight( void );
		inline DirectionalLight( const D3DXVECTOR3 &i_direction, const D3DXVECTOR3 &i_lookAt, const D3DCOLOR &i_colour,
			const float &i_nearView, const float &i_farView, const float &i_intensity, const Utilities::S_SIZE &i_size );
		
		// Destructor
		inline ~DirectionalLight( void );
		
		void Update( void );

		const D3DXMATRIX GetWorldToViewTransform( void ) const;
		const D3DXMATRIX GetViewToProjectedTransform( void ) const;
	};
}

#include "DirectionalLight.inl"

#endif	// #ifndef _DIRECTIONAL_LIGHT_H_