/**
 ****************************************************************************************************
 * \file		ParserHelper.h
 * \brief		Parser helper function declaration
 ****************************************************************************************************
*/

#ifndef _PARSER_HELPER_H_
#define _PARSER_HELPER_H_

#include <d3d9.h>
#include <string>
#include <D3DX9Shader.h>

#include "TagList.h"
#include "../Singleton/Singleton.h"

namespace Utilities
{
	class ParserHelper
	{
		friend Utilities::Singleton<ParserHelper>;

		std::string _tagList[Parser::E_INPUT_TYPE_TOTAL];

		ParserHelper( void );
		~ParserHelper( void );

		void InitializeTagList( void );
		void ReleaseTagList( void );

	public:

		Parser::E_INPUT_TYPE SetInputType( const char *i_tag );
		void CleanInputString( char *i_str );
		D3DXVECTOR3 ParseVector3Input( char *i_str );
		D3DXVECTOR2 ParseVector2Input( char *i_str );
		D3DCOLOR ParseColourInput( char *i_str );
	};
}

typedef Utilities::Singleton<Utilities::ParserHelper> g_parserHelper;

#endif	// #ifndef _PARSER_HELPER_H_