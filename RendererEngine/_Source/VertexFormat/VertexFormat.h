/**
 ****************************************************************************************************
 * \file		VertexFormat.h
 * \brief		The declaration of vertex format
 ****************************************************************************************************
*/

#ifndef _VERTEX_FORMAT_H_
#define _VERTEX_FORMAT_H_

#include <d3d9.h>
#include <D3DX9Shader.h>

namespace RendererEngine
{
	extern D3DVERTEXELEMENT9 s_normalMapVertexElements[];
	extern D3DVERTEXELEMENT9 s_basicVertexElements[];
}

#endif	// #ifndef _VERTEX_FORMAT_H_