/**
 ****************************************************************************************************
 * \file		Messaging.cpp
 * \brief		Messaging system implementation
 ****************************************************************************************************
*/

#include <vector>
#include <assert.h>
#include <algorithm>

#include "Messaging.h"

namespace GameEngine
{
	namespace Messaging
	{
		typedef struct _messageHandler
		{
			UINT32 u32Priority;
			MessageHandler handler;

			bool operator<( const _messageHandler &i_other ) const
			{
				return u32Priority < i_other.u32Priority;
			}

			bool operator==( const _messageHandler &i_other ) const
			{
				return handler == i_other.handler;
			}
		} S_MESSAGE_HANDLER;

		typedef struct _message
		{
			UINT32 u32Priority;
			Utilities::StringHash message;
			std::vector< S_MESSAGE_HANDLER > handlers;

			bool operator<( const _message &i_other ) const
			{
				return u32Priority < i_other.u32Priority;
			}

			bool operator==( const _message &i_other ) const
			{
				return message == i_other.message;
			}
		} S_MESSAGE;

		static std::vector<S_MESSAGE> *messages;
	}
}

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize messaging class
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::Messaging::Initialize( void )
{
	FUNCTION_START;

	messages = new std::vector<S_MESSAGE>();

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shutdown messaging class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Messaging::ShutDown( void )
{
	FUNCTION_START;

	messages->clear();
	delete messages;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CreateMessage( const StringHash &i_message, UINT32 i_u32Priority )
	\brief		Create message
	\param		i_message message name
	\param		i_u32Priority message priority
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Messaging::CreateMessage( const Utilities::StringHash &i_message, UINT32 i_u32Priority )
{
	S_MESSAGE temp;

	FUNCTION_START;

	temp.message = i_message;

	std::vector<S_MESSAGE>::iterator iter = std::lower_bound( messages->begin(), messages->end(), temp );

	if( iter == messages->end() || temp < (*iter) )
	{
		temp.u32Priority = i_u32Priority;
		messages->insert( iter, temp );
	}
	else
	{
		assert( iter->u32Priority == i_u32Priority );
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void AddMessageHandler( const StringHash &i_message, MessageHandler i_handler, UINT32 i_u32Priority )
	\brief		Add message handler to the message with given priority
	\param		i_message message name
	\param		i_handler the message handler
	\param		i_u32Priority message priority
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Messaging::AddMessageHandler( const Utilities::StringHash &i_message, MessageHandler i_handler, UINT32 i_u32Priority )
{
	assert( i_handler );

	S_MESSAGE lookUpMessage;
	lookUpMessage.message = i_message;
	std::vector<S_MESSAGE>::iterator iterMessage = std::find( messages->begin(), messages->end(), lookUpMessage );
	assert( iterMessage != messages->end() );

	S_MESSAGE_HANDLER lookUpHandler;
	lookUpHandler.handler = i_handler;
	std::vector<S_MESSAGE_HANDLER>::iterator iterHandler = std::lower_bound( iterMessage->handlers.begin(), iterMessage->handlers.end(), lookUpHandler );

	FUNCTION_START;

	if( ( iterHandler == iterMessage->handlers.end() )  ||  ( lookUpHandler < *iterHandler ) )
	{
		lookUpHandler.u32Priority = i_u32Priority;

		iterMessage->handlers.insert( iterHandler, lookUpHandler );
	}
	else
	{
		assert( iterHandler->u32Priority == i_u32Priority );
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ProcessMessage( const StringHash & i_message, void * i_messageData )
	\brief		Process message
	\param		i_message message name
	\param		i_messageData the message data
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Messaging::ProcessMessage( const Utilities::StringHash & i_message, void * i_messageData )
{
	assert( i_message );

	S_MESSAGE thisMessage;
	thisMessage.message = i_message;
	std::vector<S_MESSAGE>::iterator iterMessage = std::find( messages->begin(), messages->end(), thisMessage );

	FUNCTION_START;

	if( iterMessage == messages->end() )
	{
		FUNCTION_FINISH;
		return;
	}

	std::vector<S_MESSAGE_HANDLER>::iterator iter;
	
	for( iter = iterMessage->handlers.begin(); iter != iterMessage->handlers.end(); iter++ )
	{
		assert(	iter->handler );
		iter->handler( i_messageData );
	}

	FUNCTION_FINISH;
}