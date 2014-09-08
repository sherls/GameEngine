// Header Files
//=============

#include <Windows.h>

// Utilities header
#include <Debug/Debug.h>

#include <Window/Window.h>

#include "UserInput.h"

/****************************************************************************************************
			Function declaration
****************************************************************************************************/
namespace GameEngine
{
	namespace UserInput
	{
		typedef enum _e_key_state_
		{
			E_KEY_STATE_DOWN = 0,
			E_KEY_STATE_UP,
			E_KEY_STATE_UNKNOWN
		} E_KEY_STATE;

		E_KEY_STATE prevKeyState[255];
		E_KEY_STATE currKeyState[255];

		bool IsVirtualKeyPressed( const int i_virtualKeyCode );
	}
}

/****************************************************************************************************
			Function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize UserInput
	\param		NONE
	\return		boolean
	\retval		TRUE if the key is pressed
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::UserInput::Initialize( void )
{
	FUNCTION_START;

	for( UINT8 i = 0; i < 255; ++i )
	{
		prevKeyState[i] = E_KEY_STATE_UNKNOWN;
		currKeyState[i] = E_KEY_STATE_UP;
	}

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update the key state
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::UserInput::Update( void )
{
	FUNCTION_START;

	for( UINT8 i = 0; i < 255; ++i )
	{
		prevKeyState[i] = currKeyState[i];
		if( IsVirtualKeyPressed(i) )
			currKeyState[i] = E_KEY_STATE_DOWN;
		else
			currKeyState[i] = E_KEY_STATE_UP;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool IsKeyUp( const int i_virtualKeyCode )
	\brief		Check whether the keyboard key is up
	\param		i_virtualKeyCode the ASCII code of the key
	\return		boolean
	\retval		TRUE if the key is released from down to up
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::UserInput::IsKeyUp( const int i_virtualKeyCode )
{
	FUNCTION_START;

	if( (prevKeyState[i_virtualKeyCode] != E_KEY_STATE_UP) && (currKeyState[i_virtualKeyCode] == E_KEY_STATE_UP) )
	{
		E_KEY_STATE prevState = prevKeyState[i_virtualKeyCode];
		E_KEY_STATE currState = currKeyState[i_virtualKeyCode];

		FUNCTION_FINISH;
		return TRUE;
	}

	FUNCTION_FINISH;
	return FALSE;
}

/**
 ****************************************************************************************************
	\fn			bool IsKeyDown( const int i_virtualKeyCode )
	\brief		Check whether the keyboard key is down
	\param		i_virtualKeyCode the ASCII code of the key
	\return		boolean
	\retval		TRUE if the key is pressed from up
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::UserInput::IsKeyDown( const int i_virtualKeyCode )
{
	FUNCTION_START;

	if( (prevKeyState[i_virtualKeyCode] != E_KEY_STATE_DOWN) && (currKeyState[i_virtualKeyCode] == E_KEY_STATE_DOWN) )
	{
		E_KEY_STATE prevState = prevKeyState[i_virtualKeyCode];
		E_KEY_STATE currState = currKeyState[i_virtualKeyCode];

		FUNCTION_FINISH;
		return TRUE;
	}

	FUNCTION_FINISH;
	return FALSE;
}

/**
 ****************************************************************************************************
	\fn			bool IsKeyPressed( const int i_virtualKeyCode )
	\brief		Check whether the keyboard key is pressed
	\param		i_virtualKeyCode the ASCII code of the key
	\return		boolean
	\retval		TRUE if the key is pressed
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::UserInput::IsKeyPressed( const int i_virtualKeyCode )
{
	FUNCTION_START;

	FUNCTION_FINISH;
	return IsVirtualKeyPressed( i_virtualKeyCode );
}

/**
 ****************************************************************************************************
	\fn			bool IsButtonPressed( const int i_virtualButtonCode )
	\brief		Check whether the mouse button is pressed
	\param		i_virtualButtonCode the mouse button code
	\return		boolean
	\retval		TRUE if the key is pressed
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::UserInput::IsButtonPressed( const int i_virtualButtonCode )
{
	FUNCTION_START;

	FUNCTION_FINISH;
	return IsVirtualKeyPressed( i_virtualButtonCode );
}

/**
 ****************************************************************************************************
	\fn			bool IsVirtualKeyPressed( const int i_virtualKeyCode )
	\brief		Check whether the given key code is pressed
	\param		i_virtualKeyCode keyboard key or mouse button code
	\return		boolean
	\retval		TRUE if the key is pressed
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::UserInput::IsVirtualKeyPressed( const int i_virtualKeyCode )
{
	FUNCTION_START;

	if( g_mainWindow::Get().GetHandle() != GetForegroundWindow() )
		return false;

	short keyState = GetAsyncKeyState( i_virtualKeyCode );
	const short isKeyDownMask = ~1;

	FUNCTION_FINISH;
	return ( keyState & isKeyDownMask ) != 0;
}