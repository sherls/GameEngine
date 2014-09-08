/**
 ****************************************************************************************************
 * \file		diffuseColour.hlsl
 * \brief		Compute the diffuse colour from texture
 *
 * \date		5 April 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

/**
 ****************************************************************************************************
	\fn			float4 ComputeDiffuseColour( in const float2 i_texcoord, in const float4 i_colour )
	\brief		Compute the diffuse colour
	\param		i_texcoord texture coordinate
	\param		i_colour mesh colour
	\return		float4
	\retval		The diffuse colour

	\date		5 April 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
float4 ComputeDiffuseColour( in const float2 i_texcoord, in const float4 i_colour )
{
	float4 textureSample = tex2D( g_sampler, i_texcoord );
	//textureSample = max( float4(0.2, 0.2, 0.2, 0.1), textureSample );
	float4 diffuseColour = textureSample + i_colour;

	return diffuseColour;
}
