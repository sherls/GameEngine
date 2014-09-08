/**
 ****************************************************************************************************
 * \file		plain.hlsl
 * \brief		Plain fragment shader
 *
 * \date		1 February 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

// Uniforms
//=========

uniform float g_secondsElapsed = 1.0;
uniform float4 g_pointLight_parameter;

// Shader program
//===============

/**
 ****************************************************************************************************
	\fn			void main( in const float2 i_position : POSITION, out float4 o_position : POSITION, out float4 o_color : COLOR0 )
	\brief		Main procedure
	\param		i_position_world
	\param		i_normal_world
	\param		i_texcoord
	\param		i_color input colour from vertex shader
	\return		o_color colour output to be drawn on screen

	\date		1 February 2013
	\author		Sherly Yunita \n
				Switch the input colour \n
				\n\n
 ****************************************************************************************************
*/
void main( in const float3 i_position_world : TEXCOORD1, in const float3 i_normal_world : NORMAL, in const float2 i_texcoord : TEXCOORD0, in float4 i_color : COLOR0,
	out float4 o_color : COLOR0 )
{
	o_color = float4( g_pointLight_parameter.xyz, 1.0 );
}
