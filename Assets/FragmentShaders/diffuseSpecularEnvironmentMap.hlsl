/**
 ****************************************************************************************************
 * \file		diffuseSpecularEnvironmentMap.hlsl
 * \brief		Diffuse and specular lightning plus environment mapping effect
 *
 * \date		8 March 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

// Uniforms
//=========
// Diffuse light
uniform float g_secondsElapsed = 1.0;
uniform float g_pointLight_radius;
uniform float3 g_ambientLight;
uniform float3 g_pointLight_position;
uniform float4 g_pointLight_parameter;
uniform float g_reflectance;

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
sampler2D g_normalMap;
samplerCUBE g_environmentMap_sampler;

#include "pointLight.hlsl"
#include "diffuseColour.hlsl"
#include "environmentMap.hlsl"
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
void main( in const float3 i_position_world : TEXCOORD1, in const float3 i_normal_world : NORMAL,
	in const float2 i_texcoord : TEXCOORD0, in float4 i_color : COLOR0,
	//in const float3 i_tangent_world : TANGENT, in const float3 i_biNormal_world : BINORMAL,
	in float4 i_position_light_view : TEXCOORD4, in float4 i_position_light_projected : TEXCOORD5,
	out float4 o_color : COLOR0 )
{
	// Normalize normal, tangent, and biNormal
	float3 normal_world = normalize( i_normal_world );
	//float3 tangent_world = normalize( i_tangent_world );
	//float3 biNormal_world = normalize( i_biNormal_world );

	// Get the diffuse colour
	float4 diffuseColor = ComputeDiffuseColour( i_texcoord, i_color );

	// Environment map
	diffuseColor.rgb = ComputeEnvironmentMapColour( normal_world, i_position_world, diffuseColor.rgb );

	// Point light
	float4 diffusePointLight = float4( ComputeDiffusePointLight(normal_world, i_position_world), 1.0 );
	float4 specularPointLight = float4( ComputeSpecularPointLight(normal_world, i_position_world), 1.0 );

	// Directional light
	float4 diffuseDirectionalLight = float4( ComputeDiffuseDirectionalLight(normal_world, i_position_light_view, i_position_light_projected), 1.0 );
	float4 specularDirectionalLight = float4( ComputeSpecularDirectionalLight(normal_world, i_position_world, i_position_light_view, i_position_light_projected), 1.0 );

	o_color = (diffuseColor * diffusePointLight) + specularPointLight + (diffuseColor * diffuseDirectionalLight) + specularDirectionalLight;
}