/**
 ****************************************************************************************************
 * \file		shiny.hlsl
 * \brief		Shiny fragment shader
 *
 * \date		1 February 2013
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
sampler2D g_sampler;

/**
 ****************************************************************************************************
	\fn			void main( in const float2 i_position : POSITION, out float4 o_position : POSITION, out float4 o_color : COLOR0 )
	\brief		Main procedure
	\param		i_position_world
	\param		i_normal_world
	\param		i_texcoord
	\param		i_color input colour from vertex shader
	\return		o_color colour output to be drawn on screen

	\date		1 February 2013
	\author		Sherly Yunita \n
				Created \n
				\n\n
 ****************************************************************************************************
*/
void main( in const float3 i_position_world : TEXCOORD1, in const float3 i_normal_world : NORMAL, in const float2 i_texcoord : TEXCOORD0, in float4 i_color : COLOR0,
	out float4 o_color : COLOR0 )
{
	// Multiply mesh colour and texture
	float4 interpolatedVertexColor = i_color;
	float4 textureSample = tex2D( g_sampler, i_texcoord );
	textureSample = max( float4(0.2, 0.2, 0.2, 0.1), textureSample );
	float4 diffuseColor = textureSample + interpolatedVertexColor;

	// Get the angle between face normal and light
	float3 lightDirection = normalize( g_pointLight_position - i_position_world );
	float dotProduct = dot( lightDirection, normalize(i_normal_world) );

	// Get light diffuse colour
	float3 lightColour = g_pointLight_parameter.xyz;
	float lightIntensity = g_pointLight_parameter.w;
	float3 diffuseLight = (lightColour * lightIntensity) * saturate( dotProduct );

	o_color = diffuseColor * float4( diffuseLight, 1.0 );
}
