/*
	This is an example of a fragment shader
*/

// Uniforms
//=========
sampler2D g_sampler;
bool g_textureUsage;

// Shader program
//===============

#include "diffuseColour.hlsl"

/**
 ****************************************************************************************************
	\fn			void main( in const float2 i_texcoord : TEXCOORD0, in float4 i_colour : COLOR0, out float4 o_colour : COLOR0 )
	\brief		Main procedure
	\param		i_position_world
	\param		i_normal_world
	\param		i_texcoord
	\param		i_colour input colour from vertex shader
	\return		o_colour colour output to be drawn on screen

	\date		5 April 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
void main( in const float2 i_texcoord : TEXCOORD0, in float4 i_colour : COLOR0, out float4 o_colour : COLOR0 )
{
	if( g_textureUsage )
	{
		float4 textureSample = tex2D( g_sampler, i_texcoord );
		clip( textureSample.w - 0.2 );

		float4 diffuseColour = tex2D( g_sampler, i_texcoord );

		o_colour = diffuseColour;
	}
	else
		o_colour = i_colour;
}
