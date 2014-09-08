/**
 ****************************************************************************************************
 * \file		cMaterialBuilder.h
 * \brief		cMaterialBuilder class declaration which copy Material.txt
 *
 * \date		1 February 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

#ifndef CS6963_CMATERIALBUILDER_H
#define CS6963_CMATERIALBUILDER_H

// Header Files
//=============

#include "../GenericBuilder/cbGenericBuilder.h"

// Class Definition
//=================

namespace Tools
{
	class cMaterialBuilder : public cbGenericBuilder
	{
		// Inherited Interface
		//====================

	private:

		virtual const char* GetToolName() const { return "MaterialBuilder"; }

		// Inherited Implementation
		//=========================

	private:

		virtual bool Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const;
	};
}

#endif	// CS6963_CMATERIALBUILDER_H
