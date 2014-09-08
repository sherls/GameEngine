/**
 ****************************************************************************************************
 * \file		Sphere.cpp
 * \brief		Sphere class implementation
 ****************************************************************************************************
*/

// Utilities header
#include <UtilitiesDefault.h>

#include <math.h>

#include "Sphere.h"
#include "../Setter/Setter.h"
#include "../Logging/Logging.h"
#include "../Renderer/MainRenderer.h"

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize the sphere class
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Sphere::Initialize( void )
{
	_vertexBuffer = NULL;

	m_hashedVertexShader = Utilities::StringHash( "original.vp" );
	if ( !CreateVertexShader("original.vp") )
		return FAIL;

	m_hashedFragmentShader = Utilities::StringHash( "original.fp" );
	if( !CreateFragmentShader("original.fp") )
		return FAIL;

	S_SPHERE_TO_DRAW sphere = { D3DXVECTOR3(0.0f, 0.0f, 0.0f), Utilities::WHITE, Utilities::DEFAULT_DEBUG_SPHERE_RADIUS };
	if( !LoadVertexBuffer(sphere) )
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
void RendererEngine::Sphere::ShutDown( void )
{
	if( _indexBuffer )
	{
		_indexBuffer->Release();
		_indexBuffer = NULL;
		LogMessage( "Released sphere index buffer" );
	}

	if( _vertexBuffer )
	{
		_vertexBuffer->Release();
		_vertexBuffer = NULL;
		LogMessage( "Released sphere vertex buffer" );
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
void RendererEngine::Sphere::Draw( void )
{
	IDirect3DDevice9* direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Draw sphere" );
#endif	// #ifdef _DEBUG

	Setter::SetVertexBuffer( _vertexBuffer, sizeof(Utilities::S_BASIC_VERTEX_DATA) );
	Setter::SetIndexBuffer( _indexBuffer );

	// Render objects from the current streams
	{
		// We are using triangles as the "primitive" type,
		// and we have defined the vertex buffer as a triangle list
		// (meaning that every triangle is defined by three vertices)
		D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
		// It's possible to start rendering primitives in the middle of the stream
		unsigned int indexOfFirstVertexToRender = 0;
		// We are currently only rendering a single triangle
		unsigned int startIndex = 0;
		//HRESULT result = direct3dDevice->DrawPrimitive( primitiveType, indexOfFirstVertexToRender, primitiveCountToRender );
		HRESULT result = g_mainRenderer::Get().GetDirect3dDevice()->DrawIndexedPrimitive( primitiveType, indexOfFirstVertexToRender,
			0, _u32VertexCount, startIndex, _u32PrimitiveCount );
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
bool RendererEngine::Sphere::LoadVertexBuffer( const S_SPHERE_TO_DRAW &i_sphereData, const UINT8 &i_u8M, const UINT8 &i_u8N )
{
	IDirect3DDevice9* direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	DWORD usage = g_mainRenderer::Get().GetVertexProcessingType();

	float alpha = 2.0f * (float)Utilities::PI / (float) i_u8M;
	float theta = (float)Utilities::PI / (float) i_u8N;

	// Create the vertex buffer
	{
		_u32VertexCount = (i_u8M + 1) * (i_u8N + 1);
		unsigned int bufferSize = _u32VertexCount * sizeof( Utilities::S_BASIC_VERTEX_DATA );
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
	}

	// Create the index buffer
	{
		_u32PrimitiveCount = i_u8N * (2 * i_u8M);
		unsigned int bufferSize = _u32PrimitiveCount * 3 * sizeof( uint16_t );
		D3DFORMAT format = D3DFMT_INDEX16;
		D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
		HANDLE *notUsed = NULL;

		HRESULT result = direct3dDevice->CreateIndexBuffer( bufferSize, usage, format, useDefaultPool,
			&_indexBuffer, notUsed );
		if( FAILED(result) )
		{
			LogMessage( "Failed to create the index buffer" );
			return FAIL;
		}
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
		// Set the buffer
		{
			for( int i = 0; i < (i_u8M + 1); ++i )
			{
				for( int j = 0; j < (i_u8N + 1); ++j )
				{
					vertexData[j * (i_u8M + 1) + i].position.z = i_sphereData.radius * (float)cos( j * theta );
					vertexData[j * (i_u8M + 1) + i].position.x = i_sphereData.radius * \
						(float)sin( j * theta ) * (float)cos( i * alpha );
					vertexData[j * (i_u8M + 1) + i].position.y = i_sphereData.radius * \
						(float)sin( j * theta ) * (float)sin( i * alpha );
					vertexData[j * (i_u8M + 1) + i].colour = i_sphereData.colour; 
				}
			}
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


	// Fill the index buffer
	{
		uint16_t* indices;
		{
			unsigned int lockEntireBuffer = 0;
			DWORD useDefaultLockingBehavior = 0;
			HRESULT result = _indexBuffer->Lock( lockEntireBuffer, lockEntireBuffer,
				reinterpret_cast<void**>( &indices ), useDefaultLockingBehavior );
			if ( FAILED( result ) )
			{
				LogMessage( "Failed to lock the index buffer" );
				return FAIL;
			}
		}

		// Set the buffer
		for( int j = 0; j < i_u8N; ++j )
		{
			for( int i = 0; i < i_u8M; ++i )
			{
				indices[0 + (i * 6) + j * (6 * i_u8M)] = i + 0 + j * (i_u8M + 1);
				indices[1 + (i * 6) + j * (6 * i_u8M)] = i + (i_u8M + 1) + j * (i_u8M + 1);
				indices[2 + (i * 6) + j * (6 * i_u8M)] = i + (i_u8M + 2) + j * (i_u8M + 1);
				indices[3 + (i * 6) + j * (6 * i_u8M)] = i + 0 + j * (i_u8M + 1);
				indices[4 + (i * 6) + j * (6 * i_u8M)] = i + 1 + j * (i_u8M + 1);
				indices[5 + (i * 6) + j * (6 * i_u8M)] = i + (i_u8M + 2) + j * (i_u8M + 1);
			}
		}

		// The buffer must be "unlocked" before it can be used
		{
			HRESULT result = _indexBuffer->Unlock();
			if ( FAILED( result ) )
			{
				LogMessage( "Failed to unlock the index buffer" );
				return FAIL;
			}
		}
	}
	return SUCCESS;
}