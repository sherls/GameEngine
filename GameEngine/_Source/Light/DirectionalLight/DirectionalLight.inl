/**
 ****************************************************************************************************
 * \file		DirectionalLight.inl
 * \brief		The inline functions implementation of point light
 ****************************************************************************************************
*/

#include <float.h>
#include <assert.h>

namespace GameEngine
{
/**
 ****************************************************************************************************
	\fn			DirectionalLight( void )
	\brief		Constructor of DirectionalLight class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
DirectionalLight::DirectionalLight( void ) :
	m_colour( D3DCOLOR_XRGB(255, 255, 255) ),
	_lookAt( 0.0f, 0.0f, 0.0f ),
	_nearView( 0.1f ),
	m_farView( 100.0f )
{
	m_direction.x = 0.0f;
	m_direction.y = 0.0f;
	m_direction.z = 0.0f;
}

/**
 ****************************************************************************************************
	\fn			DirectionalLight( const D3DXVECTOR3 &i_direction, const D3DXVECTOR3 &i_lookAt, const D3DCOLOR &i_colourconst float &i_nearView, const float &i_farView, const float &i_intensity, const Utilities::S)SIZE &i_size )
	\brief		Constructor of DirectionalLight class
	\param		i_position position of light
	\param		i_colour colour of light
	\param		i_intensity intensity of light
	\return		NONE
 ****************************************************************************************************
*/
DirectionalLight::DirectionalLight( const D3DXVECTOR3 &i_direction, const D3DXVECTOR3 &i_lookAt, const D3DCOLOR &i_colour,
	const float &i_nearView, const float &i_farView, const float &i_intensity, const Utilities::S_SIZE &i_size) :
	m_colour( i_colour ),
	_lookAt( i_lookAt ),
	_nearView( i_nearView ),
	m_farView( i_farView ),
	m_intensity( i_intensity ),
	_width( i_size.width ),
	_height( i_size.height )
{
	D3DXVec3Normalize( &m_direction, &i_direction );
}

/**
 ****************************************************************************************************
	\fn			~DirectionalLight( void )
	\brief		Destructor of DirectionalLight class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
DirectionalLight::~DirectionalLight( void )
{
}
}	// namespace GameEngine