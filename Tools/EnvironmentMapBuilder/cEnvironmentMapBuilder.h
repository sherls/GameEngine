/*
	This class builds textures
*/

#ifndef CS6963_CENVIRONMENTMAPBUILDER_H
#define CS6963_CENVIRONMENTMAPBUILDER_H

// Header Files
//=============

#include "../GenericBuilder/cbGenericBuilder.h"

// Class Definition
//=================

namespace Tools
{
	class cEnvironmentMapBuilder : public cbGenericBuilder
	{
		// Inherited Interface
		//====================

	private:

		virtual const char* GetToolName() const { return "EnvironmentMapBuilder"; }

		// Inherited Implementation
		//=========================

	private:

		virtual bool Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const;
	};
}

#endif	// CS6963_CTEXTUREBUILDER_H
