/*
	This file defines a base class that all Asset Builders can derive from
*/

#ifndef CS6963_CBGENERICBUILDER_H
#define CS6963_CBGENERICBUILDER_H

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

// Miscellaneous
//--------------

#include <string>

// Class Definition
//=================

namespace Tools
{
	class cbGenericBuilder
	{
		// Interface
		//==========

	public:

		// This should be called from main() and passed its command-line arguments
		bool Build( const int i_argumentCount, char** i_arguments ) const;

		// Use this function to output an error message in a format that will show up in Visual Studio's error list
		void FormatAndOutputErrorMessage( const std::string& i_errorMessage ) const;
		// This is a convenience function to get error messages from Windows
		static std::string GetLastWindowsError();

		void LogMessage( const std::string &i_message ) const;

		// Inheritable Interface
		//======================

	protected:

		// Define this to return the name that should be displayed as part of asset build error messages
		// (e.g. "ShaderBuilder" or "TextureBuilder")
		virtual const char* GetToolName() const = 0;

		// Inheritable Implementation
		//===========================

	private:

		// Define this to actually build the target file given the source file
		virtual bool Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const = 0;
	};
}

#endif	// CS6963_CBGENERICBUILDER_H
