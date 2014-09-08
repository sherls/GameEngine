/**
 ****************************************************************************************************
 * \file		GameEngine.cpp
 * \brief		Game Engine global function implementation
 ****************************************************************************************************
*/

#include <assert.h>

// Utilities header
#include <Utilities.h>
#include <Debug/Debug.h>
#include <UtilitiesDefault.h>
#include <SmartPtr/SmartPtr.h>
#ifdef _DEBUG
	#include <BitWise/BitWise.h>
	#include <MemoryPool/MemoryPool.h>
#endif	// #ifdef _DEBUG

#include "AI/AI.h"
#include "GameEngine.h"
#include "Audio/Audio.h"
#include "World/World.h"
#include "Camera/Camera.h"
#include "Network/Network.h"
#include "Physics/Physics.h"
#include "Renderer/Renderer.h"
#include "Collision/Collision.h"
#include "DebugMenu/DebugMenu.h"
#include "Messaging/Messaging.h"
#include "UserInput/UserInput.h"
#include "TriggerBox/TriggerBox.h"
#include "Utilities/Profiler/Profiler.h"
#include "Utilities/IDCreator/IDCreator.h"
#include "Utilities/Configuration/Configuration.h"

#ifdef _DEBUG
	#include "UnitTest/UnitTest.h"
	#include "Math/Matrix/Matrix.h"
	#include "Math/Vector3/FastVector3.h"
#endif	// #ifdef _DEBUG

/****************************************************************************************************
			Global variables
****************************************************************************************************/
namespace GameEngine
{
	bool bDebugMenuActivated = false;
	bool bEngineInitialized = false;
	bool bInFrame = false;
}

/****************************************************************************************************
			Function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool StartUp( const char *i_filename )
	\brief		Starting up game engine
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::StartUp( const char *i_filename )
{
	FUNCTION_START;

	if( !Configuration::LoadMasterConfig(i_filename) )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool Initialize( const char* i_GameName, UINT16 i_u16Width, UINT16 i_u16Height )
	\brief		Initialize game engine
	\param		*i_GameName title of the game
	\param		i_u16Width screen width
	\param		i_u16Height screen height
	\param		i_currInstance current window instance
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::Initialize( const char* i_GameName, UINT16 i_u16Width, UINT16 i_u16Height,
	const HINSTANCE &i_currInstance, const int &i_initialWindowDisplayState, const HWND &i_hwnd )
{
	FUNCTION_START;

	if( !Utilities::Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !g_debugMenu::Get().Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !Network::Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !UserInput::Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !Messaging::Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !g_world::Get().Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !AI::Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !Physics::Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !Renderer::Initialize(i_GameName, i_u16Width, i_u16Height, i_currInstance, i_initialWindowDisplayState, i_hwnd) )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !Collision::Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !TriggerBox::Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !g_debugMenu::Get().SetUpDebugMenuRenderer() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !Audio::Initialize() )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

#ifdef _DEBUG
	Math::FastVector3::UnitTest();
	RingBufferUnitTest();
	FloatNumberPrecisionTest();
	Utilities::BitWise::UnitTest();
	Utilities::MemoryPool::UnitTest();
	Math::Matrix::UnitTest();
#endif	// #ifdef _DEBUG

	bEngineInitialized = true;

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin updating game engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::BeginUpdate( void )
{
	FUNCTION_START;

	Network::BeginUpdate();

	if( Network::IsServer() || Network::IsConnected() )
	{
		UserInput::Update();
		Utilities::Time::OnNewFrame();

		g_world::Get().BeginUpdate();
		AI::BeginUpdate();
		Physics::BeginUpdate();
		Collision::BeginUpdate();
		TriggerBox::BeginUpdate();
		Renderer::BeginUpdate();
		g_debugMenu::Get().BeginUpdate();
		Audio::BeginUpdate();
	}

	bInFrame = true;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Updating game engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Update( void )
{
	float frameTime = Utilities::Time::GetTimeElapsedThisFrame_ms();

	PROFILE_SCOPE_BEGIN( "Game Engine Update" );
	FUNCTION_START;

#ifdef SHOW_FPS
	char buffer[50];
	sprintf_s( buffer, "FPS: %f", 1/(frameTime/1000) );
	g_debugMenu::Get().Log( buffer );
#endif	// #ifdef SHOW_FPS

	assert( bInFrame );

	Network::Update();

	if( Network::IsServer() || Network::IsConnected() )
	{
		g_world::Get().Update();
		AI::Update();
		Physics::Update();
		Collision::Update();
		TriggerBox::Update();
		if( g_world::Get().m_camera )
			g_world::Get().m_camera->Update();
		Renderer::Update();
		g_debugMenu::Get().Update();
		Renderer::UpdateGUI();
		g_debugMenu::Get().UpdateGUI();
		Audio::Update();
	}

	FUNCTION_FINISH;
	PROFILE_SCOPE_END();
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End updating game engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::EndUpdate( void )
{
	FUNCTION_START;

	bInFrame = false;

	if( Network::IsServer() || Network::IsConnected() )
	{
		TriggerBox::EndUpdate();
		Collision::EndUpdate();
		AI::EndUpdate();
		g_world::Get().EndUpdate();
		Physics::EndUpdate();
		if( g_world::Get().m_camera )
			g_world::Get().m_camera->EndUpdate();
		Renderer::EndUpdate();
		g_debugMenu::Get().EndUpdate();
		Audio::EndUpdate();
	}

	Network::EndUpdate();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Destroy game engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::ShutDown( void )
{
	FUNCTION_START;

	if( bEngineInitialized )
	{
		Renderer::ShutDown();
		TriggerBox::ShutDown();
		Collision::ShutDown();
		Physics::ShutDown();
		AI::ShutDown();
		g_world::Get().ShutDown();
		Messaging::ShutDown();
		Configuration::UnloadMasterConfig();
		Network::ShutDown();
		g_debugMenu::Get().ShutDown();
		Utilities::ShutDown();
		Audio::ShutDown();
		bEngineInitialized = false;
	}

	PROFILE_PRINT_RESULTS();

	g_debugMenu::Release();
	g_world::Release();
	g_IDCreator::Release();
	g_Profiler::Release();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool IsDebugMenuActivated( void )
	\brief		Check whether debug menu is activated
	\param		NONE
	\return		boolean
	\retval		TRUE if debug menu is currently activated
	\retval		FALSE if debug menu is currently not activated
 ****************************************************************************************************
*/
bool GameEngine::IsDebugMenuActivated( void )
{
	return bDebugMenuActivated;
}

/**
 ****************************************************************************************************
	\fn			void SetDebugMenu( bool &i_state )
	\brief		Set the debug menu status
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::SetDebugMenu( const bool i_state )
{
	bDebugMenuActivated = i_state;
}