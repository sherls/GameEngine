#include "Utilities.h"
#include "Time/Time.h"
#include "Debug/Debug.h"
#include "UtilitiesTypes.h"
#include "SmartPtr/SmartPtr.h"
#include "StringHash/StringHash.h"

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize utilities
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
bool Utilities::Initialize( void )
{
	FUNCTION_START;

	if( !Pointer::Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	Time::Initialize();
	srand( static_cast<UINT32>(Time::GetCurrentTick()) );

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool ShutDown( void )
	\brief		Shut down utilities
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
bool Utilities::ShutDown( void )
{
	FUNCTION_START;

	Pointer::ShutDown();

	FUNCTION_FINISH;
	return SUCCESS;
}