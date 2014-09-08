/**
 ****************************************************************************************************
 * \file		Window.cpp
 * \brief		Window class declaration
 ****************************************************************************************************
*/

#include <Windows.h>

#include "Window.h"
#include "../Logging/Logging.h"
#include "../Resources/resource.h"
#include "../Renderer/MainRenderer.h"

namespace RendererEngine
{
	std::string GetLastWindowsError( void );
}

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( const HINSTANCE i_thisInstanceOfTheApplication,
				const int i_initialWindowDisplayState, const char* i_windowName,
				const UINT32 &i_u32Width, const UINT32 &i_u32Height )
	\brief		Initialize window
	\param		i_thisInstanceOfTheApplication the instance of the application
	\param		i_initialWindowDisplayState the initial window display state
	\param		i_windowName the name of the window
	\param		i_u32Width the width of the window
	\param		i_u32Height the height of the window
	\return		boolean
	\retval		SUCCESS if success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Window::Initialize( const HINSTANCE &i_thisInstanceOfTheApplication,
	const int &i_initialWindowDisplayState, const char* i_windowName,
	const UINT32 &i_u32Width, const UINT32 &i_u32Height )
{
	_applicationInstance = i_thisInstanceOfTheApplication;
	_windowName = i_windowName;
	m_u32Width = i_u32Width;
	m_u32Height = i_u32Height;

	if( !RegisterClass() )
		return FAIL;

	if( !Create(i_initialWindowDisplayState) )
		return FAIL;

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool SetWindow( const HWND &i_hwnd, const UINT32 &i_u32Width, const UINT32 &i_u32Height )
	\brief		Set window handle the pre-created window handle
	\param		i_hwnd window handle
	\param		i_u32Width the width of the window
	\param		i_u32Height the height of the window
	\return		boolean
	\retval		SUCCESS if success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Window::SetWindow( const HWND &i_hwnd, const UINT32 &i_u32Width, const UINT32 &i_u32Height )
{
	m_u32Width = i_u32Width;
	m_u32Height = i_u32Height;

	_window = i_hwnd;

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool ShutDown( void )
	\brief		Shut down the window
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Window::ShutDown( void )
{
	Destroy();
	UnregisterClass();

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			HWND GetHandle( void ) const
	\brief		Get the window handle
	\param		NONE
	\return		HWND
	\retval		The handle of the window
 ****************************************************************************************************
*/
HWND RendererEngine::Window::GetHandle( void ) const
{
	return _window;
}

/****************************************************************************************************
			Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			Window( void )
	\brief		Default constructor of window
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
RendererEngine::Window::Window( void ) :
	_applicationInstance( NULL ),
	_class( NULL ),
	_window( NULL )
{
}

/**
 ****************************************************************************************************
	\fn			~Window( void )
	\brief		Default destructor of window
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
RendererEngine::Window::~Window( void )
{
	ShutDown();
}

/**
 ****************************************************************************************************
	\fn			bool Create( const int i_initialWindowDisplay )
	\brief		Create the window
	\param		i_initialWindowDisplay the initial state of the window
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Window::Create( const int i_initialWindowDisplayState )
{
	// The window's style
	DWORD windowStyle =
		// "Overlapped" is basically the same as "top-level"
		WS_OVERLAPPED
		// The caption is the title bar when in windowed-mode
		| WS_CAPTION
		// The window should never change dimensions, so only a minimize box is allowed
		| WS_MINIMIZEBOX
		// The system menu appears when you right-click the title bar
		| WS_SYSMENU;
	// The window's extended style
	DWORD windowStyle_extended =
		// The following is a macro to make the extended style the default top-level look
		WS_EX_OVERLAPPEDWINDOW;
	// The width and height of the window
	// (The renderer is concerned about the width and height of the actual "client area",
	// which is the part of the window that doesn't include the borders and title bar.
	// the initial window, then, will created with default values that Windows chooses
	// and then resized after creation)
	int width = CW_USEDEFAULT;
	int height = CW_USEDEFAULT;
	// The initial position of the window
	// (We don't care, and will let Windows decide)
	int position_x = CW_USEDEFAULT;
	int position_y = CW_USEDEFAULT;
	// Handle to the parent of this window
	// (Since this is our main window, it can't have a parent)
	HWND hParent = NULL;
	// Handle to the menu for this window
	HMENU hMenu = NULL;
	// Handle to the instance of the application this window should be associated with
	HINSTANCE hApplication = _applicationInstance;
	// Any arbitrary pointer can be associated with this window;
	// usually an application's representation of the window will be used, like this:
	void* userData = this;
	// (Since the main window is a global singleton, though, this program won't actually use this)

	// Ask Windows to create the specified window.
	// CreateWindowEx() will return a handle to the window,
	// which is what we'll use to communicate with Windows about this window
	_window = CreateWindowEx( windowStyle_extended, _windowName, _windowName, windowStyle,
		position_x, position_y, width, height,
		hParent, hMenu, hApplication, userData );
	if ( _window )
	{
		// Change the window's size based on the requested user settings
		{
			// Calculate how much of the window is coming from the "non-client area"
			// (the borders and title bar)
			RECT windowCoordinates;
			struct
			{
				long width;
				long height;
			} nonClientAreaSize;
			{
				// Get the coordinates of the entire window
				if ( GetWindowRect( _window, &windowCoordinates ) == FAIL )
				{
					LogMessage( std::string( "Windows failed to get the coordinates of the main window: " ) + GetLastWindowsError() );
					return FAIL;
				}
				// Get the dimensions of the client area
				RECT clientDimensions;
				if ( GetClientRect( _window, &clientDimensions ) == FAIL )
				{
					LogMessage( std::string( "Windows failed to get the dimensions of the main window's client area: " )
						+ GetLastWindowsError() );
					return FAIL;
				}
				// Get the difference between them
				nonClientAreaSize.width = ( windowCoordinates.right - windowCoordinates.left ) - clientDimensions.right;
				nonClientAreaSize.height = ( windowCoordinates.bottom - windowCoordinates.top ) - clientDimensions.bottom;
			}
			// Resize the window
			{
				BOOL shouldTheResizedWindowBeRedrawn = SUCCESS;
				if ( MoveWindow( _window, windowCoordinates.left, windowCoordinates.top,
					m_u32Width + nonClientAreaSize.width, m_u32Height + nonClientAreaSize.height,
					shouldTheResizedWindowBeRedrawn ) == FAIL )
				{
					LogMessage( std::string( "Windows failed to resize the main window: " ) + GetLastWindowsError() );
					return FAIL;
				}
			}
		}

		// Display the window in the initial state that Windows requested
		ShowWindow( _window, i_initialWindowDisplayState );

		LogMessage( "Created the main window" );
		return SUCCESS;
	}
	else
	{
		LogMessage( std::string( "Windows failed to create the main window: " ) + GetLastWindowsError() );
		return FAIL;
	}
}

/**
 ****************************************************************************************************
	\fn			bool Destroy( void )
	\brief		Destroy the window
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Window::Destroy( void )
{
	bool bErrorFound = FAIL;

	// If the window is created outside, then no need to destroy it here
	if( !_applicationInstance )
		return SUCCESS;

	if ( _window )
	{
		if ( DestroyWindow( _window ) == SUCCESS )
			LogMessage( "Destroyed the main window" );
		else
		{
			LogMessage( std::string( "Windows failed to destroy the main window: " ) + GetLastWindowsError() );
			bErrorFound = SUCCESS;
		}
		_window = NULL;
	}

	return !bErrorFound;
}

/**
 ****************************************************************************************************
	\fn			bool RegisterClass( void )
	\brief		Register the window class
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Window::RegisterClass( void )
{
	WNDCLASSEX wndClassEx = { 0 };
	wndClassEx.cbSize = sizeof( WNDCLASSEX );
	wndClassEx.hInstance = _applicationInstance;

	// The class's style
	// (We don't have to worry about any of these)
	wndClassEx.style = 0;
	// The function that will process all of the messages
	// that Windows will send to windows of this class
	wndClassEx.lpfnWndProc = OnMessageReceived;
	// Extra bytes can be set aside in the class for user data
	// (This is rarely used, but can be useful)
	wndClassEx.cbClsExtra = 0;
	// Extra bytes can be set aside for each window of the class,
	// but this is usually specified for each window individually
	wndClassEx.cbWndExtra = 0;
	// The large and small icons windows of this class should use
	// (These can be found in the Resources folder; feel free to change them)
	wndClassEx.hIcon = LoadIcon( _applicationInstance, MAKEINTRESOURCE( IDI_RENDERER ) );
	wndClassEx.hIconSm = LoadIcon( _applicationInstance, MAKEINTRESOURCE( IDI_SMALL ) );
	// The cursor that should display when the mouse pointer is over windows of this class
	wndClassEx.hCursor = LoadCursor( NULL, IDC_ARROW );
	// The brush windows of this class will use as a background
	// (Setting this is a bit confusing, so don't be alarmed if the next line looks scary)
	wndClassEx.hbrBackground = reinterpret_cast<HBRUSH>( IntToPtr( COLOR_BACKGROUND + 1 ) );
	// A menu can be specified that all windows of this class would use by default,
	// but usually this is set for each window individually
	wndClassEx.lpszMenuName = NULL;
	// The class name (see comments where this is initialized)
	wndClassEx.lpszClassName = _windowName;

	// Now we can provide this information to Windows.
	// If all goes well, the class will be successfully registered
	// and we can specify it by name when creating our windows.
	_class = RegisterClassEx( &wndClassEx );
	if ( _class != NULL )
	{
		LogMessage( "Registered the main window class" );
		return SUCCESS;
	}
	else
	{
		LogMessage( std::string( "Windows failed to register the main window's class: " ) + GetLastWindowsError() );
		return FAIL;
	}
}

/**
 ****************************************************************************************************
	\fn			bool UnregisterClass( void )
	\brief		Unregister window class
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Window::UnregisterClass( void )
{
	bool bErrorFound = FAIL;

	if ( _class )
	{
		if ( ::UnregisterClass( _windowName, _applicationInstance ) == SUCCESS )
		{
			LogMessage( "Unregistered the main window class" );
		}
		else
		{
			LogMessage( std::string( "Windows failed to unregister the main window's class: " ) + GetLastWindowsError() );
		}
		_class = NULL;
	}

	return !bErrorFound;
}

/**
 ****************************************************************************************************
	\fn			LRESULT CALLBACK OnMessageReceived( HWND i_window, UINT i_message, WPARAM i_wParam, LPARAM i_lParam )
	\brief		On message received callback
	\param		i_window the window instance
	\param		i_message the message received
	\param		i_wParam the w parameter
	\param		i_lParam the l parameter
	\return		NONE
 ****************************************************************************************************
*/
LRESULT CALLBACK RendererEngine::Window::OnMessageReceived( HWND i_window, UINT i_message, WPARAM i_wParam, LPARAM i_lParam )
{
	// DispatchMessage() will send messages to the main window here.
	// There are many messages that get sent to a window,
	// but the renderer application can ignore most of them
	// and let Windows handle them in the default way.

	// Process any messages that the renderer cares about
	// (any messages that are processed here should return a value
	// rather than letting the default processing function try to handle them a second time)
	switch( i_message )
	{
	case WM_CHAR:
		{
			// A key has been pressed down, and this is the translated character
			//------------------------------------------------------------------

			// This isn't usually the best way to handle keyboard input,
			// but it is a convenient way to handle an exit key
			if ( i_wParam == VK_ESCAPE )
			{
				LogMessage( "The escape key was pressed; the application will now exit" );

				int exitCode = 0;
				PostQuitMessage( exitCode );
			}

			break;
		}
	case WM_NCDESTROY:
		{
			// The window's nonclient area is being destroyed
			//-----------------------------------------------

			LogMessage( "Main window is being destroyed" );

			// This is the last message a window will receive
			// (Any child windows have already been destroyed).
			// After this message has been processed the window's handle will be invalid
			g_mainWindow::Get()._window = NULL;

			// When the main window is destroyed
			// a WM_QUIT message should be sent
			// (if this isn't done the application would continue to run with no window).
			// This is where the exitCode in cMainWindow::WaitForShutdown() comes from:
			int exitCode = 0;
			PostQuitMessage( exitCode );

			break;
		}
	}

	// Pass any messages that weren't handled on to Windows
	return DefWindowProc( i_window, i_message, i_wParam, i_lParam );
}

/****************************************************************************************************
			Helper function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			std::string GetLastWindowsError( void )
	\brief		Get window error information
	\param		NONE
	\return		std::string
	\retval		The window error information
 ****************************************************************************************************
*/
std::string RendererEngine::GetLastWindowsError( void )
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
