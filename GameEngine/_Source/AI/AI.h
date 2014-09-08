/**
 ****************************************************************************************************
 * \file		AI.h
 * \brief		AI engine global function declaration
 ****************************************************************************************************
*/

#ifndef _AI_H_
#define _AI_H_

// Utilities header
#include <UtilitiesTypes.h>

#include "WayPoint.h"
#include "../World/Entity.h"

namespace GameEngine
{
	namespace AI
	{
		bool Initialize( void );
		void BeginUpdate( void );
		void Update( void );
		void EndUpdate( void );
		void ShutDown( void );

		void AddWayPoint( const UINT32 &i_u32ID, const S_WAY_POINT &i_wayPoint );
		void AddWayPointLink( const S_WAY_POINT_LINK &i_newWayPointLink );

		UINT8 AddAIEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity );
		void RemoveAIEntity( UINT8 &i_u8Index );
		void UpdateAIDestinationTo( const UINT8 &i_u8Index, const UINT8 &i_u8NodeID );
		void AbortAI( const UINT8 &i_u8Index );
		bool GetAIState( const UINT8 &i_u8Index );
		void FindClosestNodeIDFromPosition( const D3DXVECTOR3 &i_vCurrPosition, UINT32 &o_u32NodeID );
		float FindDistanceToNodeID( const D3DXVECTOR3 &i_vCurrPosition, const UINT32 &i_u32NodeID );
	}
}

#endif	// #ifndef _AI_H_