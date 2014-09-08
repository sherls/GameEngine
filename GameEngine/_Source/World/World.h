/**
 ****************************************************************************************************
 * \file		World.h
 * \brief		World global function interface. The world holds all of the entity in the game including
 *          World holds all entities in the game's world with it's characteristic
 ****************************************************************************************************
*/

#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>

// Utilities header
#include <SmartPtr/SmartPtr.h>
#include <Singleton/Singleton.h>
#include <StringHash/StringHash.h>

// GameEngine
#include "../TriggerBox/TriggerBox.h"
#include "../Utilities/GameEngineTypes.h"

namespace GameEngine
{
	class Entity;
	class Camera;
	class PointLight;
	class DirectionalLight;

	namespace Collision
	{
		class CollisionHandler;
	}

	class World
	{
		friend Utilities::Singleton<World>;
		std::vector< Utilities::Pointer::SmartPtr<Entity> > *_entityDatabase;

		World( void ){ }
		~World( void ){ }

		void RemoveDeadEntities( void );

	public:
		Camera *m_camera;
		PointLight *m_pointLight;
		DirectionalLight *m_directionalLight;

		bool Initialize( void );
		void BeginUpdate( void );
		void Update( void );
		void EndUpdate( void );
		void ShutDown( void );

		void AddEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity );

		const UINT32 GetTotalEntityInWorldByID( UINT8 &i_ID );
		Utilities::Pointer::SmartPtr<Entity> GetEntityByName( const Utilities::StringHash &i_name );

		// Physics related
		void SetFriction( Utilities::Pointer::SmartPtr<Entity> &i_entity, const float i_friction );

		// Collision related
		void SetCollisionDetectionType( E_COLLISION_BY i_collisionBy );
		void CreateCollisionEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity, const char *i_collisionFile );
		void SetCollisionHandler( Utilities::Pointer::SmartPtr<Entity> &i_entity, Collision::CollisionHandler *i_collisionHandler );

		// TriggerBox related
		void CreateTriggerBoxEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity );
		void SetTriggerBoxHandler( Utilities::Pointer::SmartPtr<Entity> &i_entity, TriggerBox::TriggerBoxHandler *i_triggerBoxHandler );

		// AI related
		void CreateAIEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity );
		void UpdateAIDestinationTo( const UINT32 &i_u32AIEntityIndex, const UINT8 &i_u8NodeID );
		void AbortAI( const UINT32 &i_u32AIEntityIndex );
		bool GetAIState( const UINT32 &i_u32AIEntityIndex );

		// 2D graphics related
		void CreateSprite( Utilities::Pointer::SmartPtr<Entity> &i_entity, const D3DCOLOR &i_colour, const char *i_textureFile );

		// 3D graphics related
		void CreateMesh( Utilities::Pointer::SmartPtr<Entity> &i_entity, const char *i_entityFile );
		void CreateCamera( Utilities::Pointer::SmartPtr<Entity> &i_entity,
			D3DXVECTOR3 &i_position, D3DXVECTOR3 &i_lookAtPoint,
			float i_angle, float i_aspectRatio, float i_nearView, float i_farView );
		void CreatePointLight( const D3DXVECTOR3 &i_position, const D3DCOLOR &i_colour, const D3DCOLOR &i_ambient,
				const float &i_intensity, const float &i_attenuator, const float &i_radius );
		void CreateDirectionalLight( const D3DXVECTOR3 &i_direction, const D3DXVECTOR3 &i_lookAt, const D3DCOLOR &i_colour,
				const Utilities::S_SIZE &i_size, const float &i_nearView, const float &i_farView, const float &i_intensity );
	};
}

typedef Utilities::Singleton<GameEngine::World> g_world;

#endif	// #ifndef _WORLD_H_