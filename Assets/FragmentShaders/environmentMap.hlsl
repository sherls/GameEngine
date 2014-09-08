/**
 ****************************************************************************************************
 * \file		normalMap.hlsl
 * \brief		Normal map computation function
 *
 * \date		5 April 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

/**
 ****************************************************************************************************
	\fn			float3 ComputeEnvironmentMapColour( in const float3 i_normal_world, in const float3 i_position_world,
				in const float3 i_diffuseColour )
	\brief		Compute the diffuse colour in environment map
	\param		i_normal_world normal in world space
	\param		i_position_world position in world space
	\param		i_diffuseColour diffuse colour
	\return		float3
	\retval		The amount of diffuse light from environment

	\date		5 April 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
float3 ComputeEnvironmentMapColour( in const float3 i_normal_world, in const float3 i_position_world,
	in const float3 i_diffuseColour )
{
	float3 viewDirection = normalize( g_camera_position - i_position_world );
	float3 reflectedDirection = reflect( -viewDirection, i_normal_world );
	float3 reflectedColor = texCUBE( g_environmentMap_sampler, reflectedDirection ).rgb;
	float reflectance = g_reflectance;
	float fresnel = 1.0 - saturate( dot(i_normal_world, viewDirection) );
	fresnel = pow( fresnel, 5.0 );
	float reflectAmount = reflectance + ( (1.0 - reflectance) * fresnel );
	float3 diffuseColour = lerp( i_diffuseColour, reflectedColor, reflectAmount );

	return diffuseColour;
}
