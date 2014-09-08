// Header Files
//=============

#include "cbGenericBuilder.h"

#include <iostream>
#include <sstream>

// Interface
//==========

bool Tools::cbGenericBuilder::Build( const int i_argumentCount, char** i_arguments ) const
{
	// Parse the command-line arguments to get the input and output file names
	const char* fileName_source;
	const char* fileName_target;
	{
		const unsigned int expectedArgumentCount = 2;
		// The array of command-line arguments includes the command (i.e. the name of the executable that was run)
		const unsigned int commandCount = 1;
		if ( i_argumentCount == ( expectedArgumentCount + commandCount ) )
		{
			fileName_source = i_arguments[0 + commandCount];
			fileName_target = i_arguments[1 + commandCount];
		}
		else
		{
			std::ostringstream errorMessage;
			errorMessage << "Exactly " << expectedArgumentCount << " command-line arguments (the source and target filenames) must be provided"
				" (instead of " << ( i_argumentCount - commandCount ) << ")";
			FormatAndOutputErrorMessage( errorMessage.str() );
			return false;
		}
	}

	return Build_derived( fileName_source, fileName_target );
}

void Tools::cbGenericBuilder::FormatAndOutputErrorMessage( const std::string& i_errorMessage ) const
{
	std::cerr << GetToolName() << ": error: " << i_errorMessage << "\n";
}

void Tools::cbGenericBuilder::LogMessage( const std::string & i_message ) const
{
	std::cout << GetToolName() << ":" << i_message << "\n";
}

std::string Tools::cbGenericBuilder::GetLastWindowsError()
{
	// Windows stores the error as a code
	DWORD errorCode = GetLastError();
	// Format the message
	std::string errorMessage;
	{
		DWORD formattingOptions =
			// Get the error message from Windows
			FORMAT_MESSAGE_FROM_SYSTEM
			// The space for the error message should be allocated by Windows
			| FORMAT_MESSAGE_ALLOCATE_BUFFER
			// Any potentially deferred inserts should be ignored
			// (i.e. the error message should be in its final form)
			| FORMAT_MESSAGE_IGNORE_INSERTS;
		const void* messageIsFromWindows = NULL;
		DWORD useTheDefaultLanguage = 0;
		char* messageBuffer = NULL;
		DWORD minimumCharacterCountToAllocate = 1;
		va_list* insertsAreIgnored = NULL;
		DWORD storedCharacterCount = FormatMessage( formattingOptions, messageIsFromWindows, errorCode,
			useTheDefaultLanguage, reinterpret_cast<LPSTR>( &messageBuffer ), minimumCharacterCountToAllocate, insertsAreIgnored );
		if ( storedCharacterCount != 0 )
		{
			errorMessage = messageBuffer;
		}
		else
		{
			// If there's an error GetLastError() can be called again,
			// but that is too complicated for this program :)
			errorMessage = "Unknown Windows Error";
		}
		// Try to free the memory regardless of whether formatting worked or not,
		// and ignore any error messages
		LocalFree( messageBuffer );
	}
	return errorMessage;
}
