/**
 ****************************************************************************************************
 * \file		pointLight.hlsl
 * \brief		Point light computation function
 *
 * \date		5 April 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

/**
 ****************************************************************************************************
	\fn			float3 ComputeDiffusePointLight( in const float3 i_normal_world, in const float3 i_position_world )
	\brief		Compute diffuse light of point light
	\param		i_normal_world normal in world space
	\param		i_position_world position in world space
	\return		float3
	\retval		The amount of diffuse light

	\date		5 April 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
float3 ComputeDiffusePointLight( in const float3 i_normal_world, in const float3 i_position_world )
{
	// Get the angle between face normal and light
	float3 lightDirection = normalize( g_pointLight_position - i_position_world );
	float dotProduct = dot( lightDirection, i_normal_world );

	// Get light parameter
	float3 lightColour = g_pointLight_parameter.xyz;
	float lightIntensity = g_pointLight_parameter.w;

	// Calculate attenuation
	float distance = length( g_pointLight_position - i_position_world );
	float attenuator = 1 / pow( (1+(distance/g_pointLight_radius)), 2 );

	// Get light diffuse colour
	float3 diffuseLight = (lightColour * lightIntensity * attenuator) * saturate( dotProduct );
	diffuseLight = diffuseLight + g_ambientLight;

	return diffuseLight;
}

/**
 ****************************************************************************************************
	\fn			float3 ComputeSpecularPointLight( in const float3 i_normal_world, in const float3 i_position_world )
	\brief		Compute specular light of point light
	\param		i_normal_world normal in world space
	\param		i_position_world position in world space
	\return		float3
	\retval		The amount of specular light

	\date		5 April 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
float3 ComputeSpecularPointLight( in const float3 i_normal_world, in const float3 i_position_world )
{
	// Get the angle between face normal and light
	float3 lightDirection = normalize( g_pointLight_position - i_position_world );

	// Get light parameter
	float3 lightColour = g_pointLight_parameter.xyz;
	float lightIntensity = g_pointLight_parameter.w;

	// Calculate specular light
	float3 r = reflect( -lightDirection, i_normal_world );
	float3 v = normalize( g_camera_position - i_position_world );
	float dotProduct = saturate( dot(r, v) );
	float specularLight = pow( dotProduct, g_specular_exponent );
	specularLight *= dot( lightDirection, i_normal_world );
	specularLight *= lightIntensity;

	return (specularLight * lightColour);
}