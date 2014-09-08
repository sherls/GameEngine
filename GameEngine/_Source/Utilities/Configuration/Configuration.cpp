/**
 ****************************************************************************************************
 * \file		Configuration.cpp
 * \brief		Implementation of configuration utility
 ****************************************************************************************************
*/

#include <assert.h>

// Utilities header
#include <Debug/Debug.h>


#include "Configuration.h"
#include "../GameEngineTypes.h"
#include "../../GameEngineDefault.h"
namespace GameEngine
{
	namespace Configuration
	{
		static libconfig::Config *g_masterCfg;
	}
}

/**
 ****************************************************************************************************
	\fn			bool LoadMasterConfig( const char *i_masterConfigFile )
	\brief		Load configuration file
	\param		i_masterConfigFile name of configuration file
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::Configuration::LoadMasterConfig( const char *i_masterConfigFile )
{
	assert( i_masterConfigFile );

	g_masterCfg = new libconfig::Config;
	assert( g_masterCfg );

	if( !g_masterCfg->readFile(i_masterConfigFile) )
		return FAIL;

	libconfig::Setting *gameEngineSetting = g_masterCfg->lookup( "GameEngine" );

	if( gameEngineSetting )
	{
		bool bRun = false;

		if( gameEngineSetting->lookupValue("run", bRun) )
		{
			if( bRun ) return SUCCESS;
		}
	}

	delete g_masterCfg;

	FUNCTION_FINISH;
	return FAIL;
}

/**
 ****************************************************************************************************
	\fn			bool UnloadMasterConfig( void )
	\brief		Unload configuration file
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
void GameEngine::Configuration::UnloadMasterConfig( void )
{
	FUNCTION_START;

	if( g_masterCfg )
	{
		delete g_masterCfg;
		g_masterCfg = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			libconfig::Config &GetMasterConfig( void )
	\brief		Get configuration data
	\param		NONE
	\return		libconfig::Config 
	\retval		Loaded configuration data
 ****************************************************************************************************
*/
libconfig::Config &GameEngine::Configuration::GetMasterConfig( void )
{
	assert( g_masterCfg );
	
	return *g_masterCfg;
}