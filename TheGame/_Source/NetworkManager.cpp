/**
 ****************************************************************************************************
 * \file		NetworkManager.cpp
 * \brief		The implementation of NetworkManager class
 ****************************************************************************************************
*/

#include <RakNet/BitStream.h>
#include <Raknet/RakPeerInterface.h>

// Utilities
#include <SmartPtr/SmartPtr.h>

// GameEngine
#include <AI/AI.h>
#include <GameEngine.h>
#include <Audio/Audio.h>
#include <World/World.h>
#include <World/Entity.h>
#include <Network/Network.h>

#include "NetworkManager.h"
#include "CaptureTheFlag.h"

void NetworkManager::Initialize( void )
{
 player = g_world::Get().GetEntityByName( "Player" );
	enemy = g_world::Get().GetEntityByName( "Enemy" );
}

void NetworkManager::BeginUpdate( void )
{
	RakNet::Packet *packet = GameEngine::Network::ReceivePacket();

	while( packet )
	{
		switch( packet->data[0] )
		{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				GameEngine::Network::SetOtherAddress( packet->systemAddress );
				break;

			case ID_NEW_INCOMING_CONNECTION:
				GameEngine::Network::SetOtherAddress( packet->systemAddress );
				if( GameEngine::Network::IsServer() )
				{
					enemy->SetController( NULL );
					GameEngine::AI::RemoveAIEntity( enemy->m_u8AIEntityIndex );

					S_START_UP_PACKET startUpPacket;
					startUpPacket.serverPositionX = player->m_v3Position.X();
					startUpPacket.serverPositionY = player->m_v3Position.Y();
					startUpPacket.serverPositionZ = player->m_v3Position.Z();
					startUpPacket.clientPositionX = enemy->m_v3Position.X();
					startUpPacket.clientPositionY = enemy->m_v3Position.Y();
					startUpPacket.clientPositionZ = enemy->m_v3Position.Z();
					startUpPacket.serverTeam = g_captureTheFlag::Get().m_playerTeam;
					startUpPacket.clientTeam = g_captureTheFlag::Get().m_enemyTeam;
					startUpPacket.u32ServerScore = g_captureTheFlag::Get().m_u32PlayerScore;
					startUpPacket.u32ClientScore = g_captureTheFlag::Get().m_u32EnemyScore;
					startUpPacket.u8ServerAreaID = g_captureTheFlag::Get().m_u8PlayerAreaID;
					startUpPacket.u8ClientAreaID = g_captureTheFlag::Get().m_u8EnemyAreaID;
					startUpPacket.bServerHasFlag = g_captureTheFlag::Get().m_bPlayerHasFlag;
					startUpPacket.bClientHasFlag = g_captureTheFlag::Get().m_bEnemyHasFlag;

					GameEngine::Network::SendHighPriorityPackage( E_ID_START_UP, (char*)&startUpPacket, sizeof(startUpPacket) );
				}
				break;

			case ID_CONNECTION_ATTEMPT_FAILED:
				g_captureTheFlag::Get().m_bNetworkReady = false;
				exit( 0 );
				break;

			case ID_CONNECTION_LOST:
			case ID_DISCONNECTION_NOTIFICATION:
				g_captureTheFlag::Get().m_bNetworkReady = false;
				GameEngine::Network::IsConnected( false );
				break;

			case E_ID_START_UP:
				if( !GameEngine::Network::IsServer() )
				{
					S_START_UP_PACKET startUpPacket;
					RakNet::BitStream bs( packet->data, packet->length, false );

					bs.IgnoreBytes( sizeof(RakNet::MessageID) );
					bs.Read( (char*)&startUpPacket, sizeof(startUpPacket) );

					g_captureTheFlag::Get().m_playerTeam = startUpPacket.clientTeam;
					g_captureTheFlag::Get().m_enemyTeam = startUpPacket.serverTeam;
					g_captureTheFlag::Get().m_u32PlayerScore = startUpPacket.u32ClientScore;
					g_captureTheFlag::Get().m_u32EnemyScore = startUpPacket.u32ServerScore;
					g_captureTheFlag::Get().m_u8PlayerAreaID = startUpPacket.u8ClientAreaID;
					g_captureTheFlag::Get().m_u8EnemyAreaID = startUpPacket.u8ServerAreaID;
					g_captureTheFlag::Get().m_bPlayerHasFlag = startUpPacket.bClientHasFlag;
					g_captureTheFlag::Get().m_bEnemyHasFlag = startUpPacket.bServerHasFlag;

					D3DXVECTOR3 playerPosition( startUpPacket.clientPositionX, startUpPacket.clientPositionY, startUpPacket.clientPositionZ );
					D3DXVECTOR3 enemyPosition( startUpPacket.serverPositionX, startUpPacket.serverPositionY, startUpPacket.serverPositionZ );

					if( !g_captureTheFlag::Get().Initialize(playerPosition,enemyPosition) )
					{
						GameEngine::ShutDown();
						return;
					}

					g_captureTheFlag::Get().m_bNetworkReady = true;
				}
				break;

			case E_ID_UPDATE:
			{
				RakNet::BitStream bs( packet->data, packet->length, false );
				D3DXVECTOR3 position;

				bs.IgnoreBytes( sizeof(RakNet::MessageID) );
				bs.Read( (char*)&position, sizeof(position) );

				enemy->m_v3Position = position;
				enemy->m_v3ProjectedPosition = position;
				break;

			}

			case E_ID_WORLD_EVENT:
				if( GameEngine::Network::IsServer() )
					return;

				S_EVENT_PACKET eventPacket;
				RakNet::BitStream bs( packet->data, packet->length, false );

				bs.IgnoreBytes( sizeof(RakNet::MessageID) );
				bs.Read( (char*)&eventPacket, sizeof(eventPacket) );

				switch( eventPacket.u8Type )
				{
					case E_WORLD_EVENT_SCORE:
					{
						UINT32 prevEnemyScore = g_captureTheFlag::Get().m_u32PlayerScore;
						UINT32 prevPlayerScore = g_captureTheFlag::Get().m_u32EnemyScore;

						g_captureTheFlag::Get().m_u32PlayerScore = eventPacket.data[1];
						g_captureTheFlag::Get().m_u32EnemyScore = eventPacket.data[0];

						if( prevEnemyScore < g_captureTheFlag::Get().m_u32EnemyScore )
							GameEngine::Audio::Play2DSoundEffect( g_captureTheFlag::Get().m_u32EnemyScore, "enemyScore.wav" );
						else if( prevPlayerScore < g_captureTheFlag::Get().m_u32PlayerScore )
							GameEngine::Audio::Play2DSoundEffect( g_captureTheFlag::Get().m_u32PlayerScore, "playerScore.wav" );
						break;
					}

					case E_WORLD_EVENT_FLAG_PICK_UP:
						if( eventPacket.data[0] )
						{
							g_captureTheFlag::Get().m_bEnemyHasFlag = true;
							GameEngine::Audio::Play2DSoundEffect( g_captureTheFlag::Get().m_u32EnemyPickUpSfxID, "enemyPickFlag.wav" );
						}
						else
						{
							g_captureTheFlag::Get().m_bPlayerHasFlag = true;
							GameEngine::Audio::Play2DSoundEffect( g_captureTheFlag::Get().m_u32PlayerPickUpSfxID, "playerPickFlag.wav" );
						}
						break;

					case E_WORLD_EVENT_FLAG_RESET:
						if( eventPacket.data[0] )
							g_captureTheFlag::Get().m_bEnemyHasFlag = false;
						else
							g_captureTheFlag::Get().m_bPlayerHasFlag = false;
						GameEngine::Audio::Play2DSoundEffect( g_captureTheFlag::Get().m_u32FlagResetSfxID, "flagReset.wav" );
						break;
				}
				break;
		}

		packet = GameEngine::Network::ReceivePacket( packet );
	}
}

void NetworkManager::Update( void )
{
}

void NetworkManager::EndUpdate( void )
{
	// Send player position
	if( GameEngine::Network::IsConnected() && player )
	{
		D3DXVECTOR3 position(player->m_v3Position.X(), player->m_v3Position.Y(), player->m_v3Position.Z() );
		GameEngine::Network::SendHighPriorityPackage( E_ID_UPDATE, (char*)&position, sizeof(position) );
	}
}

void NetworkManager::SendFlagEvent( bool bIsServer )
{
	// Only server can send this information
	if( !GameEngine::Network::IsServer() )
		return;

	S_EVENT_PACKET eventPacket;
	bool bIsHolding;

	if( bIsServer )
		bIsHolding = g_captureTheFlag::Get().m_bPlayerHasFlag;
	else
		bIsHolding = g_captureTheFlag::Get().m_bEnemyHasFlag;

	if( bIsHolding )
		eventPacket.u8Type = E_WORLD_EVENT_FLAG_PICK_UP;
	else
		eventPacket.u8Type = E_WORLD_EVENT_FLAG_RESET;

	eventPacket.data[0] = (char) bIsServer;

	GameEngine::Network::SendHighPriorityPackage( E_ID_WORLD_EVENT, (char*)&eventPacket, sizeof(eventPacket) );
}

void NetworkManager::SendScoreEvent( void )
{
	// Only server can send this information
	if( !GameEngine::Network::IsServer() )
		return;

	S_EVENT_PACKET eventPacket;

	eventPacket.u8Type = E_WORLD_EVENT_SCORE;
	eventPacket.data[0] = g_captureTheFlag::Get().m_u32PlayerScore;
	eventPacket.data[1] = g_captureTheFlag::Get().m_u32EnemyScore;

	GameEngine::Network::SendHighPriorityPackage( E_ID_WORLD_EVENT, (char*)&eventPacket, sizeof(eventPacket) );
}