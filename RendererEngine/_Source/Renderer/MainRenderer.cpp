/**
 ****************************************************************************************************
 * \file		Renderer.cpp
 * \brief		MainRenderer class implementation
 ****************************************************************************************************
*/

#include "MainRenderer.h"
#include "../Window/Window.h"
#include "../RendererEngine.h"
#include "../VertexFormat/VertexFormat.h"
#include "../Logging/Logging.h"

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			MainRenderer( void )
	\brief		Default constructor of MainRenderer
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
RendererEngine::MainRenderer::MainRenderer( void ) :
	_direct3dInterface( NULL ),
	_normalMapVertexDeclaration( NULL )
{
}

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize the main renderer class
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::Initialize( void )
{
	// Initialize the interface to the Direct3D9 library
	{
		// D3D_SDK_VERSION is #defined by the Direct3D header files,
		// and is just a way to make sure that everything is up-to-date
		_direct3dInterface = Direct3DCreate9( D3D_SDK_VERSION );
		if ( _direct3dInterface )
		{
			LogMessage( "Created a Direct3D9 interface" );
		}
		else
		{
			LogMessage( "Failed to create a Direct3D9 interface" );
			return FAIL;
		}
	}

	// Create an interface to a Direct3D device
	{
		UINT useDefaultDevice = D3DADAPTER_DEFAULT;
		D3DDEVTYPE useHardwareRendering = D3DDEVTYPE_HAL;
		HWND renderInMainWindow = g_mainWindow::Get().GetHandle();
		DWORD useHardwareVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		D3DPRESENT_PARAMETERS presentationParameters = { 0 };
		{
			presentationParameters.BackBufferWidth = g_mainWindow::Get().m_u32Width;
			presentationParameters.BackBufferHeight = g_mainWindow::Get().m_u32Height;
			presentationParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
			presentationParameters.BackBufferCount = 1;
			presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			presentationParameters.hDeviceWindow = renderInMainWindow;
			presentationParameters.Windowed = TRUE;	// FAIL if full screen is enabled
			presentationParameters.EnableAutoDepthStencil = TRUE;
			presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
			presentationParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
		}
		HRESULT result = _direct3dInterface->CreateDevice( useDefaultDevice, useHardwareRendering,
			renderInMainWindow, useHardwareVertexProcessing, &presentationParameters, &_direct3dDevice );
		if ( SUCCEEDED( result ) )
		{
			LogMessage( "Created a Direct3D device" );

			//_direct3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

#if defined( SUPPORT_SHADOW ) || defined( SUPPORT_TRANSPARENT_OBJECT )
			DWORD firstRenderTarget = 0;
			result = _direct3dDevice->GetRenderTarget( firstRenderTarget, &_backBuffer );
			if( SUCCEEDED(result) )
			{
				LogMessage( "Acquired the back buffer surface" );
			}
			else
			{
				LogMessage( "Failed to acquire the back buffer surface" );
				return FAIL;
			}
#endif	// #if defined( SUPPORT_SHADOW ) || defined( SUPPORT_TRANSPARENT_OBJECT )
		}
		else
		{
			LogMessage( "Failed to create a Direct3D device" );
			return FAIL;
		}
	}

	HRESULT result = _direct3dDevice->CreateVertexDeclaration( s_basicVertexElements, &_basicVertexDeclaration );
	if( !SUCCEEDED(result) )
	{
		LogMessage( "Failed to create the basic vertex declaration" );
		return FAIL;
	}
	else
		LogMessage( "Created basic vertex declaration" );

	result = _direct3dDevice->CreateVertexDeclaration( s_normalMapVertexElements, &_normalMapVertexDeclaration );
	if( !SUCCEEDED(result) )
	{
		LogMessage( "Failed to create the normal map vertex declaration" );
		return FAIL;
	}
	else
		LogMessage( "Created normal map vertex declaration" );

	if( !SetVertexProcessingType() )
		return FAIL;

	if( !Load() )
		return FAIL;

#ifdef SUPPORT_SHADOW
	if( !CreateZBufferSurfaceTexture() )
		return FAIL;

	if( !CreateShadowBufferTexture() )
		return FAIL;
#endif	// #ifdef SUPPORT_SHADOW

#ifdef SUPPORT_TRANSPARENT_OBJECT
	if( !CreateOpaqueSurfaceTexture() )
		return FAIL;

	if( !CreateScreenBufferTexture() )
		return FAIL;
#endif	// #ifdef SUPPORT_TRANSPARENT_OBJECT

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool ShutDown( void )
	\brief		Shut down the main renderer
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::ShutDown( void )
{
	bool bErrorFound = FAIL;

	if( !ReleaseScene() )
		bErrorFound = SUCCESS;

	if ( _direct3dInterface )
	{
		_direct3dInterface->Release();
		_direct3dInterface = NULL;
		LogMessage( "Released the Direct3D9 interface" );
	}

	if ( _direct3dDevice )
	{
		_direct3dDevice->Release();
		_direct3dDevice = NULL;
		LogMessage( "Released the Direct3D device" );
	}

	if( _basicVertexDeclaration )
	{
		_basicVertexDeclaration->Release();
		_basicVertexDeclaration = NULL;
		LogMessage( "Released the vertex declaration" );
	}

	if( _normalMapVertexDeclaration )
	{
		_normalMapVertexDeclaration->Release();
		_normalMapVertexDeclaration = NULL;
		LogMessage( "Released the normal map vertex declaration" );
	}

#if defined( SUPPORT_SHADOW ) || defined( SUPPORT_TRANSPARENT_OBJECT )
	if( _backBuffer )
	{
		_backBuffer->Release();
		_backBuffer = NULL;
		LogMessage( "Released back buffer" );
	}
#endif	//#if defined( SUPPORT_SHADOW ) || defined( SUPPORT_TRANSPARENT_OBJECT )

#ifdef SUPPORT_SHADOW
	if( _zBuffer_surface )
	{
		_zBuffer_surface->Release();
		_zBuffer_surface = NULL;
		LogMessage( "Released z buffer" );
	}

	if( m_zBuffer_texture )
	{
		m_zBuffer_texture->Release();
		m_zBuffer_texture = NULL;
		LogMessage( "Released z texture" );
	}
#endif//#ifdef SUPPORT_SHADOW

#ifdef SUPPORT_TRANSPARENT_OBJECT
	if( _opaqueBuffer_surface )
	{
		_opaqueBuffer_surface->Release();
		_opaqueBuffer_surface = NULL;
		LogMessage( "Released opaque buffer" );
	}

	if( m_opaqueBuffer_texture )
	{
		m_opaqueBuffer_texture->Release();
		m_opaqueBuffer_texture = NULL;
		LogMessage( "Released opaque texture" );
	}

	if( _screenBuffer_surface )
	{
		_screenBuffer_surface->Release();
		_screenBuffer_surface = NULL;
		LogMessage( "Released screen buffer" );
	}

	if( m_screenBuffer_texture )
	{
		m_screenBuffer_texture->Release();
		m_screenBuffer_texture = NULL;
		LogMessage( "Released screen texture" );
	}
#endif	// #ifdef SUPPORT_TRANSPARENT_OBJECT

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool SetBasicVertexDeclaration( void )
	\brief		Set the vertex declaration to the basic one
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::SetBasicVertexDeclaration( void )
{
	HRESULT result = _direct3dDevice->SetVertexDeclaration( _basicVertexDeclaration );
	if ( SUCCEEDED( result ) )
	{
		return SUCCESS;
	}
	else
	{
		LogMessage( "Failed to set the vertex declaration" );
		return FAIL;
	}
}

/**
 ****************************************************************************************************
	\fn			bool SetNormalMapVertexDeclaration( void )
	\brief		Set the vertex declaration to the normal map supported one
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::SetNormalMapVertexDeclaration( void )
{
	HRESULT result = _direct3dDevice->SetVertexDeclaration( _normalMapVertexDeclaration );
	if ( SUCCEEDED( result ) )
	{
		return SUCCESS;
	}
	else
	{
		LogMessage( "Failed to set the vertex declaration" );
		return FAIL;
	}
}

/**
 ****************************************************************************************************
	\fn			IDirect3DDevice9* GetDirect3dDevice( void )
	\brief		Get the DirectX device
	\param		NONE
	\return		IDirect3DDevice9*
	\retval		The pointer to the DirectX device
 ****************************************************************************************************
*/
IDirect3DDevice9* RendererEngine::MainRenderer::GetDirect3dDevice( void )
{
	return _direct3dDevice;
}

/**
 ****************************************************************************************************
	\fn			DWORD GetVertexProcessingType( void )
	\brief		Get the vertex processing type
	\param		NONE
	\return		DWORD
	\retval		The vertex processing type
 ****************************************************************************************************
*/
DWORD RendererEngine::MainRenderer::GetVertexProcessingType( void )
{
	return _vertexProcessingType;
}

#if defined( SUPPORT_SHADOW ) || defined( SUPPORT_TRANSPARENT_OBJECT )
/**
 ****************************************************************************************************
	\fn			void RefreshBackBuffer( void )
	\brief		Refresh the back buffer
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::MainRenderer::RefreshBackBuffer( void )
{
	// Restore back buffer
	DWORD firstRenderTarget = 0;
	HRESULT result = _direct3dDevice->SetRenderTarget( firstRenderTarget, _backBuffer );
	assert( SUCCEEDED(result) );

	// Copy the opaque buffer to it
	{
		const RECT* copyEntireSurface = NULL;	// do the entire thing
		D3DTEXTUREFILTERTYPE noFiltering = D3DTEXF_NONE;
		HRESULT result = _direct3dDevice->StretchRect( _screenBuffer_surface, copyEntireSurface,
			_backBuffer, copyEntireSurface, noFiltering );
		assert( SUCCEEDED(result) );
	}
}
#endif	// #if defined( SUPPORT_SHADOW ) || defined( SUPPORT_TRANSPARENT_OBJECT )

#ifdef SUPPORT_SHADOW
/**
 ****************************************************************************************************
	\fn			void SetZBufferRenderTarget( void )
	\brief		Set the z buffer as the render target
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::MainRenderer::SetZBufferRenderTarget( void )
{
	DWORD firstRenderTarget = 0;
	HRESULT result = _direct3dDevice->SetRenderTarget( firstRenderTarget, _zBuffer_surface );
	assert( SUCCEEDED(result) );

	{
		const D3DRECT* subRectanglesToClear = NULL;
		DWORD subRectangleCount = 0;
		DWORD clearTheRenderTarget = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
		D3DCOLOR clearColor = D3DCOLOR_XRGB( 255, 0, 0 );
		float clearDepth = 1.0f;
		DWORD noStencilBuffer = 0;
		HRESULT result = _direct3dDevice->Clear( subRectangleCount, subRectanglesToClear,
			clearTheRenderTarget, clearColor, clearDepth, noStencilBuffer );
		assert( SUCCEEDED( result ) );
	}
}

/**
 ****************************************************************************************************
	\fn			void SetShadowBufferRenderTarget( void )
	\brief		Set the shadow buffer as the render target
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::MainRenderer::SetShadowBufferRenderTarget( void )
{
	DWORD firstRenderTarget = 0;
	HRESULT result = _direct3dDevice->SetRenderTarget( firstRenderTarget, _shadowBuffer_surface );
	assert( SUCCEEDED(result) );

	{
		const D3DRECT* subRectanglesToClear = NULL;
		DWORD subRectangleCount = 0;
		DWORD clearTheRenderTarget = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
		D3DCOLOR clearColor = D3DCOLOR_XRGB( 255, 0, 0 );
		float clearDepth = 1.0f;
		DWORD noStencilBuffer = 0;
		HRESULT result = _direct3dDevice->Clear( subRectangleCount, subRectanglesToClear,
			clearTheRenderTarget, clearColor, clearDepth, noStencilBuffer );
		assert( SUCCEEDED( result ) );
	}
}
#endif	// #ifdef SUPPORT_SHADOW

#ifdef SUPPORT_TRANSPARENT_OBJECT
/**
 ****************************************************************************************************
	\fn			void SetOpaqueRenderTarget( void )
	\brief		Set the opaque render target
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::MainRenderer::SetOpaqueRenderTarget( void )
{
	DWORD firstRenderTarget = 0;
	HRESULT result = _direct3dDevice->SetRenderTarget( firstRenderTarget, _opaqueBuffer_surface );
	assert( SUCCEEDED(result) );

	{
		const D3DRECT* subRectanglesToClear = NULL;
		DWORD subRectangleCount = 0;
		DWORD clearTheRenderTarget = D3DCLEAR_TARGET;// | D3DCLEAR_ZBUFFER;
		//D3DCOLOR clearColor = D3DCOLOR_XRGB( 100, 149, 237 );
		D3DCOLOR clearColor = D3DCOLOR_XRGB( 50, 50, 50 );
		float clearDepth = 0.0f;
		DWORD noStencilBuffer = 0;
		HRESULT result = _direct3dDevice->Clear( subRectangleCount, subRectanglesToClear,
			clearTheRenderTarget, clearColor, clearDepth, noStencilBuffer );
		assert( SUCCEEDED( result ) );
	}
}

/**
 ****************************************************************************************************
	\fn			void SetScreenRenderTarget( void )
	\brief		Set the screen as the render target
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::MainRenderer::SetScreenRenderTarget( void )
{
	DWORD firstRenderTarget = 0;
	HRESULT result = _direct3dDevice->SetRenderTarget( firstRenderTarget, _screenBuffer_surface );
	assert( SUCCEEDED(result) );

	// Copy the opaque buffer to screen surface
	{
		const RECT* copyEntireSurface = NULL;	// do the entire thing
		D3DTEXTUREFILTERTYPE noFiltering = D3DTEXF_NONE;
		HRESULT result = _direct3dDevice->StretchRect( _opaqueBuffer_surface, copyEntireSurface,
			_screenBuffer_surface, copyEntireSurface, noFiltering );
		assert( SUCCEEDED(result) );
	}
}
#endif	// #ifdef SUPPORT_TRANSPARENT_OBJECT

/****************************************************************************************************
			Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool SetVertexProcessingType( void )
	\brief		Set the vertex processing type
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::SetVertexProcessingType( void )
{
	_vertexProcessingType = 0;

	// Our code will only ever write to the buffer
	_vertexProcessingType |= D3DUSAGE_WRITEONLY;
	// The type of vertex processing should match what was specified when the device interface was created
	{
		D3DDEVICE_CREATION_PARAMETERS deviceCreationParameters;
		HRESULT result = _direct3dDevice->GetCreationParameters( &deviceCreationParameters );
		if( SUCCEEDED( result ) )
		{
			DWORD vertexProcessingType = deviceCreationParameters.BehaviorFlags &
				( D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING );
			_vertexProcessingType |= ( vertexProcessingType != D3DCREATE_SOFTWARE_VERTEXPROCESSING ) ?
				0 : D3DUSAGE_SOFTWAREPROCESSING;
		}
		else
		{
			LogMessage( "Failed to get the device's creation parameters" );
			return FAIL;
		}
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool Load( void )
	\brief		Load main renderer
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::Load( void )
{
	//_scene.Load();
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool ReleaseScene( void )
	\brief		Release the scene
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::ReleaseScene( void )
{
	//return Scene::Release();
	return SUCCESS;
}

#ifdef SUPPORT_SHADOW
/**
 ****************************************************************************************************
	\fn			bool CreateZBufferSurfaceTexture( void )
	\brief		Create the z buffer surface texture
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::CreateZBufferSurfaceTexture( void )
{
	unsigned int noMipMaps = 1;
	DWORD usage = D3DUSAGE_RENDERTARGET;
	D3DFORMAT format = D3DFMT_R16F;
	D3DPOOL pool = D3DPOOL_DEFAULT;
	HANDLE *notUsed = NULL;
	HRESULT result = _direct3dDevice->CreateTexture( g_mainWindow::Get().m_u32Width, g_mainWindow::Get().m_u32Height,
		noMipMaps, usage, format, pool, &m_zBuffer_texture, notUsed );

	if( SUCCEEDED(result) )
	{
		LogMessage( "Created the Z buffer texture" );

		// Get the surface
		unsigned int topLevel = 0;
		HRESULT result = m_zBuffer_texture->GetSurfaceLevel( topLevel, &_zBuffer_surface );
		if( SUCCEEDED(result) )
		{
			LogMessage( "Acquired the Z buffer surface" );
		}
		else
		{
			LogMessage( "Failed to acquire the Z buffer surface" );
			return false;
		}
	}
	else
	{
		LogMessage( "Failed to create Z buffer texture" );
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool CreateShadowBufferTexture( void )
	\brief		Create the shadow buffer texture
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::CreateShadowBufferTexture( void )
{
	unsigned int noMipMaps = 1;
	DWORD usage = D3DUSAGE_RENDERTARGET;
	D3DFORMAT format = D3DFMT_R16F;
	D3DPOOL pool = D3DPOOL_DEFAULT;
	HANDLE *notUsed = NULL;
	HRESULT result = _direct3dDevice->CreateTexture( g_mainWindow::Get().m_u32Width, g_mainWindow::Get().m_u32Height,
		noMipMaps, usage, format, pool, &m_shadowBuffer_texture, notUsed );

	if( SUCCEEDED(result) )
	{
		LogMessage( "Created the shadow buffer texture" );

		// Get the surface
		unsigned int topLevel = 0;
		HRESULT result = m_shadowBuffer_texture->GetSurfaceLevel( topLevel, &_shadowBuffer_surface );
		if( SUCCEEDED(result) )
		{
			LogMessage( "Acquired the shadow buffer surface" );
		}
		else
		{
			LogMessage( "Failed to acquire the shadow buffer surface" );
			return false;
		}
	}
	else
	{
		LogMessage( "Failed to create shadow buffer texture" );
	}

	return SUCCESS;
}
#endif	// #ifdef SUPPORT_SHADOW

#ifdef SUPPORT_TRANSPARENT_OBJECT
/**
 ****************************************************************************************************
	\fn			bool CreateOpaqueSurfaceTexture( void )
	\brief		Create the opaque surface texture
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::CreateOpaqueSurfaceTexture( void )
{
	unsigned int noMipMaps = 1;
	DWORD usage = D3DUSAGE_RENDERTARGET;
	D3DFORMAT format = D3DFMT_X8R8G8B8;
	D3DPOOL pool = D3DPOOL_DEFAULT;
	HANDLE *notUsed = NULL;
	HRESULT result = _direct3dDevice->CreateTexture( g_mainWindow::Get().m_u32Width, g_mainWindow::Get().m_u32Height,
		noMipMaps, usage, format, pool, &m_opaqueBuffer_texture, notUsed );

	if( SUCCEEDED(result) )
	{
		LogMessage( "Created the opaque buffer texture" );

		// Get the surface
		unsigned int topLevel = 0;
		HRESULT result = m_opaqueBuffer_texture->GetSurfaceLevel( topLevel, &_opaqueBuffer_surface );
		if( SUCCEEDED(result) )
		{
			LogMessage( "Acquired the opaque buffer surface" );
		}
		else
		{
			LogMessage( "Failed to acquire the opaque buffer surface" );
			return FAIL;
		}
	}
	else
	{
		LogMessage( "Failed to create opaque buffer texture" );
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool CreateScreenBufferTexture( void )
	\brief		Create the screen buffer texture
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::MainRenderer::CreateScreenBufferTexture( void )
{
	unsigned int noMipMaps = 1;
	DWORD usage = D3DUSAGE_RENDERTARGET;
	D3DFORMAT format = D3DFMT_X8R8G8B8;
	D3DPOOL pool = D3DPOOL_DEFAULT;
	HANDLE *notUsed = NULL;
	HRESULT result = _direct3dDevice->CreateTexture( g_mainWindow::Get().m_u32Width, g_mainWindow::Get().m_u32Height,
		noMipMaps, usage, format, pool, &m_screenBuffer_texture, notUsed );

	if( SUCCEEDED(result) )
	{
		LogMessage( "Created the render buffer texture" );

		// Get the surface
		unsigned int topLevel = 0;
		HRESULT result = m_screenBuffer_texture->GetSurfaceLevel( topLevel, &_screenBuffer_surface );
		if( SUCCEEDED(result) )
		{
			LogMessage( "Acquired the render buffer surface" );
		}
		else
		{
			LogMessage( "Failed to acquire the render buffer surface" );
			return FAIL;
		}
	}
	else
	{
		LogMessage( "Failed to create render buffer texture" );
	}

	return SUCCESS;
}
#endif	// #ifdef SUPPORT_TRANSPARENT_OBJECT

HRESULT RendererEngine::MainRenderer::GetTransform( const D3DTRANSFORMSTATETYPE &i_type, D3DXMATRIX &o_matrix )
{
	return _direct3dDevice->GetTransform( i_type, &o_matrix );
}

HRESULT RendererEngine::MainRenderer::GetViewPort( D3DVIEWPORT9 &o_viewport )
{
	return _direct3dDevice->GetViewport( &o_viewport );
}