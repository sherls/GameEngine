/**
 ****************************************************************************************************
 * \file		directionalLight.hlsl
 * \brief		Directional light computation function
 *
 * \date		5 April 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

#include "shadow.hlsl"

/**
 ****************************************************************************************************
	\fn			float3 ComputeDiffuseDirectionalLight( in const float3 i_normal_world, in const float4 i_position_light_projected )
	\brief		Compute diffuse light of directional light
	\param		i_normal_world normal in world space
	\param		i_position_light_projected fragment position in light projected space
	\return		float3
	\retval		The amount of diffuse light

	\date		5 April 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
float3 ComputeDiffuseDirectionalLight( in const float3 i_normal_world,
	in const float4 i_position_light_view, in const float4 i_position_light_projected )
{
	// Get light parameter
	float3 lightColour = g_directionalLight_parameter.xyz;
	float lightIntensity = g_directionalLight_parameter.w;

	float dotProduct = dot( -g_directionalLight_direction, i_normal_world );

	// Get light diffuse colour
	float3 diffuseLight = lightColour * lightIntensity * saturate( dotProduct );
	diffuseLight = diffuseLight + g_ambientLight;
	diffuseLight *= ComputeShadowAmount( i_position_light_view, i_position_light_projected );

	return diffuseLight;
}

/**
 ****************************************************************************************************
	\fn			float3 ComputeSpecularDirectionalLight( in const float3 i_normal_world, in const float4 i_position_world, in const float3 i_position_light_projected )
	\brief		Compute specular light of directional light
	\param		i_normal_world normal in world space
	\param		i_position_world position in world space
	\param		i_position_light_projected fragment position in light projected space
	\return		float3
	\retval		The amount of specular light

	\date		5 April 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
float3 ComputeSpecularDirectionalLight( in const float3 i_normal_world, in const float3 i_position_world,
	in const float4 i_position_light_view, in const float4 i_position_light_projected )
{
	// Get light parameter
	float3 lightColour = g_directionalLight_parameter.xyz;
	float lightIntensity = g_directionalLight_parameter.w;

	// Calculate specular light
	float3 r = reflect( -g_directionalLight_direction, i_normal_world );
	float3 v = normalize( g_camera_position - i_position_world );
	float dotProduct = saturate( dot(r, v) );
	float3 specularLight = pow( dotProduct, g_specular_exponent );
	specularLight *= dot( g_directionalLight_direction, i_normal_world );
	specularLight *= lightIntensity;
	specularLight *= ComputeShadowAmount( i_position_light_view, i_position_light_projected );

	return (specularLight * lightColour);
}