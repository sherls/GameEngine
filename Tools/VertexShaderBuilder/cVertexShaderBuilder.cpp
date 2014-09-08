// Header Files
//=============

#include "cVertexShaderBuilder.h"

#include <sstream>

// Inherited Implementation
//=========================

bool Tools::cVertexShaderBuilder::Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const
{
	// Get the path to the shader compiler
	std::string path_fxc;
	{
		// Get the path to the DirectX SDK
		std::string path_sdk;
		{
			const DWORD bufferSize = 256;
			char buffer[bufferSize];
			DWORD characterCount = GetEnvironmentVariable( "DXSDK_DIR", buffer, bufferSize );
			if ( characterCount < bufferSize )
			{
				path_sdk = buffer;
			}
			else if ( characterCount != 0 )
			{
				std::ostringstream errorMessage;
				errorMessage << "Couldn't get the DirectX SDK directory; it requires a buffer of size "
					<< characterCount << " but the provided buffer only had a size of " << bufferSize;
				FormatAndOutputErrorMessage( errorMessage.str() );
				return false;
			}
			else
			{
				std::string errorMessage = std::string( "Couldn't get the DirectX SDK directory: " )
					+ GetLastWindowsError();
				FormatAndOutputErrorMessage( errorMessage );
				return false;
			}
		}
		path_fxc = path_sdk + "Utilities/bin/" +
#ifndef _WIN64
			"x86"
#else
			"x64"
#endif
			+ "/fxc.exe";
	}

	// Create the command to run
	std::string command = std::string( "\"" ) + path_fxc + "\" "
		// Target profile
		+ "/Tvs_3_0 "
		// Entry point
		+ "/Emain "

#ifdef _DEBUG
		// Disable optimizations so debugging is easier
		+ "/Od "
		// Enable debugging
		+ "/Zi "
#endif
		
		// Target file
		+ "/Fo\"" + i_fileName_target + "\" "

		// Don't output the logo
		+ "/nologo "

		// Source file
		+ "\"" + i_fileName_source + "\""
		;

	// Run the command
	{
		const DWORD bufferSize = 512;
		char commandBuffer[bufferSize];
		if ( bufferSize > command.length() )
		{
			strcpy_s( commandBuffer, bufferSize, command.c_str() );
		}
		else
		{
			std::ostringstream errorMessage;
			errorMessage << "Couldn't copy the command (of length " << command.length()
				<< ") into a non-const buffer of size " << bufferSize;
			FormatAndOutputErrorMessage( errorMessage.str() );
			return false;
		}
		SECURITY_ATTRIBUTES* useDefaultAttributes = NULL;
		BOOL dontInheritHandles = FALSE;
		DWORD createDefaultProcess = 0;
		void* useCallingProcessEnvironment = NULL;
		const char* useCallingProcessCurrentDirectory = NULL;
		STARTUPINFO startupInfo = { 0 };
		{
			startupInfo.cb = sizeof( startupInfo );
		}
		PROCESS_INFORMATION processInformation = { 0 };
		if ( CreateProcess( path_fxc.c_str(), commandBuffer, useDefaultAttributes, useDefaultAttributes,
			dontInheritHandles, createDefaultProcess, useCallingProcessEnvironment, useCallingProcessCurrentDirectory,
			&startupInfo, &processInformation ) == TRUE )
		{
			// Wait for FXC to finish
			DWORD exitCode = -1;
			if ( WaitForSingleObject( processInformation.hProcess, INFINITE ) != WAIT_FAILED )
			{
				// Get the exit code
				if ( GetExitCodeProcess( processInformation.hProcess, &exitCode ) == FALSE )
				{
					FormatAndOutputErrorMessage( std::string( "Windows failed to get the exit code of FXC: " )
						+ GetLastWindowsError() );
				}
			}
			else
			{
				FormatAndOutputErrorMessage( std::string( "Windows failed to wait for FXC to finish: " )
					+ GetLastWindowsError() );
			}
			// Close the process handles
			if ( CloseHandle( processInformation.hProcess ) == FALSE )
			{
				FormatAndOutputErrorMessage( std::string( "Windows failed to to close the handle to the FXC process: " )
					+ GetLastWindowsError() );
			}
			if ( CloseHandle( processInformation.hThread ) == FALSE )
			{
				FormatAndOutputErrorMessage( std::string( "Windows failed to to close the handle to the FXC thread: " )
					+ GetLastWindowsError() );
			}

			return exitCode == 0;
		}
		else
		{
			std::string errorMessage = std::string( "Couldn't start the process " ) + path_fxc
				+ " using the command line \"" + command + "\": " + GetLastWindowsError();
			FormatAndOutputErrorMessage( errorMessage );
			return false;
		}
	}
}
