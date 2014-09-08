/**
 ****************************************************************************************************
 * \file		MonsterChase.h
 * \brief		Monster Chase global function interface
 ****************************************************************************************************
*/

#ifndef _MONSTER_CHASE_H_
#define _MONSTER_CHASE_H_

#include <string>

// Utilities header
#include <SmartPtr/SmartPtr.h>
#include <Singleton/Singleton.h>

#include "World/Entity.h"
#include "Utilities/GameEngineTypes.h"
#include "Math/Vector3/Vector3.h"

class CaptureTheFlag
{
	friend Utilities::Singleton<CaptureTheFlag>;

	void CreateEnemy( void );
	void CreatePlayer( void );
	void CreateEnemy( const D3DXVECTOR3 &i_position );
	void CreatePlayer( const D3DXVECTOR3 &i_position );
	void CreateEntities( void );
	void CreateCamera( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity );
	void CreatePointLight( void );
	void CreateDirectionalLight( void );
	void CreateCollisionEntities( void );
	void LoadWorldConfiguration( void );
	void InitializeSfx( void );

	CaptureTheFlag( void ) {}
	~CaptureTheFlag( void ) {}

public:
	GameEngine::Math::Vector3 m_v3AmbientPos;
	GameEngine::Math::Vector3 m_v3GroundPos;
	Utilities::S_SIZE m_windowSize;
	Utilities::StringHash m_playerTeam;
	Utilities::StringHash m_enemyTeam;
	UINT32 m_u32PlayerScore;
	UINT32 m_u32EnemyScore;
	UINT32 m_u32PlayerPickUpSfxID;
	UINT32 m_u32PlayerScoreSfxID;
	UINT32 m_u32EnemyPickUpSfxID;
	UINT32 m_u32EnemyScoreSfxID;
	UINT32 m_u32SprintSfxID;
	UINT32 m_u32GroundSfxID;
	UINT32 m_u32StairSfxID;
	UINT32 m_u32FlagResetSfxID;
	UINT32 m_u32AmbientSfxID;
	UINT8 m_u8PlayerAreaID;
	UINT8 m_u8EnemyAreaID;
	bool m_bPlayerHasFlag;
	bool m_bEnemyHasFlag;
	bool m_bNetworkReady;

	bool StartUp( void );
	bool Initialize( void );
	bool Initialize( const D3DXVECTOR3 &i_playerPosition, const D3DXVECTOR3 &i_enemyPosition );
	void BeginUpdate( void );
	void Update( void );
	void EndUpdate( void );
	void ShutDown( void );

	const bool IsGameOver( void ) const;
	const Utilities::S_SIZE &GetWindowSize( void ) const;
};

typedef Utilities::Singleton<CaptureTheFlag> g_captureTheFlag;

#endif	// #ifndef _MONSTER_CHASE_H_