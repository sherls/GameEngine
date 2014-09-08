/**
 ****************************************************************************************************
 * \file		GlobalConstant.h
 * \brief		Global constant declaration
 ****************************************************************************************************
*/

#ifndef _GLOBAL_CONSTANT_H_
#define _GLOBAL_CONSTANT_H_

#include "Utilities/GameEngineTypes.h"

#define DEFAULT_CHARACTER_SIZE		30
#define MAX_MONSTER_NAME_LEN		20

namespace GlobalConstant
{
	const float VIEW_DISTANCE = 1000.0f;
	const float DEFAULT_FRICTION = 0.002f;
	const UINT32 MAX_POWER = 1000;
	const UINT32 SPRINT_POWER_PER_FRAME = 10;

	const float DEFAULT_SPRINT_SPEED = 4.0f;
	const float MINIMUM_DISTANCE_TO_PLAYER = 200.0f;
	const float TAG_DISTANCE = 50.0f;

	const UINT32 GOAL_NODE_ID = 22;
	const UINT32 RED_FLAG_NODE_ID = 14;
	const UINT32 BLUE_FLAG_NODE_ID = 0;
}

#endif	// #ifndef _GLOBAL_CONSTANT_H_