/**
 ****************************************************************************************************
 * \file		Setter.h
 * \brief		Setter class implementation
 ****************************************************************************************************
*/

// Utilities header
//#include <Time/Time.h>
#include <UtilitiesDefault.h>

#include "Setter.h"
#include "../Logging/Logging.h"
#include "../Renderer/MainRenderer.h"

/**
 ****************************************************************************************************
	\fn			void SetVertexShader( IDirect3DVertexShader9* i_vertexShader, ID3DXConstantTable* i_constantTable,
				const bool i_supportCamera,
				const D3DXMATRIX &i_cameraWorldToViewTransform, const D3DXMATRIX &i_cameraViewToProjectedTransform,
				const D3DXMATRIX &i_directionalLightWorldToViewTransform, const D3DXMATRIX &i_directionalLightViewToProjectedTransform )
	\brief		Set the vertex shader to be used by the device
	\param		i_vertexShader the vertex shader to be set
	\param		i_constantTable the constant table of the vertex shader
	\param		i_supportCamera whether this vertex shader support camera
	\param		i_cameraWorldToViewTransform the world to view transformation matrix of camera
	\param		i_cameraViewToProjectedTransform the view to projected transformation matrix of camera
	\param		i_directionalLightWorldToViewTransform the world to view transformation matrix of directional light
	\param		i_directionalLightViewToProjectedTransform the view to projected transformation matrix of directional light
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetVertexShader( IDirect3DVertexShader9* i_vertexShader, ID3DXConstantTable* i_constantTable,
	const bool i_supportCamera,
	const D3DXMATRIX &i_cameraWorldToViewTransform, const D3DXMATRIX &i_cameraViewToProjectedTransform,
	const D3DXMATRIX &i_directionalLightWorldToViewTransform, const D3DXMATRIX &i_directionalLightViewToProjectedTransform )
{
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Vertex shader" );
#endif	// #ifdef _DEBUG
	//LogMessage( "Set vertex shader" );

	HRESULT result = direct3dDevice->SetVertexShader( i_vertexShader );
	assert( SUCCEEDED(result) );

	// Set world to view transformation
	{
		D3DXHANDLE variable;
		D3DXHANDLE topLevelVariable = NULL;
		const char *variableName = "g_transform_worldToView";
		variable = i_constantTable->GetConstantByName( topLevelVariable, variableName );
		if( variable != NULL )
		{
			D3DXMATRIX transform_worldToView;
			if( i_supportCamera )
				transform_worldToView = i_cameraWorldToViewTransform;
			else
				transform_worldToView = i_directionalLightWorldToViewTransform;
			result = i_constantTable->SetMatrix( direct3dDevice, variable, &transform_worldToView );
		}
	}

	// Set view to projected transformation
	{
		D3DXHANDLE variable;
		D3DXHANDLE topLevelVariable = NULL;
		const char *variableName = "g_transform_viewToProjected";
		variable = i_constantTable->GetConstantByName( topLevelVariable, variableName );
		if( variable != NULL )
		{
			D3DXMATRIX transform_viewToProjected;
			if( i_supportCamera )
				transform_viewToProjected = i_cameraViewToProjectedTransform;
			else
				transform_viewToProjected = i_directionalLightViewToProjectedTransform;
			result = i_constantTable->SetMatrix( direct3dDevice, variable, &transform_viewToProjected );
		}
	}

	if( !i_supportCamera )
	{
		// Set directional light world to view transformation
		{
			D3DXHANDLE variable;
			D3DXHANDLE topLevelVariable = NULL;
			const char *variableName = "g_directionalLight_worldToView";
			variable = i_constantTable->GetConstantByName( topLevelVariable, variableName );
			if( variable != NULL )
			{
				D3DXMATRIX transform_worldToView = i_directionalLightWorldToViewTransform;
				result = i_constantTable->SetMatrix( direct3dDevice, variable, &transform_worldToView );
			}
		}

		// Set view to projected transformation
		{
			D3DXHANDLE variable;
			D3DXHANDLE topLevelVariable = NULL;
			const char *variableName = "g_directionalLight_viewToProjected";
			variable = i_constantTable->GetConstantByName( topLevelVariable, variableName );
			if( variable != NULL )
			{
				D3DXMATRIX transform_viewToProjected = i_directionalLightViewToProjectedTransform;
				result = i_constantTable->SetMatrix( direct3dDevice, variable, &transform_viewToProjected );
			}
		}
	}

#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif
}

/**
 ****************************************************************************************************
	\fn			void SetVertexShader( IDirect3DVertexShader9* i_vertexShader )
	\brief		Set the vertex shader to be used by the device
	\param		i_vertexShader the vertex shader to be set
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetVertexShader( IDirect3DVertexShader9* i_vertexShader )
{
	IDirect3DDevice9* direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set vertex shader" );
#endif	// #ifdef _DEBUG
	HRESULT result = direct3dDevice->SetVertexShader( i_vertexShader );
	assert( SUCCEEDED(result) );
#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void SetFragmentShader( IDirect3DPixelShader9* i_fragmentShader )
	\brief		Set the fragment shader to be used by the device
	\param		i_fragmentShader the fragment shader to be set
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetFragmentShader( IDirect3DPixelShader9* i_fragmentShader )
{
	IDirect3DDevice9* direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

	HRESULT result = direct3dDevice->SetPixelShader( i_fragmentShader );
	assert( SUCCEEDED(result) );
}

/**
 ****************************************************************************************************
	\fn			void SetVertexBuffer( IDirect3DVertexBuffer9* i_vertexBuffer, const unsigned int &i_bufferStride )
	\brief		Set the vertex buffer to be used by the device
	\param		i_vertexBuffer the vertex buffer to be set
	\param		i_bufferStride the "stride" which defines how large a single vertex is in the stream of data
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetVertexBuffer( IDirect3DVertexBuffer9* i_vertexBuffer, const unsigned int &i_bufferStride )
{
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

	// Bind a specific vertex buffer to the device as a data source
	{
		// There can be multiple streams of data feeding the display adaptor at the same time
		unsigned int streamIndex = 0;
		// It's possible to start streaming data in the middle of a vertex buffer
		unsigned int bufferOffset = 0;
		HRESULT result = direct3dDevice->SetStreamSource( streamIndex, i_vertexBuffer, bufferOffset, i_bufferStride );
		assert( SUCCEEDED(result) );
	}
}

/**
 ****************************************************************************************************
	\fn			void SetIndexBuffer( IDirect3DIndexBuffer9* i_indexBuffer )
	\brief		Set index buffer to be used by the device
	\param		i_indexBuffer the index buffer to be set
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetIndexBuffer( IDirect3DIndexBuffer9* i_indexBuffer )
{
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

	HRESULT result = direct3dDevice->SetIndices( i_indexBuffer );
	assert( SUCCEEDED(result) );
}

/**
 ****************************************************************************************************
	\fn			void SetCameraPosition( ID3DXConstantTable* i_fragmentShaderConstantTable, const D3DXVECTOR3 &i_cameraPosition,
					const float &i_cameraFarView )
	\brief		Set the camera position in the scene
	\param		i_fragmentShaderConstantTable the fragment shader constant table
	\param		i_cameraPosition the camera position
	\param		i_cameraFarView the far view of camera
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetCamera( ID3DXConstantTable* i_fragmentShaderConstantTable, const D3DXVECTOR3 &i_cameraPosition,
	const float &i_cameraFarView )
{
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set camera" );
#endif	// #ifdef _DEBUG

	// Position
	{
		D3DXHANDLE variable;
		{
			D3DXHANDLE topLevelVariable = NULL;
			const char* variableName = "g_camera_position";
			variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		}
		if ( variable != NULL )
		{
			//D3DXMATRIX transform_worldToView = cScene::GetWorldToViewTransform();
			HRESULT result = i_fragmentShaderConstantTable->SetFloatArray( direct3dDevice, variable, i_cameraPosition, 3 );
			assert( SUCCEEDED(result) );
		}
		else
		{
			// If the variable isn't used by the shader it won't be included in the compiled version;
			// this can make it hard to know whether to know a NULL handle is an error or not.
			// It is up to you how to handle this
		}
	}

	// Far view
	{
		D3DXHANDLE variable;
		{
			D3DXHANDLE topLevelVariable = NULL;
			const char* variableName = "g_distance_farPlane";
			variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		}
		if ( variable != NULL )
		{
			HRESULT result = i_fragmentShaderConstantTable->SetFloat( direct3dDevice, variable, i_cameraFarView );
			assert( SUCCEEDED(result) );
		}
		else
		{
			// If the variable isn't used by the shader it won't be included in the compiled version;
			// this can make it hard to know whether to know a NULL handle is an error or not.
			// It is up to you how to handle this
		}
	}
	// }

#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void SetPointLightPosition( ID3DXConstantTable* i_fragmentShaderConstantTable, const D3DXVECTOR3 &i_pointLightPosition )
	\brief		Set the point light position in the scene
	\param		i_fragmentShaderConstantTable the fragment shader constant table
	\param		i_pointLightPosition the point light position
	\param		i_pointLightColour the colour of point light
	\param		i_pointLightAmbient the ambient colour of point light
	\param		i_pointLightIntensity the intensity of point light
	\param		i_pointLightRadius the radius of point light
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetPointLight( ID3DXConstantTable* i_fragmentShaderConstantTable, const D3DXVECTOR3 &i_pointLightPosition,
	const D3DCOLOR &i_pointLightColour, const D3DCOLOR &i_pointLightAmbient,
	const float &i_pointLightIntensity, const float &i_pointLightRadius )
{
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	HRESULT result;

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set point light" );
#endif	// #ifdef _DEBUG

	// Set position
	{
		D3DXHANDLE variable;
		{
			D3DXHANDLE topLevelVariable = NULL;
			const char* variableName = "g_pointLight_position";
			variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		}
		if ( variable != NULL )
		{
			float float_array[] = { i_pointLightPosition.x, i_pointLightPosition.y, i_pointLightPosition.z };
			result = i_fragmentShaderConstantTable->SetFloatArray( direct3dDevice, variable, float_array, 3 );
			assert( SUCCEEDED(result) );
		}
		else
		{
			// If the variable isn't used by the shader it won't be included in the compiled version;
			// this can make it hard to know whether to know a NULL handle is an error or not.
			// It is up to you how to handle this
		}
	}

	// Set colour
	{
		D3DXHANDLE variable;
		{
			D3DXHANDLE topLevelVariable = NULL;
			const char* variableName = "g_pointLight_parameter";
			variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		}
		if ( variable != NULL )
		{
			float pointLightParameter[4] = { static_cast<float>(((i_pointLightColour >> 16)&0xFF)/255),
				static_cast<float>(((i_pointLightColour >> 8)&0xFF)/255),
				static_cast<float>((i_pointLightColour & 0xFF)/255),
				i_pointLightIntensity };
			result = i_fragmentShaderConstantTable->SetFloatArray( direct3dDevice, variable, pointLightParameter, 4 );
			assert( SUCCEEDED(result) );
		}
		else
		{
			// If the variable isn't used by the shader it won't be included in the compiled version;
			// this can make it hard to know whether to know a NULL handle is an error or not.
			// It is up to you how to handle this
		}
	}

	// Set ambient
	{
		D3DXHANDLE variable;
		{
			D3DXHANDLE topLevelVariable = NULL;
			const char* variableName = "g_ambientLight";
			variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		}
		if ( variable != NULL )
		{
			float float_array[3] = { static_cast<float>(((i_pointLightAmbient >> 16)&0xFF)/255),
				static_cast<float>(((i_pointLightAmbient >> 8)&0xFF)/255),
				static_cast<float>((i_pointLightAmbient & 0xFF)/255) };
			HRESULT result = i_fragmentShaderConstantTable->SetFloatArray( direct3dDevice, variable, float_array, 3 );
			assert( SUCCEEDED(result) );
		}
		else
		{
			// If the variable isn't used by the shader it won't be included in the compiled version;
			// this can make it hard to know whether to know a NULL handle is an error or not.
			// It is up to you how to handle this
		}
	}

	// Set radius
	{
		D3DXHANDLE variable;
		{
			D3DXHANDLE topLevelVariable = NULL;
			const char* variableName = "g_pointLight_radius";
			variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		}
		if ( variable != NULL )
		{
			HRESULT result = i_fragmentShaderConstantTable->SetFloat( direct3dDevice, variable, i_pointLightRadius );
			assert( SUCCEEDED(result) );
		}
		else
		{
			// If the variable isn't used by the shader it won't be included in the compiled version;
			// this can make it hard to know whether to know a NULL handle is an error or not.
			// It is up to you how to handle this
		}
	}

#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void SetFragmentShader( ID3DXConstantTable* i_fragmentShaderConstantTable,
					const D3DXVECTOR3 &i_directionalLightDirection, const D3DCOLOR &i_directionalLightColour,
					const float &i_directionalLightIntensity, const float &i_directionalLightFarView )
	\brief		Set the fragment shader to be used by the device
	\param		i_fragmentShaderConstantTable the constant table of the fragment shader
	\param		i_directionalLightDirection the direction of directional light
	\param		i_directionalLightColour the colour of directional light
	\param		i_directionalLightIntensity the intensity of directional light
	\param		i_directionalLightFarView the far view of directional light
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetDirectionalLight( ID3DXConstantTable* i_fragmentShaderConstantTable,
	const D3DXVECTOR3 &i_directionalLightDirection, const D3DCOLOR &i_directionalLightColour,
	const float &i_directionalLightIntensity, const float &i_directionalLightFarView )
{
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	HRESULT result;

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set directional light" );
#endif	// #ifdef _DEBUG

	// Set directional light direction
	{
		D3DXHANDLE variable;
		{
			D3DXHANDLE topLevelVariable = NULL;
			const char* variableName = "g_directionalLight_direction";
			variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		}
		if ( variable != NULL )
		{
			float float_array[] = { i_directionalLightDirection.x, i_directionalLightDirection.y, i_directionalLightDirection.z };
			result = i_fragmentShaderConstantTable->SetFloatArray( direct3dDevice, variable, float_array, 3 );
			assert( SUCCEEDED(result) );
		}
		else
		{
			// If the variable isn't used by the shader it won't be included in the compiled version;
			// this can make it hard to know whether to know a NULL handle is an error or not.
			// It is up to you how to handle this
		}
	}

	// Set directional light colour and intensity to fragment shader
	{
		D3DXHANDLE variable;
		{
			D3DXHANDLE topLevelVariable = NULL;
			const char* variableName = "g_directionalLight_parameter";
			variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		}
		if ( variable != NULL )
		{
			float directionalLightParameter[4] = { (float)((i_directionalLightColour >> 16)&0xFF)/255,
											static_cast<float>(((i_directionalLightColour >> 8)&0xFF)/255),
											static_cast<float>((i_directionalLightColour & 0xFF)/255),
											i_directionalLightIntensity };
			result = i_fragmentShaderConstantTable->SetFloatArray( direct3dDevice, variable, directionalLightParameter, 4 );
			assert( SUCCEEDED(result) );
		}
		else
		{
			// If the variable isn't used by the shader it won't be included in the compiled version;
			// this can make it hard to know whether to know a NULL handle is an error or not.
			// It is up to you how to handle this
		}
	}

	{
		D3DXHANDLE variable;
		{
			D3DXHANDLE topLevelVariable = NULL;
			const char* variableName = "g_distance_shadowFarPlane";
			variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		}
		if ( variable != NULL )
		{
			result = i_fragmentShaderConstantTable->SetFloat( direct3dDevice, variable, i_directionalLightFarView);
			assert( SUCCEEDED(result) );
		}
		else
		{
			// If the variable isn't used by the shader it won't be included in the compiled version;
			// this can make it hard to know whether to know a NULL handle is an error or not.
			// It is up to you how to handle this
		}
	}

#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void SetTexture( IDirect3DTexture9* i_texture, ID3DXConstantTable* i_fragmentShaderConstantTable )
	\brief		Set texture to be used by the device
	\param		i_texture the texture data
	\param		i_fragmentShaderConstantTable the constant table of fragment shader
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetTexture( IDirect3DTexture9* i_texture, ID3DXConstantTable* i_fragmentShaderConstantTable )
{
#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set diffuse texture" );
#endif	// #ifdef _DEBUG

	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	D3DXHANDLE variable;
	D3DXHANDLE topLevelVariable = NULL;
	const char * variableName = "g_sampler";

	variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );

	if( variable != NULL )
	{
		unsigned int index = i_fragmentShaderConstantTable->GetSamplerIndex( variable );

		if( index != static_cast<unsigned int>(-1) )
		{
			// Linear texture filtering
			// {
			direct3dDevice->SetSamplerState( static_cast<DWORD>(index), D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
			direct3dDevice->SetSamplerState( static_cast<DWORD>(index), D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
			direct3dDevice->SetSamplerState( static_cast<DWORD>(index), D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
			// }
			HRESULT result = direct3dDevice->SetTexture( static_cast<DWORD>(index), i_texture );
			assert( SUCCEEDED(result) );
		}
	}

#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void SetNormalMap( IDirect3DTexture9* i_normalMap, ID3DXConstantTable* i_fragmentShaderConstantTable )
	\brief		Set the normal map to be used by the device
	\param		i_normalMap the normal map
	\param		i_fragmentShaderConstantTable the constant table of fragment shader
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetNormalMap( IDirect3DTexture9* i_normalMap, ID3DXConstantTable *i_fragmentShaderConstantTable )
{
#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set normal map" );
#endif	// #ifdef _DEBUG

	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	D3DXHANDLE variable;
	D3DXHANDLE topLevelVariable = NULL;
	const char * variableName = "g_normalMap";

	variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );

	if( variable != NULL )
	{
		unsigned int index = i_fragmentShaderConstantTable->GetSamplerIndex( variable );

		if( index != static_cast<unsigned int>(-1) )
		{
			// Linear texture filtering
			// {
			direct3dDevice->SetSamplerState( static_cast<DWORD>(index), D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
			direct3dDevice->SetSamplerState( static_cast<DWORD>(index), D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
			direct3dDevice->SetSamplerState( static_cast<DWORD>(index), D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
			// }
			HRESULT result = direct3dDevice->SetTexture( static_cast<DWORD>(index), i_normalMap );
			assert( SUCCEEDED(result) );
		}
	}
#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void SetTextureUsage( bool i_usage, ID3DXConstantTable* i_fragmentShaderConstantTable )
	\brief		Set the condition whether texture is used or not
	\param		i_usage the usage of texture
	\param		i_fragmentShaderConstantTable the constant table of fragment shader
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetTextureUsage( bool i_usage, ID3DXConstantTable* i_fragmentShaderConstantTable )
{
#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set texture usage" );
#endif	// #ifdef _DEBUG

	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();
	D3DXHANDLE variable;
	D3DXHANDLE topLevelVariable = NULL;
	const char * variableName = "g_textureUsage";

	variable = i_fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );

	if( variable != NULL )
	{
		HRESULT result = i_fragmentShaderConstantTable->SetBool( direct3dDevice, variable, i_usage );
		assert( SUCCEEDED(result) );
	}
#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void SetModelToWorldTransformation( ID3DXConstantTable* i_vertexShaderConstantTable )
	\brief		Set model to world transformation of vertex shader to Matrix identity
	\param		i_vertexShaderConstantTable the constant table of vertex shader
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetModelToWorldTransformation( ID3DXConstantTable* i_vertexShaderConstantTable )
{
#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set model to world transformation" );
#endif	// #ifdef _DEBUG
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

	// Set model to world transformation matrix
	{
		D3DXHANDLE variable;
		D3DXHANDLE topLevelVariable = NULL;
		const char *variableName = "g_transform_modelToWorld";
		variable = i_vertexShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		if( variable != NULL )
		{
			D3DXMATRIX matrixIdentity;
			D3DXMatrixIdentity( &matrixIdentity );
			HRESULT result = i_vertexShaderConstantTable->SetMatrix( direct3dDevice, variable, &matrixIdentity );
			assert( SUCCEEDED(result) );
		}
	}
#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void SetModelToWorldTransformation( ID3DXConstantTable* i_vertexShaderConstantTable,
				const D3DXVECTOR3 &i_position, const float &i_orientation )
	\brief		Set model to world transformation of vertex shader
	\param		i_vertexShaderConstantTable the constant table of vertex shader
	\param		i_position the mesh position
	\param		i_orientation the orientation of the mesh
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetModelToWorldTransformation( ID3DXConstantTable* i_vertexShaderConstantTable,
	const D3DXVECTOR3 &i_position, const D3DXVECTOR3 &i_scale, const float &i_orientation )
{
#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set model to world transformation" );
#endif	// #ifdef _DEBUG
	IDirect3DDevice9 *direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

	// Set model to world transformation matrix
	{
		D3DXHANDLE variable;
		D3DXHANDLE topLevelVariable = NULL;
		const char *variableName = "g_transform_modelToWorld";
		variable = i_vertexShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
		if( variable != NULL )
		{
			D3DXMATRIX transform_modelToWorld;
			D3DXQUATERNION rotationQuaternion;
			D3DXQuaternionRotationAxis( &rotationQuaternion, &Utilities::UP_DIRECTION, i_orientation );
			D3DXMatrixTransformation( &transform_modelToWorld, NULL, NULL, &i_scale, NULL, &rotationQuaternion, &i_position );
			HRESULT result = i_vertexShaderConstantTable->SetMatrix( direct3dDevice, variable, &transform_modelToWorld );
			assert( SUCCEEDED(result) );
		}
	}
#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void SetSize( ID3DXConstantTable* i_vertexShaderConstantTable, const Utilities::S_SIZE &i_size )
	\brief		Set the size of the mesh
	\param		i_vertexShaderConstantTable the constant table of vertex shader
	\param		i_size the size of the mesh
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetSize( ID3DXConstantTable* i_vertexShaderConstantTable, const Utilities::S_SIZE &i_size )
{
	IDirect3DDevice9* direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set quad size" );
#endif	// #ifdef _DEBUG
	D3DXHANDLE variable;
	{
		D3DXHANDLE topLevelVariable = NULL;
		const char* variableName = "g_size";
		variable = i_vertexShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
	}
	if ( variable != NULL )
	{
		float size[2] = { i_size.width, i_size.height };
		HRESULT result = i_vertexShaderConstantTable->SetFloatArray( direct3dDevice, variable, size, 2 );
		assert( SUCCEEDED(result) );
	}
#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void SetPosition( ID3DXConstantTable* i_vertexShaderConstantTable, const D3DXVECTOR2 &i_position )
	\brief		Set the position of the mesh
	\param		i_vertexShaderConstantTable the constant table of vertex shader
	\param		i_position the position of the mesh
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Setter::SetPosition( ID3DXConstantTable* i_vertexShaderConstantTable, const D3DXVECTOR2 &i_position )
{
	IDirect3DDevice9* direct3dDevice = g_mainRenderer::Get().GetDirect3dDevice();

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Set quad position" );
#endif	// #ifdef _DEBUG
	D3DXHANDLE variable;
	{
		D3DXHANDLE topLevelVariable = NULL;
		const char* variableName = "g_position";
		variable = i_vertexShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
	}
	if ( variable != NULL )
	{
		float position[2] = { i_position.x, i_position.y };
		HRESULT result = i_vertexShaderConstantTable->SetFloatArray( direct3dDevice, variable, position, 2 );
		assert( SUCCEEDED(result) );
	}
#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}