/**
 ****************************************************************************************************
 * \file		depth.hlsl
 * \brief		Depth texture rendering
 *
 * \date		22 March 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

// Uniforms
//=========

sampler2D g_sampler;
uniform float g_distance_farPlane;

// Shader program
//===============

/**
 ****************************************************************************************************
	\fn			void main( in const float3 i_position_world : TEXCOORD1, out float4 o_color : COLOR0 )
	\brief		Main procedure
	\param		i_position_world position in world space
	\return		o_color colour output to be drawn on screen

	\date		22 March 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
void main( in const float4 i_position_projected : TEXCOORD2, in const float4 i_position_view : TEXCOORD3, in const float2 i_texcoord : TEXCOORD0, out float4 o_color : COLOR0 )
{
	// Alpha binary clipping
	float4 textureSample = tex2D( g_sampler, i_texcoord );
	clip( textureSample.w - 0.2 );

	float depth = i_position_view.z;
	float depth_normalized = depth / g_distance_farPlane;

	// Output depth in the red channel
	// (because the render target only has a single floating point channel)
	o_color = float4( depth_normalized, 0.0, 0.0, 1.0 );
}
