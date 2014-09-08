/**
 ****************************************************************************************************
 * \file		Network.h
 * \brief		Network engine global function declaration
 ****************************************************************************************************
*/

#ifndef _NETWORK_H_
#define _NETWORK_H_

// Utilities header
#include <StringHash/StringHash.h>

#define SERVER_IP_ADDRESS "127.0.0.1"

namespace RakNet
{
	struct Packet;
	struct SystemAddress;
}

namespace GameEngine
{
	namespace Network
	{
		bool Initialize( void );
		void BeginUpdate( void );
		void Update( void );
		void EndUpdate( void );
		void ShutDown( void );

		bool IsServer( void );
		bool IsConnected( void );
		void IsServer( const bool i_bIsServer );
		void IsConnected( const bool i_bIsConnected );
		void SendHighPriorityPackage( UINT32 i_u32Id, const char *i_data, size_t i_size );
		void SendLowPriorityPackage( UINT32 i_u32Id, const char *i_data, size_t i_size );
		RakNet::Packet* ReceivePacket( void );
		RakNet::Packet* ReceivePacket( RakNet::Packet *i_packet );
		void SetOtherAddress( RakNet::SystemAddress &i_address );
	}
}

#endif	// #ifndef _NETWORK_H_