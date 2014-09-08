/**
 ****************************************************************************************************
 * \file		Text.h
 * \brief		Text class declaration
 ****************************************************************************************************
*/

#ifndef _TEXT_H_
#define _TEXT_H_

// Utilities header
#include <Singleton/Singleton.h>

#include "../RendererEngine.h"
#include "../RendererEngineTypes.h"

namespace RendererEngine
{
	class Text
	{
		friend Utilities::Singleton<Text>;

		ID3DXFont*		_font;
		ID3DXSprite*	_fontSprite;

		Text( void ){ }
		~Text( void ){ }

	public:
		bool Initialize( const char *i_fontName, const UINT32 &i_fontHeight );
		void ShutDown( void );

		void BeginText( void );
		void Draw( const S_TEXT_TO_DRAW &i_textToDraw );
		void EndText( void );
	};
}

typedef Utilities::Singleton<RendererEngine::Text> g_text;

#endif	// #ifndef _TEXT_H_