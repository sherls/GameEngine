/*
	This class is an example of how one could make a new file format using Lua
*/

#ifndef CS6963_CEXAMPLELUABUILDER_H
#define CS6963_CEXAMPLELUABUILDER_H

// Header Files
//=============

#include "../GenericBuilder/cbGenericBuilder.h"

// Class Definition
//=================

namespace cs6963
{
	class cExampleLuaBuilder : public cbGenericBuilder
	{
		// Inherited Interface
		//====================

	private:

		virtual const char* GetToolName() const { return "ExampleLuaBuilder"; }

		// Inherited Implementation
		//=========================

	private:

		virtual bool Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const;
	};
}

#endif	// CS6963_CEXAMPLELUABUILDER_H
