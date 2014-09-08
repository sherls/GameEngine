/**
 ****************************************************************************************************
 * \file		Entity.h
 * \brief		The header of Entity class
 ****************************************************************************************************
*/

#ifndef _ENTITY_H_
#define _ENTITY_H_

// Utilities header
#include <SmartPtr/SmartPtr.h>
#include <StringHash/StringHash.h>

#include "../GameEngineDefault.h"
//#include "../Collision/BoundingBox.h"
#include "../Math/Vector3/Vector3.h"

namespace GameEngine
{
	class EntityController;
	class Utilities::MemoryPool;

	class Entity
	{
		static Utilities::MemoryPool	*_entityPool;

		EntityController							*_controller;

		// Default constructor
		inline Entity( void );
		inline Entity( const Math::Vector3 &i_position, EntityController *i_controller = 0, const char* i_name = DEFAULT_NAME );

		void *operator new( size_t i_size );

		// Make it non-copyable
		Entity( const Entity &i_rhs );
		Entity &operator=( const Entity &i_other );

		// Make it incomparable
		bool &operator==( const Entity &i_other ) const;

	public:
		Utilities::StringHash			m_hashedName;
		Utilities::S_SIZE					m_size;
		D3DXVECTOR3								m_vLookAt;
		D3DXVECTOR3								m_vScale;
		Math::Vector3							m_v3Acceleration;
		Math::Vector3							m_v3Position;
		Math::Vector3							m_v3ProjectedPosition;
		Math::Vector3							m_v3Velocity;
		Math::Vector3							m_v3ProjectedVelocity;
		std::string*							m_tag;
		float											m_orientation;
		UINT32										m_u32CollisionMask;
		UINT8											m_u8AIEntityIndex;
		UINT8											m_u8PhysicsEntityIndex;
		UINT8											m_u8CollisionEntityIndex;
		UINT8											m_u8TriggerBoxEntityIndex;
		UINT8											m_u8EntityID;
		bool											m_isDestroyed : 1;
		bool											m_applyPhysics : 1;

		// Standard constructor
		static Utilities::Pointer::SmartPtr<Entity> Create( const Math::Vector3 &i_position, EntityController *i_controller = 0, const char* i_name = DEFAULT_NAME );

		~Entity( void );

		inline void SetController( EntityController *i_newController );
		inline void SetName( const char *i_name );
		void operator delete( void *i_ptr );

		static bool Initialize( void );
		static void ShutDown( void );

		inline bool IsName( const Utilities::StringHash &i_name );
		inline EntityController* GetEntityController( void ) const;
	};

	class EntityController
	{
	public:
		virtual ~EntityController( void ) {}
		virtual void BeginUpdate( Entity &i_object ) {}
		virtual void Update( Entity &i_object ) = 0;
		virtual void EndUpdate( Entity &i_object ) = 0;
	};
}

#include "Entity.inl"

#endif	// #ifndef _ENTITY_H_