/**
 ****************************************************************************************************
 * \file		PlayerController.h
 * \brief		The header of PlayerController class
 ****************************************************************************************************
*/

#ifndef _PLAYER_CONTROLLER_H_
#define _PLAYER_CONTROLLER_H_

namespace GameEngine
{
	class Entity;
	class EntityController;
	namespace Collision
	{
		class CollisionHandler;
	}
	namespace TriggerBox
	{
		class TriggerBoxHandler;
	}
}

class PlayerController : public GameEngine::EntityController
{
	float _pitchAngle;
	UINT32 _u32Power;

	void RotatePlayerView( GameEngine::Entity &i_entity, const float &i_yaw, const float &i_pitch );

public:
	PlayerController( void );
	~PlayerController( void ) {}

	void BeginUpdate( GameEngine::Entity &i_entity ) {}
	void Update( GameEngine::Entity &i_entity );
	void EndUpdate( GameEngine::Entity &i_entity );

	static bool IsPlayerQuit( void );
};

class PlayerCollisionHandler : public GameEngine::Collision::CollisionHandler
{
	GameEngine::Math::Vector3 _groundPos;

public:
	PlayerCollisionHandler( void ) {}
	~PlayerCollisionHandler( void ) {}

	void HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity, Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other,
		const float i_collisionTime, const GameEngine::Math::Vector3 &i_collisionVector, const Utilities::StringHash &i_hashedTag );
};

class PlayerTriggerBoxHandler : public GameEngine::TriggerBox::TriggerBoxHandler
{
public:
	PlayerTriggerBoxHandler( void ) {}
	~PlayerTriggerBoxHandler( void ) {}

	void OnEnter( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime, \
		const GameEngine::Math::Vector3 &i_collisionNormal );
	void OnLeave( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other );
};

#endif	// #ifndef _PLAYER_CONTROLLER_H_