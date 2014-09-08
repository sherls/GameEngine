// Header Files
//=============

#include "cAssetBuilder.h"

#include <iostream>
#include <sstream>
#include <shlobj.h>
#include <string>

extern "C"
{
	#include "../../External/Lua/5.2.1/lua.h"
	#include "../../External/Lua/5.2.1/lualib.h"
	#include "../../External/Lua/5.2.1/lauxlib.h"
}

// Static Data Initialization
//===========================

namespace
{
	std::string s_directory_assets;
	std::string s_directory_data;
	std::string s_directory_tools;
	std::string s_path_assetList;
	std::string s_path_buildScript;
}

// Helper Function Declarations
//=============================

namespace
{
	int CreateDirectoryIfNecessary( lua_State* io_luaState );
	int DoesFileExist( lua_State* io_luaState );
	bool ExecuteLuaScript();
	std::string GetLastWindowsError( DWORD* o_errorCode = NULL );
	int GetLastWriteTime( lua_State* io_luaState );
	void OutputErrorMessage( const std::string& i_errorMessage );
	void OutputLuaErrorMessage( const std::string& i_errorMessage );
	bool ParseCommandLine( const int i_argumentCount, char** i_arguments );
	bool RunBuildScript( lua_State& io_luaState );
}

// Interface
//==========

bool Tools::cAssetBuilder::Build( const int i_argumentCount, char** i_arguments )
{
	if ( ParseCommandLine( i_argumentCount, i_arguments ) )
	{
		s_path_assetList = s_directory_assets + "AssetList.lua";
	}
	else
	{
		return false;
	}

	return ExecuteLuaScript();
}

// Helper Function Definitions
//============================

namespace
{
	int CreateDirectoryIfNecessary( lua_State* io_luaState )
	{
		// Get the path
		std::string path;
		{
			if ( lua_isstring( io_luaState, 1 ) )
			{
				path = lua_tostring( io_luaState, 1 );
			}
			else
			{
				return luaL_error( io_luaState, "A path must be provided" );
			}
		}
		// If the path is to a file (likely), remove it so only the directory remains
		std::string directory;
		{
			size_t pos_slash = path.find_last_of( "\\/" );
			if ( pos_slash != std::string::npos )
			{
				directory = path.substr( 0, pos_slash );
			}
			else
			{
				directory = path;
			}
		}
		// Get the path in a form Windows likes (without any ".."s)
		const DWORD bufferSize = 256;
		char buffer[bufferSize];
		{
			char** pathIsDirectory = NULL;
			DWORD characterCount = GetFullPathName( directory.c_str(), bufferSize, buffer, pathIsDirectory );
			if ( characterCount < bufferSize )
			{
				// Create the directory
				int result;
				{
					HWND noWindowIsDisplayed = NULL;
					const SECURITY_ATTRIBUTES* useDefaultSecurityAttributes = NULL;
					result = SHCreateDirectoryEx( noWindowIsDisplayed, buffer, useDefaultSecurityAttributes );
				}
				if ( result != ERROR_SUCCESS )
				{
					if ( ( result != ERROR_FILE_EXISTS ) && ( result != ERROR_ALREADY_EXISTS ) )
					{
						std::ostringstream errorMessage;
						errorMessage << "Windows failed to create the directory \"" << buffer
							<< "\" with the error code " << result
							<< "... Contact John-Paul";
						OutputErrorMessage( errorMessage.str() );
					}
				}
			}
			else if ( characterCount != bufferSize )
			{
				std::ostringstream errorMessage;
				errorMessage << "Couldn't get the full path name of \"" << directory << "\"; it requires a buffer of size "
					<< characterCount << " but the provided buffer only had a size of " << bufferSize;
				OutputErrorMessage( errorMessage.str() );
			}
			else
			{
				std::string errorMessage = std::string( "Couldn't get the full path name of \"" ) + directory + ": "
					+ GetLastWindowsError();
				OutputErrorMessage( errorMessage);
			}
		}

		return 0;
	}

	int DoesFileExist( lua_State* io_luaState )
	{
		// Get the file path
		std::string path;
		{
			if ( lua_isstring( io_luaState, 1 ) )
			{
				path = lua_tostring( io_luaState, 1 );
			}
			else
			{
				return luaL_error( io_luaState, "A file path must be provided" );
			}
		}

		// Try to get information about the file
		WIN32_FIND_DATA fileData;
		HANDLE file = FindFirstFile( path.c_str(), &fileData );
		if ( file != INVALID_HANDLE_VALUE )
		{
			if ( FindClose( file ) == TRUE )
			{
				lua_pushboolean( io_luaState, 1 );
				return 1;
			}
			else
			{
				return luaL_error( io_luaState, GetLastWindowsError().c_str() );
			}
		}
		else
		{
			DWORD errorCode;
			std::string errorMessage = GetLastWindowsError( &errorCode );
			if ( ( errorCode == ERROR_FILE_NOT_FOUND ) || ( errorCode == ERROR_PATH_NOT_FOUND ) )
			{
				lua_pushboolean( io_luaState, 0 );
				return 1;
			}
			else
			{
				return luaL_error( io_luaState, errorMessage.c_str() );
			}
		}
	}

	bool ExecuteLuaScript()
	{
		bool didBuildSucceed = true;

		// Create a new Lua state
		lua_State* luaState = NULL;
		{
			luaState = luaL_newstate();
			if ( luaState != NULL )
			{
				// Open the standard Lua libraries
				luaL_openlibs( luaState );
			}
			else
			{
				OutputErrorMessage( "Memory allocation error while creating new Lua state!" );
				didBuildSucceed = false;
				goto CleanUp;
			}
		}

		// Register C functions that can be used by a Lua script
		{
			lua_register( luaState, "CreateDirectoryIfNecessary", CreateDirectoryIfNecessary );
			lua_register( luaState, "DoesFileExist", DoesFileExist );
			lua_register( luaState, "GetLastWriteTime", GetLastWriteTime );
		}

		// Run the actual build script
		if ( !RunBuildScript( *luaState ) )
		{
			didBuildSucceed = false;
			goto CleanUp;
		}

		// Free the Lua environment

	CleanUp:

		if ( luaState )
		{
			// Sanity Check
			{
				int stackItemCount = lua_gettop( luaState );
				if ( stackItemCount != 0 )
				{
					std::ostringstream errorMessage;
					errorMessage << "There are still " << stackItemCount
						<< " items in the Lua stack when the Lua state is being closed";
					OutputErrorMessage( errorMessage.str() );
					didBuildSucceed = false;
				}
			}
			lua_close( luaState );
		}

		return didBuildSucceed;
	}

	std::string GetLastWindowsError( DWORD* o_errorCode )
	{
		// Windows stores the error as a code
		DWORD errorCode = GetLastError();
		if ( o_errorCode )
		{
			*o_errorCode = errorCode;
		}
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

	int GetLastWriteTime( lua_State* io_luaState )
	{
		// Get the file path
		std::string path;
		{
			if ( lua_isstring( io_luaState, 1 ) )
			{
				path = lua_tostring( io_luaState, 1 );
			}
			else
			{
				return luaL_error( io_luaState, "A file path must be provided" );
			}
		}

		// Get the last time the file was written to
		double lastWriteTime;
		{
			ULARGE_INTEGER lastWriteTime_int;
			{
				WIN32_FIND_DATA fileData;
				{
					HANDLE file = FindFirstFile( path.c_str(), &fileData );
					if ( file != INVALID_HANDLE_VALUE )
					{
						if ( FindClose( file ) == FALSE )
						{
							return luaL_error( io_luaState, GetLastWindowsError().c_str() );
						}
					}
					else
					{
						return luaL_error( io_luaState, GetLastWindowsError().c_str() );
					}
				}
				FILETIME fileTime = fileData.ftLastWriteTime;
				lastWriteTime_int.HighPart = fileTime.dwHighDateTime;
				lastWriteTime_int.LowPart = fileTime.dwLowDateTime;
			}
			lastWriteTime = static_cast<double>( lastWriteTime_int.QuadPart );
		}

		lua_pushnumber( io_luaState, lastWriteTime );
		return 1;
	}

	void OutputErrorMessage( const std::string& i_errorMessage )
	{
		std::cerr << "Asset Build: error: " << i_errorMessage << "\n";
	}

	void OutputLuaErrorMessage( const std::string& i_errorMessage )
	{
		// Report the Lua error's so that they are double-clickable from Visual Studio
		size_t pos_file = i_errorMessage.find_first_of( ':' );
		if ( pos_file != std::string::npos )
		{
			// If the path is absolute find the next colon
			if ( pos_file == 1 )
			{
				pos_file = i_errorMessage.find_first_of( ':', pos_file + 1 );
				if ( pos_file == std::string::npos )
				{
					OutputErrorMessage( i_errorMessage );
					return;
				}
			}
			// The file should either be the build script or the asset list
			std::string file = i_errorMessage.substr( 0, pos_file );
			{
				size_t pos_fileName = file.find_last_of( "/\\" );
				if ( pos_fileName != std::string::npos )
				{
					pos_fileName += 1;
					std::string fileName = file.substr( pos_fileName );
					if ( fileName == "AssetList.lua" )
					{
						file = s_path_assetList;
					}
					else if ( fileName == "BuildAssets.lua" )
					{
						file = s_path_buildScript;
					}
				}
			}
			pos_file += 1;
			size_t pos_message = i_errorMessage.find_first_of( ':', pos_file );
			if ( pos_message != std::string::npos )
			{
				std::string lineNumber = i_errorMessage.substr( pos_file, pos_message - pos_file );
				pos_message += 1;
				std::string errorMessage = i_errorMessage.substr( pos_message );
				std::cerr << file << "(" << lineNumber << "): error: " << errorMessage << "\n";
			}
			else
			{
				OutputErrorMessage( i_errorMessage );
			}
		}
		else
		{
			OutputErrorMessage( i_errorMessage );
		}
	}

	bool ParseCommandLine( const int i_argumentCount, char** i_arguments )
	{
		const unsigned int expectedArgumentCount = 4;
		// The array of command-line arguments includes the command (i.e. the name of the executable that was run)
		const unsigned int commandCount = 1;
		if ( i_argumentCount == ( expectedArgumentCount + commandCount ) )
		{
			s_directory_assets = i_arguments[0 + commandCount];
			s_directory_data = i_arguments[1 + commandCount];
			s_directory_tools = i_arguments[2 + commandCount];
			s_path_buildScript = i_arguments[3 + commandCount];
			return true;
		}
		else
		{
			std::ostringstream errorMessage;
			errorMessage << "Exactly " << expectedArgumentCount << " command-line arguments"
				" (the source asset directory, target data directory, tools directory, and build script path) must be provided"
				" (instead of " << ( i_argumentCount - commandCount ) << ")";
			OutputErrorMessage( errorMessage.str() );
			return false;
		}
	}

	bool RunBuildScript( lua_State& io_luaState )
	{
		// Load the Lua build script
		int result = luaL_loadfile( &io_luaState, s_path_buildScript.c_str() );
		if ( result == LUA_OK )
		{
			// Execute the script to obtain a function
			int noArguments = 0;
			int functionShouldBeReturned = 1;
			int noErrorMessageHandler = 0;
			result = lua_pcall( &io_luaState, noArguments, functionShouldBeReturned, noErrorMessageHandler );
			if ( result == LUA_OK )
			{
				// The script's function should now be on the stack
				if ( lua_isfunction( &io_luaState, -1 ) )
				{
					// Push each of the arguments the function expects
					int argumentCount = 0;
					{
						lua_pushstring( &io_luaState, s_directory_assets.c_str() );
						++argumentCount;
						lua_pushstring( &io_luaState, s_directory_data.c_str() );
						++argumentCount;
						lua_pushstring( &io_luaState, s_directory_tools.c_str() );
						++argumentCount;
						lua_pushstring( &io_luaState, s_path_assetList.c_str() );
						++argumentCount;
					}
					int noReturnValues = 0;
					// Call the function
					result = lua_pcall( &io_luaState, argumentCount, noReturnValues, noErrorMessageHandler );
					if ( result == LUA_OK )
					{
						return true;
					}
					else
					{
						std::string errorMessage = lua_tostring( &io_luaState, -1 );
						lua_pop( &io_luaState, 1 );
						if ( result == LUA_ERRRUN )
						{
							OutputLuaErrorMessage( errorMessage );
						}
						else
						{
							OutputErrorMessage( errorMessage );
						}
						return false;
					}
				}
				else
				{
					if ( lua_isnil( &io_luaState, -1 ) )
					{
						OutputLuaErrorMessage( ":0:The build script must return a function" );
					}
					else
					{
						std::string errorMessage = std::string( ":0:The build script must return a function instead of a " )
							+ lua_typename( &io_luaState, lua_type( &io_luaState, -1 ) );
						OutputLuaErrorMessage( errorMessage );
					}
					return false;
				}
			}
			else
			{
				std::string errorMessage = lua_tostring( &io_luaState, -1 );
				lua_pop( &io_luaState, 1 );
				if ( result == LUA_ERRRUN )
				{
					OutputLuaErrorMessage( errorMessage );
				}
				else
				{
					OutputErrorMessage( errorMessage );
				}
				return false;
			}
		}
		else
		{
			std::string errorMessage = lua_tostring( &io_luaState, -1 );
			lua_pop( &io_luaState, 1 );
			if ( result == LUA_ERRSYNTAX )
			{
				OutputLuaErrorMessage( errorMessage );
			}
			else
			{
				errorMessage = std::string( "Error loading asset build script: " ) + errorMessage;
				OutputErrorMessage( errorMessage );
			}
			return false;
		}
	}
}
