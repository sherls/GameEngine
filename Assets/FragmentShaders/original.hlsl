/*
	This is an example of a fragment shader
*/

// Uniforms
//=========

uniform float g_secondsElapsed = 1.0;

// Shader program
//===============

void main( in float4 i_color : COLOR0, out float4 o_color : COLOR0 )
{
	o_color = i_color;//float4( 1.0, 1.0, 1.0, 1.0 );
}
