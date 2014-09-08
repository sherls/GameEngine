/**
 ****************************************************************************************************
 * \file		IDCreator.h
 * \brief		The header of IDCreator class
 ****************************************************************************************************
*/

#ifndef _ID_CREATOR_H_
#define _ID_CREATOR_H_

#include <vector>

// Utilities header
#include <Debug/Debug.h>
#include <UtilitiesDefault.h>
#include <Singleton/Singleton.h>
#include <StringHash/StringHash.h>

#include "../GameEngineTypes.h"

namespace GameEngine
{
class IDCreator
{
	friend Utilities::Singleton<IDCreator>;

	struct _TrackedID
	{
		bool									_inUse;
		Utilities::StringHash	_hash;
	};

	struct _TrackedID _IDs[Utilities::DEFAULT_ID_SIZE];

	inline IDCreator( void );
	inline ~IDCreator( void );

public:
	void ReleaseID( const UINT32 &i_ID );
	UINT8 GetID( const char* i_entityType );
	UINT32 IDtoBitMask( const UINT32 &i_ID );
};
}	// namespace GameEngine

typedef Utilities::Singleton<GameEngine::IDCreator> g_IDCreator;

#include "IDCreator.inl"

#endif	// #ifndef _ID_CREATOR_H_