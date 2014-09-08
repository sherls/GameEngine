/**
 ****************************************************************************************************
 * \file		alphaBlendNormalMap.hlsl
 * \brief		Alpha blend effect
 *
 * \date		8 March 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

// Uniforms
//=========
uniform float g_secondsElapsed = 1.0;
uniform float3 g_pointLight_position;
uniform float4 g_pointLight_parameter;
uniform float3 g_ambientLight;
uniform float g_pointLight_radius;
uniform float g_transparency;

// Specular light
uniform float3 g_camera_position;
uniform float g_specular_exponent;

// Directional light
uniform float3 g_directionalLight_direction;
uniform float4 g_directionalLight_parameter;

// Shadow
uniform float g_distance_shadowFarPlane;
sampler2D g_shadowMap_sampler;

sampler2D g_colourBuffer;

#include "pointLight.hlsl"
#include "directionalLight.hlsl"

/**
 ****************************************************************************************************
	\fn			void main( in const float2 i_position : POSITION, out float4 o_position : POSITION, out float4 o_color : COLOR0 )
	\brief		Main procedure
	\param		i_position_world
	\param		i_normal_world
	\param		i_texcoord
	\param		i_color input colour from vertex shader
	\return		o_color colour output to be drawn on screen

	\date		8 March 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
void main( in const float3 i_position_world : TEXCOORD1, in const float4 i_position_projected : TEXCOORD2,
	in const float3 i_normal_world : NORMAL,
	in const float2 i_texcoord : TEXCOORD0, in float4 i_color : COLOR0,
	in const float3 i_tangent_world : TANGENT, in const float3 i_biNormal_world : BINORMAL,
	in float4 i_position_light_view : TEXCOORD4, in float4 i_position_light_projected : TEXCOORD5,
	out float4 o_color : COLOR0 )
{
	// Normalize normal, tangent, and biNormal
	float3 normal_world = normalize( i_normal_world );
	float3 tangent_world = normalize( i_tangent_world );
	float3 biNormal_world = normalize( i_biNormal_world );

	float3 position_projected = i_position_projected.xyz / i_position_projected.w;
	float2 texcoord_screen = (position_projected.xy * float2(0.5, -0.5)) + 0.5;

	float4 diffuseColor;
	{
		float x = cos( 1 * g_secondsElapsed ) / 10;
		float y = sin( 2 * g_secondsElapsed ) / 10;
		float2 texcoord_distortion = texcoord_screen + float2( x, y );
		float4 textureSample = float4( tex2D(g_colourBuffer, texcoord_distortion).rgb, 1.0 );
		diffuseColor = textureSample;
	}

	// Point light
	float3 diffusePointLight = ComputeDiffusePointLight( normal_world, i_position_world );

	// Directional light
	float4 diffuseDirectionalLight = float4( ComputeDiffuseDirectionalLight(normal_world, i_position_light_view, i_position_light_projected), g_transparency );

	o_color = diffuseColor * float4( diffusePointLight, g_transparency ) + (diffuseColor * diffuseDirectionalLight);
}
