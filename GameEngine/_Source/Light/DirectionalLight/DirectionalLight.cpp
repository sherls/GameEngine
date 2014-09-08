/**
 ****************************************************************************************************
 * \file		cDirectionalLight.cpp
 * \brief		Directional light class implementation
 ****************************************************************************************************
*/

#include <D3DX9Shader.h>

#include "DirectionalLight.h"

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update light
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DirectionalLight::Update( void )
{
}

/**
 ****************************************************************************************************
	\fn			const D3DXMATRIX GetWorldToViewTransform( void ) const
	\brief		Get world to view matrix transformation of light
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
const D3DXMATRIX GameEngine::DirectionalLight::GetWorldToViewTransform( void ) const
{
	D3DXVECTOR3 up;
	D3DXMATRIX transform_worldToView;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	D3DXVECTOR3 position = -m_direction * 10;

	D3DXMatrixLookAtLH( &transform_worldToView, &position, &_lookAt, &up );
	
	return transform_worldToView;
}

/**
 ****************************************************************************************************
	\fn			const D3DXMATRIX GetViewToProjectedTransform( void ) const
	\brief		Get view to projected matrix transformation of light
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
const D3DXMATRIX GameEngine::DirectionalLight::GetViewToProjectedTransform( void ) const
{
	D3DXMATRIX transform_viewToProjected;

	D3DXMatrixOrthoLH( &transform_viewToProjected, _width, _height, _nearView, m_farView );

	return transform_viewToProjected;
}