/*
	This is an example of a vertex shader
*/

// Uniforms
//=========
uniform float2 g_position;
uniform float2 g_size;

// Shader program
//===============

/**
 ****************************************************************************************************
	\fn			void main( in const float3 i_position_model : POSITION, in const float4 i_color : COLOR0, out float4 o_position_gpu : POSITION, out float4 o_color : COLOR0 )
	\brief		Main procedure
	\param		i_position_model position input
	\param		i_color color input
	\param		o_position position output
	\param		o_color color output
	\return		NONE

	\date		12 January 2013
	\author		Sherly Yunita \n
				Added scale and rotate procedure \n
				\n\n
 ****************************************************************************************************
*/
void main( in const float3 i_position_model : POSITION, in const float2 i_texcoord : TEXCOORD0, in const float4 i_color : COLOR0,
	out float4 o_position_gpu : POSITION, out float2 o_texcoord : TEXCOORD0, out float4 o_color : COLOR0
	)
{
	float width = i_position_model.x * g_size.x;
	float height = i_position_model.y * g_size.y;
	o_position_gpu = float4( width + g_position.x, height + g_position.y, 0.0, 1.0 );
	o_texcoord = i_texcoord;
	o_color = i_color;
}
