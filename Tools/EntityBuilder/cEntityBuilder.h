/**
 ****************************************************************************************************
 * \file		cEntityBuilder.h
 * \brief		cEntityBuilder class declaration which copy Entity.txt
 *
 * \date		1 February 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

#ifndef CS6963_CENTITYBUILDER_H
#define CS6963_CENTITYBUILDER_H

// Header Files
//=============

#include "../GenericBuilder/cbGenericBuilder.h"

// Class Definition
//=================

namespace Tools
{
	class cEntityBuilder : public cbGenericBuilder
	{
		// Inherited Interface
		//====================

	private:

		virtual const char* GetToolName() const { return "EntityBuilder"; }

		// Inherited Implementation
		//=========================

	private:

		virtual bool Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const;
	};
}

#endif	// CS6963_CENTITYBUILDER_H
