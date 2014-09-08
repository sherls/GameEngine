/**
 ****************************************************************************************************
 * \file		Renderer.h
 * \brief		Renderer global function declaration
 ****************************************************************************************************
*/

#ifndef _RENDERER_H_
#define _RENDERER_H_

// Utilities header
#include <UtilitiesDefault.h>
#include <SmartPtr/SmartPtr.h>

#include "../Utilities/GameEngineTypes.h"

namespace GameEngine
{
	class Entity;

	namespace Renderer
	{
		bool Initialize( const char* i_GameName, const UINT16 &i_u16Width, const UINT16 &i_u16Height,
			const HINSTANCE &i_currInstance, const int &i_initialWindowDisplayState, const HWND &i_hwnd );
		void BeginUpdate( void );
		void Update( void );
		void UpdateGUI( void );
		void EndUpdate( void );
		void ShutDown( void );

		// 2D objects
		void AddSprite( Utilities::Pointer::SmartPtr<Entity> &i_entity, const D3DCOLOR &i_colour,
			const char *i_textureFile );
		void DrawText( const D3DXVECTOR2 &i_position, const D3DXVECTOR2 &i_size, D3DCOLOR i_colour, \
			const std::string &i_text, UINT32 i_hAlign );
		void DrawSlider( const D3DXVECTOR2 &i_position, const UINT32 &i_u32CurrValue, const UINT32 &i_u32MinValue, \
			const UINT32 &i_u32MaxValue, \
			const D3DCOLOR &i_backgroundColour = Utilities::BLACK, const D3DCOLOR &i_foregroundColour = Utilities::GREEN );

		// 3D objects using RendererEngine
		void AddMesh( Utilities::Pointer::SmartPtr<Entity> &i_entity, const char *i_entityFile );

		Utilities::S_SIZE &GetWindowSize( void );
	}
}

#endif	// #ifndef _RENDERER_H_