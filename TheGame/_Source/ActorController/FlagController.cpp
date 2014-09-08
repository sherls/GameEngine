/*
 ****************************************************************************************************
 * \file		FlagController.cpp
 * \brief		Implementation of non-inline functions of FlagController class
 ****************************************************************************************************
*/

// Utilities header
#include <Time/Time.h>
#include <Debug/Debug.h>

// Game engine header
#include "World/World.h"
#include "World/Entity.h"

#include "FlagController.h"

void FlagController::EndUpdate( GameEngine::Entity &i_entity )
{
	FUNCTION_START;

	if( _bPlayerHasThis )
	{
		Utilities::Pointer::SmartPtr<GameEngine::Entity> player = g_world::Get().GetEntityByName( _playerName );
		Utilities::Pointer::SmartPtr<GameEngine::Entity> triggerBox = g_world::Get().GetEntityByName( _flagAreaName );
		i_entity.m_v3Position = player->m_v3Position - triggerBox->m_v3Position;
	}

	FUNCTION_FINISH;
}
