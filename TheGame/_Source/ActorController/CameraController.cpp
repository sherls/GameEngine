/*
 ****************************************************************************************************
 * \file		CameraController.cpp
 * \brief		Implementation of non-inline functions of CameraController class
 ****************************************************************************************************
*/

// Utilities header
#include <Debug/Debug.h>

// Game engine header
#include "World/World.h"
#include "World/Entity.h"
#include "Camera/Camera.h"
#include "Utilities/IDCreator/IDCreator.h"

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
void CameraController::Update( GameEngine::Entity &i_entity )
{
	FUNCTION_START;

	i_entity.m_v3ProjectedPosition = m_followEntity->m_v3Position;
	i_entity.m_v3ProjectedPosition -= g_world::Get().m_camera->m_viewDirection * g_world::Get().m_camera->m_backDistance;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			CameraCollisionHandler::HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity,
				Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other, const float i_collisionTime, const GameEngine::Math::Vector3 &i_collisionVector )
	\brief		Collision handler of camera
	\param		i_entity current entity
	\param		i_other other entity which is collided with
	\param		i_collisionTime time of collision
	\param		i_collisionVector direction of collision response
	\return		NONE
 ****************************************************************************************************
*/
void CameraCollisionHandler::HandleCollision( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity, Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_other,
	const float i_collisionTime, const GameEngine::Math::Vector3 &i_collisionPoint )
{
	FUNCTION_START;

	if( i_other->m_u8EntityID == g_IDCreator::Get().GetID("COL") )
	{
		i_entity->m_v3ProjectedPosition = i_entity->m_v3Position;
	}

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private functions implementation
****************************************************************************************************/
