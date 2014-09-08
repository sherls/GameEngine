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

// Game engine header
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
}

class CameraController : public GameEngine::EntityController
{
public:
	Utilities::Pointer::SmartPtr<GameEngine::Entity> m_followEntity;

	CameraController( void ) {}

	void BeginUpdate( GameEngine::Entity &i_entity ) {}
	void Update( GameEngine::Entity &i_entity );
	void EndUpdate( GameEngine::Entity &i_entity ) {}
	~CameraController( void ) {}
};

class CameraCollisionHandler : public GameEngine::Collision::CollisionHandler
{
public:
	CameraCollisionHandler( void ) {}
	~CameraCollisionHandler( void ) {}

	void HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity, Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other,
		const float i_collisionTime, const GameEngine::Math::Vector3 &i_collisionVector );
};

#endif	// #ifndef _CAMERA_CONTROLLER_H_