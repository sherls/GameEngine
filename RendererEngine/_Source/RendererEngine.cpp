/**
 ****************************************************************************************************
 * \file		RendererEngine.h
 * \brief		Renderer Engine global function implementation
 ****************************************************************************************************
*/

#include <map>
#include <stdint.h>

// Utilities header
#include <UtilitiesDefault.h>

#include "Line/Line.h"
#include "Quad/Quad.h"
#include "Text/Text.h"
#include "Loader/Loader.h"
#include "Sphere/Sphere.h"
#include "Setter/Setter.h"
#include "Window/Window.h"
#include "RendererEngine.h"
#include "Logging/Logging.h"

#include "Renderer/MainRenderer.h"

#define SET_REQUIRED_ITERATOR( i_itr, i_value, i_map ) ( (i_itr = i_map->find(i_value)) != i_map->end() )

namespace RendererEngine
{
	static bool bBeginScene = false;

	static std::map<Utilities::StringHash, S_EFFECT> *effectDatabase = NULL;
	static std::map<Utilities::StringHash, S_ENTITY> *entityDatabase = NULL;
	static std::map<Utilities::StringHash, S_FRAGMENT_SHADER> *fragmentShaderDatabase = NULL;
	static std::map<Utilities::StringHash, S_MATERIAL> *materialDatabase = NULL;
	static std::map<Utilities::StringHash, S_VERTEX_SHADER> *vertexShaderDatabase = NULL;
	static std::map<Utilities::StringHash, S_DEPTH_SHADER> *depthShaderDatabase = NULL;
	static std::map<Utilities::StringHash, S_MESH> *meshDatabase = NULL;
	static std::map<Utilities::StringHash, IDirect3DTexture9*> *textureDatabase = NULL;
	static std::map<Utilities::StringHash, IDirect3DTexture9*> *normalMapTextureDatabase = NULL;
	static std::map<Utilities::StringHash, IDirect3DCubeTexture9*> *environmentMapDatabase = NULL;

	void SortEntitiesToDraw( std::vector<S_ENTITY_TO_DRAW> &io_entitiesToDraw );
	void QuickSortByRenderState( std::vector<S_ENTITY_TO_DRAW> &io_entitiesToDraw, const INT8 &i_left, const INT8 &i_right );
}

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( const char *i_windowName, UINT32 i_u32Width, UINT32 i_u32Height,
				HINSTANCE i_currInstance, int i_initialWindowDisplayState )
	\brief		Initialize renderer engine
	\param		i_windowName the name of the window
	\param		i_u32Width the width of the window
	\param		i_u32Height the height of the window
	\param		i_currInstance the current instance of the window
	\param		i_initialWindowDisplayState the initial window display state
	\return		NONE
 ****************************************************************************************************
*/
bool RendererEngine::Initialize( const char *i_windowName, const UINT32 &i_u32Width, const UINT32 &i_u32Height,
	const HINSTANCE &i_currInstance, int i_initialWindowDisplayState, const HWND &i_hwnd )
{
	bool bReturn;

	bBeginScene = false;

	effectDatabase = new std::map<Utilities::StringHash, S_EFFECT>;
	entityDatabase = new std::map<Utilities::StringHash, S_ENTITY>;
	fragmentShaderDatabase = new std::map<Utilities::StringHash, S_FRAGMENT_SHADER>;
	materialDatabase = new std::map<Utilities::StringHash, S_MATERIAL>;
	vertexShaderDatabase = new std::map<Utilities::StringHash, S_VERTEX_SHADER>;
	depthShaderDatabase = new std::map<Utilities::StringHash, S_DEPTH_SHADER>;
	meshDatabase = new std::map<Utilities::StringHash, S_MESH>;
	textureDatabase = new std::map<Utilities::StringHash, IDirect3DTexture9*>;
	normalMapTextureDatabase = new std::map<Utilities::StringHash, IDirect3DTexture9*>;
	environmentMapDatabase = new std::map<Utilities::StringHash, IDirect3DCubeTexture9*>;

	if( !i_currInstance && i_hwnd )
		bReturn = g_mainWindow::Get().SetWindow( i_hwnd, i_u32Width, i_u32Height );
	else
		bReturn = g_mainWindow::Get().Initialize( i_currInstance, i_initialWindowDisplayState, i_windowName, i_u32Width, i_u32Height );
	if( bReturn == FAIL )
		return FAIL;

	bReturn = g_mainRenderer::Get().Initialize();
	if( bReturn == FAIL )
		return FAIL;

	bReturn = g_text::Get().Initialize( "Arial", Utilities::FONT_HEIGHT );
	if( bReturn == FAIL )
		return FAIL;

	bReturn = g_line::Get().Initialize();
	if( bReturn == FAIL )
		return FAIL;

	bReturn = g_sphere::Get().Initialize();
	if( bReturn == FAIL )
		return FAIL;

	bReturn = g_quad::Get().Initialize();
	if( bReturn == FAIL )
		return FAIL;

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginScene( void )
	\brief		Begin the scene drawing sequence
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
bool RendererEngine::BeginScene( D3DCOLOR i_clearColour )
{
	if( !bBeginScene )
	{
		const D3DRECT* subRectanglesToClear = NULL;
		DWORD subRectangleCount = 0;
		DWORD clearTheRenderTarget = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
		float clearDepth = 1.0f;
		DWORD noStencilBuffer = 0;
		HRESULT result = g_mainRenderer::Get().GetDirect3dDevice()->Clear( subRectangleCount, subRectanglesToClear,
			clearTheRenderTarget, i_clearColour, clearDepth, noStencilBuffer );
		assert( SUCCEEDED( result ) );

		result = g_mainRenderer::Get().GetDirect3dDevice()->BeginScene();

		if( !SUCCEEDED(result) )
			return FAIL;

		bBeginScene = true;
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void Draw3D( const std::vector<S_ENTITY_TO_DRAW> &i_entitiesToDraw, const std::vector<S_LINE_TO_DRAW> &i_linesToDraw,
				const std::vector<S_SPHERE_TO_DRAW> &i_sphereToDraw,
				const D3DXMATRIX &i_cameraWorldToViewTransform, const D3DXMATRIX &i_cameraViewToProjectedTransform,
				const D3DXVECTOR3 &i_cameraPosition, const float &i_cameraFarView,
				const D3DXMATRIX &i_directionalLightWorldToViewTransform, const D3DXMATRIX &i_directionalLightViewToProjectedTransform,
				const D3DXVECTOR3 &i_directionalLightDirection, const D3DCOLOR &i_directionalLightColour,
				const float &i_directionalLightIntensity, const float &i_directionalLightFarView,
				const D3DCOLOR &i_pointLightColour, const D3DCOLOR &i_pointLightAmbient, const D3DXVECTOR3 &i_pointLightPosition,
				const float &i_pointLightIntensity, const float &i_pointLightRadius )
	\brief		Draw the 3D objects of the scene
	\param		i_entitiesToDraw the entities to be drawn
	\param		i_linesToDraw the lines to be drawn
	\param		i_sphereToDraw the sphere to be drawn
	\param		i_cameraWorldToViewTransform the camera world to view transformation matrix
	\param		i_cameraViewToProjectedTransform the camera view to projected transformation matrix
	\param		i_cameraPosition the position of camera
	\param		i_cameraFarView the far view distance of camera
	\param		i_directionalLightWorldToViewTransform the camera world to view transformation matrix
	\param		i_directionalLightViewToProjectedTransform the camera view to projected transformation matrix
	\param		i_directionalLightDirection the direction of directional light
	\param		i_directionalLightColour the colour of directional light
	\param		i_directionalLightIntensity the intensity of directional light
	\param		i_directionalLightFarView the far view of directional light
	\param		i_pointLightColour the colour of point light
	\param		i_pointLightAmbient the ambient colour of point light
	\param		i_pointLightPosition the position of point light
	\param		i_pointLightIntensity the intensity of point light
	\param		i_pointLightRadius the radius of point light
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Draw3D( const std::vector<S_ENTITY_TO_DRAW> &i_entitiesToDraw, const std::vector<S_LINE_TO_DRAW> &i_linesToDraw,
	const std::vector<S_SPHERE_TO_DRAW> &i_sphereToDraw,
	const D3DXMATRIX &i_cameraWorldToViewTransform, const D3DXMATRIX &i_cameraViewToProjectedTransform,
	const D3DXVECTOR3 &i_cameraPosition, const float &i_cameraFarView,
	const D3DXMATRIX &i_directionalLightWorldToViewTransform, const D3DXMATRIX &i_directionalLightViewToProjectedTransform,
	const D3DXVECTOR3 &i_directionalLightDirection, const D3DCOLOR &i_directionalLightColour,
	const float &i_directionalLightIntensity, const float &i_directionalLightFarView,
	const D3DCOLOR &i_pointLightColour, const D3DCOLOR &i_pointLightAmbient, const D3DXVECTOR3 &i_pointLightPosition,
	const float &i_pointLightIntensity, const float &i_pointLightRadius )
{
	Utilities::StringHash emptyHash = Utilities::StringHash( "" );

	std::map<Utilities::StringHash, S_ENTITY>::const_iterator entityIterator;
	std::map<Utilities::StringHash, S_EFFECT>::const_iterator effectIterator;
	std::map<Utilities::StringHash, S_FRAGMENT_SHADER>::const_iterator fragmentShaderIterator;
	std::map<Utilities::StringHash, S_MATERIAL>::const_iterator materialIterator;
	std::map<Utilities::StringHash, S_VERTEX_SHADER>::const_iterator vertexShaderIterator;
	std::map<Utilities::StringHash, S_MESH>::const_iterator meshIterator;
	std::map<Utilities::StringHash, IDirect3DTexture9*>::const_iterator textureIterator;
	std::map<Utilities::StringHash, IDirect3DTexture9*>::const_iterator normalMapTextureIterator;

	Utilities::StringHash prevEntityFile = emptyHash;
	Utilities::StringHash prevMaterialFile = emptyHash;
	Utilities::StringHash prevEffectFile = emptyHash;
	Utilities::StringHash prevMeshFile = emptyHash;
	Utilities::StringHash prevFragmentShaderFile = emptyHash;
	Utilities::StringHash prevVertexShaderFile = emptyHash;
	Utilities::StringHash prevTextureFile = emptyHash;

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Render 3D objects" );
#endif	// #ifdef _DEBUG

	// Draw 3D model
	if( i_entitiesToDraw.size() > 0 )
	{
		if( !g_mainRenderer::Get().SetNormalMapVertexDeclaration() )
			return;
	}

	for( std::vector<S_ENTITY_TO_DRAW>::const_iterator iter = i_entitiesToDraw.begin(); iter != i_entitiesToDraw.end(); iter++ )
	{
		// Get and set the correct data for this entity
		if( prevEntityFile != iter->entity )
		{
			prevEntityFile = iter->entity;
			if( !SET_REQUIRED_ITERATOR(entityIterator, iter->entity, entityDatabase) )
				continue;
		}

		// Get and set the correct data for this entity's material
		if( prevMaterialFile != entityIterator->second.materialFile )
		{
			if( !SET_REQUIRED_ITERATOR(materialIterator, entityIterator->second.materialFile, materialDatabase) )
				continue;
		}

		// Get and set the correct data for this entity's effect
		if( prevEffectFile != materialIterator->second.effectFile )
		{
			prevEffectFile = materialIterator->second.effectFile;
			if( !SET_REQUIRED_ITERATOR(effectIterator, materialIterator->second.effectFile, effectDatabase) )
				continue;
		}

		// Get and set the correct data for this entity's mesh
		if( prevMeshFile != entityIterator->second.meshFile )
		{
			prevMeshFile = entityIterator->second.meshFile;
			if( !SET_REQUIRED_ITERATOR(meshIterator, entityIterator->second.meshFile, meshDatabase) )
				continue;
		}

		// Get and set the correct data for this entity's fragment shader
		if( prevFragmentShaderFile != effectIterator->second.fragmentShaderFile )
		{
			if( !SET_REQUIRED_ITERATOR(fragmentShaderIterator, effectIterator->second.fragmentShaderFile, fragmentShaderDatabase) )
				continue;
		}

		// Get and set the correct data for this entity's vertex shader
		if( prevVertexShaderFile != effectIterator->second.vertexShaderFile )
		{
			if( !SET_REQUIRED_ITERATOR(vertexShaderIterator, effectIterator->second.vertexShaderFile, vertexShaderDatabase) )
				continue;
		}

		// Get and set the correct data for this entity's texture
		if( (prevTextureFile != materialIterator->second.textureFile) && (materialIterator->second.textureFile != emptyHash) )
		{
			if( !SET_REQUIRED_ITERATOR(textureIterator, materialIterator->second.textureFile, textureDatabase) )
				continue;
		}

		// Get and set the correct data for this entity's texture
		if( effectIterator->second.u8TextureMode & NORMAL_MAP )
		{
			if( !SET_REQUIRED_ITERATOR(normalMapTextureIterator, materialIterator->second.normalMapTexture, normalMapTextureDatabase) )
				continue;
		}

		if( prevVertexShaderFile != effectIterator->second.vertexShaderFile )
		{
			prevVertexShaderFile = effectIterator->second.vertexShaderFile;

			Setter::SetVertexShader( vertexShaderIterator->second.compiledShader, vertexShaderIterator->second.constantTable,
				TRUE, i_cameraWorldToViewTransform, i_cameraViewToProjectedTransform,
				i_directionalLightWorldToViewTransform, i_directionalLightViewToProjectedTransform );
		}

		if( prevFragmentShaderFile != effectIterator->second.fragmentShaderFile )
		{
		#ifdef _DEBUG
			D3DPERF_BeginEvent( 0 , L"Fragment shader" );
		#endif	// #ifdef _DEBUG
			prevFragmentShaderFile = effectIterator->second.fragmentShaderFile;

			Setter::SetFragmentShader( fragmentShaderIterator->second.compiledShader );
			Setter::SetDirectionalLight( fragmentShaderIterator->second.constantTable,
				i_directionalLightDirection, i_directionalLightColour, i_directionalLightIntensity, i_directionalLightFarView );
			Setter::SetPointLight( fragmentShaderIterator->second.constantTable, i_pointLightPosition, i_pointLightColour, i_pointLightAmbient,
				i_pointLightIntensity, i_pointLightRadius );
			Setter::SetCamera( fragmentShaderIterator->second.constantTable, i_cameraPosition, i_cameraFarView );
		#ifdef _DEBUG
			D3DPERF_EndEvent();
		#endif	// #ifdef _DEBUG
		}

		if( prevMaterialFile != entityIterator->second.materialFile )
		{
		#ifdef _DEBUG
			D3DPERF_BeginEvent( 0, L"Material" );
		#endif// #ifdef _DEBUG
			prevMaterialFile = entityIterator->second.materialFile;

			if( (prevTextureFile != materialIterator->second.textureFile) && (materialIterator->second.textureFile != emptyHash) )
			{
				prevTextureFile = materialIterator->second.textureFile;
				Setter::SetTexture( textureIterator->second, fragmentShaderIterator->second.constantTable );
			}
			if( effectIterator->second.u8TextureMode & NORMAL_MAP )
				Setter::SetNormalMap( normalMapTextureIterator->second, fragmentShaderIterator->second.constantTable );
		#ifdef _DEBUG
			D3DPERF_EndEvent();
		#endif	// #ifdef _DEBUG
		}

	#ifdef _DEBUG
		D3DPERF_BeginEvent( 0, L"Entity" );
	#endif// #ifdef _DEBUG
		Setter::SetModelToWorldTransformation( vertexShaderIterator->second.constantTable,
			iter->position, iter->scale, iter->orientation );

		Setter::SetVertexBuffer( meshIterator->second.vertexBuffer, sizeof(Utilities::S_NORMAL_MAP_VERTEX_DATA) );
		Setter::SetIndexBuffer( meshIterator->second.indexBuffer );

		// Render objects from the current streams
		{
			// We are using triangles as the "primitive" type,
			// and we have defined the vertex buffer as a triangle list
			// (meaning that every triangle is defined by three vertices)
			D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
			// It's possible to start rendering primitives in the middle of the stream
			unsigned int indexOfFirstVertexToRender = 0;
			// We are currently only rendering a single triangle
			unsigned int primitiveCountToRender = meshIterator->second.u32PrimitiveCount;
			unsigned int startIndex = 0;
			//HRESULT result = direct3dDevice->DrawPrimitive( primitiveType, indexOfFirstVertexToRender, primitiveCountToRender );
			HRESULT result = g_mainRenderer::Get().GetDirect3dDevice()->DrawIndexedPrimitive( primitiveType, indexOfFirstVertexToRender,
				0, meshIterator->second.u32VertexCount, startIndex, primitiveCountToRender );
			assert( SUCCEEDED(result) );
		}
	#ifdef _DEBUG
		D3DPERF_EndEvent();
	#endif	// #ifdef _DEBUG
	}

	// Set the basic vertex declaration
	if( (i_linesToDraw.size() > 0) || (i_sphereToDraw.size() > 0) )
	{
		if( !g_mainRenderer::Get().SetBasicVertexDeclaration() )
			return;
	}

	// Draw Line
	if( i_linesToDraw.size() > 0 )
	{
		bool bErrorFound = false;

		if( !SET_REQUIRED_ITERATOR(fragmentShaderIterator, g_line::Get().m_hashedFragmentShader, fragmentShaderDatabase) )
			bErrorFound = true;

		if( !SET_REQUIRED_ITERATOR(vertexShaderIterator, g_line::Get().m_hashedVertexShader, vertexShaderDatabase) )
			bErrorFound = true;

		if( !bErrorFound )
		{
			Setter::SetVertexShader( vertexShaderIterator->second.compiledShader, vertexShaderIterator->second.constantTable,
			TRUE, i_cameraWorldToViewTransform, i_cameraViewToProjectedTransform,
			i_directionalLightWorldToViewTransform, i_directionalLightViewToProjectedTransform );

		#ifdef _DEBUG
			D3DPERF_BeginEvent( 0 , L"Fragment shader" );
		#endif	// #ifdef _DEBUG
			Setter::SetFragmentShader( fragmentShaderIterator->second.compiledShader );
		#ifdef _DEBUG
			D3DPERF_EndEvent();
		#endif	// #ifdef _DEBUG

			for( std::vector<S_LINE_TO_DRAW>::const_iterator iter = i_linesToDraw.begin(); iter != i_linesToDraw.end(); ++iter )
			{
			#ifdef _DEBUG
				D3DPERF_BeginEvent( 0, L"Entity" );
			#endif// #ifdef _DEBUG
				g_line::Get().LoadVertexBuffer( (*iter) );
				Setter::SetModelToWorldTransformation( vertexShaderIterator->second.constantTable );

				g_line::Get().Draw();
			#ifdef _DEBUG
				D3DPERF_EndEvent();
			#endif	// #ifdef _DEBUG
			}
		}
	}

	// Draw sphere
	if( i_sphereToDraw.size() > 0 )
	{
		bool bErrorFound = false;

		g_mainRenderer::Get().GetDirect3dDevice()->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

		if( !SET_REQUIRED_ITERATOR(fragmentShaderIterator, g_line::Get().m_hashedFragmentShader, fragmentShaderDatabase) )
			bErrorFound = true;

		if( !SET_REQUIRED_ITERATOR(vertexShaderIterator, g_line::Get().m_hashedVertexShader, vertexShaderDatabase) )
			bErrorFound = true;

		if( !bErrorFound )
		{
			Setter::SetVertexShader( vertexShaderIterator->second.compiledShader, vertexShaderIterator->second.constantTable,
				TRUE, i_cameraWorldToViewTransform, i_cameraViewToProjectedTransform,
				i_directionalLightWorldToViewTransform, i_directionalLightViewToProjectedTransform );

#ifdef _DEBUG
			D3DPERF_BeginEvent( 0 , L"Fragment shader" );
#endif	// #ifdef _DEBUG
			Setter::SetFragmentShader( fragmentShaderIterator->second.compiledShader );
#ifdef _DEBUG
			D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG

			for( std::vector<S_SPHERE_TO_DRAW>::const_iterator iter = i_sphereToDraw.begin(); iter != i_sphereToDraw.end(); ++iter )
			{
#ifdef _DEBUG
				D3DPERF_BeginEvent( 0, L"Entity" );
#endif// #ifdef _DEBUG
				//g_sphere::Get().LoadVertexBuffer( (*iter) );
				float scale = iter->radius / Utilities::DEFAULT_DEBUG_SPHERE_RADIUS;
				Setter::SetModelToWorldTransformation( vertexShaderIterator->second.constantTable,
					iter->centre, D3DXVECTOR3(scale, scale, scale), 0.0f );

				g_sphere::Get().Draw();
#ifdef _DEBUG
				D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
			}
		}
		g_mainRenderer::Get().GetDirect3dDevice()->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}

#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			void Draw2D( const std::vector<S_QUAD_TO_DRAW> &i_quadsToDraw, const std::vector<S_TEXT_TO_DRAW> &i_textToDraw )
	\brief		Draw the 2D objects of the scene
	\param		i_quadsToDraw the quads to be drawn
	\param		i_textToDraw the text to be drawn
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::Draw2D( const std::vector<S_QUAD_TO_DRAW> &i_quadsToDraw, const std::vector<S_TEXT_TO_DRAW> &i_textToDraw )
{
	Utilities::StringHash emptyHash = Utilities::StringHash( "" );
	std::map<Utilities::StringHash, S_ENTITY>::const_iterator entityIterator;
	std::map<Utilities::StringHash, S_EFFECT>::const_iterator effectIterator;
	std::map<Utilities::StringHash, S_FRAGMENT_SHADER>::const_iterator fragmentShaderIterator;
	std::map<Utilities::StringHash, S_MATERIAL>::const_iterator materialIterator;
	std::map<Utilities::StringHash, S_VERTEX_SHADER>::const_iterator vertexShaderIterator;
	std::map<Utilities::StringHash, S_MESH>::const_iterator meshIterator;
	std::map<Utilities::StringHash, IDirect3DTexture9*>::const_iterator textureIterator;

#ifdef _DEBUG
	D3DPERF_BeginEvent( 0 , L"Render 2D objects" );
#endif	// #ifdef _DEBUG

	// Set the basic vertex declaration
	if( i_quadsToDraw.size() > 0 )
	{
		if( !g_mainRenderer::Get().SetBasicVertexDeclaration() )
			return;
	}

	// Draw quad
	if( i_quadsToDraw.size() > 0 )
	{
		bool bErrorFound = false;

		if( !SET_REQUIRED_ITERATOR(vertexShaderIterator, g_quad::Get().m_hashedVertexShader, vertexShaderDatabase) )
			bErrorFound = true;

		if( !SET_REQUIRED_ITERATOR(fragmentShaderIterator, g_quad::Get().m_hashedFragmentShader, fragmentShaderDatabase) )
			bErrorFound = true;

		if( !bErrorFound )
		{
			Setter::SetVertexShader( vertexShaderIterator->second.compiledShader );

		#ifdef _DEBUG
			D3DPERF_BeginEvent( 0 , L"Fragment shader" );
		#endif	// #ifdef _DEBUG
			Setter::SetFragmentShader( fragmentShaderIterator->second.compiledShader );
		#ifdef _DEBUG
			D3DPERF_EndEvent();
		#endif	// #ifdef _DEBUG

			for( std::vector<S_QUAD_TO_DRAW>::const_iterator iter = i_quadsToDraw.begin(); iter != i_quadsToDraw.end(); iter++ )
			{
				if( iter->texture != emptyHash )
				{
					Setter::SetTextureUsage( true, fragmentShaderIterator->second.constantTable );

					if( !SET_REQUIRED_ITERATOR(textureIterator, iter->texture, textureDatabase) )
						continue;

				#ifdef _DEBUG
					D3DPERF_BeginEvent( 0, L"Material" );
				#endif// #ifdef _DEBUG
					Setter::SetTexture( textureIterator->second, fragmentShaderIterator->second.constantTable );
				#ifdef _DEBUG
					D3DPERF_EndEvent();
				#endif	// #ifdef _DEBUG
				}
				else
					Setter::SetTextureUsage( false, fragmentShaderIterator->second.constantTable );

				Setter::SetSize( vertexShaderIterator->second.constantTable, iter->size );
				Setter::SetPosition( vertexShaderIterator->second.constantTable, iter->position );

				g_quad::Get().LoadVertexBuffer( iter->colour );
				g_quad::Get().Draw();
			}
		}
	}

	// Text


	if( i_textToDraw.size() > 0 )
	{
		g_text::Get().BeginText();
		for( UINT8 i = 0; i < i_textToDraw.size(); ++i )
		{
			g_text::Get().Draw( i_textToDraw[i] );
		}
		g_text::Get().EndText();
	}

#ifdef _DEBUG
	D3DPERF_EndEvent();
#endif	// #ifdef _DEBUG
}

/**
 ****************************************************************************************************
	\fn			bool EndScene( void )
	\brief		End of scene drawing
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::EndScene( void )
{
	if( bBeginScene )
	{
		HRESULT result = g_mainRenderer::Get().GetDirect3dDevice()->EndScene();

		if( !SUCCEEDED(result) )
			return FAIL;

		bBeginScene = false;

		// Everything has been drawn to the "back buffer", which is just an image in memory.
		// In order to display it, the contents of the back buffer must be "presented"
		// (to the front buffer)
		{
			const RECT* noSourceRectangle = NULL;
			const RECT* noDestinationRectangle = NULL;
			HWND useDefaultWindow = NULL;
			const RGNDATA* noDirtyRegion = NULL;
			HRESULT result = g_mainRenderer::Get().GetDirect3dDevice()->Present( noSourceRectangle, noDestinationRectangle,
				useDefaultWindow, noDirtyRegion );
			assert( SUCCEEDED( result ) );
		}
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shut down the renderer engine
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void RendererEngine::ShutDown( void )
{
	std::map<Utilities::StringHash, S_MESH>::iterator meshIterator = meshDatabase->begin();
	std::map<Utilities::StringHash, S_FRAGMENT_SHADER>::iterator fragmentShaderIterator = fragmentShaderDatabase->begin();
	std::map<Utilities::StringHash, S_VERTEX_SHADER>::iterator vertexShaderIterator = vertexShaderDatabase->begin();
	std::map<Utilities::StringHash, S_DEPTH_SHADER>::iterator depthShaderIterator = depthShaderDatabase->begin();
	std::map<Utilities::StringHash, IDirect3DTexture9*>::iterator textureIterator = textureDatabase->begin();
	std::map<Utilities::StringHash, IDirect3DTexture9*>::iterator normalMapTextureIterator = normalMapTextureDatabase->begin();
	std::map<Utilities::StringHash, IDirect3DCubeTexture9*>::iterator environmentMapIterator = environmentMapDatabase->begin();

	for( ; meshIterator != meshDatabase->end(); ++meshIterator )
	{
		meshIterator->second.indexBuffer->Release();
		meshIterator->second.vertexBuffer->Release();
	}

	for( ; fragmentShaderIterator != fragmentShaderDatabase->end(); ++fragmentShaderIterator )
	{
		fragmentShaderIterator->second.compiledShader->Release();
		fragmentShaderIterator->second.constantTable->Release();
	}

	for( ; vertexShaderIterator != vertexShaderDatabase->end(); ++vertexShaderIterator )
	{
		vertexShaderIterator->second.compiledShader->Release();
		vertexShaderIterator->second.constantTable->Release();
	}

	for( ; depthShaderIterator != depthShaderDatabase->end(); ++depthShaderIterator )
	{
		depthShaderIterator->second.compiledShader->Release();
		depthShaderIterator->second.constantTable->Release();
	}

	for( ; textureIterator != textureDatabase->end(); ++textureIterator )
	{
		textureIterator->second->Release();
	}

	for( ; normalMapTextureIterator != normalMapTextureDatabase->end(); ++normalMapTextureIterator )
	{
		normalMapTextureIterator->second->Release();
	}

	for( ; environmentMapIterator != environmentMapDatabase->end();++ environmentMapIterator )
	{
		environmentMapIterator->second->Release();
	}

	delete effectDatabase;
	delete entityDatabase;
	delete fragmentShaderDatabase;
	delete materialDatabase;
	delete vertexShaderDatabase;
	delete depthShaderDatabase;
	delete textureDatabase;
	delete normalMapTextureDatabase;
	delete meshDatabase;
	delete environmentMapDatabase;

	g_text::Get().ShutDown();
 	g_line::Get().ShutDown();
	g_sphere::Get().ShutDown();
	g_quad::Get().ShutDown();
	g_mainRenderer::Get().ShutDown();
	g_mainWindow::Get().ShutDown();

	g_text::Release();
	g_line::Release();
	g_sphere::Release();
	g_quad::Release();
	g_mainRenderer::Release();
	g_mainWindow::Release();
}

/**
 ****************************************************************************************************
	\fn			bool CreateEffect( const Utilities::StringHash &i_name, const Utilities::StringHash &i_vertexShaderFile,
				const Utilities::StringHash &i_fragmentShaderFile, const Utilities::E_ALPHA_MODE &i_alphaMode, const UINT8 &i_textureMode )
	\brief		Create the effect data to be added in database
	\param		i_name name of the effect file
	\param		i_vertexShaderFile vertex shader used in this effect
	\param		i_fragmentShaderFile fragment shader used in this effect
	\param		i_alphaMode the alpha mode in this effect
	\param		i_textureMode the texture mode in this effect
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::CreateEffect( const Utilities::StringHash &i_name, const Utilities::StringHash &i_vertexShaderFile,
	const Utilities::StringHash &i_fragmentShaderFile, const Utilities::E_ALPHA_MODE &i_alphaMode, const UINT8 &i_textureMode )
{
	std::map<Utilities::StringHash, S_EFFECT>::iterator iter;

	iter = effectDatabase->find( i_name );
	if( iter == effectDatabase->end() )
	{
		S_EFFECT newData = { i_vertexShaderFile, i_fragmentShaderFile, i_alphaMode, i_textureMode };
		std::pair<Utilities::StringHash, S_EFFECT> insertingPair( i_name, newData );
		effectDatabase->insert( insertingPair );
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool CreateEntity( const Utilities::StringHash &i_name, const Utilities::StringHash &i_meshFile,
				const Utilities::StringHash &i_materialFile )
	\brief		Create the entity data to be added in database
	\param		i_name name of the entity file
	\param		i_meshFile mesh file used in this entity
	\param		i_materialFile material file used in this entity
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::CreateEntity( const Utilities::StringHash &i_name, const Utilities::StringHash &i_meshFile,
	const Utilities::StringHash &i_materialFile )
{
	std::map<Utilities::StringHash, S_ENTITY>::iterator iter;

	iter = entityDatabase->find( i_name );
	if( iter == entityDatabase->end() )
	{
		S_ENTITY newData = { i_meshFile, i_materialFile };
		std::pair<Utilities::StringHash, S_ENTITY> insertingPair( i_name, newData );
		entityDatabase->insert( insertingPair );
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool CreateMaterial( const Utilities::StringHash &i_name, const Utilities::StringHash &i_effectFile,
				const Utilities::StringHash &i_textureFile, const Utilities::StringHash &i_normalMapTexture,
				const float &i_transparency, const float &i_shininess, const float &i_reflectance )
	\brief		Create the material data to be added in database
	\param		i_name name of the material
	\param		i_effectFile effect file used in this material
	\param		i_textureFile texture file used in this material
	\param		i_normalMapTextureFile normal map texture file used in this material
	\param		i_transparency transparency of this material
	\param		i_shininess the shininess of this material
	\param		i_reflectance the reflectance of this material
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::CreateMaterial( const Utilities::StringHash &i_name, const Utilities::StringHash &i_effectFile,
	const Utilities::StringHash &i_textureFile, const Utilities::StringHash &i_normalMapTexture,
	const float &i_transparency, const float &i_shininess, const float &i_reflectance )
{
	std::map<Utilities::StringHash, S_MATERIAL>::iterator iter;

	iter = materialDatabase->find( i_name );
	if( iter == materialDatabase->end() )
	{
		S_MATERIAL newData = { i_effectFile, i_textureFile, i_normalMapTexture, i_transparency, i_shininess, i_reflectance };
		std::pair<Utilities::StringHash, S_MATERIAL> insertingPair( i_name, newData );
		materialDatabase->insert( insertingPair );
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool CreateFragmentShader( const char *i_fileName )
	\brief		Create the fragment shader data to be added in database
	\param		i_fileName name of the fragment shader file
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::CreateFragmentShader( const char *i_fileName )
{
	Utilities::StringHash hashedFileName = Utilities::StringHash( i_fileName );
	std::map<Utilities::StringHash, S_FRAGMENT_SHADER>::iterator iter;

	iter = fragmentShaderDatabase->find( hashedFileName );
	if( iter == fragmentShaderDatabase->end() )
	{
		S_FRAGMENT_SHADER newData = { NULL, NULL };
		Loader::LoadFragmentShader( i_fileName, newData.compiledShader, newData.constantTable );
		std::pair<Utilities::StringHash, S_FRAGMENT_SHADER> insertingPair( hashedFileName, newData );
		fragmentShaderDatabase->insert( insertingPair );
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool CreateVertexShader( const char *i_fileName )
	\brief		Create the vertex shader data to be added in database
	\param		i_fileName name of the vertex shader file
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::CreateVertexShader( const char *i_fileName )
{
	Utilities::StringHash hashedFileName = Utilities::StringHash( i_fileName );
	std::map<Utilities::StringHash, S_VERTEX_SHADER>::iterator iter;

	iter = vertexShaderDatabase->find( hashedFileName );
	if( iter == vertexShaderDatabase->end() )
	{
		S_VERTEX_SHADER newData = { NULL, NULL };
		Loader::LoadVertexShader(i_fileName, newData.compiledShader, newData.constantTable );
		std::pair<Utilities::StringHash, S_VERTEX_SHADER> insertingPair( hashedFileName, newData );
		vertexShaderDatabase->insert( insertingPair );
	}

	return SUCCESS;
}

/**
****************************************************************************************************
	\fn			bool CreateDepthShader( const char *i_fileName )
	\brief		Create the depth shader data to be added in database
	\param		i_fileName name of the shader file
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::CreateDepthShader( const char *i_fileName )
{
	Utilities::StringHash hashedFileName = Utilities::StringHash( i_fileName );
	std::map<Utilities::StringHash, S_DEPTH_SHADER>::iterator iter;

	iter = depthShaderDatabase->find( hashedFileName );
	if( iter == depthShaderDatabase->end() )
	{
		S_DEPTH_SHADER newData = { NULL, NULL };
		Loader::LoadDepthShader( i_fileName, newData.compiledShader, newData.constantTable );
		std::pair<Utilities::StringHash, S_DEPTH_SHADER> insertingPair( hashedFileName, newData );
		depthShaderDatabase->insert( insertingPair );
	}

		return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool CreateNormalMap( const char *i_fileName )
	\brief		Create the normal map texture data to be added in database
	\param		i_fileName name of the normal map texture file
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::CreateNormalMap( const char *i_fileName )
{
	Utilities::StringHash hashedFileName = Utilities::StringHash( i_fileName );
	std::map<Utilities::StringHash, IDirect3DTexture9*>::iterator iter;

	iter = normalMapTextureDatabase->find( hashedFileName );
	if( iter == normalMapTextureDatabase->end() )
	{
		IDirect3DTexture9* newData = NULL;
		Loader::LoadNormalMap( i_fileName, newData );
		std::pair<Utilities::StringHash, IDirect3DTexture9*> insertingPair( hashedFileName, newData );
		normalMapTextureDatabase->insert( insertingPair );
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool CreateTexture( const char *i_fileName )
	\brief		Create the texture data to be added in database
	\param		i_fileName name of the texture file
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::CreateTexture( const char *i_fileName )
{
	Utilities::StringHash hashedFileName = Utilities::StringHash( i_fileName );
	std::map<Utilities::StringHash, IDirect3DTexture9*>::iterator iter;

	iter = textureDatabase->find( hashedFileName );
	if( iter == textureDatabase->end() )
	{
		IDirect3DTexture9* newData = NULL;
		Loader::LoadTexture( i_fileName, newData );
		std::pair<Utilities::StringHash, IDirect3DTexture9*> insertingPair( hashedFileName, newData );
		textureDatabase->insert( insertingPair );
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool CreateMesh( const char* i_fileName )
	\brief		Create the mesh data to be added in database
	\param		i_fileName name of the mesh file
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool RendererEngine::CreateMesh( const char *i_fileName )
{
	Utilities::StringHash hashedFileName = Utilities::StringHash( i_fileName );
	std::map<Utilities::StringHash, S_MESH>::iterator iter;

	iter = meshDatabase->find( hashedFileName );
	if( iter == meshDatabase->end() )
	{
		S_MESH newData = { 0, 0, NULL, NULL };
		Loader::LoadMeshData( i_fileName, newData.u32VertexCount, newData.u32PrimitiveCount,
			newData.indexBuffer, newData.vertexBuffer );
		std::pair<Utilities::StringHash, S_MESH> insertingPair( hashedFileName, newData );
		meshDatabase->insert( insertingPair );
	}
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			HRESULT GetViewPort( D3DVIEWPORT9 &o_viewport )
	\brief		Get the viewport of current renderer
	\param		o_viewport the output viewport
	\return		HRESULT
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
HRESULT RendererEngine::GetViewPort( D3DVIEWPORT9 &o_viewport )
{
	return g_mainRenderer::Get().GetViewPort( o_viewport );
}

/**
 ****************************************************************************************************
	\fn			HRESULT GetTransform( const D3DTRANSFORMSTATETYPE &i_type, D3DXMATRIX &o_matrix )
	\brief		Get the transform matrix of renderer
	\param		i_type type of transformation
	\param		o_matrix the output matrix
	\return		HRESULT
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
HRESULT RendererEngine::GetTransform( const D3DTRANSFORMSTATETYPE &i_type, D3DXMATRIX &o_matrix )
{
	return g_mainRenderer::Get().GetTransform( i_type, o_matrix );
}

void RendererEngine::SortEntitiesToDraw( std::vector<S_ENTITY_TO_DRAW> &io_entitiesToDraw )
{
	UINT8 startSameRenderStateIdx = 0;
	UINT8 endSameRenderStateIdx = 0;
	UINT8 startSameFragmentShaderIdx = 0;
	UINT8 endSameFragmentShaderIdx = 0;
	UINT8 startSameVertexShaderIdx = 0;
	UINT8 endSameVertexShaderIdx = 0;
	UINT8 startSameMaterialIdx = 0;
	UINT8 endSameMaterialIdx = 0;
	UINT8 assetSize = io_entitiesToDraw.size();

	//QuickSortByRenderState( 0, assetSize-1 );
}

void RendererEngine::QuickSortByRenderState( std::vector<S_ENTITY_TO_DRAW> &io_entitiesToDraw, const INT8 &i_left, const INT8 &i_right )
{
	//if( i_left < i_right )
	//{
	//	INT8 i = i_left;
	//	INT8 j = i_right + 1;

	//	UINT32 pivot = io_entitiesToDraw->at(i)->m_renderState;

	//	do
	//	{
	//		do
	//		{
	//			++i;
	//		} while( (m_assetDatabase->at(i)->m_renderState < pivot) && (i < i_right) );

	//		do
	//		{
	//			j--;
	//		} while( (j > 0) && (m_assetDatabase->at(j)->m_renderState > pivot)  );

	//		if( i < j )
	//		{
	//			GameEngine::Pointer::SmartPtr<cAsset> temp = m_assetDatabase->at(i);
	//			m_assetDatabase->at(i) = m_assetDatabase->at(j);
	//			m_assetDatabase->at(j) = temp;
	//		}

	//	} while( i < j );

	//	GameEngine::Pointer::SmartPtr<cAsset> temp = m_assetDatabase->at(i_left);
	//	m_assetDatabase->at(i_left) = m_assetDatabase->at(j);
	//	m_assetDatabase->at(j) = temp;

	//	QuickSortByRenderState( i_left, j-1 );
	//	QuickSortByRenderState( j+1, i_right );
	//}
}
