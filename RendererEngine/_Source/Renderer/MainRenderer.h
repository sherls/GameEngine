/**
 ****************************************************************************************************
 * \file		MainRenderer.h
 * \brief		Renderer class declaration
 ****************************************************************************************************
*/

#ifndef _RENDERER_ENGINE_RENDERER_H_
#define _RENDERER_ENGINE_RENDERER_H_

// Utilities header
#include <Singleton/Singleton.h>

#include "../RendererEngineTypes.h"

namespace RendererEngine
{
	class MainRenderer
	{
		friend Utilities::Singleton<MainRenderer>;

		DWORD _vertexProcessingType;
		IDirect3D9* _direct3dInterface;
		IDirect3DDevice9* _direct3dDevice;
		IDirect3DVertexDeclaration9* _basicVertexDeclaration;
		IDirect3DVertexDeclaration9* _normalMapVertexDeclaration;

#if defined( SUPPORT_SHADOW ) || defined( SUPPORT_TRANSPARENT_OBJECT )
		IDirect3DSurface9* _backBuffer;
#endif	// #if defined( SUPPORT_SHADOW ) || defined( SUPPORT_TRANSPARENT_OBJECT )
#ifdef SUPPORT_SHADOW
		IDirect3DSurface9* _zBuffer_surface;
		IDirect3DSurface9* _shadowBuffer_surface;
#endif	// #ifdef SUPPORT_SHAdOW
#ifdef SUPPORT_TRANSPARENT_OBJECT
		IDirect3DSurface9* _opaqueBuffer_surface;
		IDirect3DSurface9* _screenBuffer_surface;
#endif	// #ifdef SUPPORT_TRANSPARENT_OBJECT

		MainRenderer( void );
		~MainRenderer( void ) { }

		bool SetVertexProcessingType( void );
		bool Load( void );
		bool ReleaseScene( void );

#ifdef SUPPORT_SHADOW
		bool CreateZBufferSurfaceTexture( void );
		bool CreateShadowBufferTexture( void );
#endif	// #ifdef SUPPORT_SHADOW
#ifdef SUPPORT_TRANSPARENT_OBJECT
		bool CreateOpaqueSurfaceTexture( void );
		bool CreateScreenBufferTexture( void );
#endif	// #ifdef SUPPORT_TRANSPARENT_OBJECT

	public:
#ifdef SUPPORT_SHADOW
		IDirect3DTexture9* m_zBuffer_texture;
		IDirect3DTexture9* m_shadowBuffer_texture;
#endif// #ifdef SUPPORT_SHADOW
#ifdef SUPPORT_TRANSPARENT_OBJECT
		IDirect3DTexture9* m_opaqueBuffer_texture;
		IDirect3DTexture9* m_screenBuffer_texture;
#endif	// #ifdef SUPPORT_TRANSPARENT_OBJECT

		bool Initialize( void );
		void Update( void ) const;
		bool ShutDown( void );

		bool SetBasicVertexDeclaration( void );
		bool SetNormalMapVertexDeclaration( void );

		IDirect3DDevice9* GetDirect3dDevice( void );
		DWORD GetVertexProcessingType( void );

#if defined( SUPPORT_SHADOW ) || defined( SUPPORT_TRANSPARENT_OBJECT )
		void RefreshBackBuffer( void );
#endif	// #if defined( SUPPORT_SHADOW ) || defined( SUPPORT_TRANSPARENT_OBJECT )

#ifdef SUPPORT_SHADOW
		// Z buffer texture for shadow
		void SetZBufferRenderTarget( void );

		// Shadow
		void SetShadowBufferRenderTarget( void );
#endif	//#ifdef SUPPORT_SHADOW

#ifdef SUPPORT_TRANSPARENT_OBJECT
		// Opaque buffer texture
		void SetOpaqueRenderTarget( void );

		// Screen buffer
		void SetScreenRenderTarget( void );
#endif	// #ifdef SUPPORT_TRANSPARENT_OBJECT

		HRESULT GetTransform( const D3DTRANSFORMSTATETYPE &i_type, D3DXMATRIX &o_matrix );
		HRESULT GetViewPort( D3DVIEWPORT9 &o_viewport );
	};
}	// namespace RendererEngine

typedef Utilities::Singleton<RendererEngine::MainRenderer> g_mainRenderer;

//#include "Renderer.inl"

#endif	// #ifndef _RENDERER_ENGINE_RENDERER_H_