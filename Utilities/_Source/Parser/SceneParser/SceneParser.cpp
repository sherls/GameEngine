/**
 ****************************************************************************************************
 * \file		SceneParser.cpp
 * \brief		Scene parser class implementation
 ****************************************************************************************************
*/

#include <fstream>

#include "../TagList.h"
#include "../ParserHelper.h"
#include "../../Debug/Debug.h"

#include "SceneParser.h"

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void LoadSceneData( const char* i_fileName )
	\brief		Load scene data from input file
	\param		i_fileName name of the file where scene data is located
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::SceneParser::LoadSceneData( const char* i_fileName )
{
	std::ifstream in;
	UINT32 u32Ctr = 0;
	char chInput[MAX_INPUT_LEN];
	bool validArrayData = false;
	Utilities::Parser::E_INPUT_TYPE currMajorData = Utilities::Parser::E_INPUT_TYPE_TOTAL;
	Utilities::Parser::E_INPUT_TYPE currMinorType = Utilities::Parser::E_INPUT_TYPE_TOTAL;

	char fileName[MAX_FILENAME_INPUT];
	strcpy_s( fileName, MAX_FILENAME_INPUT, "../../External/Data/Scenes/" );
	strcat_s( fileName, MAX_FILENAME_INPUT, i_fileName );

	in.open( fileName );

	if( in.fail() )
	{
		DEBUG_MSG( DBG_RENDERER, D_ERR, "Failed to open scene data %s", i_fileName );
		return;
	}

	in.getline( &chInput[0], MAX_FILENAME_INPUT );
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

				Utilities::Parser::E_INPUT_TYPE currInputType = g_parserHelper::Get().SetInputType( tagStr );
				
				if( currMajorData == currInputType )
				{
					currMajorData = Utilities::Parser::E_INPUT_TYPE_TOTAL;
					currMinorType = Utilities::Parser::E_INPUT_TYPE_TOTAL;
				}
				else if( currMinorType == currInputType )
				{
					currMinorType = Utilities::Parser::E_INPUT_TYPE_TOTAL;
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

				Utilities::Parser::E_INPUT_TYPE currInputType = g_parserHelper::Get().SetInputType( tagStr );

				if( (currInputType == Utilities::Parser::E_TYPE_ENTITY)
					|| (currInputType == Utilities::Parser::E_TYPE_CAMERA)
					|| (currInputType == Utilities::Parser::E_TYPE_LIGHT)
					|| (currInputType == Utilities::Parser::E_TYPE_TEXTURE)
					|| (currInputType == Utilities::Parser::E_TYPE_DIRECTIONAL_LIGHT)
					)
				{
					currMajorData = currInputType;

					if( currMajorData == Utilities::Parser::E_TYPE_ENTITY )
					{
						in.getline( &chInput[0], MAX_INPUT_LEN );
						g_parserHelper::Get().CleanInputString( chInput );
						m_u32TotalEntity = atoi( chInput );
						
						m_entity = new S_ENTITY[m_u32TotalEntity];
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
			case Utilities::Parser::E_TYPE_ENTITY:
				if( strcmp(chInput, "}") == 0 )
				{
					if( validArrayData )
					{
						u32Ctr++;
						validArrayData = false;
						if( u32Ctr >= m_u32TotalEntity )
						{
							in.close();
							return;
						}
					}
					else
					{
						DEBUG_MSG( DBG_RENDERER, D_ERR, "Invalid array data" );
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
					case Utilities::Parser::E_TYPE_FILE:
						m_entity[u32Ctr].file = chInput;																		break;

					case Utilities::Parser::E_TYPE_POSITION:
						m_entity[u32Ctr].position = g_parserHelper::Get().ParseVector3Input( chInput );	break;

					case Utilities::Parser::E_TYPE_ORIENTATION:
						m_entity[u32Ctr].orientation = (float)atof( chInput );							break;
					}
				}
				break;

			case Utilities::Parser::E_TYPE_CAMERA:
				switch( currMinorType )
				{
				case Utilities::Parser::E_TYPE_POSITION:
					m_cameraPos = g_parserHelper::Get().ParseVector3Input( chInput );		break;

				case Utilities::Parser::E_TYPE_LOOK_AT:
					m_cameraLook = g_parserHelper::Get().ParseVector3Input( chInput );	break;

				case Utilities::Parser::E_TYPE_UP:
					m_cameraUp = g_parserHelper::Get().ParseVector3Input( chInput );		break;

				case Utilities::Parser::E_TYPE_ASPECT:
					m_cameraAspect = (float) atof( chInput );														break;

				case Utilities::Parser::E_TYPE_NEAR:
					m_cameraNear = (float) atof( chInput );															break;

				case Utilities::Parser::E_TYPE_FAR:
					m_cameraFar = (float) atof( chInput );															break;

				case Utilities::Parser::E_TYPE_ORIENTATION:
					m_cameraAngle = (float) atof( chInput );														break;
				}
				break;

			case Utilities::Parser::E_TYPE_LIGHT:
				switch( currMinorType )
				{
					case Utilities::Parser::E_TYPE_POSITION:
						m_lightPos = g_parserHelper::Get().ParseVector3Input( chInput );		break;
					case Utilities::Parser::E_TYPE_COLOUR:
						m_lightColour = g_parserHelper::Get().ParseColourInput( chInput );	break;
					case Utilities::Parser::E_TYPE_INTENSITY:
						m_lightIntensity = (float) atof( chInput );													break;
					case Utilities::Parser::E_TYPE_ATTENTUATOR:
						m_lightAttenuator = (float) atof( chInput );												break;
					case Utilities::Parser::E_TYPE_RADIUS:
						m_lightRadius = (float) atof( chInput );														break;
					case Utilities::Parser::E_TYPE_AMBIENT:
						m_ambientLight = g_parserHelper::Get().ParseColourInput( chInput );	break;
				}
				break;

			case Utilities::Parser::E_TYPE_TEXTURE:
				m_environmentMap = chInput;
				break;

			case Utilities::Parser::E_TYPE_DIRECTIONAL_LIGHT:
				switch( currMinorType )
				{
					case Utilities::Parser::E_TYPE_ORIENTATION:
						m_directionalLightDir = g_parserHelper::Get().ParseVector3Input( chInput );			break;
					case Utilities::Parser::E_TYPE_COLOUR:
						m_directionalLightColour = g_parserHelper::Get().ParseColourInput( chInput );		break;
					case Utilities::Parser::E_TYPE_AMBIENT:
						m_directionalAmbientLight = g_parserHelper::Get().ParseColourInput( chInput );	break;
					case Utilities::Parser::E_TYPE_INTENSITY:
						m_directionalLightIntensity = (float) atof( chInput );													break;
					case Utilities::Parser::E_TYPE_LOOK_AT:
						m_lightLook = g_parserHelper::Get().ParseVector3Input( chInput );								break;
					case Utilities::Parser::E_TYPE_NEAR:
						m_lightNear = (float) atof( chInput );																					break;
					case Utilities::Parser::E_TYPE_FAR:
						m_lightFar = (float) atof( chInput );																						break;
					case Utilities::Parser::E_TYPE_WIDTH:
						m_lightWidth = (float) atof( chInput );																					break;
					case Utilities::Parser::E_TYPE_HEIGHT:
						m_lightHeight = (float) atof( chInput );																				break;
				}
				break;
			}
		}

		in.getline( &chInput[0], MAX_INPUT_LEN );
		g_parserHelper::Get().CleanInputString( chInput );
	} while ( !in.eof() );

	in.close();

	return;
}