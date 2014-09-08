/**
 ****************************************************************************************************
 * \file		shadow.hlsl
 * \brief		Shadow fragment shader
 *
 * \date		5 April 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

/**
 ****************************************************************************************************
	\fn			float ComputeShadowAmount( in const float3 i_position_light, in const float4 i_position_light_projected )
	\brief		Compute the amount of shadow
	\param		i_position_light position of light
	\param		i_position_light_projected position of light in camera view
	\return		float
	\retval		The amount of shadow

	\date		5 April 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
float ComputeShadowAmount( in const float4 i_position_light_view, in const float4 i_position_light_projected )
{
	float2 texcoord_screen = (i_position_light_projected.xy * float2(0.5, -0.5)) + 0.5;
	float depth_current = i_position_light_view.z;
	float depth_previous = tex2D( g_shadowMap_sampler, texcoord_screen ).x;
	depth_previous *= g_distance_shadowFarPlane;

	const float epsilon = 1.0f;
	return depth_current < depth_previous + epsilon;
}