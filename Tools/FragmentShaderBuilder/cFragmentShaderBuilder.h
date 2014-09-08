/*
	This class builds fragment shaders
*/

#ifndef CS6963_CFRAGMENTSHADERBUILDER_H
#define CS6963_CFRAGMENTSHADERBUILDER_H

// Header Files
//=============

#include "../GenericBuilder/cbGenericBuilder.h"

// Class Definition
//=================

namespace Tools
{
	class cFragmentShaderBuilder : public cbGenericBuilder
	{
		// Inherited Interface
		//====================

	private:

		virtual const char* GetToolName() const { return "FragmentShaderBuilder"; }

		// Inherited Implementation
		//=========================

	private:

		virtual bool Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const;
	};
}

#endif	// CS6963_CFRAGMENTSHADERBUILDER_H
