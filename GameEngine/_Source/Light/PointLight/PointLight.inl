/**
 ****************************************************************************************************
 * \file		PointLight.inl
 * \brief		The inline functions implementation of point light
 ****************************************************************************************************
*/

#include <float.h>
#include <assert.h>

namespace GameEngine
{
/**
 ****************************************************************************************************
	\fn			PointLight( void )
	\brief		Constructor of PointLight class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
PointLight::PointLight( void ) :
	m_colour( D3DCOLOR_XRGB(255, 255, 255) ),
	m_ambient( D3DCOLOR_XRGB(0, 0, 0) ),
	m_intensity( 1.0f ),
	m_attentuator( 1.0f ),
	m_radius( 100.0f )
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;
}

/**
 ****************************************************************************************************
	\fn			PointLight( const float i_x, const float i_y, const float i_z )
	\brief		Constructor of PointLight class
	\param		i_x initialize x position
	\param		i_y initialize y position
	\param		i_z initialize z position
	\return		NONE
 ****************************************************************************************************
*/
PointLight::PointLight( const float i_x, const float i_y, const float i_z ) :
	m_colour( D3DCOLOR_XRGB(255, 255, 255) ),
	m_ambient( D3DCOLOR_XRGB(0, 0, 0) ),
	m_intensity( 1.0f ),
	m_attentuator( 1.0f ),
	m_radius( 100.0f )
{
	assert( !_isnan(i_x) );
	assert( !_isnan(i_y) );
	assert( !_isnan(i_z) );

	m_position.x = i_x;
	m_position.y = i_y;
	m_position.z = i_z;
}

/**
 ****************************************************************************************************
	\fn			PointLight( const D3DXVECTOR3 &i_position )
	\brief		Constructor of PointLight class
	\param		i_position position of light
	\return		NONE
 ****************************************************************************************************
*/
PointLight::PointLight( const D3DXVECTOR3 &i_position ) :
	m_position( i_position ),
	m_colour( D3DCOLOR_XRGB(255, 255, 255) ),
	m_ambient( D3DCOLOR_XRGB(0, 0, 0) ),
	m_intensity( 1.0f ),
	m_attentuator( 1.0f ),
	m_radius( 100.0f )
{
}

/**
 ****************************************************************************************************
	\fn			PointLight( const D3DXVECTOR3 &i_position, const D3DCOLOR &i_colour, const D3DCOLOR &i_ambient, 
					const float &i_intensity, const float &i_attenuator, const float &i_radius )
	\brief		Constructor of PointLight class
	\param		i_position position of light
	\param		i_colour colour of light
	\param		i_ambient ambient light of light
	\param		i_intensity intensity of light
	\param		i_attenuator attenuator of light
	\param		i_radius radius of light
	\return		NONE
 ****************************************************************************************************
*/
PointLight::PointLight( const D3DXVECTOR3 &i_position, const D3DCOLOR &i_colour, const D3DCOLOR &i_ambient,
	const float &i_intensity, const float &i_attenuator, const float &i_radius ) :
	m_position( i_position ),
	m_colour( i_colour ),
	m_ambient( i_ambient ),
	m_intensity( i_intensity ),
	m_attentuator( i_attenuator ),
	m_radius( i_radius )
{
}

/**
 ****************************************************************************************************
	\fn			~PointLight( void )
	\brief		Destructor of PointLight class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
PointLight::~PointLight( void )
{
}
}	// namespace GameEngine