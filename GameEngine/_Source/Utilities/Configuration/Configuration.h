/**
 ****************************************************************************************************
 * \file		Configuration.h
 * \brief		The header of Configuration utility
 ****************************************************************************************************
*/

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <libconfig.hpp>

namespace GameEngine
{
	namespace Configuration
	{
		bool LoadMasterConfig( const char *i_masterConfigFile );
		void UnloadMasterConfig( void );

		libconfig::Config &GetMasterConfig( void );
	}
}

#endif	// #ifndef _CONFIGURATION_H_