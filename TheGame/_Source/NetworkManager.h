/**
 ****************************************************************************************************
 * \file		NetworkManager.h
 * \brief		The header of NetworkManager class
 ****************************************************************************************************
*/

#ifndef _NETWORK_MANAGER_H_
#define _NETWORK_MANAGER_H_

#include <RakNet/MessageIdentifiers.h>

#include <Singleton/Singleton.h>

class NetworkManager
{
	typedef enum _e_network_event_
	{
		E_NETWORK_EVENT_PICKUP,
		E_NETWORK_EVENT_SCORE,
		E_NETWORK_EVENT_DROP
	} E_NETWORK_EVENT;

	typedef enum _e_world_event_
	{
		E_WORLD_EVENT_FLAG_PICK_UP,
		E_WORLD_EVENT_FLAG_RESET,
		E_WORLD_EVENT_SCORE
	};

	typedef enum _e_id_
	{
		E_ID_START_UP = ID_USER_PACKET_ENUM + 1,
		E_ID_WORLD_EVENT,
		E_ID_UPDATE,
	} E_ID;

	typedef struct _s_start_up_packet_
	{
		Utilities::StringHash serverTeam;
		Utilities::StringHash clientTeam;
		float serverPositionX;
		float serverPositionY;
		float serverPositionZ;
		float clientPositionX;
		float clientPositionY;
		float clientPositionZ;
		UINT32 u32ServerScore;
		UINT32 u32ClientScore;
		UINT8 u8ServerAreaID;
		UINT8 u8ClientAreaID;
		bool bServerHasFlag;
		bool bClientHasFlag;
	} S_START_UP_PACKET;

	typedef struct _s_event_packet_
	{
		UINT8 u8Type;
		char data[32];
	} S_EVENT_PACKET;

	typedef struct _s_update_packet_
	{
		D3DXVECTOR3 position;
	} S_UPDATE_PACKET;

	friend Utilities::Singleton<NetworkManager>;

	Utilities::Pointer::SmartPtr<GameEngine::Entity> player;
	Utilities::Pointer::SmartPtr<GameEngine::Entity> enemy;

	NetworkManager( void ) {}
	~NetworkManager( void ) {}

public:
	void Initialize( void );

	void BeginUpdate( void );
	void Update( void );
	void EndUpdate( void );

	void SendFlagEvent( bool bIsServer );
	void SendScoreEvent( void );
};

typedef Utilities::Singleton<NetworkManager> g_networkManager;

#endif	// #ifndef _NETWORK_MANAGER_H_