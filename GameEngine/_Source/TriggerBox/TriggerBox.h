/**
 ****************************************************************************************************
 * \file		TriggerBox.h
 * \brief		The header of TriggerBox class
 ****************************************************************************************************
*/

#ifndef _TRIGGER_BOX_H_
#define _TRIGGER_BOX_H_

#include "../Utilities/GameEngineTypes.h"

namespace GameEngine
{
	class Entity;

	namespace Math
	{
		class Vector3;
	}

	namespace TriggerBox
	{
		class TriggerBoxHandler
		{
			UINT32 _u32CollidedObjectID;

		public:
			TriggerBoxHandler( void ) :
			  _u32CollidedObjectID( 0 )
			{
			}
			virtual ~TriggerBoxHandler( void ) {}
			virtual void OnEnter( Utilities::Pointer::SmartPtr<Entity> &i_other, const float i_intersectionTime, \
				const Math::Vector3 &i_v3IntersectionNormal ) {}
			virtual void OnLeave( Utilities::Pointer::SmartPtr<Entity> &i_other ) {}
			void HandleIntersection( Utilities::Pointer::SmartPtr<Entity> &i_entity, Utilities::Pointer::SmartPtr<Entity> &i_other,
				const float i_intersectionTime, const Math::Vector3 &i_v3IntersectionNormal );
			void OnLeavingTriggerBox( Utilities::Pointer::SmartPtr<Entity> &i_entity, Utilities::Pointer::SmartPtr<Entity> &i_other );
		};

		bool Initialize( void );
		void BeginUpdate( void );
		void Update( void );
		void EndUpdate( void );
		void ShutDown( void );

		UINT8 AddTriggerBoxEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity );
		void SetTriggerBoxHandler( const UINT8 &i_index, TriggerBoxHandler *i_triggerBoxHandler );
	}	// namespace Collision
}	// namespace GameEngine

#endif  // #ifndef _TRIGGER_BOX_H_