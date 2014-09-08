/**
 ****************************************************************************************************
 * \file		IDCreator.inl
 * \brief		The inline functions implementation of IDCreator class
 ****************************************************************************************************
*/

#include <assert.h>

#include "../GameEngineTypes.h"

/**
 ****************************************************************************************************
	\fn			IDCreator( void )
	\brief		IDCreator default constructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::IDCreator::IDCreator( void )
{
	FUNCTION_START;

	for( UINT32 i = 0; i < Utilities::DEFAULT_ID_SIZE; ++i )
		_IDs[i]._inUse = false;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			~IDCreator( void )
	\brief		IDCreator default destructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::IDCreator::~IDCreator( void )
{
	FUNCTION_START;

	for( UINT32 i = 0; i < Utilities::DEFAULT_ID_SIZE; ++i )
		_IDs[i]._inUse = false;

	FUNCTION_FINISH;
}
