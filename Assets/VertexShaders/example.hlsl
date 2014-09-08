/*
	This is an example of a vertex shader
*/

// Uniforms
//=========

uniform float g_secondsElapsed = 1.0;
uniform float4x4 g_transform_worldToView;
uniform float4x4 g_transform_modelToWorld;
uniform float4x4 g_transform_viewToProjected;
uniform float4x4 g_directionalLight_worldToView;
uniform float4x4 g_directionalLight_viewToProjected;

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
 ****************************************************************************************************
*/
void main( in const float3 i_position_model : POSITION, in const float3 i_normal_model : NORMAL,
	in const float2 i_texcoord : TEXCOORD0, in const float3 i_color : COLOR0,
	out float4 o_position_gpu : POSITION, out float3 o_position_world : TEXCOORD1, out float3 o_normal_world : NORMAL,
	out float2 o_texcoord : TEXCOORD0, out float4 o_color : COLOR0,
	out float4 o_position_projected : TEXCOORD2, out float4 o_position_view : TEXCOORD3,
	out float4 o_position_light_view : TEXCOORD4, out float4 o_position_light_projected : TEXCOORD5
	)
{
	float4 position_world = mul( float4(i_position_model, 1.0), g_transform_modelToWorld );
	float4 position_view = mul( position_world, g_transform_worldToView );
	float4 position_projected = mul( position_view, g_transform_viewToProjected );

	o_position_world = position_world.xyz;
	o_position_gpu = position_projected;
	o_position_projected = position_projected;
	o_position_view = position_view;

	o_position_light_view = mul( position_world, g_directionalLight_worldToView );
	o_position_light_projected = mul( o_position_light_view, g_directionalLight_viewToProjected );

	float3x3 rotation_modelToWorld =
	{
		g_transform_modelToWorld[0].xyz,
		g_transform_modelToWorld[1].xyz,
		g_transform_modelToWorld[2].xyz
	};
	float3 normal_world = mul( normalize(i_normal_model), rotation_modelToWorld );
	o_normal_world = normal_world;
	
	o_texcoord = i_texcoord;

	o_color = float4( i_color, 1.0 );
}
