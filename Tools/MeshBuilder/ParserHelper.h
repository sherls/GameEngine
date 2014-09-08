/**
 ****************************************************************************************************
 * \file		ParserHelper.h
 * \brief		Parser helper function declaration
 *
 * \date		15 February 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

#ifndef _PARSER_HELPER_H_
#define _PARSER_HELPER_H_

#include "TagList.h"

#include <string>

namespace Tools
{
	class ParserHelper
	{
		std::string _tagList[Parser::E_INPUT_TYPE_TOTAL];

	public:
		ParserHelper( void );
		~ParserHelper( void );

		Parser::E_INPUT_TYPE SetInputType( const char *i_tag );
		void CleanInputString( char *i_str );
		Parser::S_FLOAT3 ParseVector3Input( char *i_str );
		Parser::S_FLOAT2 ParseVector2Input( char *i_str );
		unsigned int ParseColourInput( char *i_str );
	};
}

#endif	// #ifndef _PARSER_HELPER_H_