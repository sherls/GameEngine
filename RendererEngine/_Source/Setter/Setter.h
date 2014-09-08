/**
 ****************************************************************************************************
 * \file		Setter.h
 * \brief		Setter class to help Draw call set the required setting for 3D renderer
 ****************************************************************************************************
*/

#include "../RendererEngineTypes.h"

namespace RendererEngine
{
	namespace Setter
	{
		// Shaders
		void SetVertexShader( IDirect3DVertexShader9* i_vertexShader, ID3DXConstantTable* i_constantTable,
			const bool i_supportCamera,
			const D3DXMATRIX &i_cameraWorldToViewTransform, const D3DXMATRIX &i_cameraViewToProjectedTransform,
			const D3DXMATRIX &i_directionalLightWorldToViewTransform, const D3DXMATRIX &i_directionalLightViewToProjectedTransform );
		void SetVertexShader( IDirect3DVertexShader9* i_vertexShader );
		void SetFragmentShader( IDirect3DPixelShader9* i_fragmentShader );

		// Buffers
		void SetVertexBuffer( IDirect3DVertexBuffer9* i_vertexBuffer, const unsigned int &i_bufferStride );
		void SetIndexBuffer( IDirect3DIndexBuffer9* i_indexBuffer );

		// Fragment shader uniform
		void SetCamera( ID3DXConstantTable* i_fragmentShaderConstantTable, const D3DXVECTOR3 &i_cameraPosition,const float &i_cameraFarView );
		void SetPointLight( ID3DXConstantTable* i_fragmentShaderConstantTable, const D3DXVECTOR3 &i_pointLightPosition,
			const D3DCOLOR &i_pointLightColour, const D3DCOLOR &i_pointLightAmbient,
			const float &i_pointLightIntensity, const float &i_pointLightRadius );
		void SetDirectionalLight( ID3DXConstantTable* i_fragmentShaderConstantTable,
			const D3DXVECTOR3 &i_directionalLightDirection, const D3DCOLOR &i_directionalLightColour,
			const float &i_directionalLightIntensity, const float &i_directionalLightFarView );
		void SetTexture( IDirect3DTexture9* i_texture, ID3DXConstantTable* i_fragmentShaderConstantTable );
		void SetNormalMap( IDirect3DTexture9* i_normalMap, ID3DXConstantTable* i_fragmentShaderConstantTable );
		void SetTextureUsage( bool i_usage, ID3DXConstantTable* i_fragmentShaderConstantTable );

		// Vertex shader uniform
		void SetModelToWorldTransformation( ID3DXConstantTable* i_vertexShaderConstantTable );
		void SetModelToWorldTransformation( ID3DXConstantTable* i_vertexShaderConstantTable, const D3DXVECTOR3 &i_position, \
			const D3DXVECTOR3 &i_scale, const float &i_orientation );
		void SetSize( ID3DXConstantTable* i_vertexShaderConstantTable, const Utilities::S_SIZE &i_size );
		void SetPosition( ID3DXConstantTable* i_vertexShaderConstantTable, const D3DXVECTOR2 &i_position );

		void SetEnvironmentMap( const UINT32 &i_index );
		void SetMaterialTransparency( const UINT32 &i_index );
		void SetMaterialShininess( const UINT32 &i_index );
		void SetColourBuffer( const UINT32 &i_index );
	}
}