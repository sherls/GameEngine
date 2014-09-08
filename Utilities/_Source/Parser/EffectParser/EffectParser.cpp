/**
 ****************************************************************************************************
 * \file		EffectParser.cpp
 * \brief		Effect parser class implementation
 ****************************************************************************************************
*/

#include <fstream>

#include "../TagList.h"
#include "../ParserHelper.h"
#include "../../UtilitiesTypes.h"
#include "../../Debug/Debug.h"

#include "EffectParser.h"

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void LoadEffectData( const char *i_fileName )
	\brief		Load effect data from input file
	\param		i_fileName name of the file where effect data is located
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::EffectParser::LoadEffectData( const char *i_fileName )
{
	std::ifstream in;
	char chInput[MAX_INPUT_LEN];
	Parser::E_INPUT_TYPE currInputType = Parser::E_INPUT_TYPE_TOTAL;

	char fileName[MAX_FILENAME_INPUT];
	strcpy_s( fileName, MAX_FILENAME_INPUT, "../../External/Data/Effects/" );
	strcat_s( fileName, MAX_FILENAME_INPUT, i_fileName );

	in.open( fileName );

	if( in.fail() )
	{
		DEBUG_MSG( DBG_RENDERER, D_ERR, "Failed to open effect data %s", i_fileName );
		return;
	}

	m_fragmentShader = new std::vector<std::string>();

	m_textureMode = 0;
	in.getline( &chInput[0], MAX_INPUT_LEN );
	g_parserHelper::Get().CleanInputString( chInput );

	do
	{
		// Skip comment
		if( chInput[0] == '/' && chInput[1] == '/' )
		{
			in.getline( &chInput[0], MAX_INPUT_LEN );
			g_parserHelper::Get().CleanInputString( chInput );
		}

		// Check for tags
		// {
		if( chInput[0] == '<' )
		{
			// Closing tag
			if( chInput[1] == '/' )
			{
				char *tagStr;
				char *nextToken = NULL;
				tagStr = strtok_s( chInput, "</>", &nextToken );

				Parser::E_INPUT_TYPE currType = g_parserHelper::Get().SetInputType( tagStr );

				if( currType == currInputType )
					currInputType = Parser::E_INPUT_TYPE_TOTAL;
				else
					DEBUG_MSG( DBG_RENDERER, D_ERR, "Invalid closing tag" );
			}
			// Start tag
			else
			{
				char *tagStr;
				char *nextToken = NULL;
				tagStr = strtok_s( chInput, "</>", &nextToken );

				currInputType = g_parserHelper::Get().SetInputType( tagStr );
			}
		}
		else
		{
			switch( currInputType )
			{
			case Parser::E_TYPE_VERTEX_SHADER:
				m_vertexShader = chInput;				break;

			case Parser::E_TYPE_FRAGMENT_SHADER:
				m_fragmentShader->push_back( chInput );	break;

			case Parser::E_TYPE_RENDER_STATE:
				if( strcmp(chInput, "alpha_blend") == 0 )
					m_renderState = E_ALPHA_MODE_BLEND;
				else if( strcmp(chInput, "alpha_binary") == 0 )
					m_renderState = E_ALPHA_MODE_BINARY;
				else if( strcmp(chInput, "alpha_additive") == 0 )
					m_renderState = E_ALPHA_MODE_ADDITIVE;
				else
					m_renderState = (E_ALPHA_MODE)atoi(chInput);
				break;

			case Parser::E_TYPE_TEXTURE_MODE:
				if( strcmp(chInput, "diffuse_map") == 0 )
					m_textureMode |= DIFFUSE_MAP;
				else if( strcmp(chInput, "normal_map") == 0 )
					m_textureMode |= NORMAL_MAP;
				else if( strcmp(chInput, "environment_map") == 0 )
					m_textureMode |= ENVIRONMENT_MAP;
				else if( strcmp(chInput, "opaque_buffer") == 0 )
					m_textureMode |= OPAQUE_BUFFER;
				else if( strcmp(chInput, "z_buffer") == 0 )
					m_textureMode |= Z_BUFFER;
				break;
			}
		}

		in.getline( &chInput[0], MAX_INPUT_LEN );
		g_parserHelper::Get().CleanInputString( chInput );
	} while ( !in.eof() );

	in.close();
	return;
}