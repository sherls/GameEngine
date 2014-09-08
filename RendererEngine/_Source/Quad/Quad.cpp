/**
 ****************************************************************************************************
 * \file		Quad.cpp
 * \brief		Quad class implementation
 ****************************************************************************************************
*/

#include "Quad.h"
#include "../Loader/Loader.h"
#include "../RendererEngine.h"
#include "../Logging/Logging.h"
#include "../Renderer/MainRenderer.h"

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize the quad class
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Quad::Initialize( void )
{
	_vertexBuffer = NULL;

	m_hashedVertexShader = Utilities::StringHash( "screenSpace.vp" );
	if( !CreateVertexShader("screenSpace.vp") )
		return FAIL;

	m_hashedFragmentShader = Utilities::StringHash( "GUI.fp" );
	if( !CreateFragmentShader("GUI.fp") )
		return FAIL;

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shut down the quad class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Quad::ShutDown( void )
{
	if( _vertexBuffer )
	{
		_vertexBuffer->Release();
		_vertexBuffer = NULL;
		LogMessage( "Released quad vertex buffer" );
	}
}

/**
 ****************************************************************************************************
	\fn			void Draw( void )
	\brief		Draw the quad on screen
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Quad::Draw( void )
{
	IDirect3DDevice9* direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Draw quad" );
#endif	// #ifdef _DEBUG
	// Bind a specific vertex buffer to the device as a data source
	{
		// There can be multiple streams of data feeding the display adaptor at the same time
		unsigned int streamIndex = 0;
		// It's possible to start streaming data in the middle of a vertex buffer
		unsigned int bufferOffset = 0;
		// The "stride" defines how large a single vertex is in the stream of data
		unsigned int bufferStride = sizeof( Utilities::S_BASIC_VERTEX_DATA );
		HRESULT result = direct3dDevice->SetStreamSource( streamIndex, _vertexBuffer, bufferOffset, bufferStride );
		assert( SUCCEEDED( result ) );
	}
	// Render objects from the current streams
	{
		// We are using triangles as the "primitive" type,
		// and we have defined the vertex buffer as a triangle list
		// (meaning that every triangle is defined by three vertices)
		D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
		// It's possible to start rendering primitives in the middle of the stream
		unsigned int indexOfFirstVertexToRender = 0;
		// We are currently only rendering a single triangle
		unsigned int primitiveCountToRender = 2;
		HRESULT result = direct3dDevice->DrawPrimitive( primitiveType, indexOfFirstVertexToRender, primitiveCountToRender );
		assert( SUCCEEDED(result) );
	}
#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/****************************************************************************************************
			Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool LoadVertexBuffer( const D3DCOLOR &i_colour )
	\brief		Load the vertex buffer of quad
	\param		i_colour the colour of vertex buffer
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Quad::LoadVertexBuffer( const D3DCOLOR &i_colour )
{
	IDirect3DDevice9* direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	DWORD usage = g_mainRenderer::Get().GetVertexProcessingType();
	unsigned int bufferSize = 6 * sizeof( Utilities::S_BASIC_VERTEX_DATA );

	DWORD useSeparateVertexDeclaration = 0;
	D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
	HANDLE* notUsed = NULL;

	if( _vertexBuffer )
	{
		_vertexBuffer->Release();
		_vertexBuffer = NULL;
	}

	HRESULT result = direct3dDevice->CreateVertexBuffer( bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
		&_vertexBuffer, notUsed );
	if ( FAILED( result ) )
	{
		LogMessage( "Failed to create the vertex buffer" );
		return FAIL;
	}

	// Fill the vertex buffer with the triangle's vertices
	{
		// Before the vertex buffer can be changed it must be "locked"
		Utilities::S_BASIC_VERTEX_DATA* vertexData;
		{
			unsigned int lockEntireBuffer = 0;
			DWORD useDefaultLockingBehavior = 0;
			HRESULT result = _vertexBuffer->Lock( lockEntireBuffer, lockEntireBuffer,
				reinterpret_cast<void**>( &vertexData ), useDefaultLockingBehavior );
			if ( FAILED( result ) )
			{
				LogMessage( "Failed to lock the vertex buffer" );
				return FAIL;
			}
		}
		// Fill the buffer
		{
			// Vertices position
			vertexData[0].position.x = -1.0f;
			vertexData[0].position.y = -1.0f;
			vertexData[0].position.z = 0.0f;
			vertexData[1].position.x = -1.0f;
			vertexData[1].position.y = 1.0f;
			vertexData[1].position.z = 0.0f;
			vertexData[2].position.x = 1.0f;
			vertexData[2].position.y = -1.0f;
			vertexData[2].position.z = 0.0f;
			vertexData[3].position.x = 1.0f;
			vertexData[3].position.y = -1.0f;
			vertexData[3].position.z = 0.0f;
			vertexData[4].position.x = -1.0f;
			vertexData[4].position.y = 1.0f;
			vertexData[4].position.z = 0.0f;
			vertexData[5].position.x = 1.0f;
			vertexData[5].position.y = 1.0f;
			vertexData[5].position.z = 0.0f;
			// Vertices texture coord
			vertexData[0].texcoord.x = 0;
			vertexData[0].texcoord.y = 1;
			vertexData[1].texcoord.x = 0;
			vertexData[1].texcoord.y = 0;
			vertexData[2].texcoord.x = 1;
			vertexData[2].texcoord.y = 1;
			vertexData[3].texcoord.x = 1;
			vertexData[3].texcoord.y = 1;
			vertexData[4].texcoord.x = 0;
			vertexData[4].texcoord.y = 0;
			vertexData[5].texcoord.x = 1;
			vertexData[5].texcoord.y = 0;
			// Colour
			vertexData[0].colour = i_colour;
			vertexData[1].colour = i_colour;
			vertexData[2].colour = i_colour;
			vertexData[3].colour = i_colour;
			vertexData[4].colour = i_colour;
			vertexData[5].colour = i_colour;
		}
		// Unlock the buffer
		{
			HRESULT result = _vertexBuffer->Unlock();
			if ( FAILED( result ) )
			{
				LogMessage( "Failed to unlock the vertex buffer" );
				return FAIL;
			}
		}
	}

	return SUCCESS;
}