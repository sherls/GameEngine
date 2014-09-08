/**
 ****************************************************************************************************
 * \file		EnemyController.h
 * \brief		The header of EnemyController class
 ****************************************************************************************************
*/

#ifndef _ENEMY_CONTROLLER_H_
#define _ENEMY_CONTROLLER_H_

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

class EnemyController : public GameEngine::EntityController
{
	typedef enum _e_enemy_state_
	{
		E_ENEMY_IDLE = 0,
		E_ENEMY_SEARCH_FLAG,
		E_ENEMY_SEARCH_GOAL,
		E_ENEMY_TAG_PLAYER,
		E_ENEMY_TAGGED,
		E_ENEMY_ESCAPE,
		E_ENEMY_MAX
	} E_ENEMY_STATE;

	static bool _bAwareOfPlayer;
	static E_ENEMY_STATE _enemyState;
	bool _bSprint;
	UINT32 _u32Power;
	UINT32 _u32NewTargetNode;
	E_ENEMY_STATE _prevState;

public:
	EnemyController( void ) :
		_bSprint( false ),
		_u32Power( 0 ),
		_u32NewTargetNode( Utilities::MAX_UINT32 )
	{
		ResetEnemyController();
		_prevState = _enemyState;
	}
	~EnemyController( void ) {}

	void BeginUpdate( GameEngine::Entity &i_entity ) {}
	void Update( GameEngine::Entity &i_entity );
	void EndUpdate( GameEngine::Entity &i_entity );

	static void ResetEnemyController( void );
};

class EnemyCollisionHandler : public GameEngine::Collision::CollisionHandler
{
public:
	EnemyCollisionHandler( void ) {}
	~EnemyCollisionHandler( void ) {}

	void HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity, Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other,
		const float i_collisionTime, const GameEngine::Math::Vector3 &i_collisionVector, const Utilities::StringHash &i_hashedTag );
};

class EnemyTriggerBoxHandler : public GameEngine::TriggerBox::TriggerBoxHandler
{
public:
	EnemyTriggerBoxHandler( void ) {}
	~EnemyTriggerBoxHandler( void ) {}

	void OnEnter( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime, \
		const GameEngine::Math::Vector3 &i_collisionNormal );
	void OnLeave( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other );
};

#endif