/*
	This is an example of a vertex shader
*/

// Uniforms
//=========

uniform float g_secondsElapsed = 1.0;

uniform float4x4 g_transform_modelToWorld;
uniform float4x4 g_transform_worldToView;
uniform float4x4 g_transform_viewToProjected;

// Shader program
//===============

void main( in const float3 i_position_model : POSITION, in const float4 i_color : COLOR0,
	out float4 o_position : POSITION, out float4 o_color : COLOR0 )
{
	float4 position_world = mul( float4(i_position_model, 1.0), g_transform_modelToWorld );
	float4 position_view = mul( position_world, g_transform_worldToView );
	float4 position_projected = mul( position_view, g_transform_viewToProjected );
	o_position = position_projected;

	o_color = i_color;
}
