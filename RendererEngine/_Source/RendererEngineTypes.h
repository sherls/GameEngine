/**
 ****************************************************************************************************
 * \file		Types.h
 * \brief		Custom types
 ****************************************************************************************************
*/

#ifndef _RENDERER_ENGINE_TYPES_H_
#define _RENDERER_ENGINE_TYPES_H_

#include <vector>
#include <d3d9.h>
#include <D3DX9Shader.h>

// Utilities project header
#include <UtilitiesTypes.h>
#include <StringHash/StringHash.h>

// #define SUPPORT_SHADOW
// #define SUPPORT_TRANSPARENT_OBJECT

namespace RendererEngine
{
	typedef struct _s_effect_
	{
		Utilities::StringHash vertexShaderFile;
		Utilities::StringHash fragmentShaderFile;
		//std::vector<GameEngine::StringHash> fragmentShaderFile;
		Utilities::E_ALPHA_MODE renderState;
		UINT8 u8TextureMode; 
	} S_EFFECT;

	typedef struct _s_entity_
	{
		Utilities::StringHash meshFile;
		Utilities::StringHash materialFile;
	} S_ENTITY;

	typedef struct _s_material_
	{
		Utilities::StringHash effectFile;
		Utilities::StringHash textureFile;
		Utilities::StringHash normalMapTexture;
		float transparency;
		float shininess;
		float reflectance;
	} S_MATERIAL;

	typedef struct _s_fragment_shader_
	{
		IDirect3DPixelShader9* compiledShader;
		ID3DXConstantTable* constantTable;
	} S_FRAGMENT_SHADER;

	typedef struct _s_vertex_shader_
	{
		IDirect3DVertexShader9* compiledShader;
		ID3DXConstantTable* constantTable;
	} S_VERTEX_SHADER;

	typedef struct _s_depth_shader_
	{
		IDirect3DPixelShader9* compiledShader;
		ID3DXConstantTable* constantTable;
	} S_DEPTH_SHADER;

	typedef struct _s_mesh_
	{
		UINT32 u32VertexCount;
		UINT32 u32PrimitiveCount;
		IDirect3DIndexBuffer9* indexBuffer;
		IDirect3DVertexBuffer9* vertexBuffer;
	} S_MESH;
}

#endif// #ifndef _RENDERER_ENGINE_TYPES_H_