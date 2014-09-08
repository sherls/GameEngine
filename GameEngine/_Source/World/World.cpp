/**
 ****************************************************************************************************
 * \file		World.cpp
 * \brief		World implementation and control
 ****************************************************************************************************
*/

#include <vector>

// Utilities header
#include <Time/Time.h>

#include "AI/AI.h"
#include "World.h"
#include "Entity.h"
#include "../Camera/Camera.h"
#include "../Physics/Physics.h"
#include "../Renderer/Renderer.h"
#include "../Collision/Collision.h"
#include "../Utilities/GameEngineTypes.h"
#include "../Light/PointLight/PointLight.h"
#include "../Light/DirectionalLight/DirectionalLight.h"

/****************************************************************************************************
			Helper functions
****************************************************************************************************/

/****************************************************************************************************
			Global functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize game world
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::World::Initialize( void )
{
	FUNCTION_START;

	if( !Entity::Initialize() )
		return FAIL;

	m_camera = NULL;
	m_directionalLight = NULL;
	m_pointLight = NULL;

	_entityDatabase = new std::vector< Utilities::Pointer::SmartPtr<Entity> >;

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin updating world
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::BeginUpdate( void )
{
	std::vector< Utilities::Pointer::SmartPtr<Entity> >::iterator iter;

	FUNCTION_START;

	for( iter = _entityDatabase->begin(); iter != _entityDatabase->end(); ++iter )
	{
		if( (*iter) != NULL )
		{
			if( (*iter)->GetEntityController() )
				(*iter)->GetEntityController()->BeginUpdate( *(*iter) );
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update game world
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::Update( void )
{
	std::vector< Utilities::Pointer::SmartPtr<Entity> >::iterator iter;

	FUNCTION_START;

	for( iter = _entityDatabase->begin(); iter != _entityDatabase->end(); ++iter )
	{
		if( (*iter) != NULL )
		{
			if( (*iter)->GetEntityController() )
				(*iter)->GetEntityController()->Update( *(*iter) );
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End updating world
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::EndUpdate( void )
{
	std::vector< Utilities::Pointer::SmartPtr<Entity> >::iterator iter;

	FUNCTION_START;

	RemoveDeadEntities();

	for( iter = _entityDatabase->begin(); iter != _entityDatabase->end(); ++iter )
	{
		if( (*iter) != NULL )
		{
			if( (*iter)->GetEntityController() )
				(*iter)->GetEntityController()->EndUpdate( *(*iter) );
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shut down game world
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::ShutDown( void )
{
	FUNCTION_START;

	if( m_camera )
	{
		delete m_camera;
		m_camera = NULL;
	}

	if( m_pointLight )
	{
		delete m_pointLight;
		m_pointLight = NULL;
	}

	if( m_directionalLight )
	{
		delete m_directionalLight;
		m_directionalLight = NULL;
	}

	if( _entityDatabase )
	{
		delete _entityDatabase;
		_entityDatabase = NULL;
	}

	Entity::ShutDown();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void AddEntity( Pointer::SmartPtr<Entity> &i_entity )
	\brief		Add object to world
	\param		i_entity entity to be added to world
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::AddEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
{
	FUNCTION_START;

	_entityDatabase->push_back( i_entity );

	if( (i_entity->m_u8PhysicsEntityIndex == Utilities::MAX_UINT8) && i_entity->m_applyPhysics )
		i_entity->m_u8PhysicsEntityIndex = Physics::AddPhysicsEntity( i_entity );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void RemoveDeadEntities( void )
	\brief		Removes dead entities from database
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::RemoveDeadEntities( void )
{
	FUNCTION_START;

	for( UINT32 i = 0; i < _entityDatabase->size(); ++i )
	{
		if( _entityDatabase->at(i)->m_isDestroyed )
		{
			_entityDatabase->at(i) = _entityDatabase->back();
			_entityDatabase->pop_back();
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void SetFriction( Pointer::SmartPtr<Entity> &i_entity, const float i_friction )
	\brief		Set friction of this entity in the physics engine
	\param		i_entity the entity whose friction to be set
	\param		i_friction new entity friction
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::SetFriction( Utilities::Pointer::SmartPtr<Entity> &i_entity, const float i_friction )
{
	FUNCTION_START;

	if( i_entity->m_u8PhysicsEntityIndex != Utilities::MAX_UINT8 )
		Physics::SetFriction( i_entity->m_u8PhysicsEntityIndex, i_friction );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void SetCollisionType( E_COLLISION_BY i_collisionBy )
	\brief		Set collision type
	\param		i_collisionBy collision type
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::SetCollisionDetectionType( E_COLLISION_BY i_collisionBy )
{
	Collision::SetCollisionDetectionType( i_collisionBy );
}

/**
 ****************************************************************************************************
	\fn			void CreateCollisionEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity, const char *i_collisionFile )
	\brief		Create collision entity
	\param		i_entity the entity whose collision to be created
	\param		i_collisionFile the collision file
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::CreateCollisionEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity, const char *i_collisionFile )
{
	i_entity->m_u8CollisionEntityIndex = Collision::AddCollisionEntity( i_entity, i_collisionFile );
}

/**
 ****************************************************************************************************
	\fn			void SetCollisionHandler( Pointer::SmartPtr<Entity> &i_entity, Collision::CollisionHandler *i_collisionHandler )
	\brief		Set collision handler
	\param		i_entity the entity whose collision handler to be added
	\param		i_collisionhandler the collision handler
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::SetCollisionHandler( Utilities::Pointer::SmartPtr<Entity> &i_entity, Collision::CollisionHandler *i_collisionHandler )
{
	FUNCTION_START;

	if( (i_entity->m_u32CollisionMask != 0) && (i_entity->m_u8CollisionEntityIndex != Utilities::MAX_UINT8) )
		Collision::SetCollisionHandler( i_entity->m_u8CollisionEntityIndex, i_collisionHandler );
	else
		delete i_collisionHandler;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreateTriggerBoxEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
	\brief		Create trigger box entity
	\param		i_entity the entity whose trigger box to be created
	\param		i_centre the center of the trigger box
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::CreateTriggerBoxEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
{
	FUNCTION_START;

	i_entity->m_u8TriggerBoxEntityIndex = TriggerBox::AddTriggerBoxEntity( i_entity );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void SetTriggerBoxHandler( Utilities::Pointer::SmartPtr<Entity> &i_entity, TriggerBox::TriggerBoxHandler *i_triggerBoxHandler )
	\brief		Set trigger box handler
	\param		i_entity the entity whose trigger box handler to be added
	\param		i_triggerBoxhandler the trigger box handler
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::SetTriggerBoxHandler( Utilities::Pointer::SmartPtr<Entity> &i_entity, TriggerBox::TriggerBoxHandler *i_triggerBoxHandler )
{
	FUNCTION_START;

	if( (i_entity->m_u32CollisionMask != 0) && (i_entity->m_u8CollisionEntityIndex != Utilities::MAX_UINT8) )
		TriggerBox::SetTriggerBoxHandler( i_entity->m_u8TriggerBoxEntityIndex, i_triggerBoxHandler );
	else
		delete i_triggerBoxHandler;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreateAIEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
	\brief		Create AI entity
	\param		i_entity the entity whose AI to be created
	\param		i_centre the center of the AI
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::CreateAIEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
{
	FUNCTION_START;

	i_entity->m_u8AIEntityIndex = AI::AddAIEntity( i_entity );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void UpdateAIDestinationTo( const UINT32 &i_u32AIEntityIndex,, const UINT8 &i_u8NodeID )
	\brief		Update current AI destination to given node ID
	\param		i_u32AIEntityIndex the index of AI entity
	\param		i_u8NodeID the ID of the destination node
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::UpdateAIDestinationTo( const UINT32 &i_u32AIEntityIndex, const UINT8 &i_u8NodeID )
{
	FUNCTION_START;

	AI::UpdateAIDestinationTo( i_u32AIEntityIndex, i_u8NodeID );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void AbortAI( const UINT32 &i_u32AIEntityIndex )
	\brief		Abort AI operation
	\param		i_u32AIEntityIndex the index of AI entity
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::AbortAI( const UINT32 &i_u32AIEntityIndex )
{
	FUNCTION_START;

	AI::AbortAI( i_u32AIEntityIndex );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool GetAIState( const UINT32 &i_u32AIEntityIndex )
	\brief		Get current AI state of the entity
	\param		i_u32AIEntityIndex the index of AI entity
	\return		BOOLEAN
	\retval		TRUE if activated
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::World::GetAIState( const UINT32 &i_u32AIEntityIndex )
{
	FUNCTION_START;

	FUNCTION_FINISH;
	return AI::GetAIState( i_u32AIEntityIndex );
}

/**
 ****************************************************************************************************
	\fn			void CreateSprite( Pointer::SmartPtr<Entity> &i_entity, const D3DCOLOR &i_colour,
					const char *i_textureFile )
	\brief		Create sprite for object
	\param		i_entity entity whose sprite to be created
	\param		i_colour colour of sprite
	\param		i_textureFile the texture of this sprite
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::CreateSprite( Utilities::Pointer::SmartPtr<Entity> &i_entity, const D3DCOLOR &i_colour, \
	const char *i_textureFile )
{
	FUNCTION_START;

	GameEngine::Renderer::AddSprite( i_entity, i_colour, i_textureFile );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreateAsset( Pointer::SmartPtr<Entity> &i_entity,, const char *i_entityFile )
	\brief		Create asset for object
	\param		i_entity entity whose asset to be created
	\param		i_entityFile the entity file
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::CreateMesh( Utilities::Pointer::SmartPtr<Entity> &i_entity, const char *i_entityFile )
{
	FUNCTION_START;

	Renderer::AddMesh( i_entity, i_entityFile );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
 \fn			void CreateCamera( Utilities::Pointer::SmartPtr<Entity> &i_entity,
					D3DXVECTOR3 &i_position, D3DXVECTOR3 &i_lookAtPoint, 
					float &i_angle, float &i_aspectRatio, float &i_nearView, float &i_farView )
	\brief		Create camera
	\param		i_entity the entity this camera refer to
	\param		i_position the position of the camera in the world
	\param		i_lookAtPoint look at position
	\param		i_angle camera angle
	\param		i_aspectRatio camera aspect ratio
	\param		i_nearView camera near view
	\param		i_farView camera far view
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::CreateCamera( Utilities::Pointer::SmartPtr<Entity> &i_entity,
	D3DXVECTOR3 &i_position, D3DXVECTOR3 &i_lookAtPoint,
	float i_angle, float i_aspectRatio, float i_nearView, float i_farView )
{
	assert( m_camera == NULL );

	m_camera = new Camera( i_entity, i_position, i_lookAtPoint, i_angle, i_aspectRatio, i_nearView, i_farView );
}

/**
 ****************************************************************************************************
 \fn			void CreatePointLight( const D3DXVECTOR3 &i_position, const D3DCOLOR &i_colour, const D3DCOLOR &i_ambient,
					const float &i_intensity, const float &i_attenuator, const float &i_radius )
	\brief		Create point light
	\param		i_position the position of the light in the world
	\param		i_colour point light colour
	\param		i_ambient point light ambient
	\param		i_intensity point light intensity
	\param		i_attenuator point light attenuator
	\param		i_radius point light radius
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::CreatePointLight( const D3DXVECTOR3 &i_position, const D3DCOLOR &i_colour, const D3DCOLOR &i_ambient,
	const float &i_intensity, const float &i_attenuator, const float &i_radius )
{
	FUNCTION_START;

	m_pointLight = new PointLight( i_position, i_colour, i_ambient, i_intensity, i_attenuator, i_radius );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
 \fn			void CreateDirectionalLight( const D3DXVECTOR3 &i_direction, const D3DXVECTOR3 &i_lookAt, const D3DCOLOR &i_colour,
 const Utilities::S_SIZE &i_size, const float &i_nearView, const float &i_farView, const float &i_intensity )
	\brief		Create directional light
	\param		i_direction direction of directional light
	\param		i_lookAt look at direction
	\param		i_colour directional light colour
	\param		i_size size of directional light
	\param		i_nearView near view distance of directional light
	\param		i_farView far view distance of directional light
	\param		i_intensity directional light intensity
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::World::CreateDirectionalLight( const D3DXVECTOR3 &i_direction, const D3DXVECTOR3 &i_lookAt, const D3DCOLOR &i_colour,
	const Utilities::S_SIZE &i_size, const float &i_nearView, const float &i_farView, const float &i_intensity )
{
	FUNCTION_START;

	m_directionalLight = new DirectionalLight( i_direction, i_lookAt, i_colour, i_nearView, i_farView, i_intensity, i_size );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			const UINT32 GetTotalEntityInWorldByID( UINT8 &i_ID )
	\brief		Get total entity in world by ID
	\param		i_ID the ID of entity
	\return		UINT32
	\retval		Total entity of this ID in world
 ****************************************************************************************************
*/
const UINT32 GameEngine::World::GetTotalEntityInWorldByID( UINT8 &i_ID )
{
	UINT32 u32Total = 0;
	std::vector< Utilities::Pointer::SmartPtr<Entity> >::iterator iter;

	FUNCTION_START;

	for( iter = _entityDatabase->begin(); iter != _entityDatabase->end(); ++iter )
	{
		if( (*iter)->m_u8EntityID == i_ID )
		u32Total++;
	}

	FUNCTION_FINISH;
	return u32Total;
}

/**
 ****************************************************************************************************
	\fn			Entity *GetEntityByName( const char *i_name )
	\brief		Get entity with i_name
	\param		i_name name of Entity
	\return		Address of entity with the required name
 ****************************************************************************************************
*/
Utilities::Pointer::SmartPtr<GameEngine::Entity> GameEngine::World::GetEntityByName( const Utilities::StringHash &i_name )
{
	std::vector< Utilities::Pointer::SmartPtr<Entity> >::iterator iter;

	FUNCTION_START;

	for( iter = _entityDatabase->begin(); iter != _entityDatabase->end(); ++iter )
	{
		if( (*iter) != NULL )
		{
			if( (*iter)->IsName(i_name) )
				return (*iter);
			/*if( strcmp((*iter)->GetName(), i_name) == 0 )
				return (*iter);*/
		}
	}

	FUNCTION_FINISH;

	return Utilities::Pointer::SmartPtr<Entity>();
}