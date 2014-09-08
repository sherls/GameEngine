/**
 ****************************************************************************************************
 * \file		GameEngine.h
 * \brief		Game Engine global function header
 ****************************************************************************************************
*/

#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include "Utilities/GameEngineTypes.h"

namespace GameEngine
{
	bool StartUp( const char *i_filename );
	bool Initialize( const char* i_GameName, UINT16 i_u16Width, UINT16 i_u16Height,
		const HINSTANCE &i_currInstance, const int &i_initialWindowDisplayState, const HWND &i_hwnd );
	void BeginUpdate( void );
	void Update( void );
	void EndUpdate( void );
	void ShutDown( void );

	bool IsDebugMenuActivated( void );
	void SetDebugMenu( const bool i_state );
}

#endif