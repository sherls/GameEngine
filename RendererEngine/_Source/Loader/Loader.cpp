/**
 ****************************************************************************************************
 * \file		Loader.cpp
 * \brief		Loader class implementation
 ****************************************************************************************************
*/

// Utilities header
#include <UtilitiesDefault.h>
#include <Parser/MeshParser/MeshParser.h>

#include "Loader.h"
#include "../Logging/Logging.h"
#include "../Renderer/MainRenderer.h"

/**
 ****************************************************************************************************
	\fn			bool LoadDepthShader( const char *i_fileName, IDirect3DPixelShader9* &o_compiledShader, ID3DXConstantTable* &o_constantTable )
	\brief		Load the depth shader
	\param		i_fileName the filename of the depth shader
	\param		o_compiledShader the output depth shader
	\param		o_constantTable the output contant table of depth shader
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Loader::LoadDepthShader( const char *i_fileName, IDirect3DPixelShader9* &o_compiledShader, ID3DXConstantTable* &o_constantTable )
{
	char fileName[MAX_INPUT_LEN];
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

	LogMessage( "Depth shader for this entity is " + std::string(i_fileName) );

	// Create the fragment shader
	{
		// Load the compiled fragment shader from disk
		void* compiledFragmentShader;
		{
			// Open the file
			strcpy_s( fileName, MAX_INPUT_LEN, "../../External/Data/FragmentShaders/" );
			strcat_s( fileName, MAX_INPUT_LEN, i_fileName );
			HANDLE fileHandle;
			{
				DWORD desiredAccess = GENERIC_READ;
				DWORD otherProgramsCanStillReadTheFile = FILE_SHARE_READ;
				SECURITY_ATTRIBUTES* useDefaultSecurity = NULL;
				DWORD onlySucceedIfFileExists = OPEN_EXISTING;
				DWORD useDefaultAttributes = FILE_ATTRIBUTE_NORMAL;
				HANDLE dontUseTemplateFile = NULL;
				fileHandle = CreateFile( fileName, desiredAccess, otherProgramsCanStillReadTheFile,
					useDefaultSecurity, onlySucceedIfFileExists, useDefaultAttributes, dontUseTemplateFile );
				if ( fileHandle == INVALID_HANDLE_VALUE )
				{
					LogMessage( std::string( "Failed to open depth shader " ) + fileName );
					return FAIL;
				}
			}
			// Get the file's size
			DWORD fileSize;
			{
				LARGE_INTEGER fileSize_integer;
				if ( GetFileSizeEx( fileHandle, &fileSize_integer ) == SUCCESS )
				{
					// This is unsafe, because the file's size could be bigger than a DWORD
					fileSize = static_cast<DWORD>( fileSize_integer.QuadPart );
				}
				else
				{
					LogMessage( std::string( "Couldn't get the size of depth shader " ) + fileName );
					CloseHandle( fileHandle );
					return FAIL;
				}
			}
			// Read the file's contents into temporary memory
			compiledFragmentShader = malloc( fileSize );
			{
				DWORD bytesReadCount;
				OVERLAPPED* readSynchronously = NULL;
				if ( ReadFile( fileHandle, compiledFragmentShader, fileSize,
					&bytesReadCount, readSynchronously ) == SUCCESS )
				{
					CloseHandle( fileHandle );
				}
				else
				{
					LogMessage( std::string( "Couldn't read the contents of depth shader " ) + fileName );
					free( compiledFragmentShader );
					CloseHandle( fileHandle );
					return FAIL;
				}
			}
		}
		// Create the depth shader object
		{
			HRESULT result = direct3dDevice->CreatePixelShader( reinterpret_cast<DWORD*>( compiledFragmentShader ), &o_compiledShader );
			if ( SUCCEEDED( result ) )
			{
				// Get the constant table from the fragment shader
				result = D3DXGetShaderConstantTable( reinterpret_cast<DWORD*>( compiledFragmentShader ), &o_constantTable );
				if ( SUCCEEDED( result ) )
				{
					// Set the default values
					result = o_constantTable->SetDefaults( direct3dDevice );
					if ( FAILED( result ) )
					{
						LogMessage( "Couldn't set the depth shader constant defaults" );
					}
				}
				free( compiledFragmentShader );
				if ( FAILED( result ) )
				{
					LogMessage( "Couldn't get the depth shader constant table" );
					return FAIL;
				}
			}
			else
			{
				LogMessage( "Couldn't create the depth shader object" );
				free( compiledFragmentShader );
				return FAIL;
			}
		}
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool LoadFragmentShader( const char *i_fileName, IDirect3DPixelShader9* &o_compiledShader, ID3DXConstantTable* &o_constantTable )
	\brief		Load the fragment shader
	\param		i_fileName the filename of the fragment shader
	\param		o_compiledShader the output fragment shader
	\param		o_constantTable the output contant table of fragment shader
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Loader::LoadFragmentShader( const char *i_fileName, IDirect3DPixelShader9* &o_compiledShader, ID3DXConstantTable* &o_constantTable )
{
	char fileName[MAX_INPUT_LEN];
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

	LogMessage( "Fragment shader for this entity is " + std::string(i_fileName) );

	// Create the fragment shader
	{
		// Load the compiled fragment shader from disk
		void* compiledFragmentShader;
		{
			// Open the file
			strcpy_s( fileName, MAX_INPUT_LEN, "../../External/Data/FragmentShaders/" );
			strcat_s( fileName, MAX_INPUT_LEN, i_fileName );
			HANDLE fileHandle;
			{
				DWORD desiredAccess = GENERIC_READ;
				DWORD otherProgramsCanStillReadTheFile = FILE_SHARE_READ;
				SECURITY_ATTRIBUTES* useDefaultSecurity = NULL;
				DWORD onlySucceedIfFileExists = OPEN_EXISTING;
				DWORD useDefaultAttributes = FILE_ATTRIBUTE_NORMAL;
				HANDLE dontUseTemplateFile = NULL;
				fileHandle = CreateFile( fileName, desiredAccess, otherProgramsCanStillReadTheFile,
					useDefaultSecurity, onlySucceedIfFileExists, useDefaultAttributes, dontUseTemplateFile );
				if ( fileHandle == INVALID_HANDLE_VALUE )
				{
					LogMessage( std::string( "Failed to open fragment shader " ) + fileName );
					return FAIL;
				}
			}
			// Get the file's size
			DWORD fileSize;
			{
				LARGE_INTEGER fileSize_integer;
				if ( GetFileSizeEx( fileHandle, &fileSize_integer ) == SUCCESS )
				{
					// This is unsafe, because the file's size could be bigger than a DWORD
					fileSize = static_cast<DWORD>( fileSize_integer.QuadPart );
				}
				else
				{
					LogMessage( std::string( "Couldn't get the size of fragment shader " ) + fileName );
					CloseHandle( fileHandle );
					return FAIL;
				}
			}
			// Read the file's contents into temporary memory
			compiledFragmentShader = malloc( fileSize );
			{
				DWORD bytesReadCount;
				OVERLAPPED* readSynchronously = NULL;
				if ( ReadFile( fileHandle, compiledFragmentShader, fileSize,
					&bytesReadCount, readSynchronously ) == SUCCESS )
				{
					CloseHandle( fileHandle );
				}
				else
				{
					LogMessage( std::string( "Couldn't read the contents of fragment shader " ) + fileName );
					free( compiledFragmentShader );
					CloseHandle( fileHandle );
					return FAIL;
				}
			}
		}
		// Create the fragment shader object
		{
			HRESULT result = direct3dDevice->CreatePixelShader( reinterpret_cast<DWORD*>(compiledFragmentShader), &o_compiledShader );
			if ( SUCCEEDED( result ) )
			{
				// Get the constant table from the fragment shader
				result = D3DXGetShaderConstantTable( reinterpret_cast<DWORD*>(compiledFragmentShader), &o_constantTable );
				if ( SUCCEEDED( result ) )
				{
					// Set the default values
					result = o_constantTable->SetDefaults( direct3dDevice );
					if ( FAILED( result ) )
					{
						LogMessage( "Couldn't set the fragment shader constant defaults" );
					}
				}
				free( compiledFragmentShader );
				if ( FAILED( result ) )
				{
					LogMessage( "Couldn't get the fragment shader constant table" );
					return FAIL;
				}
			}
			else
			{
				LogMessage( "Couldn't create the fragment shader object" );
				free( compiledFragmentShader );
				return FAIL;
			}
		}
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool LoadVertexShader( const char *i_fileName, IDirect3DVertexShader9* &o_compiledShader, ID3DXConstantTable* &o_constantTable )
	\brief		Load the vertex shader
	\param		i_fileName the filename of the vertex shader
	\param		o_compiledShader the output vertex shader
	\param		o_constantTable the output contant table of vertex shader
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Loader::LoadVertexShader( const char *i_fileName, IDirect3DVertexShader9* &o_compiledShader, ID3DXConstantTable* &o_constantTable )
{
	char fileName[MAX_INPUT_LEN];
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

	LogMessage( "Vertex shader for this entity is " + std::string(i_fileName) );

	// Create the vertex shader
	{
		// Load the compiled vertex shader from disk
		void* compiledVertexShader;
		{
			strcpy_s( fileName, MAX_INPUT_LEN, "../../External/Data/VertexShaders/" );
			strcat_s( fileName, MAX_INPUT_LEN, i_fileName );
			HANDLE fileHandle;
			{
				DWORD desiredAccess = GENERIC_READ;
				DWORD otherProgramsCanStillReadTheFile = FILE_SHARE_READ;
				SECURITY_ATTRIBUTES* useDefaultSecurity = NULL;
				DWORD onlySucceedIfFileExists = OPEN_EXISTING;
				DWORD useDefaultAttributes = FILE_ATTRIBUTE_NORMAL;
				HANDLE dontUseTemplateFile = NULL;
				fileHandle = CreateFile( fileName, desiredAccess, otherProgramsCanStillReadTheFile,
					useDefaultSecurity, onlySucceedIfFileExists, useDefaultAttributes, dontUseTemplateFile );
				if ( fileHandle == INVALID_HANDLE_VALUE )
				{
					LogMessage( std::string( "Failed to open vertex shader " ) + fileName );
					return FAIL;
				}
			}
			// Get the file's size
			DWORD fileSize;
			{
				LARGE_INTEGER fileSize_integer;
				if ( GetFileSizeEx( fileHandle, &fileSize_integer ) == SUCCESS )
				{
					// This is unsafe, because the file's size could be bigger than a DWORD
					fileSize = static_cast<DWORD>( fileSize_integer.QuadPart );
				}
				else
				{
					LogMessage( std::string( "Couldn't get the size of vertex shader " ) + fileName );
					CloseHandle( fileHandle );
					return FAIL;
				}
			}
			// Read the file's contents into temporary memory
			compiledVertexShader = malloc( fileSize );
			{
				DWORD bytesReadCount;
				OVERLAPPED* readSynchronously = NULL;
				if ( ReadFile( fileHandle, compiledVertexShader, fileSize,
					&bytesReadCount, readSynchronously ) == SUCCESS )
				{
					CloseHandle( fileHandle );
				}
				else
				{
					LogMessage( std::string( "Couldn't read the contents of vertex shader " ) + fileName );
					free( compiledVertexShader );
					CloseHandle( fileHandle );
					return FAIL;
				}
			}
		}

		// Create the vertex shader object
		{
			HRESULT result = direct3dDevice->CreateVertexShader( reinterpret_cast<DWORD*>( compiledVertexShader ), &o_compiledShader );
			if ( SUCCEEDED( result ) )
			{
				// Get the constant table from the vertex shader
				result = D3DXGetShaderConstantTable( reinterpret_cast<DWORD*>( compiledVertexShader ), &o_constantTable );
				if ( SUCCEEDED( result ) )
				{
					// Set the default values
					result = o_constantTable->SetDefaults( direct3dDevice );
					if ( FAILED( result ) )
					{
						LogMessage( "Couldn't set the vertex shader constant defaults" );
					}
				}
				free( compiledVertexShader );
				if ( FAILED( result ) )
				{
					LogMessage( "Couldn't get the vertex shader constant table" );
					return FAIL;
				}
			}
			else
			{
				LogMessage( "Couldn't create the vertex shader object" );
				free( compiledVertexShader );
				return FAIL;
			}
		}
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool LoadMeshData( const char *i_fileName, UINT32 &o_u32VertexCount, UINT32 &o_u32PrimitiveCount,
				IDirect3DIndexBuffer9* &o_indexBuffer, IDirect3DVertexBuffer9* &o_vertexBuffer )
	\brief		Load the mesh
	\param		i_fileName the filename of the mesh
	\param		o_u32VertexCount the output vertex count
	\param		o_u32PrimitiveCount the output primitive count
	\param		o_indexBuffer the output index buffer
	\param		o_vertexBuffer the output vertex buffer
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Loader::LoadMeshData( const char *i_fileName, UINT32 &o_u32VertexCount, UINT32 &o_u32PrimitiveCount,
	IDirect3DIndexBuffer9* &o_indexBuffer, IDirect3DVertexBuffer9* &o_vertexBuffer )
{
	const Utilities::MeshParser meshData( i_fileName );
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	DWORD usage = g_mainRenderer::Get().GetVertexProcessingType();

	LogMessage( "Mesh data for this entity is " + std::string(i_fileName) );

	o_u32VertexCount = meshData.m_u32TotalVertices;
	o_u32PrimitiveCount = meshData.m_u32TotalPrimitives;

	// Create a vertex buffer
	{
		// A triangle has three vertices
		unsigned int bufferSize = o_u32VertexCount * sizeof( Utilities::S_NORMAL_MAP_VERTEX_DATA );
		// We will define our own vertex format
		DWORD useSeparateVertexDeclaration = 0;
		// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
		D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
		HANDLE* notUsed = NULL;

		HRESULT result = direct3dDevice->CreateVertexBuffer( bufferSize, usage, useSeparateVertexDeclaration,
			useDefaultPool, &o_vertexBuffer, notUsed );
		if ( FAILED( result ) )
		{
			LogMessage( "Failed to create the vertex buffer" );
			return FAIL;
		}
	}

	// Create the index buffer
	{
		unsigned int bufferSize = o_u32PrimitiveCount * 3 * sizeof( uint16_t );
		D3DFORMAT format = D3DFMT_INDEX16;
		D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
		HANDLE *notUsed = NULL;

		HRESULT result = direct3dDevice->CreateIndexBuffer( bufferSize, usage, format, useDefaultPool,
			&o_indexBuffer, notUsed );
		if( FAILED(result) )
		{
			LogMessage( "Failed to create the index buffer" );
			return FAIL;
		}
	}

	// Fill the vertex buffer with the triangle's vertices
	{
		// Before the vertex buffer can be changed it must be "locked"
		Utilities::S_NORMAL_MAP_VERTEX_DATA* vertexData;
		{
			unsigned int lockEntireBuffer = 0;
			DWORD useDefaultLockingBehavior = 0;
			HRESULT result = o_vertexBuffer->Lock( lockEntireBuffer, lockEntireBuffer,
				reinterpret_cast<void**>( &vertexData ), useDefaultLockingBehavior );
			if ( FAILED( result ) )
			{
				LogMessage( "Failed to lock the vertex buffer" );
				return FAIL;
			}
		}
		// Fill the buffer
		{
			meshData.GetVertexData( vertexData );
		}
		// The buffer must be "unlocked" before it can be used
		{
			HRESULT result = o_vertexBuffer->Unlock();
			if ( FAILED( result ) )
			{
				LogMessage( "Failed to unlock the vertex buffer" );
				return FAIL;
			}
		}
	}

	// Fill index buffer
	{
		uint16_t* indices;
		{
			unsigned int lockEntireBuffer = 0;
			DWORD useDefaultLockingBehavior = 0;
			HRESULT result = o_indexBuffer->Lock( lockEntireBuffer, lockEntireBuffer,
				reinterpret_cast<void**>( &indices ), useDefaultLockingBehavior );
			if ( FAILED( result ) )
			{
				LogMessage( "Failed to lock the index buffer" );
				return FAIL;
			}
		}
		meshData.GetIndexData( indices );

		// The buffer must be "unlocked" before it can be used
		{
			HRESULT result = o_indexBuffer->Unlock();
			if ( FAILED( result ) )
			{
				LogMessage( "Failed to unlock the index buffer" );
				return FAIL;
			}
		}
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool LoadTexture( const char *i_fileName, IDirect3DTexture9* &o_texture )
	\brief		Load the texture
	\param		i_fileName the filename of the texture
	\param		o_texture the output texture data
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Loader::LoadTexture( const char *i_fileName, IDirect3DTexture9* &o_texture )
{
	char fileName[MAX_INPUT_LEN];

	LogMessage( "Texture data for this entity is " + std::string(i_fileName) );

	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	strcpy_s( fileName, MAX_INPUT_LEN, "../../External/Data/Textures/" );
	strcat_s( fileName, MAX_INPUT_LEN, i_fileName );

	HRESULT result = D3DXCreateTextureFromFile( direct3dDevice, fileName, &o_texture );
	if( !SUCCEEDED(result) )
	{
		LogMessage( "Failed to create texture " + std::string(i_fileName) );
		return FAIL;
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool LoadNormalMap( const char *i_fileName, IDirect3DTexture9* &o_texture )
	\brief		Load the normal map texture
	\param		i_fileName the filename of the normal map
	\param		o_texture the output normal map data
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::Loader::LoadNormalMap( const char *i_fileName, IDirect3DTexture9* &o_texture )
{
	char fileName[MAX_INPUT_LEN];

	LogMessage( "Normal map data for this entity is " + std::string(i_fileName) );

	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	strcpy_s( fileName, MAX_INPUT_LEN, "../../External/Data/Textures/" );
	strcat_s( fileName, MAX_INPUT_LEN, i_fileName );

	HRESULT result = D3DXCreateTextureFromFile( direct3dDevice, fileName, &o_texture );
	if( !SUCCEEDED(result) )
	{
		LogMessage( "Failed to create normal map " + std::string(i_fileName) );
		return FAIL;
	}

	return SUCCESS;
}