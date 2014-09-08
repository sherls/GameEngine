/**
 ****************************************************************************************************
 * \file		Entity.inl
 * \brief		The inline functions implementation of Entity.h
 ****************************************************************************************************
*/

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "../GameEngineDefault.h"

/****************************************************************************************************
			Public functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void SetController( ActionController *i_newController )
	\brief		Set the value of _controller of Entity class
	\param		*i_newController new _controller value
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Entity::SetController( EntityController *i_newController )
{

	if( _controller )
	{
		delete _controller;
		_controller = NULL;
	}

	_controller = i_newController;
}

/**
 ****************************************************************************************************
	\fn			void SetName( const char *i_name )
	\brief		Set the value of _name of Entity class
	\param		*i_name new _name value
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Entity::SetName( const char *i_name )
{
	assert( i_name );
	assert( strlen(i_name) != 0 );
	assert( i_name[0] != '\0' );

	m_hashedName = Utilities::StringHash( i_name );
}

/****************************************************************************************************
			Private functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			Entity( const Vector3 &i_position, EntityController *i_controller, const char *i_name )
	\brief		Construct Entity class and initialize the member
	\param		&i_position initial _v3Position value
	\param		*i_controller the pointer to initial _controller value
	\param		*i_name name of entity
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Entity::Entity( const Math::Vector3 &i_position, EntityController *i_controller, const char* i_name ):
	m_v3Acceleration( Math::Vector3::Zero ),
	m_vLookAt( D3DXVECTOR3_FORWARD ),
	m_v3Position( i_position ),
	m_v3ProjectedPosition( Math::Vector3::Zero ),
	m_v3Velocity( Math::Vector3::Zero ),
	m_v3ProjectedVelocity( Math::Vector3::Zero ),
	m_tag( NULL ),\
	m_vScale( D3DXVECTOR3(1.0f, 1.0f, 1.0f) ),
	m_orientation( 0.0f ),
	m_u32CollisionMask( 0 ),
	m_u8AIEntityIndex( Utilities::MAX_UINT8 ),
	m_u8PhysicsEntityIndex( Utilities::MAX_UINT8 ),
	m_u8CollisionEntityIndex( Utilities::MAX_UINT8 ),
	m_u8TriggerBoxEntityIndex( Utilities::MAX_UINT8 ),
	m_u8EntityID( Utilities::MAX_UINT8 ),
	m_isDestroyed( false ),
	m_applyPhysics( true ),
	_controller( i_controller ),
	m_hashedName( i_name )
{
	SetName( i_name );

	m_size.width = DEFAULT_SPRITE_WIDTH;
	m_size.height = DEFAULT_SPRITE_HEIGHT;
}

/**
 ****************************************************************************************************
	\fn			const char* GetName( void ) const
	\brief		Get the name of Entity class
	\param		NONE
	\return		_name the name of current Entity
 ****************************************************************************************************
*/
bool GameEngine::Entity::IsName( const Utilities::StringHash &i_name )
{
	return m_hashedName == i_name;
}

/**
 ****************************************************************************************************
	\fn			const EntityController* GetEntityController( void ) const
	\brief		Get the controller of Entity class
	\param		NONE
	\return		_name the name of current Entity
 ****************************************************************************************************
*/
GameEngine::EntityController* GameEngine::Entity::GetEntityController( void ) const
{
	return _controller;
}