/**
 ****************************************************************************************************
 * \file		Types.h
 * \brief		Custom types
 ****************************************************************************************************
*/

#ifndef _GAME_ENGINE_TYPES_H_
#define _GAME_ENGINE_TYPES_H_

// Utilities header
#include <UtilitiesTypes.h>

typedef void (*MessageHandler) ( void *i_messageData );
typedef void (*ButtonHandler) ( void );

namespace GameEngine
{
	typedef enum _e_collision_by_
	{
		E_COLLISION_BY_MESH = 0,
		E_COLLISION_BY_OCTREE,
		E_COLLISION_MAX
	} E_COLLISION_BY;
}

#endif	// #ifndef _GAME_ENGINE_TYPES_H_