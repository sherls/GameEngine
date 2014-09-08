/**
 ****************************************************************************************************
 * \file		Audio.cpp
 * \brief		Audio engine implementation
 ****************************************************************************************************
*/

#include <map>

#include "Audio.h"

// FMOD
#include <fmod.hpp>

// Utilities header
#include <Time/Time.h>
#include <Debug/Debug.h>
#include <UtilitiesDefault.h>
#include <SmartPtr/SmartPtr.h>
#include <StringHash/StringHash.h>

#include "World/World.h"
#include "World/Entity.h"
#include "GameEngineDefault.h"
#include "DebugMenu/DebugMenu.h"

namespace GameEngine
{
	namespace Audio
	{
		typedef struct _s_3d_sfx_position_
		{
			Math::Vector3 &v3Position;
			FMOD_VECTOR position;
			FMOD_VECTOR lastPosition;
		} S_3D_SFX_POSITION;

		bool bInitialized = false;
		FMOD::System *system = NULL;
		FMOD::Sound *bgSound = NULL;
		FMOD::Channel *bgChannel = NULL;
		FMOD::Channel *sfxChannel[MAX_SFX];
		FMOD::ChannelGroup *bgChGroup = NULL;
		FMOD::ChannelGroup *sfx2dChGroup = NULL;
		FMOD::ChannelGroup *sfx3dChGroup = NULL;
		FMOD::ChannelGroup *masterChGroup = NULL;

		std::map<Utilities::StringHash, FMOD::Sound*> *sfxDatabase = NULL;
		std::map<UINT32, S_3D_SFX_POSITION> *active3dSfx = NULL;

		FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
		FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };

		float generalVolume = 1.0f;
		float bgVolume = 1.0f;
		float sfx2dVolume = 1.0f;
		float sfx3dVolume = 1.0f;
		float prevGeneralVolume = 1.0f;
		float prevBgVolume = 1.0f;
		float prevSfx2dVolume = 1.0f;
		float prevSfx3dVolume = 1.0f;

		bool bGeneralMute = false;
		bool bBgMute = false;
		bool bSfx2dMute = false;
		bool bSfx3dMute = false;
		bool prevBGeneralMute = false;
		bool prevBBgMute = false;
		bool prevB2dSfxMute = false;
		bool prevB3dSfxMute = false;
	}
}

#define ERROR_CHECK( a ) if( a != FMOD_OK ){ FUNCTION_FINISH; return; }
#define ERROR_CHECK_BOOL( a ) if( a != FMOD_OK ){ FUNCTION_FINISH; return FAIL; }
#define CHANNEL_ERROR_CHECK( a ) if( (a != FMOD_OK) && (a != FMOD_ERR_INVALID_HANDLE) && (a != FMOD_ERR_CHANNEL_STOLEN) ) ERROR_CHECK(a)

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize audio engine
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::Audio::Initialize( void )
{
	FMOD_RESULT result;
	unsigned int version;

	FUNCTION_START;

	if( bInitialized )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( system == NULL )
	{
		result = FMOD::System_Create( &system );
		ERROR_CHECK_BOOL( result );
	}

	if( sfxDatabase == NULL )
		sfxDatabase = new std::map<Utilities::StringHash, FMOD::Sound*>();

	if( active3dSfx == NULL )
		active3dSfx = new std::map<UINT32, S_3D_SFX_POSITION>();

	result = system->getVersion( &version );
	ERROR_CHECK_BOOL( result );

	if( version < FMOD_VERSION )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	result = system->init( MAX_AUDIO_CHANNEL, FMOD_INIT_NORMAL, NULL );
	ERROR_CHECK_BOOL( result );

	result = system->createChannelGroup( "Background channel", &bgChGroup );
	ERROR_CHECK_BOOL( result );

	result = system->createChannelGroup( "2d sfx", &sfx2dChGroup );
	ERROR_CHECK_BOOL( result );

	result = system->createChannelGroup( "3d sfx", &sfx3dChGroup );
	ERROR_CHECK_BOOL( result );

	result = system->getMasterChannelGroup( &masterChGroup );
	ERROR_CHECK_BOOL( result );

	result = system->set3DSettings( 1.0, AUDIO_DISTANCE_FACTOR, 1.0f );
	ERROR_CHECK_BOOL( result );

	result = masterChGroup->addGroup( bgChGroup );
	ERROR_CHECK_BOOL( result );

	result = masterChGroup->addGroup( sfx2dChGroup );
	ERROR_CHECK_BOOL( result );

	result = masterChGroup->addGroup( sfx3dChGroup );
	ERROR_CHECK_BOOL( result );

	bInitialized = true;

	float minVolume = 0.0f;
	float maxVolume = 1.0f;
	float increment = 0.05f;

	g_debugMenu::Get().AddSlider( "General volume", generalVolume, minVolume, maxVolume, increment, "Audio" );
	g_debugMenu::Get().AddCheckBox( "Mute general audio", bGeneralMute, "Audio" );

	g_debugMenu::Get().AddSlider( "Background volume", bgVolume, minVolume, maxVolume, increment, "Audio" );
	g_debugMenu::Get().AddCheckBox( "Mute background audio", bBgMute, "Audio" );

	g_debugMenu::Get().AddSlider( "M2D sfx volume", sfx2dVolume, minVolume, maxVolume, increment, "Audio" );
	g_debugMenu::Get().AddCheckBox( "Mute 2D sfx audio", bSfx2dMute, "Audio" );

	g_debugMenu::Get().AddSlider( "3D sfx volume", sfx3dVolume, minVolume, maxVolume, increment, "Audio" );
	g_debugMenu::Get().AddCheckBox( "Mute 3D sfx audio", bSfx3dMute, "Audio" );

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin updating audio engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::BeginUpdate( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update audio engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::Update( void )
{
	float frameTime_ms = Utilities::Time::GetTimeElapsedThisFrame_ms();

	FUNCTION_START;

	if( prevBGeneralMute != bGeneralMute )
	{
		masterChGroup->setMute( bGeneralMute );
		prevBGeneralMute = bGeneralMute;
	}

	if( prevGeneralVolume != generalVolume )
	{
		masterChGroup->setVolume( generalVolume );
		prevGeneralVolume = generalVolume;
	}

	if( prevBBgMute != bBgMute )
	{
		bgChGroup->setMute( bBgMute );
		prevBBgMute = bBgMute;
	}

	if( prevBgVolume != bgVolume )
	{
		bgChGroup->setVolume( bgVolume );
		prevBgVolume = bgVolume;
	}

	if( prevB2dSfxMute!= bSfx2dMute )
	{
		sfx2dChGroup->setMute( bSfx2dMute );
		prevB2dSfxMute = bSfx2dMute;
	}

	if( prevSfx2dVolume != sfx2dVolume )
	{
		sfx2dChGroup->setVolume( sfx2dVolume );
		prevSfx2dVolume = sfx2dVolume;
	}

	if( prevB3dSfxMute!= bSfx3dMute )
	{
		sfx3dChGroup->setMute( bSfx3dMute );
		prevB3dSfxMute = bSfx3dMute;
	}

	if( prevSfx3dVolume != sfx3dVolume )
	{
		sfx3dChGroup->setVolume( sfx3dVolume );
		prevSfx3dVolume = sfx3dVolume;
	}

	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
	FMOD_RESULT result;
	std::map<UINT32, S_3D_SFX_POSITION>::iterator active3dSfxIter;
	for( active3dSfxIter = active3dSfx->begin(); active3dSfxIter != active3dSfx->end(); ++active3dSfxIter )
	{
		active3dSfxIter->second.position.x = active3dSfxIter->second.v3Position.X() * AUDIO_DISTANCE_FACTOR;
		active3dSfxIter->second.position.y = active3dSfxIter->second.v3Position.Y() * AUDIO_DISTANCE_FACTOR;
		active3dSfxIter->second.position.z = active3dSfxIter->second.v3Position.Z() * AUDIO_DISTANCE_FACTOR;

		velocity.x = (active3dSfxIter->second.position.x - active3dSfxIter->second.lastPosition.x) / frameTime_ms;
		velocity.y = (active3dSfxIter->second.position.y - active3dSfxIter->second.lastPosition.y) / frameTime_ms;
		velocity.z = (active3dSfxIter->second.position.z - active3dSfxIter->second.lastPosition.z) / frameTime_ms;

		result = sfxChannel[active3dSfxIter->first]->set3DAttributes( &active3dSfxIter->second.position, &velocity );

		active3dSfxIter->second.lastPosition = active3dSfxIter->second.position;
	}

	Utilities::Pointer::SmartPtr<GameEngine::Entity> player = g_world::Get().GetEntityByName( "Player" );
	if( player )
	{
		FMOD_VECTOR listenerPos = { player->m_v3Position.X() * AUDIO_DISTANCE_FACTOR, \
			player->m_v3Position.Y() * AUDIO_DISTANCE_FACTOR,\
			player->m_v3Position.Z() * AUDIO_DISTANCE_FACTOR };
		FMOD_VECTOR listenerVel= { player->m_v3Velocity.X() * AUDIO_DISTANCE_FACTOR, \
			player->m_v3Velocity.Y() * AUDIO_DISTANCE_FACTOR, \
			player->m_v3Velocity.Z() * AUDIO_DISTANCE_FACTOR };

		result = system->set3DListenerAttributes( 0, &listenerPos, &listenerVel, &forward, &up );
	}

	result = system->update();
	ERROR_CHECK( result );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End updating audio engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::EndUpdate( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shutdown audio engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::ShutDown( void )
{
	FUNCTION_START;

	std::map<Utilities::StringHash, FMOD::Sound*>::iterator iter = sfxDatabase->begin();
	for( ; iter != sfxDatabase->end(); ++iter )
		iter->second->release();

	if( bgSound )
	{
		bgSound->release();
		bgSound = NULL;
	}

	if( bgChannel )
		bgChannel = NULL;

	if( bgChGroup )
	{
		bgChGroup->release();
		bgChGroup = NULL;
	}

	if( sfx2dChGroup )
	{
		sfx2dChGroup->release();
		sfx2dChGroup = NULL;
	}

	if( sfx3dChGroup )
	{
		sfx3dChGroup->release();
		sfx3dChGroup = NULL;
	}

	if( masterChGroup )
	{
		masterChGroup->release();
		masterChGroup = NULL;
	}

	if( system )
	{
		system->close();
		system->release();
		system = NULL;
	}

	if( sfxDatabase )
	{
		delete sfxDatabase;
		sfxDatabase = NULL;
	}

	if( active3dSfx )
	{
		delete active3dSfx;
		active3dSfx = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void PlayBackgroundMusic( char *i_audioName )
	\brief		Play background music
	\param		i_audioName name of the audio
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::PlayBackgroundMusic( char *i_audioName )
{
	FMOD_RESULT result;
	char audioName[MAX_FILENAME_INPUT];
	bool bIsPlaying;

	FUNCTION_START;

	if( !bInitialized )
	{
		FUNCTION_FINISH;
		return;
	}

	if( bgSound != NULL )
	{
		bgSound->release();
		bgSound = NULL;
	}

	strcpy_s( audioName, MAX_FILENAME_INPUT, "../../External/Data/Audio/" );
	strcat_s( audioName, MAX_FILENAME_INPUT, i_audioName );

	result = system->createSound( audioName, FMOD_DEFAULT, 0, &bgSound );
	ERROR_CHECK( result );

	if( bgChGroup )
	{
		result = bgChGroup->isPlaying( &bIsPlaying );
		CHANNEL_ERROR_CHECK( result );
	}

	if( bIsPlaying )
	{
		result = bgChGroup->stop();
		CHANNEL_ERROR_CHECK( result );
	}

	result = system->playSound( bgSound, NULL, false, &bgChannel );
	ERROR_CHECK( result );

	result = bgChannel->setChannelGroup( bgChGroup );
	ERROR_CHECK( result );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Add2DSoundEffect( char *i_audioName )
	\brief		Add 2D sound effect to sound effect database
	\param		i_audioName name of the audio
	\return		UINT32 index of sound effect
 ****************************************************************************************************
*/
UINT32 GameEngine::Audio::Add2DSoundEffect( char *i_audioName )
{
	FMOD_RESULT result;
	char audioName[MAX_FILENAME_INPUT];
	Utilities::StringHash hashedAudioName( i_audioName );

	FUNCTION_START;

	std::map<Utilities::StringHash, FMOD::Sound*>::iterator iter;

	iter = sfxDatabase->find( hashedAudioName );
	if( iter == sfxDatabase->end() )
	{
		FMOD::Sound *newSound = NULL;
		strcpy_s( audioName, MAX_FILENAME_INPUT, "../../External/Data/Audio/" );
		strcat_s( audioName, MAX_FILENAME_INPUT, i_audioName );
		result = system->createSound( audioName, FMOD_2D, 0, &newSound );
		ERROR_CHECK_BOOL( result );

		std::pair<Utilities::StringHash, FMOD::Sound*> insertingPair( hashedAudioName, newSound );
		sfxDatabase->insert( insertingPair );
	}

	FUNCTION_FINISH;
	return sfxDatabase->size() - 1;
}

/**
 ****************************************************************************************************
	\fn			void Add3DSoundEffect( char *i_audioName )
	\brief		Add 3D sound effect to sound effect database
	\param		i_audioName name of the audio
	\return		UINT32 index of sound effect
 ****************************************************************************************************
*/
UINT32 GameEngine::Audio::Add3DSoundEffect( char *i_audioName )
{
	FMOD_RESULT result;
	char audioName[MAX_FILENAME_INPUT];
	Utilities::StringHash hashedAudioName( i_audioName );

	FUNCTION_START;

	std::map<Utilities::StringHash, FMOD::Sound*>::iterator iter;

	iter = sfxDatabase->find( hashedAudioName );
	if( iter == sfxDatabase->end() )
	{
		FMOD::Sound *newSound = NULL;
		strcpy_s( audioName, MAX_FILENAME_INPUT, "../../External/Data/Audio/" );
		strcat_s( audioName, MAX_FILENAME_INPUT, i_audioName );
		result = system->createSound( audioName, FMOD_3D, 0, &newSound );
		ERROR_CHECK_BOOL( result );

		result = newSound->set3DMinMaxDistance( AUDIO_3D_MIN_DISTANCE * AUDIO_DISTANCE_FACTOR, \
			AUDIO_3D_MAX_DISTANCE * AUDIO_DISTANCE_FACTOR );
		ERROR_CHECK_BOOL( result );

		result = newSound->setMode( FMOD_LOOP_NORMAL );
		ERROR_CHECK_BOOL( result );

		std::pair<Utilities::StringHash, FMOD::Sound*> insertingPair( hashedAudioName, newSound );
		sfxDatabase->insert( insertingPair );
	}

	FUNCTION_FINISH;
	return sfxDatabase->size() - 1;
}

/**
 ****************************************************************************************************
	\fn			void Play2DSoundEffect( const UINT32 &i_u32ChIndex, char *i_audioName )
	\brief		Play 2D sound effect
	\param		i_u32ChIndex designated channel index for this audio
	\param		i_audioName name of the audio
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::Play2DSoundEffect( UINT32 &i_u32ChIndex, char *i_audioName )
{
	FMOD_RESULT result;
	Utilities::StringHash hashedAudioName( i_audioName );

	FUNCTION_START;

	std::map<Utilities::StringHash, FMOD::Sound*>::iterator iter;
	iter = sfxDatabase->find( hashedAudioName );
	if( iter == sfxDatabase->end() )
		i_u32ChIndex = Add2DSoundEffect( i_audioName );

	result =  system->playSound( iter->second, NULL, false, &sfxChannel[i_u32ChIndex] );
	ERROR_CHECK( result );

	result = sfxChannel[i_u32ChIndex]->setChannelGroup( sfx2dChGroup );
	ERROR_CHECK( result );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Play3DSoundEffect( UINT32 &i_u32ChIndex, char *i_audioName, const Math::Vector3 &i_position )
	\brief		Add sound effect to sound effect database
	\param		i_u32ChIndex designated channel index for this audio
	\param		i_audioName name of the audio
	\param		i_position position of the audio source
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::Play3DSoundEffect( UINT32 &i_u32ChIndex, char *i_audioName, Math::Vector3 &i_position )
{
	FMOD_RESULT result;
	FMOD_VECTOR position = { i_position.X() * AUDIO_DISTANCE_FACTOR, i_position.Y() * AUDIO_DISTANCE_FACTOR,\
		i_position.Z() * AUDIO_DISTANCE_FACTOR };
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
	Utilities::StringHash hashedAudioName( i_audioName );

	FUNCTION_START;

	std::map<UINT32, S_3D_SFX_POSITION>::iterator active3dSfxIter;
	active3dSfxIter = active3dSfx->find( i_u32ChIndex );
	if( active3dSfxIter != active3dSfx->end() )
		return;

	S_3D_SFX_POSITION new3dPosition = { i_position, position, position };
	std::pair<UINT32, S_3D_SFX_POSITION> insertingPair( i_u32ChIndex, new3dPosition );
	active3dSfx->insert( insertingPair );

	std::map<Utilities::StringHash, FMOD::Sound*>::iterator sfxIter;
	sfxIter = sfxDatabase->find( hashedAudioName );
	if( sfxIter == sfxDatabase->end() )
		i_u32ChIndex = Add3DSoundEffect( i_audioName );

	result =  system->playSound( sfxIter->second, NULL, false, &sfxChannel[i_u32ChIndex] );
	ERROR_CHECK( result );

	result = sfxChannel[i_u32ChIndex]->set3DAttributes( &position, &velocity );
	ERROR_CHECK( result );

	result = sfxChannel[i_u32ChIndex]->setPaused( false );
	ERROR_CHECK( result );

	result = sfxChannel[i_u32ChIndex]->setChannelGroup( sfx3dChGroup );
	ERROR_CHECK( result );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Stop3DSoundEffect( UINT32 &i_u32ChIndex )
	\brief		Stop 3D sound effect
	\param		i_u32ChIndex designated channel index for this audio
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::Stop3DSoundEffect( UINT32 &i_u32ChIndex )
{
	FMOD_RESULT result;

	FUNCTION_START;

	std::map<UINT32, S_3D_SFX_POSITION>::iterator active3dSfxIter;
	active3dSfxIter = active3dSfx->find( i_u32ChIndex );
	if( active3dSfxIter != active3dSfx->end() )
	{
		result = sfxChannel[i_u32ChIndex]->setPaused( true );
		ERROR_CHECK( result );
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
 \fn			void DisableBgAudio( void )
 \brief		Disable background audio
 \param		NONE
 \return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::DisableBgAudio( void )
{
	bBgMute = true;
}

/**
 ****************************************************************************************************
 \fn			void Disable2DSfx( void )
 \brief		Disable 2D sfx
 \param		NONE
 \return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::Disable2DSfx( void )
{
	bSfx2dMute = true;
}

/**
 ****************************************************************************************************
	\fn			void Disable3DSfx( void )
	\brief		Disable 3D sfx
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Audio::Disable3DSfx( void )
{
	bSfx3dMute = true;
}