/**
 ****************************************************************************************************
 * \file		Entity.cpp
 * \brief		Implementation of non-inline functions of Entity class
 ****************************************************************************************************
*/

#include <stdio.h>

// Utilities header
#include <MemoryPool/MemoryPool.h>

#include "Entity.h"
#include "../Renderer/Renderer.h"
#include "../Utilities/GameEngineTypes.h"

Utilities::MemoryPool *GameEngine::Entity::_entityPool = NULL;

/****************************************************************************************************
			Public functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			static Create( const Vector3 &i_position, EntityController *i_controller )
	\brief		Create Entity class and initialize the member
	\param		&i_position initial _v3Position value
	\param		*i_controller the pointer to initial _controller value
	\param		*i_name name of entity
	\return		Created Entity
 ****************************************************************************************************
*/
Utilities::Pointer::SmartPtr<GameEngine::Entity> GameEngine::Entity::Create( const Math::Vector3 &i_position, EntityController *i_controller, const char* i_name )
{
	assert( _entityPool );

	return Utilities::Pointer::SmartPtr<Entity>( new Entity(i_position, i_controller, i_name) );
}

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize entity
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::Entity::Initialize( void )
{
	FUNCTION_START;

	assert( _entityPool == NULL );

	_entityPool = Utilities::MemoryPool::Create( sizeof(Entity), Utilities::DEFAULT_MEMORY_POOL_SIZE );

	assert( _entityPool );

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shut down entity
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Entity::ShutDown( void )
{
	FUNCTION_START;

	if( _entityPool )
	{
		if( _entityPool->IsEmpty() )
		{
			delete _entityPool;
			_entityPool = NULL;
		}
		else
			DBG_MSG_LEVEL( D_ERR, "Memory pool is not empty\n" );
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void operator delete( void *i_ptr )
	\brief		Delete operator of Entity class
	\param		*i_ptr pointer to be deleted
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Entity::operator delete( void *i_ptr )
{
	FUNCTION_START;

	assert( _entityPool );

	if( i_ptr )
		_entityPool->Deallocate( i_ptr );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			~Entity( void )
	\brief		Destructor of Entity class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Entity::~Entity( void )
{
	FUNCTION_START;

	if( _controller )
	{
		delete _controller;
		_controller = NULL;
	}

	if( m_tag )
	{
		delete m_tag;
		m_tag = NULL;
	}

	/*if( _name )
		delete _name;*/

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void *operator new( size_t i_size )
	\brief		New operator of Entity class
	\param		i_size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
void *GameEngine::Entity::operator new( size_t i_size )
{
	FUNCTION_START;

	assert( i_size == sizeof(GameEngine::Entity) );
	assert( _entityPool != NULL );

	FUNCTION_FINISH;

	return _entityPool->Allocate( i_size );
}