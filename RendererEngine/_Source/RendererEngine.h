/**
 ****************************************************************************************************
 * \file		RendererEngine.h
 * \brief		Renderer Engine global function header
 ****************************************************************************************************
*/

#ifndef _RENDERER_ENGINE_H_
#define _RENDERER_ENGINE_H_

#include "RendererEngineTypes.h"

namespace RendererEngine
{
	typedef struct _s_entity_to_draw_
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 scale;
		Utilities::StringHash entity;
		float orientation;
		//do rotation
	} S_ENTITY_TO_DRAW;

	typedef struct _s_line_to_draw_
	{
		D3DXVECTOR3 startPosition;
		D3DXVECTOR3 endPosition;
		D3DXCOLOR startColour;
		D3DXCOLOR endColour;
	} S_LINE_TO_DRAW;

	typedef struct _s_quad_to_draw_
	{
		Utilities::S_SIZE size;
		D3DXVECTOR2 position;
		Utilities::StringHash texture;
		D3DCOLOR colour;
	} S_QUAD_TO_DRAW;

	typedef struct _s_text_to_draw_
	{
		D3DXVECTOR2 position;
		D3DXVECTOR2 size;
		D3DCOLOR colour;
		std::string text;
		UINT32 hAlign;
	} S_TEXT_TO_DRAW;

	typedef struct _s_sphere_to_draw_
	{
		D3DXVECTOR3 centre;
		D3DCOLOR colour;
		float radius;
	} S_SPHERE_TO_DRAW;

	bool Initialize( const char *i_windowName, const UINT32 &i_u32Width, const UINT32 &i_u32Height,
		const HINSTANCE &i_currInstance, int i_initialWindowDisplayState, const HWND &i_hwnd );
	bool BeginScene( D3DCOLOR i_clearColour );
	void Draw3D( const std::vector<S_ENTITY_TO_DRAW> &i_entitiesToDraw, const std::vector<S_LINE_TO_DRAW> &i_linesToDraw,
		const std::vector<S_SPHERE_TO_DRAW> &i_sphereToDraw,
		const D3DXMATRIX &i_cameraWorldToViewTransform, const D3DXMATRIX &i_cameraViewToProjectedTransform,
		const D3DXVECTOR3 &i_cameraPosition, const float &i_cameraFarView,
		const D3DXMATRIX &i_directionalLightWorldToViewTransform, const D3DXMATRIX &i_directionalLightViewToProjectedTransform,
		const D3DXVECTOR3 &i_directionalLightDirection, const D3DCOLOR &i_directionalLightColour,
		const float &i_directionalLightIntensity, const float &i_directionalLightFarView,
		const D3DCOLOR &i_pointLightColour, const D3DCOLOR &i_pointLightAmbient, const D3DXVECTOR3 &i_pointLightPosition,
		const float &i_pointLightIntensity, const float &i_pointLightRadius );
	void Draw2D( const std::vector<S_QUAD_TO_DRAW> &i_quadsToDraw, const std::vector<S_TEXT_TO_DRAW> &i_textToDraw );
	bool EndScene( void );
	void ShutDown( void );

	bool CreateEffect( const Utilities::StringHash &i_name, const Utilities::StringHash &i_vertexShaderFile,
		const Utilities::StringHash &i_fragmentShaderFile, const Utilities::E_ALPHA_MODE &i_alphaMode, const UINT8 &i_textureMode );
	bool CreateEntity( const Utilities::StringHash &i_name, const Utilities::StringHash &i_meshFile,
		const Utilities::StringHash &i_materialFile );
	bool CreateMaterial( const Utilities::StringHash &i_name, const Utilities::StringHash &i_effectFile,
		const Utilities::StringHash &i_textureFile, const Utilities::StringHash &i_normalMapTexture,
		const float &i_transparency, const float &i_shininess, const float &i_reflectance );
	bool CreateFragmentShader( const char *i_fileName );
	bool CreateVertexShader( const char *i_fileName );
	bool CreateDepthShader( const char *i_fileName );
	bool CreateTexture( const char *i_fileName );
	bool CreateNormalMap( const char *i_fileName );
	bool CreateMesh( const char *i_fileName );

	HRESULT GetTransform( const D3DTRANSFORMSTATETYPE &i_type, D3DXMATRIX &o_matrix );
	HRESULT GetViewPort( D3DVIEWPORT9 &o_viewport );
}

#endif  // #ifndef _RENDERER_ENGINE_H_