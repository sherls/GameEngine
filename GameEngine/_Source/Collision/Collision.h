/**
 ****************************************************************************************************
 * \file		Collision.h
 * \brief		Collision engine global function declaration
 ****************************************************************************************************
*/

#ifndef _COLLISION_H_
#define _COLLISION_H_

// Utilities header
#include <SmartPtr/SmartPtr.h>

#include "../Utilities/GameEngineTypes.h"

// Grid, Quad Tree/ Oct Tree, Sphere Tree, K-D Tree, BSP Tree

namespace GameEngine
{
	namespace Math
	{
		class Vector3;
	}	// namespace Math

	class Entity;

	namespace Collision
	{
		class CollisionHandler
		{
		public:
			virtual ~CollisionHandler( void ) {}
			virtual void HandleCollision( Utilities::Pointer::SmartPtr<Entity> &i_entity, \
				Utilities::Pointer::SmartPtr<Entity> &i_other, \
				const float i_collisionTime, const Math::Vector3 &i_collisionNormal, const Utilities::StringHash &i_hashedTag = Utilities::StringHash("") ) = 0;
			virtual void OnLeavingCollision( Utilities::Pointer::SmartPtr<Entity> &i_entity, Utilities::Pointer::SmartPtr<Entity> &i_other ) {}
		};

		bool Initialize( void );
		void BeginUpdate( void );
		void Update( void );
		void EndUpdate( void );
		void ShutDown( void );

		UINT8 AddCollisionEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity, const char *i_collisionFile );
		void SetCollisionHandler( const UINT8 &i_index, CollisionHandler *i_collisionHandler );
		void SetCollisionDetectionType( const E_COLLISION_BY &i_collisionBy );
	}	// namespace Collision
}	// namespace GameEngine

#endif	// #ifndef _COLLISION_H_