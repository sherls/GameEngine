/*
	This class builds all of the assets for the renderer
*/

#ifndef CS6963_CASSETBUILDER_H
#define CS6963_CASSETBUILDER_H

// Header Files
//=============

// Windows
//--------

// Exclude extraneous Windows stuff
#define WIN32_LEAN_AND_MEAN
// Prevent Windows from creating min/max macros
#define NOMINMAX

// Initialize Windows
#include <Windows.h>

#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

// Class Definition
//=================

namespace Tools
{
	class cAssetBuilder
	{
		// Interface
		//==========

	public:

		bool Build( const int i_argumentCount, char** i_arguments );

		// Singleton Access
		//-----------------

		static inline cAssetBuilder& GetSingleton()
		{
			static cAssetBuilder singleton;
			return singleton;
		}
		#define g_assetBuilder Tools::cAssetBuilder::GetSingleton()
	};
}

#endif	// CS6963_CASSETBUILDER_H
