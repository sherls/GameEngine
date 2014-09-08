/*
	This is an example of a fragment shader
*/

// Uniforms
//=========
sampler2D g_sampler;
sampler2D g_vignetting;

// Shader program
//===============

#include "diffuseColour.hlsl"

/**
 ****************************************************************************************************
	\fn			void main( in const float2 i_texcoord : TEXCOORD0, in float4 i_color : COLOR0, out float4 o_color : COLOR0 )
	\brief		Main procedure
	\param		i_position_world
	\param		i_normal_world
	\param		i_texcoord
	\param		i_color input colour from vertex shader
	\return		o_color colour output to be drawn on screen

	\date		5 April 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
void main( in const float2 i_texcoord : TEXCOORD0, in float4 i_color : COLOR0, out float4 o_color : COLOR0 )
{
	float4 diffuseColour = tex2D( g_sampler, i_texcoord );
	float4 vignettingSample = tex2D( g_vignetting, i_texcoord );

	o_color = diffuseColour * vignettingSample;
}
