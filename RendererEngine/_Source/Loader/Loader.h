/**
 ****************************************************************************************************
 * \file		Loader.h
 * \brief		Loader class to read the necessary data for RendererEngine
 ****************************************************************************************************
*/

#ifndef _LOADER_H_
#define _LOADER_H_

#include "../RendererEngineTypes.h"

namespace RendererEngine
{
	namespace Loader
	{
		bool LoadDepthShader( const char *i_fileName, IDirect3DPixelShader9* &o_compiledShader,
			ID3DXConstantTable* &o_constantTable );
		bool LoadFragmentShader( const char *i_fileName, IDirect3DPixelShader9* &o_compiledShader,
			ID3DXConstantTable* &o_constantTable );
		bool LoadVertexShader( const char *i_fileName, IDirect3DVertexShader9* &o_compiledShader,
			ID3DXConstantTable* &o_constantTable );
		bool LoadMeshData( const char *i_fileName, UINT32 &o_u32VertexCount, UINT32 &o_u32PrimitiveCount,
			IDirect3DIndexBuffer9* &o_indexBuffer,	IDirect3DVertexBuffer9* &o_vertexBuffer);
		bool LoadTexture( const char *i_fileName, IDirect3DTexture9* &o_texture );
		bool LoadNormalMap( const char *i_fileName, IDirect3DTexture9* &o_texture );
		bool LoadEnvironmentMap( const char *i_fileName );
	}	// namespace Loader
}	// namespace RendererEngine

#endif// #ifndef _LOADER_H_