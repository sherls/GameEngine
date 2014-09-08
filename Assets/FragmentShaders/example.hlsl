/*
	This is an example of a fragment shader
*/

// Uniforms
//=========

uniform float g_secondsElapsed = 1.0;
// Sherly 28 January 2013 added
// {
uniform float3 g_pointLight_position;
sampler2D g_sampler;
// }
uniform float4 g_pointLight_parameter;

// Shader program
//===============

/**
 ****************************************************************************************************
	\fn			void main( in const float2 i_position : POSITION, out float4 o_position : POSITION, out float4 o_color : COLOR0 )
	\brief		Main procedure
	\param		i_position_world
	\param		i_normal_world
	\param		i_texcoord
	\param		i_color input colour from vertex shader
	\return		o_color colour output to be drawn on screen

	\date		12 January 2013
	\author		Sherly Yunita \n
				Switch the input colour \n
				\n\n

	\date		23 January 2013
	\author		Sherly Yunita \n
				Marked the colour switch \n
				\n\n

	\date		28 January 2013
	\author		Sherly Yunita \n
				Added texture and light control \n
				\n\n

	\date		1 February 2013
	\author		Sherly Yunita \n
				Added point light colour and intensity computation \n
				\n\n
 ****************************************************************************************************
*/
void main( in const float3 i_position_world : TEXCOORD1, in const float3 i_normal_world : NORMAL,
	in const float2 i_texcoord : TEXCOORD0, in float4 i_color : COLOR0,
	out float4 o_color : COLOR0 )
{
	// Until we have more sophisticated shaders the output color can be set to an arbitrary RGBA value:
	{
		//o_color = float4( 1.0, 1.0, 1.0, 1.0 );
	}
	// Alternatively, we could use the elapsed time to change the colors:
	{
	/*
		float red = ( sin( g_secondsElapsed ) * 0.5 ) + 0.5;
		float green = ( cos( 5.0 * g_secondsElapsed ) * 0.5 ) + 0.5f;
		float blue = 1.0 - red;
		o_color = float4( red, green, blue, 1.0 );
	*/
	}
	// We could also use the interpolated value from the vertex shader:
	{
		// Sherly 28 January 2013 added
		// {
		float4 interpolatedVertexColor = i_color;
		float4 textureSample = tex2D( g_sampler, i_texcoord );
		textureSample = max( float4(0.2, 0.2, 0.2, 0.1), textureSample );
		//float4 diffuseColor = textureSample + interpolatedVertexColor;
		float4 diffuseColor = textureSample;

		float3 lightDirection = normalize( g_pointLight_position - i_position_world );
		float dotProduct = dot( lightDirection, normalize(i_normal_world) );
		// Sherly 1 February 2013 -- > added point light colour and intensity computation
		// {
		//float diffuseLight = saturate( dotProduct );
		float3 lightColour = g_pointLight_parameter.xyz;
		float lightIntensity = g_pointLight_parameter.w;
		float3 diffuseLight = (lightColour * lightIntensity) * saturate( dotProduct );
		// }
		// }

		// Sherly 28 January 2013 modified
		// {
		o_color = diffuseColor * float4( diffuseLight, 1.0 );
		//o_color = i_color;
		// }
	}
	/* Sherly 23 January 2013 marked
	// Sherly 12 January 2013 added
	// {
	// Switch colour
	{
		float red = i_color.z;
		float green = i_color.x;
		float blue = i_color.y;
		float alpha = cos( g_secondsElapsed );

		o_color = float4( red, green, blue, alpha );
	}
	// } */
}
