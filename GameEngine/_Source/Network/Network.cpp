/**
 ****************************************************************************************************
 * \file		Network.cpp
 * \brief		Network engine implementation
 ****************************************************************************************************
*/

// RakNet
#include "../RakNet/Rand.h"
#include "../RakNet/BitStream.h"
#include "../Raknet/RakPeerInterface.h"
#include "../RakNet/MessageIdentifiers.h"

// GameEngine
#include "../GameEngineDefault.h"
#include "../DebugMenu/DebugMenu.h"

#include "Network.h"

namespace GameEngine
{
	namespace Network
	{
		RakNet::SystemAddress otherAddress;
		RakNet::RakPeerInterface *peer = NULL;
		bool bIsServer = false;
		bool bIsConnected = false;
	}
}

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize network engine
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::Network::Initialize( void )
{
	if( peer != NULL )
		return FALSE;

	peer = RakNet::RakPeerInterface::GetInstance();

	if( bIsServer )
	{
		RakNet::SocketDescriptor sd( NETWORK_PORT, 0 );
		peer->Startup( MAX_CONNECTION, &sd, 1 );
		peer->SetMaximumIncomingConnections( MAX_CONNECTION );
	}
	else
	{
		RakNet::SocketDescriptor sd;
		peer->Startup( 1, &sd, 1 );
		RakNet::ConnectionAttemptResult connectResults = peer->Connect( SERVER_IP_ADDRESS, NETWORK_PORT, 0, 0 );
		if( connectResults != RakNet::CONNECTION_ATTEMPT_STARTED )
			return FALSE;
	}

	return TRUE;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin updating network engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Network::BeginUpdate( void )
{
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update network engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Network::Update( void )
{
	if( bIsConnected )
		g_debugMenu::Get().Log( "Connected" );
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End updating audio engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Network::EndUpdate( void )
{

}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shutdown audio engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Network::ShutDown( void )
{
	if( peer )
	{
		RakNet::RakPeerInterface::DestroyInstance( peer );
		peer = NULL;
	}
}

/**
 ****************************************************************************************************
	\fn			void IsServer( const bool i_bIsServer )
	\brief		Set whether this instance is a server or not
	\param		i_bIsServer a boolean status that indicate whether this instance is a server
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Network::IsServer( const bool i_bIsServer )
{
	bIsServer = i_bIsServer;
}

bool GameEngine::Network::IsServer( void )
{
	return bIsServer;
}

bool GameEngine::Network::IsConnected( void )
{
	return bIsConnected;
}

void GameEngine::Network::IsConnected( const bool i_bIsConnected )
{
	bIsConnected = i_bIsConnected;
}

void GameEngine::Network::SendHighPriorityPackage( UINT32 i_u32Id, const char *i_data, size_t i_size )
{
	RakNet::BitStream bs;

	bs.Write( (RakNet::MessageID) i_u32Id );
	bs.Write( i_data, i_size );

	peer->Send( &bs, HIGH_PRIORITY, RELIABLE, 0, otherAddress, false );
}

void GameEngine::Network::SendLowPriorityPackage( UINT32 i_u32Id, const char *i_data, size_t i_size )
{
	RakNet::BitStream bs;

	bs.Write( (RakNet::MessageID) i_u32Id );
	bs.Write( i_data, i_size );

	peer->Send( &bs, LOW_PRIORITY, UNRELIABLE_SEQUENCED, 0, otherAddress, false );
}

RakNet::Packet* GameEngine::Network::ReceivePacket( void )
{
	return peer->Receive();
}

RakNet::Packet* GameEngine::Network::ReceivePacket( RakNet::Packet *i_packet )
{
	peer->DeallocatePacket( i_packet );
	return peer->Receive();
}

void GameEngine::Network::SetOtherAddress( RakNet::SystemAddress &i_address )
{
	bIsConnected = true;
	otherAddress = i_address;
}