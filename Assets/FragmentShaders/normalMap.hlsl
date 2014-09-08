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
	\fn			float3 ComputeNormalWorld( in const float3 i_normal_world, in const float3 i_tangent_world,
				in const float3 i_biNormal_world, in const float2 i_texcoord )
	\brief		Compute the new normal in world space
	\param		i_normal_world normal in world space
	\param		i_tangent_world tangent in world space
	\param		i_biNormal_world biNormal in world space
	\param		i_texcoord texture coordinate
	\return		float3
	\retval		The new normal in world space

	\date		5 April 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
float3 ComputeNormalWorld( in const float3 i_normal_world, in const float3 i_tangent_world,
	in const float3 i_biNormal_world, in const float2 i_texcoord )
{
	float4 normalMap = tex2D( g_normalMap, i_texcoord );
	float3 normal_texture = normalMap.xyz;
	normal_texture = ( normal_texture * 2.0 ) - 1.0;
	float3x3 rotation_textureToWorld;
	rotation_textureToWorld[0].xyz = i_tangent_world;
	rotation_textureToWorld[1].xyz = i_biNormal_world;
	rotation_textureToWorld[2].xyz = i_normal_world;
	float3 normal_world = mul( normal_texture, rotation_textureToWorld );

	return normal_world;
}
