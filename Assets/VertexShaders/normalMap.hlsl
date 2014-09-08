/**
 ****************************************************************************************************
 * \file		normalMap.hlsl
 * \brief		Normal mapping effect
 *
 * \date		1 March 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

// Uniforms
//=========

uniform float g_secondsElapsed = 1.0;
// Sherly 23 January 2013 added
// {
uniform float4x4 g_transform_modelToWorld;
uniform float4x4 g_transform_worldToView;
uniform float4x4 g_transform_viewToProjected;
// }

// Sherly 12 April 2013 added
// {
uniform float4x4 g_directionalLight_worldToView;
uniform float4x4 g_directionalLight_viewToProjected;
// }

// Shader program
//===============

/**
 ****************************************************************************************************
	\fn			void main( in const float3 i_position_model : POSITION, in const float4 i_color : COLOR0, out float4 o_position : POSITION, out float4 o_color : COLOR0 )
	\brief		Main procedure
	\param		i_position_model position input
	\param		i_color color input
	\param		o_position position output
	\param		o_color color output
	\return		NONE

	\date		1 March 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
void main( in const float3 i_position_model : POSITION, in const float3 i_normal_model : NORMAL,
	in const float2 i_texcoord : TEXCOORD0, in const float3 i_color : COLOR0,
	in const float3 i_tangent_model : TANGENT, in const float3 i_biNormal_model : BINORMAL,
	out float4 o_position_gpu : POSITION, out float3 o_position_world : TEXCOORD1, out float3 o_normal_world : NORMAL,
	out float2 o_texcoord : TEXCOORD0, out float4 o_color : COLOR0,
	out float3 o_tangent_world : TANGENT, out float3 o_biNormal_world : BINORMAL,
	out float4 o_position_projected : TEXCOORD2, out float4 o_position_view : TEXCOORD3,
	out float4 o_position_light_view : TEXCOORD4, out float4 o_position_light_projected : TEXCOORD5
	)
{
	float4 position_world = mul( float4(i_position_model, 1.0), g_transform_modelToWorld );
	float4 position_view = mul( position_world, g_transform_worldToView );
	float4 position_projected = mul( position_view, g_transform_viewToProjected );

	o_position_world = position_world.xyz;
	o_position_gpu = position_projected;
	o_position_view = position_view;

	// Calculate normal
	float3x3 rotation_modelToWorld =
	{
		g_transform_modelToWorld[0].xyz,
		g_transform_modelToWorld[1].xyz,
		g_transform_modelToWorld[2].xyz
	};
	o_texcoord = i_texcoord;
	o_color = float4( i_color, 1.0 );
	o_normal_world = mul( normalize(i_normal_model), rotation_modelToWorld );
	o_tangent_world = mul( normalize(i_tangent_model), rotation_modelToWorld );
	o_biNormal_world = mul( normalize(i_biNormal_model), rotation_modelToWorld );
	o_position_projected = position_projected;

	// Sherly 12 April 2013 added
	// {
	o_position_light_view = mul( position_world, g_directionalLight_worldToView );
	o_position_light_projected = mul( o_position_light_view, g_directionalLight_viewToProjected );
	// }
}
