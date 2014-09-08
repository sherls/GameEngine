/**
 ****************************************************************************************************
 * \file		main.cpp
 * \brief		Main procedure
 ****************************************************************************************************
*/

#include <assert.h>
#include <crtdbg.h>
#include <Windows.h>

//#include "vld.h"

// Utilities header
#include <Debug/Debug.h>

// GameEngine
#include "GameEngine.h"
#include "Audio/Audio.h"
#include "Network/Network.h"
#include "Utilities/GameEngineTypes.h"

#include "CaptureTheFlag.h"
#include "GlobalConstant.h"
#include "NetworkManager.h"

/****************************************************************************************************
		Private functions
****************************************************************************************************/
void HandleWindowMessage( bool &o_quit )
{
	// Any time something happens that Windows cares about, it will send the main window a message.
	// One of the messages it sends is that the application should quit.
	// This can be sent when a user closes the window,
	// but we can also tell Windows to send a quit message if the user chooses a quit option from within the renderer

	// Enter an infinite loop that will continue until a quit message (WM_QUIT) is received from Windows
	MSG message = { 0 };
	do
	{
		// To send us a message, Windows will add it to a queue.
		// Most Windows applications should wait until a message is received and then react to it.
		// Real-time renderers though, must continually draw new images to the screen as fast as possible
		// and only pause momentarily when there is a Windows message to deal with.

		// This means that the first thing that must be done every iteration of the render loop is to "peek" at the message queue
		// and see if there are any messages from Windows that need to be handled
		bool hasWindowsSentAMessage;
		{
			HWND getMessagesFromAnyWindowBelongingToTheCurrentThread = NULL;
			unsigned int getAllMessageTypes = 0;
			unsigned int ifAMessageExistsRemoveItFromTheQueue = PM_REMOVE;
			hasWindowsSentAMessage = PeekMessage( &message, getMessagesFromAnyWindowBelongingToTheCurrentThread,
				getAllMessageTypes, getAllMessageTypes, ifAMessageExistsRemoveItFromTheQueue ) == SUCCESS;
		}
		if ( !hasWindowsSentAMessage )
		{
			// Usually there will be no messages in the queue, and the renderer can run
			o_quit = false;
			return;
		}
		else
		{
			// If Windows _has_ sent a message, this iteration of the loop will handle it

			// First, the message must be "translated"
			// (Key presses are translated into character messages)
			TranslateMessage( &message );

			// Then, the message is sent on to the appropriate processing function.
			// This function is specified in the lpfnWndProc field of the WNDCLASSEX struct
			// used to register a class with Windows.
			// In the case of the main window it will be cMainWindow::OnMessageReceived()
			DispatchMessage( &message );
		}
	} while ( message.message != WM_QUIT );

	o_quit = true;
}

int WINAPI wWinMain( HINSTANCE hCurrInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	// Enable run-time memory check for debug builds.
#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	// #ifdef _DEBUG

	LPWSTR commandLine = GetCommandLine();
	std::wstring commands = commandLine;

#ifdef _DEBUG
	//GameEngine::DEBUG_SET_GROUP( DBG_RING_BUFFER, DBG_ON );
	//GameEngine::DEBUG_SET_GROUP( DBG_NAME, GameEngine::DBG_ON );
	//GameEngine::DEBUG_SET_GROUP( DBG_COLLISION, GameEngine::DBG_ON );
	//GameEngine::DEBUG_SET_GROUP( DBG_MATRIX, GameEngine::DBG_ON );
	Utilities::DEBUG_SET_LEVEL( D_INFO, Utilities::DBG_ON );
	Utilities::DEBUG_SET_LEVEL( D_ERR, Utilities::DBG_ON );
	//GameEngine::DEBUG_SET_LEVEL( D_UNIT_TEST, GameEngine::DBG_ON );
	//GameEngine::DEBUG_SET_LEVEL( D_FUNC, DBG_ON );
#endif

#ifdef ENABLE_PROFILING
	GameEngine::DEBUG_SET_LEVEL( D_PROFILER, GameEngine::DBG_ON );
#endif	// #ifdef ENABLE_PROFILING

	if( !GameEngine::StartUp( "../../External/master.cfg") )
	{
		GameEngine::ShutDown();
		return 1;
	}

	if( !g_captureTheFlag::Get().StartUp() )
	{
		g_captureTheFlag::Get().ShutDown();
		GameEngine::ShutDown();
		return 1;
	}

	if( commands.find(L"--disable_bg_audio") != std::string::npos )
		GameEngine::Audio::DisableBgAudio();

	if( commands.find(L"--disable_2d_sfx") != std::string::npos )
		GameEngine::Audio::Disable2DSfx();

	if( commands.find(L"--disable_3d_sfx") != std::string::npos )
		GameEngine::Audio::Disable3DSfx();

	if( commands.find(L"--s") != std::string::npos )
		GameEngine::Network::IsServer( true );

	if( commands.find(L"--c") != std::string::npos )
		GameEngine::Network::IsServer( false );

	if( !GameEngine::Initialize("Capture The Flag",
		static_cast<UINT16>(g_captureTheFlag::Get().GetWindowSize().width),
		static_cast<UINT16>(g_captureTheFlag::Get().GetWindowSize().height),
		hCurrInstance, nCmdShow, 0)
	)
		return 1;

	// Wait till the connection is made
	while( !GameEngine::Network::IsServer() && !GameEngine::Network::IsConnected() 
		&& !g_captureTheFlag::Get().m_bNetworkReady )
	{
		g_networkManager::Get().BeginUpdate();

		g_networkManager::Get().Update();

		g_networkManager::Get().EndUpdate();
	}

	if( GameEngine::Network::IsServer() && !g_captureTheFlag::Get().Initialize() )
	{
		GameEngine::ShutDown();
		return 1;
	}

	bool bQuit = false;

	while( !bQuit )
	{
		HandleWindowMessage( bQuit );
		if( bQuit )
			break;

		GameEngine::BeginUpdate();
		g_networkManager::Get().BeginUpdate();
		g_captureTheFlag::Get().BeginUpdate();

		GameEngine::Update();
		g_networkManager::Get().Update();
		g_captureTheFlag::Get().Update();

		GameEngine::EndUpdate();
		g_networkManager::Get().EndUpdate();
		g_captureTheFlag::Get().EndUpdate();

		bQuit = g_captureTheFlag::Get().IsGameOver();
	}

	g_captureTheFlag::Get().ShutDown();
	GameEngine::ShutDown();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif	// #ifdef _DEBUG

	return 0;
}