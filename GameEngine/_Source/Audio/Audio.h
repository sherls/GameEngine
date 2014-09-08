/**
 ****************************************************************************************************
 * \file		Audio.h
 * \brief		Audio engine global function declaration
 ****************************************************************************************************
*/

#ifndef _AUDIO_H_
#define _AUDIO_H_

// Utilities header
#include <UtilitiesTypes.h>

#include "Math/Vector3/Vector3.h"

namespace GameEngine
{
	namespace Audio
	{
		bool Initialize( void );
		void BeginUpdate( void );
		void Update( void );
		void EndUpdate( void );
		void ShutDown( void );

		void PlayBackgroundMusic( char *i_audioName );
		UINT32 Add2DSoundEffect( char *i_audioName );
		UINT32 Add3DSoundEffect( char *i_audioName );
		void Play2DSoundEffect( UINT32 &i_u32ChIndex, char *i_audioName );
		void Play3DSoundEffect( UINT32 &i_u32ChIndex, char *i_audioName, Math::Vector3 &i_position );
		void Stop3DSoundEffect( UINT32 &i_u32ChIndex );

		void DisableBgAudio( void );
		void Disable2DSfx( void );
		void Disable3DSfx( void );
	}
}

#endif	// #ifndef _AUDIO_H_