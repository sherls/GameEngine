/**
 ****************************************************************************************************
 * \file		CameraController.inl
 * \brief		The header of CameraController class
 ****************************************************************************************************
*/

#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

// Utilities header
#include <SmartPtr/SmartPtr.h>

#include "World/Entity.h"
#include "Collision/Collision.h"

namespace GameEngine
{
	class Entity;
	class EntityController;
	namespace Collision
	{
		class CollisionHandler;
	}

	class CameraController : public EntityController
	{
	public:
		Utilities::Pointer::SmartPtr<Entity> m_followEntity;

		CameraController( void ) {}

		void BeginUpdate( Entity &i_entity ) {}
		void Update( Entity &i_entity );
		void EndUpdate( Entity &i_entity ) {}
		~CameraController( void ) {}
	};

	class CameraCollisionHandler : public Collision::CollisionHandler
	{
		float		_originalYaw;
		float		_currYaw;
		float		_speed;
		UINT8		_u8Ctr;

	public:
		CameraCollisionHandler( void ) :
			_originalYaw( 0.0f ),
			_currYaw( 0.0f ),
			_speed( 0.0f ),
			_u8Ctr( 0 )
		{}
		~CameraCollisionHandler( void ) {}

		void HandleCollision( Utilities::Pointer::SmartPtr<Entity> &i_entity, Utilities::Pointer::SmartPtr<Entity> &i_other,
			const float i_collisionTime, const Math::Vector3 &i_collisionVector, const Utilities::StringHash &i_hashedTag );
		void OnLeavingCollision( Utilities::Pointer::SmartPtr<Entity> &i_entity, Utilities::Pointer::SmartPtr<Entity> &i_other );
	};
}

#endif	// #ifndef _CAMERA_CONTROLLER_H_