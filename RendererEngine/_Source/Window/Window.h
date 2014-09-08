/**
 ****************************************************************************************************
 * \file		Window.h
 * \brief		Window class declaration
 ****************************************************************************************************
*/

#ifndef _WINDOW_H_
#define _WINDOW_H_

// Utilities header
#include <Singleton/Singleton.h>

#include "../RendererEngineTypes.h"

namespace RendererEngine
{
	class Window
	{
		friend Utilities::Singleton<Window>;

		const char* _windowName;
		HINSTANCE _applicationInstance;
		ATOM _class;
		HWND _window;

		Window( void );
		~Window( void );

		bool Create( const int i_initialWindowDisplayState );
		bool Destroy( void );
		bool RegisterClass( void );
		bool UnregisterClass( void );

		static LRESULT CALLBACK OnMessageReceived( HWND i_window, UINT i_message, WPARAM i_wParam, LPARAM i_lParam );

	public:
		UINT32 m_u32Width;
		UINT32 m_u32Height;

		bool Initialize( const HINSTANCE &i_thisInstanceOfTheApplication, const int &i_initialWindowDisplayState, const char* i_windowName,
			const UINT32 &i_u32Width, const UINT32 &i_u32Height );
		bool SetWindow( const HWND &i_hwnd, const UINT32 &i_u32Width, const UINT32 &i_u32Height );
		bool ShutDown( void );

		HWND GetHandle( void ) const;
	};
}	// namespace RendererEngine

typedef Utilities::Singleton<RendererEngine::Window> g_mainWindow;

#endif	// #ifndef _WINDOW_H_