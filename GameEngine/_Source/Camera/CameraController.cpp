/*
 ****************************************************************************************************
 * \file		CameraController.cpp
 * \brief		Implementation of non-inline functions of CameraController class
 ****************************************************************************************************
*/

// Utilities header
#include <Time/Time.h>
#include <Debug/Debug.h>
#include <UserInput/UserInput.h>

// Game engine header
#include "../GameEngine.h"
#include "../World/World.h"
#include "../World/Entity.h"
#include "../Utilities/IDCreator/IDCreator.h"

#include "Camera.h"
#include "CameraController.h"

/****************************************************************************************************
			Public functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void Update( GameEngine::Entity &i_entity )
	\brief		Update the position of camera
	\param		i_entity entity to be updated
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::CameraController::Update( GameEngine::Entity &i_entity )
{
	FUNCTION_START;

#ifdef _DEBUG
	if( g_world::Get().m_camera->m_bOnDebugCamera && IsDebugMenuActivated() )
	{
		// Reset camera position to player position
		g_world::Get().m_camera->_position.x = m_followEntity->m_v3Position.X();
		g_world::Get().m_camera->_position.y = m_followEntity->m_v3Position.Y();
		g_world::Get().m_camera->_position.z = m_followEntity->m_v3Position.Z();
		return;
	}
	else if( g_world::Get().m_camera->m_bOnDebugCamera && !IsDebugMenuActivated() )
	{
		D3DXVECTOR3 direction;
		const float speed = Utilities::Time::GetTimeElapsedThisFrame_ms() * 0.2f;

		if( GameEngine::UserInput::IsKeyPressed('W') || GameEngine::UserInput::IsKeyPressed('w') )
		{
			g_world::Get().m_camera->m_pitchChange -= speed;
			g_world::Get().m_camera->m_bAngleChanged = true;
			g_world::Get().m_camera->m_bPitchChanged = true;
		}
		else if( GameEngine::UserInput::IsKeyPressed('S') || GameEngine::UserInput::IsKeyPressed('s') )
		{
			g_world::Get().m_camera->m_pitchChange += speed;
			g_world::Get().m_camera->m_bAngleChanged = true;
			g_world::Get().m_camera->m_bPitchChanged = true;
		}

		if( GameEngine::UserInput::IsKeyPressed('A') || GameEngine::UserInput::IsKeyPressed('a') )
		{
			g_world::Get().m_camera->m_yawChange -= speed;
			g_world::Get().m_camera->m_bAngleChanged = true;
			g_world::Get().m_camera->m_bYawChanged = true;
		}
		else if( GameEngine::UserInput::IsKeyPressed('D') || GameEngine::UserInput::IsKeyPressed('d') )
		{
			g_world::Get().m_camera->m_yawChange += speed;
			g_world::Get().m_camera->m_bAngleChanged = true;
			g_world::Get().m_camera->m_bYawChanged = true;
		}

		// Get the direction
		if( UserInput::IsKeyPressed(VK_RIGHT) )
		{
			D3DXVec3Cross( &direction, &i_entity.m_vLookAt, &g_world::Get().m_camera->m_upVector );
			D3DXVec3Normalize( &direction, &direction );

			g_world::Get().m_camera->_position -= direction * speed;
		}
		else if( UserInput::IsKeyPressed(VK_LEFT) )
		{
			D3DXVec3Cross( &direction, &i_entity.m_vLookAt, &g_world::Get().m_camera->m_upVector );
			D3DXVec3Normalize( &direction, &direction );

			g_world::Get().m_camera->_position += direction * speed;
		}

		if( UserInput::IsKeyPressed(VK_UP) )
			g_world::Get().m_camera->_position += i_entity.m_vLookAt * speed;
		else if( UserInput::IsKeyPressed(VK_DOWN) )
			g_world::Get().m_camera->_position -= i_entity.m_vLookAt * speed;

		return;
	}
#endif	// #ifdef _DEBUG

	if( !Utilities::Math::AreRelativelyEqual( g_world::Get().m_camera->m_yawChange, 0.0f) || \
		!Utilities::Math::AreRelativelyEqual( g_world::Get().m_camera->m_pitchChange, 0.0f) || \
		!Utilities::Math::AreRelativelyEqual( g_world::Get().m_camera->m_rollChange, 0.0f)
		)
	{
		if( !Utilities::Math::AreRelativelyEqual( g_world::Get().m_camera->m_yawChange, 0.0f) )
			g_world::Get().m_camera->m_bYawChanged = true;
		if( !Utilities::Math::AreRelativelyEqual( g_world::Get().m_camera->m_pitchChange, 0.0f) )
			g_world::Get().m_camera->m_bPitchChanged = true;
		if( !Utilities::Math::AreRelativelyEqual( g_world::Get().m_camera->m_rollChange, 0.0f) )
			g_world::Get().m_camera->m_bRollChanged = true;

		g_world::Get().m_camera->m_bAngleChanged = true;
		g_world::Get().m_camera->Update();
		g_world::Get().m_camera->m_bAngleChanged = false;
	}

	// Check whether player moving side-way
// 	if( m_followEntity->m_v3Velocity != Math::Vector3::Zero )
// 	{
// 		D3DXVECTOR3 cameraRight;
// 		D3DXVec3Cross( &cameraRight, &g_world::Get().m_camera->GetViewDirection(), &g_world::Get().m_camera->m_upVector );
// 		D3DXVec3Normalize( &cameraRight, &cameraRight );
// 		D3DXVECTOR3 playerVelocity( m_followEntity->m_v3Velocity.X(), m_followEntity->m_v3Velocity.Y(), m_followEntity->m_v3Velocity.Z() );
// 		D3DXVec3Normalize( &playerVelocity, &playerVelocity );
// 		D3DXVECTOR3 crossProduct;
// 		D3DXVec3Cross( &crossProduct, &cameraRight, &playerVelocity );
// 		if( Utilities::Math::AreRelativelyEqual(crossProduct.x, 0.0f)
// 			&& Utilities::Math::AreRelativelyEqual(crossProduct.y, 0.0f)
// 			&& Utilities::Math::AreRelativelyEqual(crossProduct.z, 0.0f)
// 			)
// 		{
// 			float speed = Utilities::Time::GetTimeElapsedThisFrame_ms() * 0.2f;
// 			if( Utilities::Math::AreRelativelyEqual(cameraRight.x, playerVelocity.x)
// 				&& Utilities::Math::AreRelativelyEqual(cameraRight.y, playerVelocity.y)
// 				&& Utilities::Math::AreRelativelyEqual(cameraRight.z, playerVelocity.z)
// 				)
// 				speed = -speed;
// 
// 			g_world::Get().m_camera->m_yawChange = speed;
// 			g_world::Get().m_camera->m_bAngleChanged = true;
// 			g_world::Get().m_camera->Update();
// 			g_world::Get().m_camera->m_bAngleChanged = false;
// 		}
// 	}

	i_entity.m_v3Position = m_followEntity->m_v3Position;
	i_entity.m_v3Position -= i_entity.m_vLookAt * g_world::Get().m_camera->m_backDistance;
	i_entity.m_v3ProjectedPosition = m_followEntity->m_v3Position;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			CameraCollisionHandler::HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity,
				Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime
				const GameEngine::Math::Vector3 &i_collisionVector, const Utilities::StringHash &i_hashedTag )
	\brief		Collision handler of camera
	\param		i_entity current entity
	\param		i_other other entity which is collided with
	\param		i_collisionTime time of collision
	\param		i_collisionVector direction of collision response
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::CameraCollisionHandler::HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity, \
	Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime, const GameEngine::Math::Vector3 &i_collisionPoint, \
	const Utilities::StringHash &i_hashedTag )
{
	FUNCTION_START;

	if( i_other->m_u8EntityID == g_IDCreator::Get().GetID("COL") )
	{
		if( g_world::Get().m_camera->m_bYawChanged )
		{
			g_world::Get().m_camera->m_yawChange = -g_world::Get().m_camera->m_yawChange;
			g_world::Get().m_camera->m_bAngleChanged = true;
			g_world::Get().m_camera->Update();
			g_world::Get().m_camera->m_bAngleChanged = false;
		}
		else if( g_world::Get().m_camera->m_bPitchChanged )
		{
			g_world::Get().m_camera->m_pitchChange = -g_world::Get().m_camera->m_pitchChange;
			g_world::Get().m_camera->m_bAngleChanged = true;
			g_world::Get().m_camera->Update();
			g_world::Get().m_camera->m_bAngleChanged = false;
		}
		else if( g_world::Get().m_camera->m_bRollChanged )
		{
			g_world::Get().m_camera->m_rollChange = -g_world::Get().m_camera->m_rollChange;
			g_world::Get().m_camera->m_bAngleChanged = true;
			g_world::Get().m_camera->Update();
			g_world::Get().m_camera->m_bAngleChanged = false;
		}
		else
		{
			// First collision resolution try
			if( _u8Ctr == 0 )
			{
				_speed = Utilities::Time::GetTimeElapsedThisFrame_ms() * 0.1f;
				_originalYaw = g_world::Get().m_camera->m_yawChange;
				_currYaw = 0.0f;
			}

			if( (_u8Ctr % 2) == 0 )
				_currYaw = (_u8Ctr + 1) * _speed;
			else
				_currYaw = -(_u8Ctr + 1) * _speed;

			g_world::Get().m_camera->m_yawChange = _currYaw;
			g_world::Get().m_camera->m_bAngleChanged = true;
			g_world::Get().m_camera->Update();
			g_world::Get().m_camera->m_bAngleChanged = false;
			_u8Ctr++;
		}
		i_entity->m_v3Position = i_entity->m_v3ProjectedPosition;
		i_entity->m_v3Position -= i_entity->m_vLookAt * g_world::Get().m_camera->m_backDistance;
		i_entity->m_v3ProjectedPosition = i_entity->m_v3Position;
	}

	FUNCTION_FINISH;
}


/**
 ****************************************************************************************************
	\fn			CameraCollisionHandler::OnLeavingCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity,
				Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other )
	\brief		Collision handler of camera
	\param		i_entity current entity
	\param		i_other other entity which was collided with
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::CameraCollisionHandler::OnLeavingCollision( Utilities::Pointer::SmartPtr<Entity> &i_entity, \
	Utilities::Pointer::SmartPtr<Entity> &i_other )
{
	FUNCTION_START;

	float yawAngleChange = 0.0f;

	if( _u8Ctr == 0 )
		yawAngleChange = _currYaw;
	else
	{
		if( (_u8Ctr % 2) == 0 )
			yawAngleChange = _currYaw - (_u8Ctr/2) * _speed;
		else
			yawAngleChange = _currYaw + (_u8Ctr/2) * _speed;
	}

	if( (yawAngleChange - _originalYaw) > 5.0f )
	{
		g_world::Get().m_camera->m_bAngleChanged = true;

		// Return to original yaw
		g_world::Get().m_camera->m_yawChange = -_currYaw;
		g_world::Get().m_camera->Update();

		g_world::Get().m_camera->m_yawChange = 5.0f;
		g_world::Get().m_camera->Update();
		g_world::Get().m_camera->m_bAngleChanged = false;
	}

	_u8Ctr = 0;

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private functions implementation
****************************************************************************************************/
