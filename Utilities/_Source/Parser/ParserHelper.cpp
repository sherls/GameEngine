/**
 ****************************************************************************************************
 * \file		ParserHelper.cpp
 * \brief		Parser helper function implementation
 ****************************************************************************************************
*/

#include "ParserHelper.h"

/****************************************************************************************************
			Public functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			E_INPUT_DATA SetInputType( const char *i_tag )
	\brief		Set input data type
	\param		i_tag current tag
	\return		NONE
 ****************************************************************************************************
*/
Utilities::Parser::E_INPUT_TYPE Utilities::ParserHelper::SetInputType( const char *i_tag )
{
	UINT8 i;

	for( i = 0; i < Utilities::Parser::E_INPUT_TYPE_TOTAL; ++i )
	{
		if( strcmp(i_tag, _tagList[i].c_str()) == 0 )
			return (Utilities::Parser::E_INPUT_TYPE)i;
	}

	return Utilities::Parser::E_INPUT_TYPE_TOTAL;
}

/**
 ****************************************************************************************************
	\fn			void CleanInputString( char *i_input )
	\brief		Remove space and tabulation from input string
	\param		i_input current tag
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::ParserHelper::CleanInputString( char *i_input )
{
	UINT8 inputLen = strlen( i_input );
	UINT8 j = 0;
	char *o_str = new char[MAX_INPUT_LEN];

	for( UINT8 i = 0; i < inputLen; ++i )
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
	\fn			D3DXVECTOR3 ParseVector3Input( const char *i_input )
	\brief		Parse input string into Vector3 data
	\param		i_input input string
	\return		D3DXVECTOR3
	\retval		Parsed Vector3 data from i_input
 ****************************************************************************************************
*/
D3DXVECTOR3 Utilities::ParserHelper::ParseVector3Input( char *i_input )
{
	char *token = NULL;
	char *nextToken = NULL;
	D3DXVECTOR3 retval;

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
	\fn			D3DXVECTOR2 ParseVector2Input( const char *i_input )
	\brief		Parse input string into Vector2 data
	\param		i_input input string
	\return		D3DXVECTOR2
	\retval		Parsed Vector2 data from i_input
 ****************************************************************************************************
*/
D3DXVECTOR2 Utilities::ParserHelper::ParseVector2Input( char *i_input )
{
	char *token = NULL;
	char *nextToken = NULL;
	D3DXVECTOR2 retval;

	token = strtok_s( i_input, ",", &nextToken );
	retval.x = (float) atof( token );

	token = strtok_s( NULL, ",", &nextToken );
	retval.y = (float) atof( token );

	return retval;
}

/**
 ****************************************************************************************************
	\fn			D3DCOLOR ParseColourInput( const char *i_input )
	\brief		Parse colour data from i_input
	\param		i_input input string
	\return		D3DCOLOR
	\retval		Parsed colour data from i_input
 ****************************************************************************************************
*/
D3DCOLOR Utilities::ParserHelper::ParseColourInput( char *i_input )
{
	UINT8 red;
	UINT8 green;
	UINT8 blue;
	char *token = NULL;
	char *nextToken = NULL;
	
	token = strtok_s( i_input, ",", &nextToken );
	red = (UINT8) atof( token );
	token = strtok_s( NULL, ",", &nextToken );
	green = (UINT8) atof( token );
	token = strtok_s( NULL, " ", &nextToken );
	blue = (UINT8) atof( token );

	return D3DCOLOR_XRGB( red, green, blue );
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
Utilities::ParserHelper::ParserHelper( void )
{
	_tagList[0] = "vertex";
	_tagList[1] = "index";
	_tagList[2] = "position";
	_tagList[3] = "colour";
	_tagList[4] = "normal";
	_tagList[5] = "uv";
	_tagList[6] = "entity";
	_tagList[7] = "file";
	_tagList[8] = "orientation";
	_tagList[9] = "camera";
	_tagList[10] = "lookAt";
	_tagList[11] = "up";
	_tagList[12] = "aspect";
	_tagList[13] = "near";
	_tagList[14] = "far";
	_tagList[15] = "light";
	_tagList[16] = "intensity";
	_tagList[17] = "mesh";
	_tagList[18] = "material";
	_tagList[19] = "effect";
	_tagList[20] = "texture";
	_tagList[21] = "vertexShader";
	_tagList[22] = "fragmentShader";
	_tagList[23] = "renderState";
	_tagList[24] = "textureMode";
	_tagList[25] = "ambient";
	_tagList[26] = "attenuator";
	_tagList[27] = "radius";
	_tagList[28] = "shininess";
	_tagList[29] = "transparency";
	_tagList[30] = "tangent";
	_tagList[31] = "biNormal";
	_tagList[32] = "reflectance";
	_tagList[33] = "directionalLight";
	_tagList[34] = "width";
	_tagList[35] = "height";
	_tagList[36] = "DiffuseColor";
	_tagList[37] = "NormalMap";
	_tagList[38] = "TransparentColor";
}

/**
 ****************************************************************************************************
	\fn			~ParserHelper( void )
	\brief		ParserHelper default destructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
Utilities::ParserHelper::~ParserHelper( void )
{
	if( _tagList )
	{
		for( UINT8 i = 0; i < Parser::E_INPUT_TYPE_TOTAL; ++i )
			_tagList[i].clear();
	}
}