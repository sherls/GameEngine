/**
 ****************************************************************************************************
 * \file		Physics.h
 * \brief		Physics engine global function declaration
 ****************************************************************************************************
*/

#ifndef _PHYSICS_H_
#define _PHYSICS_H_

// Utilities header
#include <SmartPtr/SmartPtr.h>

//#include "../Utilities/SmartPtr/SmartPtr.h"

namespace GameEngine
{
	class Entity;

	namespace Physics
	{
		bool Initialize( void );
		void BeginUpdate( void );
		void Update( void );
		void EndUpdate( void );
		void ShutDown( void );

		UINT8 AddPhysicsEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity );
		void SetFriction( const UINT8 &i_u8Index, const float &i_friction );
	}
}	// namespace GameEngine

#endif	// #ifndef _PHYSICS_H_