/**
 ****************************************************************************************************
 * \file		IDCreator.cpp
 * \brief		The implementation of IDCreator class
 ****************************************************************************************************
*/

#include "IDCreator.h"

/****************************************************************************************************
			PUBLIC FUNCTIONS IMPLEMENTATION
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			UINT32 GetID( const char* i_entityType )
	\brief		Get the ID of input entityType
	\param		i_entityType type of the entity
	\return		UINT32
	\retval		The ID of the entity type
 ****************************************************************************************************
*/
UINT8 GameEngine::IDCreator::GetID( const char* i_entityType )
{
	INT8 firstFree = -1;

	FUNCTION_START;

	for( UINT32 i = 0; i < Utilities::DEFAULT_ID_SIZE; ++i )
	{
		if( _IDs[i]._hash == Utilities::StringHash(i_entityType) )
		{
			FUNCTION_FINISH;
			return i + 1;
		}

		if( !_IDs[i]._inUse && (firstFree == -1) )
			firstFree = i;
	}

	_IDs[firstFree]._hash = Utilities::StringHash( i_entityType );
	_IDs[firstFree]._inUse = true;

	FUNCTION_FINISH;
	return firstFree + 1;
}

/**
 ****************************************************************************************************
	\fn			void ReleaseID( const UINT32 &i_ID )
	\brief		Release ID from the ID database
	\param		i_ID ID to be released
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::IDCreator::ReleaseID( const UINT32 &i_ID )
{
	assert( i_ID < Utilities::DEFAULT_ID_SIZE );
	FUNCTION_START;

	_IDs[i_ID]._inUse = false;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			UINT32 GetID( const UINT8 &i_ID )
	\brief		Get the bit mask of given ID
	\param		i_ID input ID
	\return		UINT32
	\retval		The bit mask of the given ID
 ****************************************************************************************************
*/
UINT32 GameEngine::IDCreator::IDtoBitMask( const UINT32& i_ID )
{
	assert( i_ID < Utilities::DEFAULT_ID_SIZE );
	FUNCTION_START;

	FUNCTION_FINISH;
	return 1 << i_ID;
}