/**
 ****************************************************************************************************
 * \file		FlagController.h
 * \brief		The header of FlagController class
 ****************************************************************************************************
*/

#ifndef _FLAG_CONTROLLER_H_
#define _FLAG_CONTROLLER_H_

namespace GameEngine
{
	class Entity;
	class EntityController;
}

class FlagController : public GameEngine::EntityController
{
public:
	FlagController( const bool &i_bPlayerHasThis, const char* i_playerName, const char *i_flagName ) :
			_bPlayerHasThis( i_bPlayerHasThis ),
			_playerName( i_playerName ),
			_flagAreaName( i_flagName )
	{

	}
	~FlagController( void ) {}

	void BeginUpdate( GameEngine::Entity &i_entity ) {}
	void Update( GameEngine::Entity &i_entity ) {}
	void EndUpdate( GameEngine::Entity &i_entity );

private:
	const bool &_bPlayerHasThis;
	const char *_playerName;
	const char *_flagAreaName;
};

#endif	// #ifndef _FLAG_CONTROLLER_H_