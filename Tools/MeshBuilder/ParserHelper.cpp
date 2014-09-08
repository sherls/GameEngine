/**
 ****************************************************************************************************
 * \file		ParserHelper.cpp
 * \brief		Parser helper function implementation
 *
 * \date		15 February 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

#include "ParserHelper.h"

/**
 ****************************************************************************************************
	\fn			E_INPUT_DATA SetInputTYpe( const char *i_tag )
	\brief		Set input data type
	\param		i_tag current tag
	\return		NONE

	\date		15 February 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
Tools::Parser::E_INPUT_TYPE Tools::ParserHelper::SetInputType( const char *i_tag )
{
	int i;

	for( i = 0; i < Parser::E_INPUT_TYPE_TOTAL; i++ )
	{
		if( strcmp(i_tag, _tagList[i].c_str()) == 0 )
			return (Parser::E_INPUT_TYPE)i;
	}

	return Parser::E_INPUT_TYPE_TOTAL;
}

/**
 ****************************************************************************************************
	\fn			void CleanInputString( char *i_input )
	\brief		Remove space and tabulation from input string
	\param		i_input current tag
	\return		NONE

	\date		1 February
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
void Tools::ParserHelper::CleanInputString( char *i_input )
{
	unsigned int inputLen = strlen( i_input );
	int j = 0;
	char *o_str = new char[MAX_INPUT_LEN];

	for( unsigned int i = 0; i < inputLen; i++ )
	{
		if( (i_input[i] != '\t') && (i_input[i] != ' ') )
			o_str[j++] = i_input[i];
	}
	o_str[j] = '\0';

	memcpy( i_input, o_str, sizeof(char) * MAX_INPUT_LEN );

	delete [] o_str;
}

/**
 ****************************************************************************************************
	\fn			S_FLOAT3 ParseVector3Input( const char *i_input )
	\brief		Parse input string into Vector3 data
	\param		i_input input string
	\return		S_FLOAT3
	\retval		Parsed Vector3 data from i_input

	\date		15 February 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
Tools::Parser::S_FLOAT3 Tools::ParserHelper::ParseVector3Input( char *i_input )
{
	char *token = NULL;
	char *nextToken = NULL;
	Parser::S_FLOAT3 retval;

	token = strtok_s( i_input, ",", &nextToken );
	retval.x = (float) atof( token );

	token = strtok_s( NULL, ",", &nextToken );
	retval.y = (float) atof( token );

	token = strtok_s( NULL, " ", &nextToken );
	retval.z = (float) atof( token );

	return retval;
}

/**
 ****************************************************************************************************
	\fn			S_FLOAT2 ParseVector2Input( const char *i_input )
	\brief		Parse input string into Vector2 data
	\param		i_input input string
	\return		S_FLOAT2
	\retval		Parsed Vector2 data from i_input

	\date		15 February 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
Tools::Parser::S_FLOAT2 Tools::ParserHelper::ParseVector2Input( char *i_input )
{
	char *token = NULL;
	char *nextToken = NULL;
	Parser::S_FLOAT2 retval;

	token = strtok_s( i_input, ",", &nextToken );
	retval.x = (float) atof( token );

	token = strtok_s( NULL, ",", &nextToken );
	retval.y = (float) atof( token );

	return retval;
}

/**
 ****************************************************************************************************
	\fn			UINT32 ParseColourInput( const char *i_input )
	\brief		Parse colour data from i_input
	\param		i_input input string
	\return		S_FLOAT3
	\retval		Parsed colour data from i_input

	\date		15 February 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
unsigned int Tools::ParserHelper::ParseColourInput( char *i_input )
{
	unsigned int retval;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha = 1;

	char *token = NULL;
	char *nextToken = NULL;
	
	token = strtok_s( i_input, ",", &nextToken );
	red = atoi( token );
	token = strtok_s( NULL, ",", &nextToken );
	green = atoi( token );
	token = strtok_s( NULL, " ", &nextToken );
	blue = atoi( token );

	retval = ( (((0xFF)&0xFF)<<24) | (((red)&0xff)<<16) | (((green)&0xff)<<8) | ((blue)&0xff) );

	return retval;
}


/****************************************************************************************************
			Private functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			ParserHelper( void )
	\brief		ParserHelper constructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
Tools::ParserHelper::ParserHelper( void )
{
	_tagList[0] = "vertex";
	_tagList[1] = "index";
	_tagList[2] = "position";
	_tagList[3] = "colour";
	_tagList[4] = "normal";
	_tagList[5] = "uv";
// 	_tagList[6] = "entity";
// 	_tagList[7] = "file";
// 	_tagList[8] = "orientation";
// 	_tagList[9] = "camera";
// 	_tagList[10] = "lookAt";
// 	_tagList[11] = "up";
// 	_tagList[12] = "aspect";
// 	_tagList[13] = "near";
// 	_tagList[14] = "far";
// 	_tagList[15] = "light";
// 	_tagList[16] = "intensity";
// 	_tagList[17] = "mesh";
// 	_tagList[18] = "material";
// 	_tagList[19] = "effect";
// 	_tagList[20] = "texture";
// 	_tagList[21] = "vertexShader";
// 	_tagList[22] = "fragmentShader";
// 	_tagList[23] = "renderState";
// 	_tagList[24] = "textureMode";
// 	_tagList[25] = "ambient";
// 	_tagList[26] = "attenuator";
// 	_tagList[27] = "radius";
// 	_tagList[28] = "shininess";
// 	_tagList[29] = "transparency";
	_tagList[6] = "tangent";
	_tagList[7] = "biNormal";
// 	_tagList[32] = "reflectance";
// 	_tagList[33] = "directionalLight";
// 	_tagList[34] = "width";
// 	_tagList[35] = "height";
}

/**
 ****************************************************************************************************
	\fn			~ParserHelper( void )
	\brief		ParserHelper default destructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
Tools::ParserHelper::~ParserHelper( void )
{
	if( _tagList )
	{
		for( int i = 0; i < Parser::E_INPUT_TYPE_TOTAL; i++ )
			_tagList[i].clear();
	}
}