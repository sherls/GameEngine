/**
 ****************************************************************************************************
 * \file		FBXLoader.h
 * \brief		FBXLoader class declaration which copy Scene.txt
 ****************************************************************************************************
*/

#ifndef _FBX_LOADER_H_
#define _FBX_LOADER_H_

// Header Files
//=============

#include "../GenericBuilder/cbGenericBuilder.h"

// Class Definition
//=================

namespace Tools
{
	class FBXLoader : public cbGenericBuilder
	{
		// Inherited Interface
		//====================
	public:
		/*FBXLoader( )
		{
			Build_derived( "Collision.fbx", "Collision.txt" );
		}*/

	private:

		virtual const char* GetToolName() const { return "FBXLoader"; }

		// Inherited Implementation
		//=========================

	private:
		virtual bool Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const;

	};
}

#endif	// _FBX_LOADER_H_
