/**
 ****************************************************************************************************
 * \file		UserInput.h
 * \brief		Manages the input from keyboard and mouse
 ****************************************************************************************************
*/

#ifndef _USERINPUT_H_
#define _USERINPUT_H_

namespace GameEngine
{
	namespace UserInput
	{
		// As of this comment, the codes for these functions can be found at:
		// http://msdn.microsoft.com/en-us/library/dd375731(v=vs.85).aspx
		// (if that doesn't work, try searching for "Virtual Key Codes")

		// For standard letter or number keys, the representative ASCII char can be used:
		// IsKeyPressed( 'A' ) or IsKeyPressed( '6' )

		bool Initialize( void );
		void Update( void );

		bool IsKeyUp( const int i_virtualKeyCode );
		bool IsKeyDown( const int i_virtualKeyCode );
		bool IsKeyPressed( const int i_virtualKeyCode );
		bool IsButtonPressed( const int i_virtualButtonCode );
	}
}

#endif	// #ifndef _USERINPUT_H_