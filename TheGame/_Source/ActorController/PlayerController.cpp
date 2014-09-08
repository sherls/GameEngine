/*
 ****************************************************************************************************
 * \file		PlayerController.cpp
 * \brief		Implementation of non-inline functions of PlayerController class
 ****************************************************************************************************
*/

// Utilities header
#include <Time/Time.h>
#include <Debug/Debug.h>
#include <SmartPtr/SmartPtr.h>
#include <StringHash/StringHash.h>

// Game engine header
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

#include "PlayerController.h"
#include "../CaptureTheFlag.h"
#include "../GlobalConstant.h"
#include "../NetworkManager.h"

static bool bQuit = false;

/****************************************************************************************************
			Public functions implementation
****************************************************************************************************/
PlayerController::PlayerController( void ) :
	_pitchAngle( 0.0f ),
	_u32Power( 0 )
{
}

/**
 ****************************************************************************************************
	\fn			void Update( GameEngine::Entity &i_entity )
	\brief		Update the position of player
	\param		i_entity entity to be updated
	\return		NONE
 ****************************************************************************************************
*/
void PlayerController::Update( GameEngine::Entity &i_entity )
{
	D3DXVECTOR3 direction;
	float speed = Utilities::Time::GetTimeElapsedThisFrame_ms() * 0.2f;

	FUNCTION_START;

	if( !GameEngine::IsDebugMenuActivated() )
	{
		i_entity.m_v3Velocity = GameEngine::Math::Vector3::Zero;

		// Disable movement while on debug camera
		if( GameEngine::UserInput::IsKeyUp(VK_SNAPSHOT) )
		{
			GameEngine::SetDebugMenu( true );

			FUNCTION_FINISH;
			return;
		}

		// Do not do anything if in debug camera
		if( GameEngine::Camera::m_bOnDebugCamera )
		{
			FUNCTION_FINISH;
			return;
		}

		// Camera control
		// {
		if( GameEngine::UserInput::IsKeyPressed('W') || GameEngine::UserInput::IsKeyPressed('w') )
			g_world::Get().m_camera->m_pitchChange -= speed;
		else if( GameEngine::UserInput::IsKeyPressed('S') || GameEngine::UserInput::IsKeyPressed('s') )
			g_world::Get().m_camera->m_pitchChange += speed;

		if( GameEngine::UserInput::IsKeyPressed('A') || GameEngine::UserInput::IsKeyPressed('a') )
			g_world::Get().m_camera->m_yawChange -= speed;
		else if( GameEngine::UserInput::IsKeyPressed('D') || GameEngine::UserInput::IsKeyPressed('d') )
			g_world::Get().m_camera->m_yawChange += speed;
		// }

		// Player direction control
		// {
		float yawAngleChange = 0.0f;
		bool bUpdatePlayerView = false;
		if( GameEngine::UserInput::IsKeyPressed('I') || GameEngine::UserInput::IsKeyPressed('i') )
		{
			_pitchAngle -= speed;
			bUpdatePlayerView = true;
		}
		else if( GameEngine::UserInput::IsKeyPressed('K') || GameEngine::UserInput::IsKeyPressed('k') )
		{
			_pitchAngle += speed;
			bUpdatePlayerView = true;
		}
		else
			_pitchAngle = 0.0f;

		if( GameEngine::UserInput::IsKeyPressed('J') || GameEngine::UserInput::IsKeyPressed('j')
			|| GameEngine::UserInput::IsKeyPressed(VK_LEFT)
			)
		{
			yawAngleChange = -speed;
			bUpdatePlayerView = true;
		}
		else if( GameEngine::UserInput::IsKeyPressed('L') || GameEngine::UserInput::IsKeyPressed('l')
			|| GameEngine::UserInput::IsKeyPressed(VK_RIGHT)
			)
		{
			yawAngleChange = speed;
			bUpdatePlayerView = true;
		}
		// }

		// Sprint
		// {
		bool bSprint = false;
		if( GameEngine::UserInput::IsKeyPressed(VK_SHIFT) && (_u32Power >= GlobalConstant::SPRINT_POWER_PER_FRAME) )
		{
			bSprint = true;
			_u32Power -= GlobalConstant::SPRINT_POWER_PER_FRAME;
		}
		else
		{
			if( _u32Power < GlobalConstant::MAX_POWER )
				_u32Power += 1;
		}
		// }

		// Arrows movement
		// {
		if( GameEngine::UserInput::IsKeyPressed(VK_RIGHT) )
		{
			D3DXVec3Cross( &direction, &g_world::Get().m_camera->GetViewDirection(), &g_world::Get().m_camera->m_upVector );
			D3DXVec3Normalize( &direction, &direction );

			i_entity.m_v3Velocity = -direction;
		}
		else if( GameEngine::UserInput::IsKeyPressed(VK_LEFT) )
		{
			D3DXVec3Cross( &direction, &g_world::Get().m_camera->GetViewDirection(), &g_world::Get().m_camera->m_upVector );
			D3DXVec3Normalize( &direction, &direction );

			i_entity.m_v3Velocity = direction;
		}

		if( bUpdatePlayerView )
		{
			RotatePlayerView( i_entity, yawAngleChange, _pitchAngle );
			i_entity.m_orientation += yawAngleChange;
		}

		bool bPlayerViewMustBeUpdated = false;
		if( GameEngine::UserInput::IsKeyPressed(VK_UP) )
		{
			bPlayerViewMustBeUpdated = true;
			i_entity.m_v3Velocity = g_world::Get().m_camera->GetViewDirection();
		}
		else if( GameEngine::UserInput::IsKeyPressed(VK_DOWN) )
		{
			bPlayerViewMustBeUpdated = true;
			i_entity.m_v3Velocity = -g_world::Get().m_camera->GetViewDirection();
		}
		// }

		if( bPlayerViewMustBeUpdated )
		{
			D3DXVECTOR3 viewDifference;
			D3DXVECTOR3 playerLookAt = i_entity.m_vLookAt;
			D3DXVec3Normalize( &playerLookAt, &playerLookAt );
			D3DXVECTOR3 cameraLookAt = g_world::Get().m_camera->GetViewDirection();
			D3DXVec3Normalize( &cameraLookAt, &cameraLookAt );
			D3DXVec3Cross( &viewDifference, &playerLookAt, &cameraLookAt );
			bool xCloseEnough = ((cameraLookAt.x - playerLookAt.x) < 0.1f) && ((cameraLookAt.x - playerLookAt.x) > -0.1f);
			bool yCloseEnough = ((cameraLookAt.y - playerLookAt.y) < 0.1f) && ((cameraLookAt.y - playerLookAt.y) > -0.1f);
			bool zCloseEnough = ((cameraLookAt.z - playerLookAt.z) < 0.1f) && ((cameraLookAt.z - playerLookAt.z) > -0.1f);;
			if( (!Utilities::Math::AreRelativelyEqual(viewDifference.x, 0.0f)
				|| !Utilities::Math::AreRelativelyEqual(viewDifference.y, 0.0f)
				|| !Utilities::Math::AreRelativelyEqual(viewDifference.z, 0.0f))
				&& !(xCloseEnough && yCloseEnough && zCloseEnough)
				)
			{
				if( viewDifference.y > 0 )
					yawAngleChange = speed;
				else
					yawAngleChange = -speed;
				RotatePlayerView( i_entity, yawAngleChange, _pitchAngle );
				i_entity.m_orientation += yawAngleChange;
			}
		}

		if( bSprint )
		{
			i_entity.m_v3Velocity *= GlobalConstant::DEFAULT_SPRINT_SPEED;
			GameEngine::Audio::Play3DSoundEffect( g_captureTheFlag::Get().m_u32SprintSfxID, "sprint.wav", i_entity.m_v3Position );
		}
		else
		{
			GameEngine::Audio::Stop3DSoundEffect( g_captureTheFlag::Get().m_u32SprintSfxID );
		}

		if( GameEngine::UserInput::IsKeyPressed(VK_ESCAPE) )
			bQuit = true;

		GameEngine::Renderer::DrawSlider( D3DXVECTOR2(16, 64), _u32Power, 0, GlobalConstant::MAX_POWER );
	}
	else
	{
		if( GameEngine::UserInput::IsKeyUp(VK_SNAPSHOT) )
			GameEngine::SetDebugMenu( false );
	}

	D3DXVECTOR3 startPosition( i_entity.m_v3Position.X(), i_entity.m_v3Position.Y(), i_entity.m_v3Position.Z() );
	D3DXVECTOR3 endPosition = startPosition + i_entity.m_vLookAt * 10;
	g_debugMenu::Get().DrawLine( startPosition, endPosition, Utilities::BLUE );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( GameEngine::Entity &i_entity )
	\brief		End updating the position of player
	\param		i_entity entity to be updated
	\return		NONE
 ****************************************************************************************************
*/
void PlayerController::EndUpdate( GameEngine::Entity &i_entity )
{
	FUNCTION_START;

	Utilities::Pointer::SmartPtr<GameEngine::Entity> enemy = g_world::Get().GetEntityByName( "Enemy" );
	float distanceToEnemy = (enemy->m_v3ProjectedPosition - i_entity.m_v3ProjectedPosition).Length();
	if( distanceToEnemy < GlobalConstant::TAG_DISTANCE )
	{
		GameEngine::Messaging::ProcessMessage( "Tag", NULL );
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool IsPlayerQuit( void )
	\brief		Check whether player pressed [Quit] button
	\param		NONE
	\return		boolean
	\retval		true [Quit] button is pressed
	\retval		false otherwise
 ****************************************************************************************************
*/
bool PlayerController::IsPlayerQuit( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
	return bQuit;
}

/**
 ****************************************************************************************************
	\fn			PlayerCollisionHandler::HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity,
				Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime,
				const GameEngine::Math::Vector3 &i_collisionVector, const Utilities::StringHash &i_hashedTag )
	\brief		Collision handler of player
	\param		i_entity current entity
	\param		i_other other entity which is collided with
	\param		i_collisionTime time of collision
	\param		i_collisionVector direction of collision response
	\return		NONE
 ****************************************************************************************************
*/
void PlayerCollisionHandler::HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity, \
	Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime, \
	const GameEngine::Math::Vector3 &i_collisionPoint, const Utilities::StringHash &i_hashedTag )
{
	FUNCTION_START;

	if( i_other->m_u8EntityID == g_IDCreator::Get().GetID("COL") )
	{
		if( Utilities::Math::AreRelativelyEqual(i_collisionTime, 1.0f) )
		{
			float newHeight = i_collisionPoint.Y() + i_entity->m_size.height;
			_groundPos = i_entity->m_v3ProjectedPosition;
			_groundPos.Y( i_collisionPoint.Y() );
			i_entity->m_v3ProjectedPosition.Y( newHeight );
			if( i_hashedTag == Utilities::StringHash("Floor") )
			{
				GameEngine::Audio::Stop3DSoundEffect( g_captureTheFlag::Get().m_u32StairSfxID );
				GameEngine::Audio::Play3DSoundEffect( g_captureTheFlag::Get().m_u32GroundSfxID, "ground.wav", _groundPos );
			}
			else if( i_hashedTag == Utilities::StringHash("Stair") )
			{
				GameEngine::Audio::Stop3DSoundEffect( g_captureTheFlag::Get().m_u32GroundSfxID );
				GameEngine::Audio::Play3DSoundEffect( g_captureTheFlag::Get().m_u32StairSfxID, "stairs.wav", _groundPos );
			}
		}
		else
		{
			i_entity->m_v3ProjectedPosition = i_entity->m_v3Position;
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
void PlayerTriggerBoxHandler::OnEnter( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime, \
	const GameEngine::Math::Vector3 &i_v3IntersectionNormal )
{
	FUNCTION_START;

	if( i_other->m_u8EntityID == g_captureTheFlag::Get().m_u8EnemyAreaID )
	{
		if( !g_captureTheFlag::Get().m_bPlayerHasFlag )
		{
			g_captureTheFlag::Get().m_bPlayerHasFlag = true;

			Utilities::Pointer::SmartPtr<GameEngine::Entity> redFlag = g_world::Get().GetEntityByName( g_captureTheFlag::Get().m_enemyTeam );
			Utilities::Pointer::SmartPtr<GameEngine::Entity> player = g_world::Get().GetEntityByName( "Player" );
			if( redFlag && player )
				redFlag->m_v3Position = player->m_v3Position - i_other->m_v3Position;

			GameEngine::Audio::Play2DSoundEffect( g_captureTheFlag::Get().m_u32PlayerPickUpSfxID, "playerPickFlag.wav" );
			g_networkManager::Get().SendFlagEvent( true );
		}
	}
	else if( i_other->m_u8EntityID == g_IDCreator::Get().GetID("GoalArea") )
	{
		GameEngine::Messaging::ProcessMessage( "PlayerScore", NULL );
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
void PlayerTriggerBoxHandler::OnLeave( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void RotatePlayerView( GameEngine::Entity &i_entity, const float &i_yaw, const float &i_pitch )
	\brief		Rotate player view direction
	\param		i_entity the entity to be controlled
	\param		i_yaw yaw angle
	\param		i_pitch pitch angle
	\return		NONE
 ****************************************************************************************************
*/
void PlayerController::RotatePlayerView( GameEngine::Entity &i_entity, const float &i_yaw, const float &i_pitch )
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