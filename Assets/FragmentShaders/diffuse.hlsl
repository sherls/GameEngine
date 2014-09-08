/**
 ****************************************************************************************************
 * \file		diffuse.hlsl
 * \brief		Diffuse lightning effect
 *
 * \date		7 February 2013
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

// Specular light
uniform float3 g_camera_position;
uniform float g_specular_exponent;

// Directional light
uniform float3 g_directionalLight_direction;
uniform float4 g_directionalLight_parameter;

// Shadow
uniform float g_distance_shadowFarPlane;
sampler2D g_shadowMap_sampler;

sampler2D g_sampler;

#include "pointLight.hlsl"
#include "diffuseColour.hlsl"
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

	\date		7 February 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
void main( in const float3 i_position_world : TEXCOORD1, in const float3 i_normal_world : NORMAL,
	in const float2 i_texcoord : TEXCOORD0, in float4 i_color : COLOR0,
	in float4 i_position_light_view : TEXCOORD4, in float4 i_position_light_projected : TEXCOORD5,
	out float4 o_color : COLOR0 )
{
	// Normalize normal
	float3 normal_world = normalize( i_normal_world );

	// Get the diffuse colour
	float4 diffuseColor = ComputeDiffuseColour( i_texcoord, i_color );

	// Point light
	float3 diffusePointLight = ComputeDiffusePointLight( normal_world, i_position_world );

	// Directional light
	float4 diffuseDirectionalLight = float4( ComputeDiffuseDirectionalLight(normal_world, i_position_light_view, i_position_light_projected), 1.0 );

	o_color = diffuseColor;// * float4( diffusePointLight, 1.0 ) + (diffuseColor * diffuseDirectionalLight);
}
