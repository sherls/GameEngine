/**
 ****************************************************************************************************
 * \file		Text.cpp
 * \brief		Text class implementation
 ****************************************************************************************************
*/

#include <assert.h>

// Utilities header
#include <UtilitiesDefault.h>

#include "Text.h"
#include "../Renderer/MainRenderer.h"

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize the text class
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Text::Initialize( const char *i_fontName, const UINT32 &i_fontHeight )
{
	_font = NULL;
	_fontSprite = NULL;

	assert( i_fontName != NULL );
	assert( i_fontName[0] != '\0' );
	assert( i_fontHeight > 0 );

	HRESULT result = D3DXCreateFont( g_mainRenderer::Get().GetDirect3dDevice(), i_fontHeight, 0, FW_BOLD, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, i_fontName, &_font );
	assert( SUCCEEDED(result) );

	result = D3DXCreateSprite( g_mainRenderer::Get().GetDirect3dDevice(), &_fontSprite );
	assert( SUCCEEDED(result) );

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shut down the text class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Text::ShutDown( void )
{
	if( _font )
	{
		_font->Release();
		_font = NULL;
	}

	if( _fontSprite )
	{
		_fontSprite->Release();
		_fontSprite = NULL;
	}
}

/**
 ****************************************************************************************************
	\fn			void BeginText( void )
	\brief		Begin drawing the text
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Text::BeginText( void )
{
	HRESULT result = _fontSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
	assert( SUCCEEDED(result) );
}

/**
 ****************************************************************************************************
	\fn			void Draw( const S_TEXT_TO_DRAW &i_textToDraw )
	\brief		Draw the text on screen
	\param		i_textToDraw the text to be drawn on screen
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Text::Draw( const S_TEXT_TO_DRAW &i_textToDraw )
{
	assert( i_textToDraw.text.length() > 0 );
	assert( i_textToDraw.text[0] != '\0' );
	RECT textRect = { static_cast<long>(i_textToDraw.position.x), static_cast<long>(i_textToDraw.position.y),
		static_cast<long>(i_textToDraw.position.x + i_textToDraw.size.x), static_cast<long>(i_textToDraw.position.y + i_textToDraw.size.y) };

	int result = _font->DrawText( _fontSprite, i_textToDraw.text.c_str(), -1, &textRect,
		i_textToDraw.hAlign | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE, i_textToDraw.colour );
	assert( result > 0 );
}

/**
 ****************************************************************************************************
	\fn			void EndText( void )
	\brief		End drawing the text
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Text::EndText( void )
{
	HRESULT result = _fontSprite->End();
	assert( SUCCEEDED(result) );
}