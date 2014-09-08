/**
 ****************************************************************************************************
 * \file		Physics.cpp
 * \brief		Physics engine implementation and control
 ****************************************************************************************************
*/

#include <vector>

// Utilities header
#include <Time/Time.h>
#include <Debug/Debug.h>
#include <MemoryPool/MemoryPool.h>

#include "Physics.h"
#include "../World/Entity.h"
#include "../Utilities/Profiler/Profiler.h"

namespace GameEngine
{
	namespace Physics
	{
		class PhysicsEntity
		{
			PhysicsEntity( void ) {}

			// Make it non-copyable
			PhysicsEntity( PhysicsEntity const &i_other );
			PhysicsEntity &operator=( const PhysicsEntity &i_other );

			// Make it incomparable
			bool &operator==( const PhysicsEntity & i_other ) const;

		public:
			static Utilities::MemoryPool *_physicsEntityPool;

			float						m_friction;
			Utilities::Pointer::SmartPtr<Entity>	m_entity;

			PhysicsEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity );

			~PhysicsEntity( void ) {}

			void *operator new( size_t i_size );
			void operator delete( void *i_ptr );

			void MoveTo( const Math::Vector3 &i_newPosition );
			void Move( void );
			void FinalizeMovement( void );
		};

		static std::vector< Utilities::Pointer::SmartPtr<GameEngine::Physics::PhysicsEntity> > *physicsEntityDatabase;
		void RemoveDeadEntities( void );
	}	// namespace Physics
}	// namespace GameEngine

Utilities::MemoryPool *GameEngine::Physics::PhysicsEntity::_physicsEntityPool = NULL;

/****************************************************************************************************
			Global functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize physics
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::Physics::Initialize( void )
{
	FUNCTION_START;

	assert( PhysicsEntity::_physicsEntityPool == NULL );

	PhysicsEntity::_physicsEntityPool = Utilities::MemoryPool::Create( sizeof(PhysicsEntity), Utilities::DEFAULT_MEMORY_POOL_SIZE );
	physicsEntityDatabase = new std::vector< Utilities::Pointer::SmartPtr<PhysicsEntity> >;

	assert( PhysicsEntity::_physicsEntityPool );

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin updating physics
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Physics::BeginUpdate( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update physics
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Physics::Update( void )
{
	PROFILE_UNSCOPED( "Physics" );

	std::vector< Utilities::Pointer::SmartPtr<PhysicsEntity> >::iterator iter;

	FUNCTION_START;

	for( iter = physicsEntityDatabase->begin(); iter != physicsEntityDatabase->end(); ++iter )
	{
		if( (*iter) != NULL )
			(*iter)->Move();
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End updating physics
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Physics::EndUpdate( void )
{
	std::vector< Utilities::Pointer::SmartPtr<PhysicsEntity> >::iterator iter;

	FUNCTION_START;

	RemoveDeadEntities();
	for( iter = physicsEntityDatabase->begin(); iter != physicsEntityDatabase->end(); ++iter )
	{
		if( (*iter) != NULL )
			(*iter)->FinalizeMovement();
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shutdown physics
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Physics::ShutDown( void )
{
	FUNCTION_START;

	if( physicsEntityDatabase )
	{
		delete physicsEntityDatabase;
		physicsEntityDatabase = NULL;
	}

	if( PhysicsEntity::_physicsEntityPool )
	{
		delete PhysicsEntity::_physicsEntityPool;
		PhysicsEntity::_physicsEntityPool = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			UINT8 AddPhysicsEntity( Entity *i_entity )
	\brief		Add physics entity to physicsEntityDatabase
	\param		*i_entity entity to be added
	\return		UINT8
	\retval		index of the new added physics entity in database
 ****************************************************************************************************
*/
UINT8 GameEngine::Physics::AddPhysicsEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
{
	FUNCTION_START;

	physicsEntityDatabase->push_back( new PhysicsEntity(i_entity) );

	FUNCTION_FINISH;
	return physicsEntityDatabase->size() - 1;
}

/**
 ****************************************************************************************************
	\fn			void SetFriction( UINT8 &i_u8Index, const float &i_friction )
	\brief		Add physics entity to physicsEntityDatabase
	\param		i_u8Index index of entity in physics entity
	\param		i_friction new friction
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Physics::SetFriction( const UINT8 &i_u8Index, const float &i_friction )
{
	FUNCTION_START;

	if( i_u8Index < physicsEntityDatabase->size() )
		physicsEntityDatabase->at(i_u8Index)->m_friction = i_friction;

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void RemoveDeadEntities( void )
	\brief		Remove dead entities
	\param		*im_entity entity to be added
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Physics::RemoveDeadEntities( void )
{
	FUNCTION_START;

	for( UINT8 i = 0; i < physicsEntityDatabase->size(); ++i )
	{
		if( physicsEntityDatabase->at(i)->m_entity->m_isDestroyed )
		{
			physicsEntityDatabase->back()->m_entity->m_u8PhysicsEntityIndex = physicsEntityDatabase->at(i)->m_entity->m_u8PhysicsEntityIndex;
			physicsEntityDatabase->at(i) = physicsEntityDatabase->back();
			physicsEntityDatabase->pop_back();
		}
	}

	FUNCTION_FINISH;
}

/****************************************************************************************************
			PhysicsEntity class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			PhysicsEntity( Pointer::SmartPtr<Entity> &im_entity )
	\brief		Default constructor of PhysicsEntity
	\param		&im_entity pointer to common entity data
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Physics::PhysicsEntity::PhysicsEntity( Utilities::Pointer::SmartPtr<Entity> &im_entity ) :
	m_entity( im_entity ),
	m_friction( DEFAULT_FRICTION )
{
}

/**
 ****************************************************************************************************
	\fn			void *operator new( size_t i_size )
	\brief		New operator of PhysicsEntity class
	\param		i_size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
void *GameEngine::Physics::PhysicsEntity::operator new( size_t i_size )
{
	FUNCTION_START;

	assert( i_size == sizeof(PhysicsEntity) );
	assert( _physicsEntityPool );

	FUNCTION_FINISH;
	return _physicsEntityPool->Allocate( sizeof(PhysicsEntity) );
}

/**
 ****************************************************************************************************
	\fn			void operator delete( void *i_ptr )
	\brief		Delete operator of PhysicsEntity class
	\param		i_ptr pointer to be deleted
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Physics::PhysicsEntity::operator delete( void *i_ptr )
{
	FUNCTION_START;

	assert( _physicsEntityPool );

	if( i_ptr )
		_physicsEntityPool->Deallocate( i_ptr );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void MoveTo( const Vector3 &i_newPosition )
	\brief		Move Entity to given position
	\param		&i_newPosition new _v3Position
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Physics::PhysicsEntity::MoveTo( const Math::Vector3 &i_newPosition )
{
	FUNCTION_START;

	m_entity->m_v3Position = i_newPosition;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Move( void )
	\brief		Move Entity to given direction
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Physics::PhysicsEntity::Move( void )
{
	float frameTime_ms = Utilities::Time::GetTimeElapsedThisFrame_ms();

	FUNCTION_START;

	m_entity->m_v3ProjectedPosition = m_entity->m_v3Position + (frameTime_ms * m_entity->m_v3Velocity);
	m_entity->m_v3Acceleration *= m_friction;
	m_entity->m_v3ProjectedVelocity = m_entity->m_v3Velocity + (frameTime_ms * m_entity->m_v3Acceleration);

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void FinalizeMovement( void )
	\brief		Finalize Entity movement to given direction
	\param		&i_frameTime_ms elapsed frame time (in ms)
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Physics::PhysicsEntity::FinalizeMovement( void )
{
	FUNCTION_START;

	m_entity->m_v3Velocity = m_entity->m_v3ProjectedVelocity;
	m_entity->m_v3Position = m_entity->m_v3ProjectedPosition;

	FUNCTION_FINISH;
}