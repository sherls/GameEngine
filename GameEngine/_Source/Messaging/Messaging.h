/**
 ****************************************************************************************************
 * \file		Messaging.h
 * \brief		Messaging system header
 ****************************************************************************************************
*/

#ifndef _MESSAGING_H_
#define _MESSAGING_H_

// Utilities header
#include <StringHash/StringHash.h>

#include "../Utilities/GameEngineTypes.h"

namespace GameEngine
{
	namespace Messaging
	{
		bool Initialize( void );
		void ShutDown( void );

		void CreateMessage( const Utilities::StringHash &i_messageName, UINT32 i_u32Priority );
		void ProcessMessage( const Utilities::StringHash &i_messageName, void *i_messageData );
		void AddMessageHandler( const Utilities::StringHash &i_messageName, MessageHandler i_messageHandler, UINT32 i_u32Priority );
	}
}

#endif  // #ifndef _MESSAGING_H_