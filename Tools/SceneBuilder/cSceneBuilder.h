/**
 ****************************************************************************************************
 * \file		cSceneBuilder.h
 * \brief		cSceneBuilder class declaration which copy Scene.txt
 *
 * \date		1 February 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

#ifndef CS6963_CSCENEBUILDER_H
#define CS6963_CSCENEBUILDER_H

// Header Files
//=============

#include "../GenericBuilder/cbGenericBuilder.h"

// Class Definition
//=================

namespace cs6963
{
	class cSceneBuilder : public cbGenericBuilder
	{
		// Inherited Interface
		//====================

	private:

		virtual const char* GetToolName() const { return "SceneBuilder"; }

		// Inherited Implementation
		//=========================

	private:

		virtual bool Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const;
	};
}

#endif	// CS6963_CSCENEBUILDER_H
