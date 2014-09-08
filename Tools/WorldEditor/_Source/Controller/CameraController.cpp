/*
 ****************************************************************************************************
 * \file		CameraController.cpp
 * \brief		Implementation of non-inline functions of CameraController class
 ****************************************************************************************************
*/

#include "../GamePanel/GamePanel.h"

// Utilities header
#include <Time/Time.h>

// GameEngine header
#include <World/World.h>
#include <World/Entity.h>
#include <Camera/Camera.h>
#include <UserInput/UserInput.h>

#include "CameraController.h"

/****************************************************************************************************
			Public functions implementation
****************************************************************************************************/
void CameraController::Update( GameEngine::Entity &i_entity )
{
	bool bPlayerMove = false;
	D3DXVECTOR3 direction;
	float speed = Utilities::Time::GetTimeElapsedThisFrame_ms() * 0.2f;

	if( GameEngine::UserInput::IsKeyUp(VK_NUMLOCK) )
		GamePanel::m_bEnableCameraControl = !GamePanel::m_bEnableCameraControl;

	if( GameEngine::UserInput::IsKeyPressed(VK_CONTROL) )
		GamePanel::m_bAddingLink = true;
	else
		GamePanel::m_bAddingLink = false;

	if( !GamePanel::m_bEnableCameraControl )
		return;

	if( GameEngine::UserInput::IsKeyPressed('W') || GameEngine::UserInput::IsKeyPressed('w') )
		g_world::Get().m_camera->m_pitchChange -= speed;
	else if( GameEngine::UserInput::IsKeyPressed('S') || GameEngine::UserInput::IsKeyPressed('s') )
		g_world::Get().m_camera->m_pitchChange += speed;

	if( GameEngine::UserInput::IsKeyPressed('A') || GameEngine::UserInput::IsKeyPressed('a') )
		g_world::Get().m_camera->m_yawChange -= speed;
	else if( GameEngine::UserInput::IsKeyPressed('D') || GameEngine::UserInput::IsKeyPressed('d') )
		g_world::Get().m_camera->m_yawChange += speed;

	if( GameEngine::UserInput::IsKeyPressed(VK_RIGHT) )
	{
		bPlayerMove = true;
		D3DXVec3Cross( &direction, &g_world::Get().m_camera->GetViewDirection(), &g_world::Get().m_camera->m_upVector );
		//D3DXVec3Cross( &direction, &i_entity.m_vLookAt, &GameEngine::D3DXVECTOR3_UP );
		D3DXVec3Normalize( &direction, &direction );

		i_entity.m_v3Velocity = -direction;
	}
	else if( GameEngine::UserInput::IsKeyPressed(VK_LEFT) )
	{
		bPlayerMove = true;
		D3DXVec3Cross( &direction, &g_world::Get().m_camera->GetViewDirection(), &g_world::Get().m_camera->m_upVector );
		//D3DXVec3Cross( &direction, &i_entity.m_vLookAt, &GameEngine::D3DXVECTOR3_UP );
		D3DXVec3Normalize( &direction, &direction );

		i_entity.m_v3Velocity = direction;
	}

	if( GameEngine::UserInput::IsKeyPressed(VK_UP) )
	{
		bPlayerMove = true;
		i_entity.m_v3Velocity = g_world::Get().m_camera->GetViewDirection();
	}
	else if( GameEngine::UserInput::IsKeyPressed(VK_DOWN) )
	{
		bPlayerMove = true;
		i_entity.m_v3Velocity = -g_world::Get().m_camera->GetViewDirection();
	}

	if( !bPlayerMove )
	{
		i_entity.m_v3Velocity = GameEngine::Math::Vector3::Zero;
	}
}
