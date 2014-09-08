/*
 ****************************************************************************************************
 * \file		EnemyController.cpp
 * \brief		Implementation of non-inline functions of EnemyController class
 ****************************************************************************************************
*/

// Utilities header
#include <Time/Time.h>
#include <Debug/Debug.h>
#include <SmartPtr/SmartPtr.h>
#include <StringHash/StringHash.h>

// Game Engine header
#include "AI/AI.h"
#include "GameEngine.h"
#include "Audio/Audio.h"
#include "World/World.h"
#include "World/Entity.h"
#include "Camera/Camera.h"
#include "Renderer/Renderer.h"
#include "Collision/Collision.h"
#include "DebugMenu/DebugMenu.h"
#include "Messaging/Messaging.h"
#include "UserInput/UserInput.h"
#include "Math/Vector3/Vector3.h"
#include "Light/PointLight/PointLight.h"
#include "Utilities/IDCreator/IDCreator.h"

#include "EnemyController.h"
#include "../CaptureTheFlag.h"
#include "../GlobalConstant.h"
#include "../NetworkManager.h"

bool EnemyController::_bAwareOfPlayer = false;
EnemyController::E_ENEMY_STATE EnemyController::_enemyState;

/****************************************************************************************************
			Public functions implementation
****************************************************************************************************/
void EnemyController::ResetEnemyController( void )
{
	FUNCTION_START;

	_bAwareOfPlayer = false;
	_enemyState = E_ENEMY_IDLE;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( GameEngine::Entity &i_entity )
	\brief		Update the position of Enemy
	\param		i_entity entity to be updated
	\return		NONE
 ****************************************************************************************************
*/
void EnemyController::Update( GameEngine::Entity &i_entity )
{
	FUNCTION_START;

	_bSprint = false;

	switch( _enemyState )
	{
	case E_ENEMY_IDLE:
		if( g_captureTheFlag::Get().m_playerTeam == Utilities::StringHash("BlueFlag") )
			g_world::Get().UpdateAIDestinationTo( i_entity.m_u8AIEntityIndex, GlobalConstant::BLUE_FLAG_NODE_ID );
		else
			g_world::Get().UpdateAIDestinationTo( i_entity.m_u8AIEntityIndex, GlobalConstant::RED_FLAG_NODE_ID );
		_prevState = _enemyState;
		_enemyState = E_ENEMY_SEARCH_FLAG;
		break;

	case E_ENEMY_SEARCH_FLAG:
		if( g_captureTheFlag::Get().m_bEnemyHasFlag == true )
		{
			g_world::Get().UpdateAIDestinationTo( i_entity.m_u8AIEntityIndex, GlobalConstant::GOAL_NODE_ID );
			_prevState = _enemyState;
			_enemyState = E_ENEMY_SEARCH_GOAL;
		}
		break;

	case E_ENEMY_SEARCH_GOAL:
		if( g_captureTheFlag::Get().m_bEnemyHasFlag == false )
		{
			_prevState = _enemyState;
			_enemyState = E_ENEMY_IDLE;
		}
		break;

	case E_ENEMY_TAG_PLAYER:
		//if( !_bAwareOfPlayer )
		{
			UINT32 u32NodeID;
			Utilities::Pointer::SmartPtr<GameEngine::Entity> player = g_world::Get().GetEntityByName( "Player" );

			D3DXVECTOR3 playerPosition( player->m_v3Position.X(), player->m_v3Position.Y(), player->m_v3Position.Z() );
			GameEngine::AI::FindClosestNodeIDFromPosition( playerPosition, u32NodeID );

			// If player moved to other area
			if( u32NodeID != _u32NewTargetNode )
				g_world::Get().UpdateAIDestinationTo( i_entity.m_u8AIEntityIndex, _u32NewTargetNode );
		}
		_bSprint = true;
		break;

	case E_ENEMY_ESCAPE:
		_bSprint = true;
		break;
	}

	if( !_bSprint && (_u32Power < GlobalConstant::MAX_POWER) )
		_u32Power += 1;

	GameEngine::Renderer::DrawSlider( D3DXVECTOR2(850, 64), _u32Power, 0, GlobalConstant::MAX_POWER );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( GameEngine::Entity &i_entity )
	\brief		End update the position of Enemy
	\param		i_entity entity to be updated
	\return		NONE
 ****************************************************************************************************
*/
void EnemyController::EndUpdate( GameEngine::Entity &i_entity )
{
	float frameTime_ms = Utilities::Time::GetTimeElapsedThisFrame_ms();

	FUNCTION_START;

	Utilities::Pointer::SmartPtr<GameEngine::Entity> player = g_world::Get().GetEntityByName( "Player" );
	float distanceToEnemy = (player->m_v3ProjectedPosition - i_entity.m_v3ProjectedPosition).Length();
	if( distanceToEnemy < GlobalConstant::TAG_DISTANCE )
	{
		GameEngine::Messaging::ProcessMessage( "Tag", NULL );
		return;
	}

	// If player get the flag, decide whether to tag player or continue current destination
	D3DXVECTOR3 fromPosition( player->m_v3ProjectedPosition.X(), player->m_v3ProjectedPosition.Y(), player->m_v3ProjectedPosition.Z() );
	if( g_captureTheFlag::Get().m_bPlayerHasFlag && (_enemyState != E_ENEMY_TAG_PLAYER) )
	{
		UINT32 u32NodeID;

		GameEngine::AI::FindClosestNodeIDFromPosition( fromPosition, u32NodeID );
		//_bAwareOfPlayer = true;
		float distanceToCurrentDestination = 0.0f;

		// If enemy still looking for flag, absolutely tag player
		if( _enemyState == E_ENEMY_SEARCH_FLAG )
		{
			_u32NewTargetNode = u32NodeID;
			_prevState = _enemyState;
			_enemyState = E_ENEMY_TAG_PLAYER;
			return;
		}
		else if( _enemyState == E_ENEMY_SEARCH_GOAL )
		{
			fromPosition = D3DXVECTOR3( i_entity.m_v3ProjectedPosition.X(), i_entity.m_v3ProjectedPosition.Y(), i_entity.m_v3ProjectedPosition.Z() );
			distanceToCurrentDestination = GameEngine::AI::FindDistanceToNodeID( fromPosition, GlobalConstant::GOAL_NODE_ID );
		}
		// Do nothing on other state
		else
			return;

		float totalFrameToDestination = distanceToCurrentDestination / frameTime_ms;

		float distanceToPlayer = 0.0f;
		fromPosition = D3DXVECTOR3( i_entity.m_v3ProjectedPosition.X(), i_entity.m_v3ProjectedPosition.Y(), i_entity.m_v3ProjectedPosition.Z() );
		distanceToPlayer = GameEngine::AI::FindDistanceToNodeID( fromPosition, u32NodeID );

		UINT32 totalFrameToPlayer = 0;
		UINT32 u32AvailableSprintFrame = _u32Power / GlobalConstant::SPRINT_POWER_PER_FRAME;
		float distanceTravelledPerFrameOnSprint = frameTime_ms * GlobalConstant::DEFAULT_SPRINT_SPEED;
		float totalDistanceOnSprint = u32AvailableSprintFrame * distanceTravelledPerFrameOnSprint;

		if( distanceToPlayer - totalDistanceOnSprint < 0 )
		{
			totalFrameToPlayer = static_cast<UINT32>(distanceToPlayer / distanceTravelledPerFrameOnSprint);
		}
		else
		{
			distanceToPlayer -= totalDistanceOnSprint;
			totalFrameToPlayer = u32AvailableSprintFrame;
			totalFrameToPlayer += static_cast<UINT32>(distanceToPlayer / frameTime_ms);
		}

		if( totalFrameToPlayer < totalFrameToDestination )
		{
			_prevState = _enemyState;
			_u32NewTargetNode = u32NodeID;
			_enemyState = E_ENEMY_TAG_PLAYER;
		}
	}

	// Check if player is close
	GameEngine::Math::Vector3 distanceToPlayer = player->m_v3ProjectedPosition - i_entity.m_v3ProjectedPosition;
	float lengthToPlayer = distanceToPlayer.Length();
	if( (lengthToPlayer < GlobalConstant::MINIMUM_DISTANCE_TO_PLAYER) && (_enemyState != E_ENEMY_TAG_PLAYER) && (_enemyState != E_ENEMY_ESCAPE) )
	{
		_prevState = _enemyState;

		GameEngine::Math::Vector3 runDirection = player->m_v3Velocity - i_entity.m_v3Velocity;
		GameEngine::Math::Vector3 v3RunToPosition = runDirection * GlobalConstant::DEFAULT_SPRINT_SPEED * 2 * GlobalConstant::MINIMUM_DISTANCE_TO_PLAYER;
		D3DXVECTOR3 runToPosition( v3RunToPosition.X(), v3RunToPosition.Y(), v3RunToPosition.Z() );

		GameEngine::AI::FindClosestNodeIDFromPosition( runToPosition, _u32NewTargetNode );
		g_world::Get().UpdateAIDestinationTo( i_entity.m_u8AIEntityIndex, _u32NewTargetNode );
		_enemyState = E_ENEMY_ESCAPE;
	}
	else if( _enemyState == E_ENEMY_TAG_PLAYER )
	{
		_enemyState = _prevState;
		if( _enemyState == E_ENEMY_SEARCH_FLAG )
		{
			if( g_captureTheFlag::Get().m_playerTeam == Utilities::StringHash("BlueFlag") )
				g_world::Get().UpdateAIDestinationTo( i_entity.m_u8AIEntityIndex, GlobalConstant::BLUE_FLAG_NODE_ID );
			else
				g_world::Get().UpdateAIDestinationTo( i_entity.m_u8AIEntityIndex, GlobalConstant::RED_FLAG_NODE_ID );
		}
		else if( _enemyState == E_ENEMY_SEARCH_GOAL )
		{
			g_world::Get().UpdateAIDestinationTo( i_entity.m_u8AIEntityIndex, GlobalConstant::GOAL_NODE_ID );
		}
	}

	if( _bSprint && (_u32Power > GlobalConstant::SPRINT_POWER_PER_FRAME) )
	{
		i_entity.m_v3Velocity *= GlobalConstant::DEFAULT_SPRINT_SPEED;
		_u32Power -= GlobalConstant::SPRINT_POWER_PER_FRAME;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			EnemyCollisionHandler::HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity,
				Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime,
				const GameEngine::Math::Vector3 &i_collisionVector, const Utilities::StringHash &i_hashedTag )
	\brief		Collision handler of Enemy
	\param		i_entity current entity
	\param		i_other other entity which is collided with
	\param		i_collisionTime time of collision
	\param		i_collisionVector direction of collision response
	\return		NONE
 ****************************************************************************************************
*/
void EnemyCollisionHandler::HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity, \
	Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime, \
	const GameEngine::Math::Vector3 &i_collisionPoint, const Utilities::StringHash &i_hashedTag )
{
	FUNCTION_START;

	if( i_other->m_u8EntityID == g_IDCreator::Get().GetID("COL") )
	{
		if( Utilities::Math::AreRelativelyEqual(i_collisionTime, 1.0f) )
		{
			float newHeight = i_collisionPoint.Y() + i_entity->m_size.height;
			i_entity->m_v3ProjectedPosition.Y( newHeight );
		}
		else
		{
			i_entity->m_v3ProjectedPosition -= ((i_entity->m_v3Velocity) * 2.0f);
			i_entity->m_v3ProjectedPosition.Y( i_entity->m_v3ProjectedPosition.Y() - 20.0f );
		}
	}
	// Gravity
	else if( i_other->m_u8EntityID == i_entity->m_u8EntityID )
	{
		float newHeight = i_entity->m_v3ProjectedPosition.Y() - 20.0f;
		i_entity->m_v3ProjectedPosition.Y( newHeight );
	}
	i_entity->m_v3ProjectedVelocity = GameEngine::Math::Vector3::Zero;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void OnLeave( GameEngine::Pointer::SmartPtr<GameEngine::Entity> &i_other )
	\brief		Trigger box handler when leaving collision
	\param		i_other other entity
	\return		NONE
 ****************************************************************************************************
*/
void EnemyTriggerBoxHandler::OnEnter( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime, \
	const GameEngine::Math::Vector3 &i_v3IntersectionNormal )
{
	FUNCTION_START;

	if( i_other->m_u8EntityID == g_captureTheFlag::Get().m_u8PlayerAreaID )
	{
		if( !g_captureTheFlag::Get().m_bEnemyHasFlag )
		{
			g_captureTheFlag::Get().m_bEnemyHasFlag = true;

			Utilities::Pointer::SmartPtr<GameEngine::Entity> blueFlag = g_world::Get().GetEntityByName( g_captureTheFlag::Get().m_playerTeam );
			Utilities::Pointer::SmartPtr<GameEngine::Entity> enemy = g_world::Get().GetEntityByName( "Enemy" );
			if( blueFlag && enemy )
				blueFlag->m_v3Position = enemy->m_v3Position - i_other->m_v3Position;

			GameEngine::Audio::Play2DSoundEffect( g_captureTheFlag::Get().m_u32EnemyPickUpSfxID, "enemyPickFlag.wav" );
			g_networkManager::Get().SendFlagEvent( false );
		}
	}
	else if( i_other->m_u8EntityID == g_IDCreator::Get().GetID("GoalArea") )
	{
		GameEngine::Messaging::ProcessMessage( "EnemyScore", NULL );
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void OnLeave( GameEngine::Pointer::SmartPtr<GameEngine::Entity> &i_other )
	\brief		Trigger box handler when leaving collision
	\param		i_other other entity
	\return		NONE
 ****************************************************************************************************
*/
void EnemyTriggerBoxHandler::OnLeave( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private functions implementation
****************************************************************************************************/
#if 0
/**
 ****************************************************************************************************
	\fn			void RotateEnemyView( GameEngine::Entity &i_entity, const float &i_yaw, const float &i_pitch )
	\brief		Rotate Enemy view direction
	\param		i_entity the entity to be controlled
	\param		i_yaw yaw angle
	\param		i_pitch pitch angle
	\return		NONE
 ****************************************************************************************************
*/
void EnemyController::RotateEnemyView( GameEngine::Entity &i_entity, const float &i_yaw, const float &i_pitch )
{
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX yawMatrix, pitchMatrix, rollMatrix;
	D3DXVECTOR3 rightVector;
	D3DXVECTOR3 position;
	D3DXVECTOR3 upDirection = GameEngine::D3DXVECTOR3_UP;

	FUNCTION_START;

	position.x = i_entity.m_v3ProjectedPosition.X();
	position.y = i_entity.m_v3ProjectedPosition.Y();
	position.z = i_entity.m_v3ProjectedPosition.Z();

	D3DXVec3Cross( &rightVector, &GameEngine::D3DXVECTOR3_UP, &i_entity.m_vLookAt );
	rightVector.y = 0.0f;
	D3DXVec3Normalize( &rightVector, &rightVector );

	D3DXMatrixRotationAxis( &pitchMatrix, &rightVector, D3DXToRadian(i_pitch) );
	D3DXMatrixRotationAxis( &yawMatrix, &upDirection, D3DXToRadian(i_yaw) );
	D3DXMatrixRotationAxis( &rollMatrix, &i_entity.m_vLookAt, D3DXToRadian(0.0f) );

	D3DXMatrixMultiply( &rotationMatrix, &yawMatrix, &pitchMatrix );
	D3DXMatrixMultiply( &rotationMatrix, &rollMatrix, &rotationMatrix );

	D3DXVec3TransformCoord( &rightVector, &rightVector, &rotationMatrix );
	D3DXVec3TransformCoord( &upDirection, &upDirection, &rotationMatrix );
	D3DXVec3Cross( &i_entity.m_vLookAt, &rightVector, &upDirection );

	if( fabs(D3DXVec3Dot(&upDirection, &rightVector)) > 0.01f )
	{
		D3DXVec3Cross( &upDirection, &i_entity.m_vLookAt, &rightVector );
	}

	D3DXVec3Normalize( &rightVector, &rightVector );
	D3DXVec3Normalize( &upDirection, &upDirection );
	D3DXVec3Normalize( &i_entity.m_vLookAt, &i_entity.m_vLookAt );

	FUNCTION_FINISH;
}
#endif