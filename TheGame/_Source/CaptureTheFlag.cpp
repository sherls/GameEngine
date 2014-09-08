/**
 ****************************************************************************************************
 * \file		MonsterChase.cpp
 * \brief		Monster chase implementation and control
 ****************************************************************************************************
*/

#include <vector>
#include <fstream>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <libconfig.hpp>

// Utilities header
#include <Debug/Debug.h>
#include <Parser/MeshParser/MeshParser.h>
#include <Parser/SceneParser/SceneParser.h>

// Game Engine
#include "AI/AI.h"
#include "Audio/Audio.h"
#include "World/World.h"
#include "World/Entity.h"
#include "Camera/Camera.h"
#include "Renderer/Renderer.h"
#include "Collision/Collision.h"
#include "DebugMenu/DebugMenu.h"
#include "Messaging/Messaging.h"
//#include "Collision/BoundingBox.h"
#include "Utilities/GameEngineTypes.h"
#include "Utilities/IDCreator/IDCreator.h"
#include "Utilities/Configuration/Configuration.h"

// Capture the flag
#include "CaptureTheFlag.h"
#include "GlobalConstant.h"
#include "NetworkManager.h"
#include "ActorController/FlagController.h"
#include "ActorController/EnemyController.h"
#include "ActorController/PlayerController.h"

/****************************************************************************************************
			Macros
****************************************************************************************************/

/****************************************************************************************************
			Global variables
****************************************************************************************************/

/****************************************************************************************************
			Private functions declaration
****************************************************************************************************/
void ResetFlagPosition( void );
void TagMessageHandler( void *i_sender );
void EnemyScoreMessageHandler( void *i_sender );
void PlayerScoreMessageHandler( void *i_sender );

/****************************************************************************************************
			Global functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool StartUp( void )
	\brief		Starting up game
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool CaptureTheFlag::StartUp( void )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	libconfig::Setting *gameSettings = config.lookup( "CaptureTheFlag" );
	const libconfig::Setting &root = config.getRoot();

	FUNCTION_START;

	if( gameSettings )
	{
		INT32 temp;

		const libconfig::Setting &windowSetting = root["CaptureTheFlag"]["window"];
		if( !windowSetting.lookupValue("width", temp) )
		{
			DBG_CONFIG_ERROR( "window width", "CaptureTheFlag" );

			FUNCTION_FINISH;
			return FAIL;
		}
		else
			m_windowSize.width = static_cast<float> (temp);
	
		if( !windowSetting.lookupValue("height", temp) )
		{
			DBG_CONFIG_ERROR( "window height", "CaptureTheFlag" );

			FUNCTION_FINISH;
			return FAIL;
		}
		else
			m_windowSize.height = static_cast<float> (temp);
	}

	m_bNetworkReady = false;
	m_bPlayerHasFlag = false;
	m_bEnemyHasFlag = false;
	m_u32PlayerScore = 0;
	m_u32EnemyScore = 0;

	m_playerTeam = Utilities::StringHash( "BlueFlag" );
	m_enemyTeam = Utilities::StringHash( "RedFlag" );
	m_u8PlayerAreaID = g_IDCreator::Get().GetID( "BlueFlagArea" );
	m_u8EnemyAreaID = g_IDCreator::Get().GetID( "RedFlagArea" );
	/*m_playerTeam = Utilities::StringHash( "RedFlag" );
	m_enemyTeam = Utilities::StringHash( "BlueFlag" );
	m_u8PlayerAreaID = g_IDCreator::Get().GetID( "RedFlagArea" );
	m_u8EnemyAreaID = g_IDCreator::Get().GetID( "BlueFlagArea" );*/

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize capture the flag
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool CaptureTheFlag::Initialize( void )
{
	FUNCTION_START;

	CreateEnemy();
	CreatePlayer();
	CreateEntities();
	CreatePointLight();
	CreateDirectionalLight();
	CreateCollisionEntities();
	LoadWorldConfiguration();

	InitializeSfx();

	GameEngine::Messaging::CreateMessage( "EnemyScore", 2 );
	GameEngine::Messaging::AddMessageHandler( "EnemyScore", EnemyScoreMessageHandler, 2 );

	GameEngine::Messaging::CreateMessage( "PlayerScore", 2 );
	GameEngine::Messaging::AddMessageHandler( "PlayerScore", PlayerScoreMessageHandler, 2 );

	GameEngine::Messaging::CreateMessage( "Tag", 1 );
	GameEngine::Messaging::AddMessageHandler( "Tag", TagMessageHandler, 1 );

	GameEngine::Audio::PlayBackgroundMusic( "Chiptune_Does_Dubstep.mp3" );

	g_networkManager::Get().Initialize();

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool Initialize( const D3DXVECTOR3 &i_playerPosition, const D3DXVECTOR3 &i_enemyPosition )
	\brief		Initialize capture the flag
	\param		i_playerPosition initial player position
	\param		i_enemyPosition initial enemy position
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool CaptureTheFlag::Initialize( const D3DXVECTOR3 &i_playerPosition, const D3DXVECTOR3 &i_enemyPosition )
{
	FUNCTION_START;

	CreateEnemy( i_enemyPosition );
	CreatePlayer( i_playerPosition );
	CreateEntities();
	CreatePointLight();
	CreateDirectionalLight();
	CreateCollisionEntities();
	LoadWorldConfiguration();

	InitializeSfx();

	GameEngine::Messaging::CreateMessage( "EnemyScore", 2 );
	GameEngine::Messaging::AddMessageHandler( "EnemyScore", EnemyScoreMessageHandler, 2 );

	GameEngine::Messaging::CreateMessage( "PlayerScore", 2 );
	GameEngine::Messaging::AddMessageHandler( "PlayerScore", PlayerScoreMessageHandler, 2 );

	GameEngine::Messaging::CreateMessage( "Tag", 1 );
	GameEngine::Messaging::AddMessageHandler( "Tag", TagMessageHandler, 1 );

	GameEngine::Audio::PlayBackgroundMusic( "Chiptune_Does_Dubstep.mp3" );

	g_networkManager::Get().Initialize();

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin updating game object
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::BeginUpdate( void )
{
	char* playerScore = new char[128];
	char* EnemyScore = new char[128];

	FUNCTION_START;

	sprintf_s( playerScore, 128, "Player score: %d", m_u32PlayerScore );
	GameEngine::Renderer::DrawText( D3DXVECTOR2(16, 16), D3DXVECTOR2(100, 32), Utilities::WHITE, \
		playerScore, DT_LEFT );

	sprintf_s( EnemyScore, 128, "Enemy score: %d", m_u32EnemyScore );
	GameEngine::Renderer::DrawText( D3DXVECTOR2(905, 16), D3DXVECTOR2(100, 32), Utilities::WHITE, \
		EnemyScore, DT_RIGHT );

	FUNCTION_FINISH;
	delete []playerScore;
	delete []EnemyScore;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update game object
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::Update( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End updating game object
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::EndUpdate( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shut down game object
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::ShutDown( void )
{
	FUNCTION_START;

	g_captureTheFlag::Release();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool IsGameOver( void )
	\brief		Check whether game is ended
	\param		NONE
	\return		boolean
	\retval		true [Quit] game is ended
	\retval		false otherwise
 ****************************************************************************************************
*/
const bool CaptureTheFlag::IsGameOver( void ) const
{
	FUNCTION_START;

	FUNCTION_FINISH;
	return PlayerController::IsPlayerQuit();
}

/**
 ****************************************************************************************************
	\fn			GameEngine::S_SIZE MonsterChase::GetWindowSize( void )
	\brief		Get window size
	\param		NONE
	\return		Cheesy::Point2D
	\retval		Size of this game window
 ****************************************************************************************************
*/
const Utilities::S_SIZE &CaptureTheFlag::GetWindowSize( void ) const
{
	return m_windowSize;
}

/**
 ****************************************************************************************************
	\fn			void CreateEnemy( void )
	\brief		Create enemy
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::CreateEnemy( void )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	const libconfig::Setting &root = config.getRoot();
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 scale;
	float friction;
	std::string meshFile;
	std::string collisionFile;
	UINT32 u32CollisionMask = 0;
	UINT8 u8ArrayCount = 0;
	Utilities::S_SIZE size;

	FUNCTION_START;

	const libconfig::Setting &enemySetting = root["CaptureTheFlag"]["enemy"];

	const libconfig::Setting &enemyPosition = root["CaptureTheFlag"]["enemy"]["position"];
	if( !enemyPosition.lookupValue("x", position.x) )
	{
		DBG_CONFIG_ERROR( "enemy x position", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemyPosition.lookupValue("y", position.y) )
	{
		DBG_CONFIG_ERROR( "enemy y position", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemyPosition.lookupValue("z", position.z) )
	{
		DBG_CONFIG_ERROR( "enemy z position", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	// Look at
	const libconfig::Setting &enemyLookAt = root["CaptureTheFlag"]["enemy"]["lookAt"];
	if( !enemyLookAt.lookupValue("x", lookAt.x) )
	{
		DBG_CONFIG_ERROR( "enemy x look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemyLookAt.lookupValue("y", lookAt.y) )
	{
		DBG_CONFIG_ERROR( "enemy y look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemyLookAt.lookupValue("z", lookAt.z) )
	{
		DBG_CONFIG_ERROR( "enemy z look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	// Size
	const libconfig::Setting &enemySize = root["CaptureTheFlag"]["enemy"]["size"];
	if( !enemySize.lookupValue("width", size.width) )
	{
		DBG_CONFIG_ERROR( "enemy width", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemySize.lookupValue("height", size.height) )
	{
		DBG_CONFIG_ERROR( "enemy height", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemySize.lookupValue("depth", size.depth) )
	{
		DBG_CONFIG_ERROR( "enemy depth", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	const libconfig::Setting &enemyScale = root["CaptureTheFlag"]["enemy"]["scale"];
	if( !enemyScale.lookupValue("x", scale.x) )
	{
		DBG_CONFIG_ERROR( "enemy x scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemyScale.lookupValue("y", scale.y) )
	{
		DBG_CONFIG_ERROR( "enemy y scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemyScale.lookupValue("z", scale.z) )
	{
		DBG_CONFIG_ERROR( "enemy z scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	size.width *= scale.x;
	size.height *= scale.y;
	size.depth *= scale.z;

	if( !enemySetting.lookupValue("friction", friction) )
	{
		DBG_CONFIG_ERROR( "enemy friction", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !enemySetting.lookupValue("meshFile", meshFile) )
	{
		DBG_CONFIG_ERROR( "enemy mesh file", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !enemySetting.lookupValue("collisionFile", collisionFile) )
	{
		DBG_CONFIG_ERROR( "enemy collision file", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	u8ArrayCount = enemySetting["collidedWith"].getLength();
	for( UINT8 i = 0; i < u8ArrayCount; ++i )
	{
		const char *tempChar = enemySetting["collidedWith"][i];
		u32CollisionMask |= g_IDCreator::Get().IDtoBitMask( g_IDCreator::Get().GetID(tempChar) );
	}
	u32CollisionMask |= g_IDCreator::Get().IDtoBitMask( m_u8PlayerAreaID );

	Utilities::Pointer::SmartPtr<GameEngine::Entity> enemy = GameEngine::Entity::Create( \
		position , new EnemyController(), "Enemy" );
	enemy->m_applyPhysics = true;
	enemy->m_u8EntityID = g_IDCreator::Get().GetID( "Enemy" );
	enemy->m_size = size;
	enemy->m_u32CollisionMask = u32CollisionMask;
	enemy->m_vLookAt = lookAt;
	enemy->m_vScale = scale;

	g_world::Get().AddEntity( enemy );
	g_world::Get().CreateMesh( enemy, meshFile.c_str() );
	g_world::Get().SetFriction( enemy, friction );

	// Collision
	g_world::Get().SetCollisionDetectionType( GameEngine::E_COLLISION_BY_MESH );
	g_world::Get().CreateCollisionEntity( enemy, collisionFile.c_str() );
	g_world::Get().SetCollisionHandler( enemy, new EnemyCollisionHandler() );

	// TriggerBox
	g_world::Get().CreateTriggerBoxEntity( enemy );
	g_world::Get().SetTriggerBoxHandler( enemy, new EnemyTriggerBoxHandler() );

	// AI
	g_world::Get().CreateAIEntity( enemy );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreateEnemy( const D3DXVECTOR3 &i_position )
	\brief		Create enemy
	\param		i_position initial position
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::CreateEnemy( const D3DXVECTOR3 &i_position )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	const libconfig::Setting &root = config.getRoot();
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 scale;
	float friction;
	std::string meshFile;
	std::string collisionFile;
	UINT32 u32CollisionMask = 0;
	UINT8 u8ArrayCount = 0;
	Utilities::S_SIZE size;

	FUNCTION_START;

	const libconfig::Setting &enemySetting = root["CaptureTheFlag"]["enemy"];

	// Look at
	const libconfig::Setting &enemyLookAt = root["CaptureTheFlag"]["enemy"]["lookAt"];
	if( !enemyLookAt.lookupValue("x", lookAt.x) )
	{
		DBG_CONFIG_ERROR( "enemy x look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemyLookAt.lookupValue("y", lookAt.y) )
	{
		DBG_CONFIG_ERROR( "enemy y look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemyLookAt.lookupValue("z", lookAt.z) )
	{
		DBG_CONFIG_ERROR( "enemy z look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	// Size
	const libconfig::Setting &enemySize = root["CaptureTheFlag"]["enemy"]["size"];
	if( !enemySize.lookupValue("width", size.width) )
	{
		DBG_CONFIG_ERROR( "enemy width", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemySize.lookupValue("height", size.height) )
	{
		DBG_CONFIG_ERROR( "enemy height", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemySize.lookupValue("depth", size.depth) )
	{
		DBG_CONFIG_ERROR( "enemy depth", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	const libconfig::Setting &enemyScale = root["CaptureTheFlag"]["enemy"]["scale"];
	if( !enemyScale.lookupValue("x", scale.x) )
	{
		DBG_CONFIG_ERROR( "enemy x scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemyScale.lookupValue("y", scale.y) )
	{
		DBG_CONFIG_ERROR( "enemy y scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !enemyScale.lookupValue("z", scale.z) )
	{
		DBG_CONFIG_ERROR( "enemy z scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	size.width *= scale.x;
	size.height *= scale.y;
	size.depth *= scale.z;

	if( !enemySetting.lookupValue("friction", friction) )
	{
		DBG_CONFIG_ERROR( "enemy friction", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !enemySetting.lookupValue("meshFile", meshFile) )
	{
		DBG_CONFIG_ERROR( "enemy mesh file", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !enemySetting.lookupValue("collisionFile", collisionFile) )
	{
		DBG_CONFIG_ERROR( "enemy collision file", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	u8ArrayCount = enemySetting["collidedWith"].getLength();
	for( UINT8 i = 0; i < u8ArrayCount; ++i )
	{
		const char *tempChar = enemySetting["collidedWith"][i];
		u32CollisionMask |= g_IDCreator::Get().IDtoBitMask( g_IDCreator::Get().GetID(tempChar) );
	}
	u32CollisionMask |= g_IDCreator::Get().IDtoBitMask( m_u8PlayerAreaID );

	Utilities::Pointer::SmartPtr<GameEngine::Entity> enemy = GameEngine::Entity::Create( \
		i_position, NULL, "Enemy" );
	enemy->m_applyPhysics = false;
	enemy->m_u8EntityID = g_IDCreator::Get().GetID( "Enemy" );
	enemy->m_size = size;
	enemy->m_u32CollisionMask = u32CollisionMask;
	enemy->m_vLookAt = lookAt;
	enemy->m_vScale = scale;

	g_world::Get().AddEntity( enemy );
	g_world::Get().CreateMesh( enemy, meshFile.c_str() );
	g_world::Get().SetFriction( enemy, friction );

	// Collision
	g_world::Get().SetCollisionDetectionType( GameEngine::E_COLLISION_BY_MESH );
	g_world::Get().CreateCollisionEntity( enemy, collisionFile.c_str() );
	g_world::Get().SetCollisionHandler( enemy, new EnemyCollisionHandler() );

	// TriggerBox
	/*g_world::Get().CreateTriggerBoxEntity( enemy );
	g_world::Get().SetTriggerBoxHandler( enemy, new EnemyTriggerBoxHandler() );*/

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreatePlayer( void )
	\brief		Create player
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::CreatePlayer( void )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	const libconfig::Setting &root = config.getRoot();
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 scale;
	float friction;
	std::string meshFile;
	std::string collisionFile;
	UINT32 u32CollisionMask = 0;
	UINT8 u8ArrayCount = 0;
	Utilities::S_SIZE size;

	FUNCTION_START;
	const libconfig::Setting &playerSetting = root["CaptureTheFlag"]["player"];

	const libconfig::Setting &playerPosition = root["CaptureTheFlag"]["player"]["position"];
	if( !playerPosition.lookupValue("x", position.x) )
	{
		DBG_CONFIG_ERROR( "player x position", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerPosition.lookupValue("y", position.y) )
	{
		DBG_CONFIG_ERROR( "player y position", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerPosition.lookupValue("z", position.z) )
	{
		DBG_CONFIG_ERROR( "player z position", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	// Look at
	const libconfig::Setting &playerLookAt = root["CaptureTheFlag"]["player"]["lookAt"];
	if( !playerLookAt.lookupValue("x", lookAt.x) )
	{
		DBG_CONFIG_ERROR( "player x look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerLookAt.lookupValue("y", lookAt.y) )
	{
		DBG_CONFIG_ERROR( "player y look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerLookAt.lookupValue("z", lookAt.z) )
	{
		DBG_CONFIG_ERROR( "player z look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	// Size
	const libconfig::Setting &playerSize = root["CaptureTheFlag"]["player"]["size"];
	if( !playerSize.lookupValue("width", size.width) )
	{
		DBG_CONFIG_ERROR( "player width", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerSize.lookupValue("height", size.height) )
	{
		DBG_CONFIG_ERROR( "player height", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerSize.lookupValue("depth", size.depth) )
	{
		DBG_CONFIG_ERROR( "player depth", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	const libconfig::Setting &playerScale = root["CaptureTheFlag"]["player"]["scale"];
	if( !playerScale.lookupValue("x", scale.x) )
	{
		DBG_CONFIG_ERROR( "player x scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerScale.lookupValue("y", scale.y) )
	{
		DBG_CONFIG_ERROR( "player y scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerScale.lookupValue("z", scale.z) )
	{
		DBG_CONFIG_ERROR( "player z scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	size.width *= scale.x;
	size.height *= scale.y;
	size.depth *= scale.z;

	if( !playerSetting.lookupValue("friction", friction) )
	{
		DBG_CONFIG_ERROR( "player friction", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !playerSetting.lookupValue("meshFile", meshFile) )
	{
		DBG_CONFIG_ERROR( "player mesh file", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !playerSetting.lookupValue("collisionFile", collisionFile) )
	{
		DBG_CONFIG_ERROR( "player collision file", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	u8ArrayCount = playerSetting["collidedWith"].getLength();
	for( UINT8 i = 0; i < u8ArrayCount; ++i )
	{
		const char *tempChar = playerSetting["collidedWith"][i];
		u32CollisionMask |= g_IDCreator::Get().IDtoBitMask( g_IDCreator::Get().GetID(tempChar) );
	}
	u32CollisionMask |= g_IDCreator::Get().IDtoBitMask( m_u8EnemyAreaID );

	Utilities::Pointer::SmartPtr<GameEngine::Entity> player = GameEngine::Entity::Create( \
		position , new PlayerController(), "Player" );
	player->m_applyPhysics = true;
	player->m_u8EntityID = g_IDCreator::Get().GetID( "Player" );
	player->m_size = size;
	player->m_u32CollisionMask = u32CollisionMask;
	player->m_vLookAt = lookAt;
	player->m_vScale = scale;

	g_world::Get().AddEntity( player );
	g_world::Get().CreateMesh( player, meshFile.c_str() );
	g_world::Get().SetFriction( player, friction );

	// Collision
	g_world::Get().SetCollisionDetectionType( GameEngine::E_COLLISION_BY_MESH );
	g_world::Get().CreateCollisionEntity( player, collisionFile.c_str() );
	g_world::Get().SetCollisionHandler( player, new PlayerCollisionHandler() );

	// TriggerBox
	g_world::Get().CreateTriggerBoxEntity( player );
	g_world::Get().SetTriggerBoxHandler( player, new PlayerTriggerBoxHandler() );

	CreateCamera( player );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreatePlayer( const D3DXVECTOR3 &i_position )
	\brief		Create player
	\param		i_position initial position
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::CreatePlayer( const D3DXVECTOR3 &i_position )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	const libconfig::Setting &root = config.getRoot();
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 scale;
	float friction;
	std::string meshFile;
	std::string collisionFile;
	UINT32 u32CollisionMask = 0;
	UINT8 u8ArrayCount = 0;
	Utilities::S_SIZE size;

	FUNCTION_START;
	const libconfig::Setting &playerSetting = root["CaptureTheFlag"]["player"];

	// Look at
	const libconfig::Setting &playerLookAt = root["CaptureTheFlag"]["player"]["lookAt"];
	if( !playerLookAt.lookupValue("x", lookAt.x) )
	{
		DBG_CONFIG_ERROR( "player x look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerLookAt.lookupValue("y", lookAt.y) )
	{
		DBG_CONFIG_ERROR( "player y look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerLookAt.lookupValue("z", lookAt.z) )
	{
		DBG_CONFIG_ERROR( "player z look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	// Size
	const libconfig::Setting &playerSize = root["CaptureTheFlag"]["player"]["size"];
	if( !playerSize.lookupValue("width", size.width) )
	{
		DBG_CONFIG_ERROR( "player width", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerSize.lookupValue("height", size.height) )
	{
		DBG_CONFIG_ERROR( "player height", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerSize.lookupValue("depth", size.depth) )
	{
		DBG_CONFIG_ERROR( "player depth", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	const libconfig::Setting &playerScale = root["CaptureTheFlag"]["player"]["scale"];
	if( !playerScale.lookupValue("x", scale.x) )
	{
		DBG_CONFIG_ERROR( "player x scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerScale.lookupValue("y", scale.y) )
	{
		DBG_CONFIG_ERROR( "player y scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	if( !playerScale.lookupValue("z", scale.z) )
	{
		DBG_CONFIG_ERROR( "player z scale", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	size.width *= scale.x;
	size.height *= scale.y;
	size.depth *= scale.z;

	if( !playerSetting.lookupValue("friction", friction) )
	{
		DBG_CONFIG_ERROR( "player friction", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !playerSetting.lookupValue("meshFile", meshFile) )
	{
		DBG_CONFIG_ERROR( "player mesh file", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !playerSetting.lookupValue("collisionFile", collisionFile) )
	{
		DBG_CONFIG_ERROR( "player collision file", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	u8ArrayCount = playerSetting["collidedWith"].getLength();
	for( UINT8 i = 0; i < u8ArrayCount; ++i )
	{
		const char *tempChar = playerSetting["collidedWith"][i];
		u32CollisionMask |= g_IDCreator::Get().IDtoBitMask( g_IDCreator::Get().GetID(tempChar) );
	}
	u32CollisionMask |= g_IDCreator::Get().IDtoBitMask( m_u8EnemyAreaID );

	Utilities::Pointer::SmartPtr<GameEngine::Entity> player = GameEngine::Entity::Create( \
		i_position , new PlayerController(), "Player" );
	player->m_applyPhysics = true;
	player->m_u8EntityID = g_IDCreator::Get().GetID( "Player" );
	player->m_size = size;
	player->m_u32CollisionMask = u32CollisionMask;
	player->m_vLookAt = lookAt;
	player->m_vScale = scale;

	g_world::Get().AddEntity( player );
	g_world::Get().CreateMesh( player, meshFile.c_str() );
	g_world::Get().SetFriction( player, friction );

	// Collision
	g_world::Get().SetCollisionDetectionType( GameEngine::E_COLLISION_BY_MESH );
	g_world::Get().CreateCollisionEntity( player, collisionFile.c_str() );
	g_world::Get().SetCollisionHandler( player, new PlayerCollisionHandler() );

	// TriggerBox
	/*g_world::Get().CreateTriggerBoxEntity( player );
	g_world::Get().SetTriggerBoxHandler( player, new PlayerTriggerBoxHandler() );*/

	CreateCamera( player );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreateEntities( void )
	\brief		Create entities
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::CreateEntities( void )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	libconfig::Setting *gameSettings = config.lookup( "CaptureTheFlag" );
	std::string sceneFile;

	FUNCTION_START;

	if( !gameSettings )
	{
		FUNCTION_FINISH;
		return;
	}

	if( !gameSettings->lookupValue("sceneFile", sceneFile) )
	{
		DBG_CONFIG_ERROR( "scene file", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	Utilities::SceneParser scene( sceneFile.c_str() );

	GameEngine::Math::Vector3 position( scene.m_entity[0].position.x, scene.m_entity[0].position.y, scene.m_entity[0].position.z );
	Utilities::Pointer::SmartPtr<GameEngine::Entity> tempEntity = GameEngine::Entity::Create( position, NULL, "Model" );

	tempEntity->m_applyPhysics = false;
	tempEntity->m_u32CollisionMask = 0;
	tempEntity->m_u8EntityID = g_IDCreator::Get().GetID( "Model" );
	tempEntity->m_orientation = scene.m_entity[0].orientation;
	tempEntity->m_v3Position = position;

	g_world::Get().AddEntity( tempEntity );

	for( UINT32 i = 0; i < scene.m_u32TotalEntity; ++i )
	{
		Utilities::MeshParser mesh( scene.m_entity[i].file.c_str() );
		if( mesh.m_tag )
		{
			Utilities::Pointer::SmartPtr<GameEngine::Entity> newEntity;
			if( !mesh.m_tag->compare("RedFlag") )
			{
				if( m_playerTeam == Utilities::StringHash("BlueFlag") )
				{
					newEntity = GameEngine::Entity::Create( position, \
						new FlagController(m_bPlayerHasFlag, "Player", "RedFlagArea"),
						mesh.m_tag->c_str());
				}
				else
				{
					newEntity = GameEngine::Entity::Create( position, \
						new FlagController(m_bEnemyHasFlag, "Enemy", "RedFlagArea"),
						mesh.m_tag->c_str());
				}
			}
			else if( !mesh.m_tag->compare("BlueFlag") )
			{
				if( m_playerTeam == Utilities::StringHash("BlueFlag") )
				{
					newEntity = GameEngine::Entity::Create( position, \
						new FlagController(m_bEnemyHasFlag, "Enemy", "BlueFlagArea"),
						mesh.m_tag->c_str());
				}
				else
				{
					newEntity = GameEngine::Entity::Create( position, \
						new FlagController(m_bPlayerHasFlag, "Player", "BlueFlagArea"),
						mesh.m_tag->c_str());
				}
			}
			else
			{
				newEntity = GameEngine::Entity::Create( position, \
				NULL, mesh.m_tag->c_str());
			}
			newEntity->m_applyPhysics = false;
			newEntity->m_u32CollisionMask = 0;
			newEntity->m_u8EntityID = g_IDCreator::Get().GetID( scene.m_entity[i].file.c_str() );
			newEntity->m_orientation = scene.m_entity[0].orientation;
			newEntity->m_v3Position = position;
			newEntity->m_tag = new std::string( mesh.m_tag->c_str() );
			g_world::Get().AddEntity( newEntity );
			g_world::Get().CreateMesh( newEntity, scene.m_entity[i].file.c_str() );
		}
		else
			g_world::Get().CreateMesh( tempEntity, scene.m_entity[i].file.c_str() );
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreateCamera( void )
	\brief		Camera creation
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::CreateCamera( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	libconfig::Setting *gameSettings = config.lookup( "CaptureTheFlag" );
	const libconfig::Setting &root = config.getRoot();

	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAtPoint;
	float angle;
	float aspectRatio;
	float nearView;
	float farView;
	float backDistance;
	float upDistance;

	FUNCTION_START;

	if( !gameSettings )
	{
		FUNCTION_FINISH;
		return;
	}

	// Position
	// {
	const libconfig::Setting &cameraPosition = root["CaptureTheFlag"]["camera"]["position"];
	if( !cameraPosition.lookupValue("x", position.x) )
	{
		DBG_CONFIG_ERROR( "camera position", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !cameraPosition.lookupValue("y", position.y) )
	{
		DBG_CONFIG_ERROR( "camera position", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !cameraPosition.lookupValue("z", position.z) )
	{
		DBG_CONFIG_ERROR( "camera position", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	// }

	// Look at
	// {
	const libconfig::Setting &cameraLookAt = root["CaptureTheFlag"]["camera"]["lookAt"];
	if( !cameraLookAt.lookupValue("x", lookAtPoint.x) )
	{
		DBG_CONFIG_ERROR( "camera look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !cameraLookAt.lookupValue("y", lookAtPoint.y) )
	{
		DBG_CONFIG_ERROR( "camera look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !cameraLookAt.lookupValue("z", lookAtPoint.z) )
	{
		DBG_CONFIG_ERROR( "camera look at", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	// }

	const libconfig::Setting &cameraSetting = root["CaptureTheFlag"]["camera"];
	if( !cameraSetting.lookupValue("angle", angle) )
	{
		DBG_CONFIG_ERROR( "camera angle", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !cameraSetting.lookupValue("aspect", aspectRatio) )
	{
		DBG_CONFIG_ERROR( "camera aspect", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !cameraSetting.lookupValue("nearView", nearView) )
	{
		DBG_CONFIG_ERROR( "camera near view", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !cameraSetting.lookupValue("farView", farView) )
	{
		DBG_CONFIG_ERROR( "camera far view", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !cameraSetting.lookupValue("backDistance", backDistance) )
	{
		DBG_CONFIG_ERROR( "camera back distance", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !cameraSetting.lookupValue("upDistance", upDistance) )
	{
		DBG_CONFIG_ERROR( "camera up distance", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	g_world::Get().CreateCamera( i_entity, position, lookAtPoint, angle, aspectRatio, nearView, farView );
	g_world::Get().m_camera->m_backDistance = backDistance;
	g_world::Get().m_camera->m_upDistance = upDistance;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreatePointLight( void )
	\brief		Point light creation
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::CreatePointLight( void )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	libconfig::Setting *gameSettings = config.lookup( "CaptureTheFlag" );
	const libconfig::Setting &root = config.getRoot();

	D3DXVECTOR3 position;
	D3DCOLOR colour;
	D3DCOLOR ambient;
	int red;
	int blue;
	int green;
	float intensity;
	float attenuator;
	float radius;

	FUNCTION_START;

	if( !gameSettings )
	{
		FUNCTION_FINISH;
		return;
	}

	// Position
	// {
	const libconfig::Setting &pointLightPosition = root["CaptureTheFlag"]["pointLight"]["position"];
	if( !pointLightPosition.lookupValue("x", position.x) )
	{
		DBG_CONFIG_ERROR( "point light position", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !pointLightPosition.lookupValue("y", position.y) )
	{
		DBG_CONFIG_ERROR( "point light position", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !pointLightPosition.lookupValue("z", position.z) )
	{
		DBG_CONFIG_ERROR( "point light position", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	// }

	// Colour
	// {
	const libconfig::Setting &pointLightColour = root["CaptureTheFlag"]["pointLight"]["colour"];
	if( !pointLightColour.lookupValue("r", red) )
	{
		DBG_CONFIG_ERROR( "point light red", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !pointLightColour.lookupValue("g", green) )
	{
		DBG_CONFIG_ERROR( "point light green", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !pointLightColour.lookupValue("b", blue) )
	{
		DBG_CONFIG_ERROR( "point light blue", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	colour = D3DCOLOR_RGBA( red, blue, green, 255 );
	// }

	// Ambient
	// {
	const libconfig::Setting &ambientColour = root["CaptureTheFlag"]["pointLight"]["ambient"];
	if( !ambientColour.lookupValue("r", red) )
	{
		DBG_CONFIG_ERROR( "ambient red", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !ambientColour.lookupValue("g", green) )
	{
		DBG_CONFIG_ERROR( "ambient green", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !ambientColour.lookupValue("b", blue) )
	{
		DBG_CONFIG_ERROR( "ambient blue", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	ambient = D3DCOLOR_RGBA( red, green, blue, 255 );
	// }

	const libconfig::Setting &pointLightSetting = root["CaptureTheFlag"]["pointLight"];
	if( !pointLightSetting.lookupValue("intensity", intensity) )
	{
		DBG_CONFIG_ERROR( "point light intensity", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !pointLightSetting.lookupValue("attenuator", attenuator) )
	{
		DBG_CONFIG_ERROR( "point light attenuator", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !pointLightSetting.lookupValue("radius", radius) )
	{
		DBG_CONFIG_ERROR( "point light radius", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	g_world::Get().CreatePointLight( position, colour, ambient, intensity, attenuator, radius );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreateDirectionalLight( void )
	\brief		Directional light creation
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::CreateDirectionalLight( void )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	libconfig::Setting *gameSettings = config.lookup( "CaptureTheFlag" );
	const libconfig::Setting &root = config.getRoot();

	Utilities::S_SIZE size;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 lookAt;
	D3DCOLOR colour;
	int red;
	int blue;
	int green;
	float intensity;
	float nearView;
	float farView;

	FUNCTION_START;

	if( !gameSettings )
	{
		FUNCTION_FINISH;
		return;
	}

	// Size
	// {
	const libconfig::Setting &directionalLightSize = root["CaptureTheFlag"]["directionalLight"]["size"];
	if( !directionalLightSize.lookupValue("width", size.width) )
	{
		DBG_CONFIG_ERROR( "directional light size", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightSize.lookupValue("height", size.height) )
	{
		DBG_CONFIG_ERROR( "directional light size", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	// }

	// Orientation
	// {
	const libconfig::Setting &directionalLightOrientation = root["CaptureTheFlag"]["directionalLight"]["direction"];
	if( !directionalLightOrientation.lookupValue("x", direction.x) )
	{
		DBG_CONFIG_ERROR( "directional light orientation", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightOrientation.lookupValue("y", direction.y) )
	{
		DBG_CONFIG_ERROR( "directional light orientation", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightOrientation.lookupValue("z", direction.z) )
	{
		DBG_CONFIG_ERROR( "directional light orientation", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	// }

	// Look at
	// {
	const libconfig::Setting &directionalLightLookAt = root["CaptureTheFlag"]["directionalLight"]["lookAt"];
	if( !directionalLightLookAt.lookupValue("x", lookAt.x) )
	{
		DBG_CONFIG_ERROR( "directional light look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightLookAt.lookupValue("y", lookAt.y) )
	{
		DBG_CONFIG_ERROR( "directional light look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightLookAt.lookupValue("z", lookAt.z) )
	{
		DBG_CONFIG_ERROR( "directional light look at", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	// }

	// Colour
	// {
	const libconfig::Setting &pointLightColour = root["CaptureTheFlag"]["directionalLight"]["colour"];
	if( !pointLightColour.lookupValue("r", red) )
	{
		DBG_CONFIG_ERROR( "point light red", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !pointLightColour.lookupValue("g", green) )
	{
		DBG_CONFIG_ERROR( "point light green", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !pointLightColour.lookupValue("b", blue) )
	{
		DBG_CONFIG_ERROR( "point light blue", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	colour = D3DCOLOR_RGBA( red, blue, green, 255 );
	// }

	const libconfig::Setting &directionalLightSetting = root["CaptureTheFlag"]["directionalLight"];
	if( !directionalLightSetting.lookupValue("intensity", intensity) )
	{
		DBG_CONFIG_ERROR( "directional light intensity", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightSetting.lookupValue("nearView", nearView) )
	{
		DBG_CONFIG_ERROR( "directional light near view", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightSetting.lookupValue("farView", farView) )
	{
		DBG_CONFIG_ERROR( "directional light far view", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	g_world::Get().CreateDirectionalLight( direction, lookAt, colour, size, nearView, farView, intensity );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreateCollisionEntities( void )
	\brief		Create collision entities
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::CreateCollisionEntities( void )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	libconfig::Setting *gameSettings = config.lookup( "CaptureTheFlag" );
	UINT32 temp;
	bool bUseOctreeCollision = false;
	std::string collisionFile;
	UINT8 u8CollisionID = g_IDCreator::Get().GetID( "COL" );
	UINT32 u32CollisionMask = g_IDCreator::Get().IDtoBitMask( g_IDCreator::Get().GetID("Player") ) \
		| g_IDCreator::Get().IDtoBitMask( g_IDCreator::Get().GetID("Camera") ) \
		| g_IDCreator::Get().IDtoBitMask( g_IDCreator::Get().GetID("Enemy") );

	FUNCTION_START;

	if( !gameSettings )
	{
		FUNCTION_FINISH;
		return;
	}

	if( !gameSettings->lookupValue("enableOctreeCollision", temp) )
	{
		DBG_CONFIG_ERROR( "collision type", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( temp != 0 )
		bUseOctreeCollision = true;

	if( bUseOctreeCollision )
	{
		if( !gameSettings->lookupValue("collisionOctreeFile", collisionFile) )
		{
			DBG_CONFIG_ERROR( "collision octree file", "CaptureTheFlag" );

			FUNCTION_FINISH;
			return;
		}

		g_world::Get().SetCollisionDetectionType( GameEngine::E_COLLISION_BY_OCTREE );

		Utilities::Pointer::SmartPtr<GameEngine::Entity> tempEntity = GameEngine::Entity::Create( GameEngine::Math::Vector3::Zero, NULL, collisionFile.c_str() );

		tempEntity->m_applyPhysics = false;
		tempEntity->m_u32CollisionMask = u32CollisionMask;
		tempEntity->m_u8EntityID = u8CollisionID;
		tempEntity->m_orientation = 0;
		tempEntity->m_v3Position = GameEngine::Math::Vector3::Zero;

		g_world::Get().AddEntity( tempEntity );
		g_world::Get().CreateCollisionEntity( tempEntity, collisionFile.c_str() );
	}
	else
	{
		if( !gameSettings->lookupValue("collisionFile", collisionFile) )
		{
			DBG_CONFIG_ERROR( "collision file", "CaptureTheFlag" );

			FUNCTION_FINISH;
			return;
		}

		g_world::Get().SetCollisionDetectionType( GameEngine::E_COLLISION_BY_MESH );

		Utilities::SceneParser collision( collisionFile.c_str() );

		for( UINT32 i = 0; i < collision.m_u32TotalEntity; ++i )
		{
			GameEngine::Math::Vector3 position( collision.m_entity[i].position.x, collision.m_entity[i].position.y, collision.m_entity[i].position.z );
			Utilities::Pointer::SmartPtr<GameEngine::Entity> tempEntity = GameEngine::Entity::Create( position, NULL, collision.m_entity[i].file.c_str() );

			tempEntity->m_applyPhysics = false;
			tempEntity->m_u32CollisionMask = u32CollisionMask;
			tempEntity->m_u8EntityID = u8CollisionID;
			tempEntity->m_orientation = collision.m_entity[i].orientation;
			tempEntity->m_v3Position = position;

			g_world::Get().AddEntity( tempEntity );
			g_world::Get().CreateCollisionEntity( tempEntity, collision.m_entity[i].file.c_str() );
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreateTriggerBoxEntities( void )
	\brief		Create trigger box entities
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::LoadWorldConfiguration( void )
{
	UINT32 u32CollisionMask = g_IDCreator::Get().IDtoBitMask( g_IDCreator::Get().GetID("Player") ) \
		| g_IDCreator::Get().IDtoBitMask( g_IDCreator::Get().GetID("Enemy") );
	std::ifstream inFile;

	inFile.open( "../../External/worldConfiguration.dat", std::ios::binary );

	FUNCTION_START;

	if( inFile.fail() )
	{
		FUNCTION_FINISH;
		return;
	}

	// Read trigger boxes
	UINT32 u32TotalTriggerBox;
	inFile.seekg( 0, std::ios::beg );
	inFile.read( (char*)&u32TotalTriggerBox, sizeof(u32TotalTriggerBox) );
	for( UINT32 i = 0; i < u32TotalTriggerBox; ++i )
	{
		char *triggerBoxName;
		UINT32 u32TriggerBoxNameLength = 0;
		D3DXVECTOR3 centre;
		Utilities::S_SIZE size;

		inFile.read( (char*)&u32TriggerBoxNameLength, sizeof(u32TriggerBoxNameLength) );
		triggerBoxName = new char[u32TriggerBoxNameLength + 1];
		inFile.read( triggerBoxName, u32TriggerBoxNameLength * sizeof(char) );
		triggerBoxName[u32TriggerBoxNameLength] = '\0';
		inFile.read( (char*)&centre, sizeof(centre) );
		inFile.read( (char*)&size, sizeof(size) );

		Utilities::Pointer::SmartPtr<GameEngine::Entity> tempEntity = GameEngine::Entity::Create( \
			centre, NULL, triggerBoxName );
		tempEntity->m_applyPhysics = false;
		tempEntity->m_u8EntityID = g_IDCreator::Get().GetID( triggerBoxName );
		tempEntity->m_size = size;
		tempEntity->m_u32CollisionMask = u32CollisionMask;

		g_world::Get().AddEntity( tempEntity );
		g_world::Get().CreateTriggerBoxEntity( tempEntity );

		delete [] triggerBoxName;
	}

	// Read way points
	UINT32 u32TotalWayPoint = 0;
	inFile.read( (char*)&u32TotalWayPoint, sizeof(u32TotalWayPoint) );
	for( UINT32 i = 0; i < u32TotalWayPoint; ++i )
	{
		UINT32 u32ID = 0;
		GameEngine::AI::S_WAY_POINT newWayPoint;
		inFile.read( (char*)&u32ID, sizeof(u32ID) );
		inFile.read( (char*)&newWayPoint, sizeof(newWayPoint) );
		GameEngine::AI::AddWayPoint( u32ID, newWayPoint );
	}

	// Read way point links
	UINT32 u32TotalWayPointLink = 0;
	inFile.read( (char*)&u32TotalWayPointLink, sizeof(u32TotalWayPointLink) );
	for( UINT32 i = 0; i < u32TotalWayPointLink; ++i )
	{
		GameEngine::AI::S_WAY_POINT_LINK newWayPointLink;
		inFile.read( (char*)&newWayPointLink, sizeof(newWayPointLink) );
		GameEngine::AI::AddWayPointLink( newWayPointLink );
	}

	inFile.close();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void InitializeSfx( void )
	\brief		Initialize sound effect
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void CaptureTheFlag::InitializeSfx( void )
{
	FUNCTION_START;

	m_u32PlayerPickUpSfxID = GameEngine::Audio::Add2DSoundEffect( "playerPickFlag.wav" );
	m_u32PlayerScoreSfxID = GameEngine::Audio::Add2DSoundEffect( "playerScore.wav" );
	m_u32EnemyPickUpSfxID = GameEngine::Audio::Add2DSoundEffect( "enemyPickFlag.wav" );
	m_u32EnemyScoreSfxID = GameEngine::Audio::Add2DSoundEffect( "enemyScore.wav" );
	m_u32FlagResetSfxID = GameEngine::Audio::Add2DSoundEffect( "flagReset.wav" );
	m_u32SprintSfxID = GameEngine::Audio::Add3DSoundEffect( "sprint.wav" );
	m_u32StairSfxID = GameEngine::Audio::Add3DSoundEffect( "stairs.wav" );
	m_u32GroundSfxID = GameEngine::Audio::Add3DSoundEffect( "ground.wav" );
	m_u32AmbientSfxID = GameEngine::Audio::Add3DSoundEffect( "drum.wav" );

	m_v3AmbientPos = GameEngine::Math::Vector3( -1490.0f, 500.0f, 1450.0f );
	GameEngine::Audio::Play3DSoundEffect( m_u32AmbientSfxID, "drum.wav", m_v3AmbientPos );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ResetFlagPosition( void )
	\brief		Reset flag position
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void ResetFlagPosition( void )
{
	FUNCTION_START;

	EnemyController::ResetEnemyController();

	GameEngine::Audio::Play2DSoundEffect( g_captureTheFlag::Get().m_u32FlagResetSfxID, "flagReset.wav" );

	if( g_captureTheFlag::Get().m_bEnemyHasFlag )
	{
		g_captureTheFlag::Get().m_bEnemyHasFlag = false;
		Utilities::Pointer::SmartPtr<GameEngine::Entity> blueFlag = g_world::Get().GetEntityByName(
			g_captureTheFlag::Get().m_playerTeam );
		blueFlag->m_v3Position = GameEngine::Math::Vector3::Zero;
		g_networkManager::Get().SendFlagEvent( false );
	}

	if( g_captureTheFlag::Get().m_bPlayerHasFlag )
	{
		g_captureTheFlag::Get().m_bPlayerHasFlag = false;
		Utilities::Pointer::SmartPtr<GameEngine::Entity> redFlag = g_world::Get().GetEntityByName(
			g_captureTheFlag::Get().m_enemyTeam );
		redFlag->m_v3Position = GameEngine::Math::Vector3::Zero;
		g_networkManager::Get().SendFlagEvent( true );
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EnemyTagPlayerMessageHandler( void *i_sender )
	\brief		Message handler when enemy tag player
	\param		i_sender the sender of the message
	\return		NONE
 ****************************************************************************************************
*/
void TagMessageHandler( void *i_sender )
{
	FUNCTION_START;

	ResetFlagPosition();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void AIScoreMessageHandler( void *i_sender )
	\brief		Message handler when AI score
	\param		i_sender the sender of the message
	\return		NONE
 ****************************************************************************************************
*/
void EnemyScoreMessageHandler( void *i_sender )
{
	FUNCTION_START;

	if( g_captureTheFlag::Get().m_bEnemyHasFlag )
	{
		++g_captureTheFlag::Get().m_u32EnemyScore;
		GameEngine::Audio::Play2DSoundEffect( g_captureTheFlag::Get().m_u32EnemyScore, "enemyScore.wav" );
		g_networkManager::Get().SendScoreEvent();
	}
	ResetFlagPosition();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void PlayerScoreMessageHandler( void *i_sender )
	\brief		Message handler when player score
	\param		i_sender the sender of the message
	\return		NONE
 ****************************************************************************************************
*/
void PlayerScoreMessageHandler( void *i_sender )
{
	FUNCTION_START;

	if( g_captureTheFlag::Get().m_bPlayerHasFlag )
	{
		++g_captureTheFlag::Get().m_u32PlayerScore;
		GameEngine::Audio::Play2DSoundEffect( g_captureTheFlag::Get().m_u32PlayerScore, "playerScore.wav" );
		g_networkManager::Get().SendScoreEvent();
	}
	ResetFlagPosition();

	FUNCTION_FINISH;
}