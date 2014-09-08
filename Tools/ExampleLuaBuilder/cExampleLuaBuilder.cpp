// Header Files
//=============

#include "cExampleLuaBuilder.h"

#include <cassert>
#include <iostream>
#include <sstream>

extern "C"
{
	#include "../../../../External/Lua/5.2.1/lua.h"
	#include "../../../../External/Lua/5.2.1/lualib.h"
	#include "../../../../External/Lua/5.2.1/lauxlib.h"
}

// Inherited Implementation
//=========================

bool cs6963::cExampleLuaBuilder::Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const
{
	// Copy the file to make the asset build process happy
	{
		BOOL shouldFailIfDestinationFileAlreadyExists = FALSE;
		if ( CopyFile( i_fileName_source, i_fileName_target, shouldFailIfDestinationFileAlreadyExists ) == FALSE )
		{
			std::string errorMessage = std::string( "Couldn't copy " ) + i_fileName_source + " to " + i_fileName_target + ":  "
				+ GetLastWindowsError();
			FormatAndOutputErrorMessage( errorMessage );
			return false;
		}
	}

	// The following code shows an example of how you can read in a Lua file and process it:

	bool didBuildSucceed = true;

	// Create a new Lua state
	lua_State* luaState = NULL;
	bool wasTableLoaded = false;
	{
		luaState = luaL_newstate();
		if ( luaState == NULL )
		{
			FormatAndOutputErrorMessage( "Memory allocation error while creating new Lua state!" );
			didBuildSucceed = false;
			goto CleanUp;
		}
	}

	// Load the file into a table
	{
		// Load the file into a function that Lua can call
		int result = luaL_loadfile( luaState, i_fileName_source );
		if ( result == LUA_OK )
		{
			// Execute the file's function
			int noArguments = 0;
			int tableShouldBeReturned = 1;
			int noErrorMessageHandler = 0;
			result = lua_pcall( luaState, noArguments, tableShouldBeReturned, noErrorMessageHandler );
			if ( result == LUA_OK )
			{
				// The table should now be on the stack
				if ( lua_istable( luaState, -1 ) )
				{
					wasTableLoaded = true;
				}
				else
				{
					if ( lua_isnil( luaState, -1 ) )
					{
						std::string errorMessage = std::string( "The file " ) + i_fileName_source
							+ " must return a table";
						FormatAndOutputErrorMessage( errorMessage );
					}
					else
					{
						std::string errorMessage = std::string( "The file " ) + i_fileName_source
							+ " must return a table instead of a "
							+ lua_typename( luaState, lua_type( luaState, -1 ) );
						FormatAndOutputErrorMessage( errorMessage );
						wasTableLoaded = true;
					}
					didBuildSucceed = false;
					goto CleanUp;
				}
			}
			else
			{
				std::string errorMessage = lua_tostring( luaState, -1 );
				lua_pop( luaState, 1 );
				if ( result == LUA_ERRRUN )
				{
					FormatAndOutputErrorMessage( errorMessage );
				}
				else
				{
					FormatAndOutputErrorMessage( errorMessage );
				}
				didBuildSucceed = false;
				goto CleanUp;
			}
		}
		else
		{
			std::string errorMessage = lua_tostring( luaState, -1 );
			lua_pop( luaState, 1 );
			if ( result == LUA_ERRSYNTAX )
			{
				FormatAndOutputErrorMessage( errorMessage );
			}
			else
			{
				errorMessage = std::string( "Error loading " ) + i_fileName_source + ": " + errorMessage;
				FormatAndOutputErrorMessage( errorMessage );
			}
			didBuildSucceed = false;
			goto CleanUp;
		}
	}

	// The following code shows an example of getting to the data from the table
	{
		// Look for the sub-table named "position":
		{
			// The table is currently at the top of the stack, meaning it's index -1;
			// pushing things onto the stack re-arranges the indices:
			lua_pushstring( luaState, "position" );
			// Now the table is at index -2, and the string "position" is at index -1.
			// We can now do a lookup into the table using the key "position":
			lua_gettable( luaState, -2 );
			// The previous call removed the key from the top of the stack
			// and placed the value that was looked up there instead.
			// In other words, the table is still at index -2, and now the value that we looked up is index -1.
			// Based on the example.lua file, we expect the value to be a table also:
			assert( lua_istable( luaState, -1 ) );
			{
				// The entries in the position table have integer keys rather than string keys,
				// meaning that it acts like an array.
				// It is possible to iterate through like this:
				int triangleCount = luaL_len( luaState, -1 );
				for ( int i = 1; i <= triangleCount; ++i )	// Lua arrays are 1-based instead of 0-based
				{
					lua_pushinteger( luaState, i );
					lua_gettable( luaState, -2 );
					// Now the ith entry of "position" is at the top of the stack, or index -1
					assert( lua_istable( luaState, -1 ) );
					{
						// This table represents a vertex with three points
						assert( luaL_len( luaState, -1 ) == 3 );
						// We could examine each of the three values:
						for ( int j = 1; j <= 3; ++j )
						{
							lua_pushinteger( luaState, j );
							lua_gettable( luaState, -2 );
							assert( lua_istable( luaState, -1 ) );
							// And likewise, we could look at each of the three numbers in this vertex:
							{
								assert( luaL_len( luaState, -1 ) == 3 );
								for ( int k = 1; k <= 3; ++k )
								{
									lua_pushinteger( luaState, k );
									lua_gettable( luaState, -2 );
									assert( lua_isnumber( luaState, -1 ) );
									float someNumber = static_cast<float>( lua_tonumber( luaState, -1 ) );
									lua_pop( luaState, 1 );
								}
							}
							lua_pop( luaState, 1 );
						}
					}
					lua_pop( luaState, 1 );
				}
			}
			// "Pop" the position table from the stack
			lua_pop( luaState, 1 );
		}
	}

	// Free the Lua environment

CleanUp:

	if ( luaState )
	{
		if ( wasTableLoaded )
		{
			lua_pop( luaState, 1 );
		}
		// Sanity Check
		{
			int stackItemCount = lua_gettop( luaState );
			if ( stackItemCount != 0 )
			{
				std::ostringstream errorMessage;
				errorMessage << "There are still " << stackItemCount
					<< " items in the Lua stack when the Lua state is being closed";
				FormatAndOutputErrorMessage( errorMessage.str() );
				didBuildSucceed = false;
			}
		}
		lua_close( luaState );
	}

	return didBuildSucceed;
}
