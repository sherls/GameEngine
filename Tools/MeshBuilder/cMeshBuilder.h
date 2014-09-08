/**
 ****************************************************************************************************
 * \file		cMeshBuilder.h
 * \brief		cMeshBuilder class declaration which copy Mesh.txt
 *
 * \date		12 January 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

#ifndef CS6963_CMESHBUILDER_H
#define CS6963_CMESHBUILDER_H

// Header Files
//=============

#include "../GenericBuilder/cbGenericBuilder.h"

// Class Definition
//=================

namespace Tools
{
	class cMeshBuilder : public cbGenericBuilder
	{
		// Inherited Interface
		//====================

	private:

		virtual const char* GetToolName() const { return "MeshBuilder"; }

		// Inherited Implementation
		//=========================

	private:

		virtual bool Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const;
	};
}

#endif	// CS6963_CMESHBUILDER_H
