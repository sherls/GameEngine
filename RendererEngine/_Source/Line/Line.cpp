/**
 ****************************************************************************************************
 * \file		Line.cpp
 * \brief		Line class implementation
 ****************************************************************************************************
*/

#include "Line.h"
#include "../Logging/Logging.h"
#include "../Renderer/MainRenderer.h"

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize the line class
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Line::Initialize( void )
{
	_vertexBuffer = NULL;

	m_hashedVertexShader = Utilities::StringHash( "original.vp" );
	if ( !CreateVertexShader("original.vp") )
		return FAIL;

	m_hashedFragmentShader = Utilities::StringHash( "original.fp" );
	if( !CreateFragmentShader("original.fp") )
		return FAIL;

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shut down the line class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Line::ShutDown( void )
{
	if( _vertexBuffer )
	{
		_vertexBuffer->Release();
		_vertexBuffer = NULL;
		LogMessage( "Released line vertex buffer" );
	}
}

/**
 ****************************************************************************************************
	\fn			void Draw( void )
	\brief		Draw the line on screen
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Line::Draw( void )
{
	IDirect3DDevice9* direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Draw line" );
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
		D3DPRIMITIVETYPE primitiveType = D3DPT_LINESTRIP;
		// It's possible to start rendering primitives in the middle of the stream
		unsigned int indexOfFirstVertexToRender = 0;
		// We are currently only rendering a single triangle
		unsigned int primitiveCountToRender = 1;
		HRESULT result = direct3dDevice->DrawPrimitive( primitiveType, indexOfFirstVertexToRender, primitiveCountToRender );
		assert( SUCCEEDED(result) );
	}
#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG

}

/**
 ****************************************************************************************************
	\fn			bool LoadVertexBuffer( const S_LINE_TO_DRAW &i_lineData )
	\brief		Load the vertex buffer
	\param		i_lineData the data of the line for the vertex buffer
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Line::LoadVertexBuffer( const S_LINE_TO_DRAW &i_lineData )
{
	IDirect3DDevice9* direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	DWORD usage = g_mainRenderer::Get().GetVertexProcessingType();
	unsigned int bufferSize = 2 * sizeof( Utilities::S_BASIC_VERTEX_DATA );

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
			vertexData[0].position.x = i_lineData.startPosition.x;
			vertexData[0].position.y = i_lineData.startPosition.y;
			vertexData[0].position.z = i_lineData.startPosition.z;
			vertexData[1].position.x = i_lineData.endPosition.x;
			vertexData[1].position.y = i_lineData.endPosition.y;
			vertexData[1].position.z = i_lineData.endPosition.z;

			// Colour
			vertexData[0].colour = i_lineData.startColour;
			vertexData[1].colour = i_lineData.endColour;
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