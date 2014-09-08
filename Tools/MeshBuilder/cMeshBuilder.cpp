/**
 ****************************************************************************************************
 * \file		cMeshBuilder.cpp
 * \brief		Implementation of cMeshBuilder class
 *
 * \date		12 January 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 *
 * \date		15 February 2013
 * \author		Sherly Yunita \n
 *				Parse the input file and output it as binary file \n
 *				\n\n
 ****************************************************************************************************
*/

#include "cMeshBuilder.h"

#include <vector>
#include <fstream>
#include <stdint.h>
#include "TagList.h"
#include "ParserHelper.h"

// Inherited Implementation
//=========================

/****************************************************************************************************
			Helper function implementation
****************************************************************************************************/

/****************************************************************************************************
			Main function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const
	\brief		Output input file
	\param		i_fileName_source name of source file
	\param		i_fileName_targe name of target file
	\return		NONE

	\date		15 February 2013
	\author		Sherly Yunita \n
				Rewrite to output as binary data \n
				\n\n

	\date		1 March 2013
	\author		Sherly Yunita \n
				Get tangents and biNormal data \n
				\n\n
****************************************************************************************************
*/
bool Tools::cMeshBuilder::Build_derived( const char* i_fileName_source, const char* i_fileName_target ) const
{
	// For now mesh don't get "built";
	// instead, copy the source mesh as-is to the target
	/* Sherly 15 February 2013 marked
	BOOL shouldFailIfDestinationFileAlreadyExists = FALSE;
	if ( CopyFile( i_fileName_source, i_fileName_target, shouldFailIfDestinationFileAlreadyExists ) == TRUE )
	{
		return true;
	}
	else
	{
		std::string errorMessage = std::string( "Couldn't copy " ) + i_fileName_source + " to " + i_fileName_target + ":  "
			+ GetLastWindowsError();
		FormatAndOutputErrorMessage( errorMessage );
		return false;
	}*/

	ParserHelper parserHelper;

	std::ifstream in;
	std::ofstream out;

	in.open( i_fileName_source );
	if( in.fail() )
	{
		std::string errorMessage = std::string( "Couldn't open source file " ) + i_fileName_source + ":  " + GetLastWindowsError();
		FormatAndOutputErrorMessage( errorMessage );
		return false;
	}

	out.open( i_fileName_target, std::ios::binary );
	if( out.fail() )
	{
		std::string errorMessage = std::string( "Couldn't create target file " ) + i_fileName_target + ":  "
			+ GetLastWindowsError();
		FormatAndOutputErrorMessage( errorMessage );
		return false;
	}

	typedef struct _s_vertex_
	{
		Parser::S_FLOAT3 position;
		Parser::S_FLOAT3 normal;
		Parser::S_FLOAT3 tangent;	// Sherly 1 March 2013 added
		Parser::S_FLOAT3 biNormal;	// Sherly 1 March 2013 added
		Parser::S_FLOAT2 texcoord;
		unsigned int colour;
	} S_VERTEX;

	char chInput[MAX_INPUT_LEN];
	bool validArrayData = false;
	UINT32 totalVertices;
	UINT32 totalPrimitives;
	std::vector<uint16_t> index;
	std::vector<S_VERTEX> vertex;
	S_VERTEX tempVertex = {
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f },
		0
	};
	Parser::E_INPUT_TYPE currMajorData = Parser::E_INPUT_TYPE_TOTAL;
	Parser::E_INPUT_TYPE currMinorType = Parser::E_INPUT_TYPE_TOTAL;

	in.getline( &chInput[0], MAX_INPUT_LEN );
	parserHelper.CleanInputString( chInput );

	do
	{
		// Skip comment
		if( chInput[0] == '/' && chInput[1] == '/' )
		{
			in.getline( &chInput[0], MAX_INPUT_LEN );
			parserHelper.CleanInputString( chInput );
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

				Parser::E_INPUT_TYPE currInputType = parserHelper.SetInputType( tagStr );
				
				if( currMajorData == currInputType )
				{
					currMajorData = Parser::E_INPUT_TYPE_TOTAL;
					currMinorType = Parser::E_INPUT_TYPE_TOTAL;
				}
				else if( currMinorType == currInputType )
				{
					currMinorType = Parser::E_INPUT_TYPE_TOTAL;
				}
				else
				{
					FormatAndOutputErrorMessage( "Invalid closing tag" );
					return false;
				}
			}
			// Start tag
			else
			{
				char *tagStr;
				char *nextToken = NULL;
				tagStr = strtok_s( chInput, "</>", &nextToken );

				Parser::E_INPUT_TYPE currInputType = parserHelper.SetInputType( tagStr );

				if( (currInputType == Parser::E_TYPE_VERTEX)
					|| (currInputType == Parser::E_TYPE_INDEX)
					)
				{
					currMajorData = currInputType;

					in.getline( &chInput[0], MAX_INPUT_LEN );
					parserHelper.CleanInputString( chInput );

					if( currMajorData == Parser::E_TYPE_VERTEX )
					{
						totalVertices = atoi( chInput );
					}
					else
					{
						totalPrimitives = atoi( chInput );
						totalPrimitives /= 3;
					}
				}
				else
				{
					currMinorType = currInputType;
				}
			}
		}
		// }
		else
		{
			switch( currMajorData )
			{
			case Parser::E_TYPE_VERTEX:
				if( strcmp(chInput, "}") == 0 )
				{
					if( validArrayData )
					{
						vertex.push_back( tempVertex );
						validArrayData = false;
					}
					else
					{
						FormatAndOutputErrorMessage( "Invalid array data" );
						return false;
					}
					break;
				}
				else if( strcmp(chInput, "{") == 0 )
				{
					validArrayData = true;
					break;
				}

				if( validArrayData )
				{
					switch( currMinorType )
					{
					case Parser::E_TYPE_POSITION:
						tempVertex.position = parserHelper.ParseVector3Input( chInput );	break;

					case Parser::E_TYPE_NORMAL:
						tempVertex.normal = parserHelper.ParseVector3Input( chInput );	break;

					// Sherly 1 March 2013 added
					// {
					case Parser::E_TYPE_TANGENT:
						tempVertex.tangent = parserHelper.ParseVector3Input( chInput ); break;

					case Parser::E_TYPE_BI_NORMAL:
						tempVertex.biNormal = parserHelper.ParseVector3Input( chInput ); break;
					// }

					case Parser::E_TYPE_UV:
						tempVertex.texcoord = parserHelper.ParseVector2Input( chInput );	break;

					case Parser::E_TYPE_COLOUR:
						tempVertex.colour = parserHelper.ParseColourInput( chInput );	break;
					}
				}
				break;

			case Parser::E_TYPE_INDEX:
				char *token = NULL;
				char *nextToken = NULL;

				token = strtok_s( chInput, ",", &nextToken );
				index.push_back( atoi(token) );

				token = strtok_s( NULL, ",", &nextToken );
				index.push_back( atoi(token) );

				token = strtok_s( NULL, " ", &nextToken );
				index.push_back( atoi(token) );
				nextToken = NULL;
				break;
			}
		}

		in.getline( &chInput[0], MAX_INPUT_LEN );
		parserHelper.CleanInputString( chInput );
	} while( !in.eof() );

	out.write( reinterpret_cast <const char*> (&totalVertices), sizeof(totalVertices) );
	out.write( reinterpret_cast <const char*> (&totalPrimitives), sizeof(totalPrimitives) );
	for( unsigned int i = 0; i < totalVertices; i++ )
		out.write( reinterpret_cast <const char*> (&vertex[i]), sizeof(S_VERTEX) );
	for( unsigned int i = 0; i < index.size(); i++ )
		out.write( reinterpret_cast <const char*>(&index[i]), sizeof(uint16_t) );

	in.close();
	out.close();

	return true;
}
