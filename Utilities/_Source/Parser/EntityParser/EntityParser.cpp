/**
 ****************************************************************************************************
 * \file		EntityParser.cpp
 * \brief		Entity parser class implementation
 ****************************************************************************************************
*/

#include <fstream>

#include "../TagList.h"
#include "../ParserHelper.h"
#include "../../Debug/Debug.h"

#include "EntityParser.h"

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void LoadEntityData( const char *i_fileName )
	\brief		Load entity data from input file
	\param		i_fileName name of the file where entity data is located
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::EntityParser::LoadEntityData( const char *i_fileName )
{
	std::ifstream in;
	char chInput[MAX_INPUT_LEN];
	Parser::E_INPUT_TYPE currInputType = Parser::E_INPUT_TYPE_TOTAL;

	char fileName[MAX_FILENAME_INPUT];
	strcpy_s( fileName, MAX_FILENAME_INPUT, "../../External/Data/Entities/" );
	strcat_s( fileName, MAX_FILENAME_INPUT, i_fileName );

	in.open( fileName );

	if( in.fail() )
	{
		DEBUG_MSG( DBG_RENDERER, D_ERR, "Failed to open entity data %s", i_fileName );
		return;
	}

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
				{
					currInputType = Parser::E_INPUT_TYPE_TOTAL;
				}
				else
				{
					DEBUG_MSG( DBG_RENDERER, D_ERR, "Invalid closing tag" );
				}
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
		// }
		else
		{
			switch( currInputType )
			{
			case Parser::E_TYPE_MESH:
				m_meshData = chInput;					break;

			case Parser::E_TYPE_MATERIAL:
				m_materialData = chInput;				break;
			}
		}

		in.getline( &chInput[0], MAX_INPUT_LEN );
		g_parserHelper::Get().CleanInputString( chInput );
	} while ( !in.eof() );

	in.close();

	return;
}