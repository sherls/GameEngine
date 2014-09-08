/**
 ****************************************************************************************************
 * \file		Renderer.cpp
 * \brief		Renderer engine implementation and control
 ****************************************************************************************************
*/

#include <math.h>
#include <vector>

// Utilities header
#include <Parser/ParserHelper.h>
#include <MemoryPool/MemoryPool.h>
#include <Parser/EntityParser/EntityParser.h>
#include <Parser/EffectParser/EffectParser.h>
#include <Parser/MaterialParser/MaterialParser.h>

// Renderer Engine
#include <RendererEngine.h>

#include "Renderer.h"

#include "../World/World.h"
#include "../Camera/Camera.h"
#include "../World/Entity.h"
#include "../GameEngineDefault.h"
#include "../DebugMenu/DebugMenu.h"
#include "../Math/Vector3/Vector3.h"
#include "../Utilities/GameEngineTypes.h"
#include "../Light/PointLight/PointLight.h"
#include "../Light/DirectionalLight/DirectionalLight.h"

/****************************************************************************************************
			Class / variable declaration
****************************************************************************************************/
namespace GameEngine
{
	namespace Renderer
	{
		typedef struct _s_slider_
		{
			UINT32 u32CurrValue;
			UINT32 u32MinValue;
			UINT32 u32MaxValue;
			D3DCOLOR backgroundColour;
			D3DCOLOR foregroundColour;
		} S_SLIDER;

		class Sprite
		{
			// Default constructor
			Sprite( void ) {}

			// Make it non-copyable
			Sprite( Sprite const &i_other );
			Sprite &operator=( const Sprite &i_other );

			// Make it incomparable
			bool &operator==( const Sprite &i_rhs ) const;

		public:
			static Utilities::MemoryPool *m_spritePool;

			Utilities::Pointer::SmartPtr<Entity> m_entity;
			Utilities::StringHash m_hashedTextureFile;
			UINT32 m_u32QuadIndex;
			D3DCOLOR m_colour;

			// Standard constructor
			Sprite( Utilities::Pointer::SmartPtr<Entity> &i_entity, const D3DCOLOR &i_colour, const char *i_textureFile );

			~Sprite( void );

			void *operator new( size_t i_size );
			void operator delete( void *i_ptr );
		};

		Utilities::S_SIZE g_windowSize;
		static std::vector<Utilities::Pointer::SmartPtr<Sprite>> *spriteDatabase;
		static std::vector<RendererEngine::S_QUAD_TO_DRAW> *quadToDraw;
		static std::vector<RendererEngine::S_TEXT_TO_DRAW> *textToDraw;
		void RemoveDeadSprites( void );

		class Mesh
		{
			std::string _entityInput;

			// Default constructor
			Mesh( void ) {}

			// Make it non-copyable
			Mesh( Mesh const &i_other );
			Mesh &operator= ( const Mesh &i_other );

			// Make it incomparable
			bool &operator ==( const Mesh &i_rhs ) const;

			void Load( void );

		public:
			static Utilities::MemoryPool *m_meshPool;

			UINT32 m_u32EntityIndex;
 			Utilities::StringHash m_meshName;
			Utilities::Pointer::SmartPtr<Entity> m_entity;

			Mesh( Utilities::Pointer::SmartPtr<Entity> &i_entity, const std::string &i_fileName );
			~Mesh( void );

			void *operator new( size_t i_size );
			void operator delete( void *i_ptr );
		};

		static bool bReadyToRender = false;
		static std::vector<Utilities::Pointer::SmartPtr<Mesh>> *meshDatabase;
		static std::vector<RendererEngine::S_ENTITY_TO_DRAW> *entityDatabase;
		static std::vector<RendererEngine::S_LINE_TO_DRAW> *linesToDraw;
		static std::vector<RendererEngine::S_SPHERE_TO_DRAW> *sphereToDraw;

		void RemoveDeadAssets( void );
	}	// namespace Renderer
}	// namespace GameEngine

Utilities::MemoryPool *GameEngine::Renderer::Sprite::m_spritePool = NULL;
Utilities::MemoryPool *GameEngine::Renderer::Mesh::m_meshPool = NULL;

/****************************************************************************************************
			Class / variable declaration
****************************************************************************************************/

/****************************************************************************************************
			Helper functions
****************************************************************************************************/

/****************************************************************************************************
			Global functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( const char* i_GameName, UINT16 i_u16Width, UINT16 i_u16Height )
	\brief		Initialize renderer engine
	\param		*i_GameName title of the game
	\param		i_u16Width screen width
	\param		i_u16Height screen height
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::Renderer::Initialize( const char* i_GameName, const UINT16 &i_u16Width, const UINT16 &i_u16Height,
	const HINSTANCE &i_currInstance, const int &i_initialWindowDisplayState, const HWND &i_hwnd )
{
	FUNCTION_START;

	if( !RendererEngine::Initialize(i_GameName, i_u16Width, i_u16Height, i_currInstance, i_initialWindowDisplayState, i_hwnd) )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	g_windowSize.width = i_u16Width;
	g_windowSize.height = i_u16Height;

	assert( Sprite::m_spritePool == NULL );
	Sprite::m_spritePool = Utilities::MemoryPool::Create( sizeof(Sprite), Utilities::DEFAULT_MEMORY_POOL_SIZE );
	spriteDatabase = new std::vector< Utilities::Pointer::SmartPtr<Sprite> >;
	assert( Sprite::m_spritePool );

	linesToDraw = new std::vector< RendererEngine::S_LINE_TO_DRAW >;
	sphereToDraw = new std::vector< RendererEngine::S_SPHERE_TO_DRAW >;
	quadToDraw = new std::vector< RendererEngine::S_QUAD_TO_DRAW >;
	textToDraw = new std::vector< RendererEngine::S_TEXT_TO_DRAW >;

	assert( Mesh::m_meshPool == NULL );
	Mesh::m_meshPool = Utilities::MemoryPool::Create( sizeof(Mesh), Utilities::DEFAULT_MEMORY_POOL_SIZE );
	meshDatabase = new std::vector< Utilities::Pointer::SmartPtr<Mesh> >;
	assert( Mesh::m_meshPool );

	entityDatabase = new std::vector< RendererEngine::S_ENTITY_TO_DRAW >;

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin update renderer
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::BeginUpdate( void )
{
	FUNCTION_START;
	
	RemoveDeadSprites();

	// The actual function calls that draw geometry must be made between paired calls to
	// BeginScene() and EndScene()
	D3DCOLOR clearColour = D3DCOLOR_XRGB( 100, 149, 237);

	if( !bReadyToRender && !RendererEngine::BeginScene(clearColour) )
	{
		FUNCTION_FINISH;
		return;
	}

	bReadyToRender = true;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update 3D objects
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::Update( void )
{
	FUNCTION_START;

	if( !bReadyToRender )
	{
		FUNCTION_FINISH;
		return;
	}

	for( UINT32 i = 0; i < entityDatabase->size(); ++i )
	{
		entityDatabase->at(i).position.x = meshDatabase->at(i)->m_entity->m_v3Position.X();
		entityDatabase->at(i).position.y = meshDatabase->at(i)->m_entity->m_v3Position.Y();
		entityDatabase->at(i).position.z = meshDatabase->at(i)->m_entity->m_v3Position.Z();
		entityDatabase->at(i).orientation = meshDatabase->at(i)->m_entity->m_orientation;
		entityDatabase->at(i).scale = meshDatabase->at(i)->m_entity->m_vScale;
	}

	RendererEngine::Draw3D( *entityDatabase, *linesToDraw, *sphereToDraw,
		g_world::Get().m_camera->m_worldToViewMatrix, g_world::Get().m_camera->GetViewToProjectedTransform(),
		g_world::Get().m_camera->GetPosition(), g_world::Get().m_camera->m_farView,
		g_world::Get().m_directionalLight->GetWorldToViewTransform(), g_world::Get().m_directionalLight->GetViewToProjectedTransform(),
		g_world::Get().m_directionalLight->m_direction, g_world::Get().m_directionalLight->m_colour,
		g_world::Get().m_directionalLight->m_intensity, g_world::Get().m_directionalLight->m_farView,
		g_world::Get().m_pointLight->m_colour, g_world::Get().m_pointLight->m_ambient, g_world::Get().m_pointLight->m_position,
		g_world::Get().m_pointLight->m_intensity, g_world::Get().m_pointLight->m_radius );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void UpdateGUI( void )
	\brief		Update 2D objects
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::UpdateGUI( void )
{
	FUNCTION_START;

	if( !bReadyToRender )
	{
		FUNCTION_FINISH;
		return;
	}

	RendererEngine::Draw2D( *quadToDraw, *textToDraw );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End update renderer
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::EndUpdate( void )
{
	FUNCTION_START;

	if( !RendererEngine::EndScene() )
	{
		FUNCTION_FINISH;
		return;
	}

	textToDraw->clear();
	quadToDraw->clear();

	bReadyToRender = false;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		ShutDown renderer
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::ShutDown( void )
{
	FUNCTION_START;

	if( spriteDatabase )
	{
		delete spriteDatabase;
		spriteDatabase = NULL;
	}

	if( linesToDraw )
	{
		delete linesToDraw;
		linesToDraw = NULL;
	}

	if( sphereToDraw )
	{
		delete sphereToDraw;
		sphereToDraw = NULL;
	}

	if( quadToDraw )
	{
		delete quadToDraw;
		quadToDraw = NULL;
	}

	if( Sprite::m_spritePool )
	{
		delete Sprite::m_spritePool;
		Sprite::m_spritePool = NULL;
	}

	if( meshDatabase )
	{
		delete meshDatabase;
		meshDatabase = NULL;
	}

	if( entityDatabase )
	{
		delete entityDatabase;
		entityDatabase = NULL;
	}

	if( textToDraw )
	{
		delete textToDraw;
		textToDraw = NULL;
	}

	if( Mesh::m_meshPool )
	{
		delete Mesh::m_meshPool;
		Mesh::m_meshPool = NULL;
	}

	g_parserHelper::Release();
	RendererEngine::ShutDown();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void AddSprite( Pointer::SmartPtr<Entity> i_entity, const D3DCOLOR &i_colour, const std::string &i_textureFile )
	\brief		Add renderer sprite
	\param		*i_entity pointer to entity
	\param		i_colour colour of sprite
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::AddSprite( Utilities::Pointer::SmartPtr<Entity> &i_entity, const D3DCOLOR &i_colour, const char *i_textureFile )
{
	FUNCTION_START;

	spriteDatabase->push_back( new Sprite(i_entity, i_colour, i_textureFile) );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			DrawText( const D3DXVECTOR2 &i_position, const D3DXVECTOR2 &i_size, D3DCOLOR i_colour, 
				const std::string &i_text, UINT32 i_hAlign )
	\brief		Add text to be drawn
	\param		i_position position of the text
	\param		i_size size of the text
	\param		i_text text to be drawn
	\param		i_hAlign horizontal alignment
	\param		i_colour colour of text
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::DrawText( const D3DXVECTOR2 &i_position, const D3DXVECTOR2 &i_size, D3DCOLOR i_colour, \
			const std::string &i_text, UINT32 i_hAlign )
{
	FUNCTION_START;

	RendererEngine::S_TEXT_TO_DRAW newTextToDraw = { i_position, i_size, i_colour, i_text, i_hAlign };
	textToDraw->push_back( newTextToDraw );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			DrawSlider( const D3DXVECTOR2 &i_position, const UINT32 &i_u32CurrValue, \
				const UINT32 &i_u32MinValue, const UINT32 &i_u32MaxValue, \
				const D3DCOLOR &i_backgroundColour, const D3DCOLOR &i_foregroundColour )
	\brief		Add slider to be drawn
	\param		i_position position of the slider
	\param		i_u32CurrValue current slider value
	\param		i_u32MinValue minimum slider value
	\param		i_u32MaxValue maximum slider value
	\param		i_backgroundColour the background colour of the slider
	\param		i_foregroundColour the foreground colur of the slider
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::DrawSlider( const D3DXVECTOR2 &i_position,  const UINT32 &i_u32CurrValue, \
	const UINT32 &i_u32MinValue, const UINT32 &i_u32MaxValue, \
	const D3DCOLOR &i_backgroundColour, const D3DCOLOR &i_foregroundColour )
{
	float sliderWidthInWorldSpace = SLIDER_WIDTH / g_windowSize.width;
	float sliderHeightInWorldSpace = SLIDER_HEIGHT / g_windowSize.height;
	D3DXVECTOR2 positionInWorldSpace;
	positionInWorldSpace.x = (i_position.x + (SLIDER_WIDTH / 2)) / (g_windowSize.width / 2) - 1.0f;
	positionInWorldSpace.y = 1.0f - i_position.y / (g_windowSize.height / 2);

	FUNCTION_START;

	RendererEngine::S_QUAD_TO_DRAW background;
	background.position = positionInWorldSpace;
	background.size.width = sliderWidthInWorldSpace;
	background.size.height = sliderHeightInWorldSpace;
	background.colour = i_backgroundColour;
	quadToDraw->push_back( background );

	float percentage = static_cast<float>(i_u32CurrValue)/static_cast<float>(i_u32MaxValue);
	RendererEngine::S_QUAD_TO_DRAW foreground;
	foreground.position = positionInWorldSpace;
	foreground.size.width = sliderWidthInWorldSpace * percentage;
	foreground.size.height = sliderHeightInWorldSpace;
	foreground.position.x = (i_position.x + (SLIDER_WIDTH / 2 * percentage)) / (g_windowSize.width / 2) - 1.0f;
	foreground.colour = i_foregroundColour;
	quadToDraw->push_back( foreground );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void AddAsset( Pointer::SmartPtr<Entity> &i_entity, const std::string &i_entityFile, const D3DXVECTOR3 &i_position, const float &i_orientation )
	\brief		Add renderer asset
	\param		*i_entity pointer to entity
	\param		i_entityFile the file entity
	\param		i_position the position of the asset in the world
	\param		i_orientation the orientation of the asset in the world
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::AddMesh( Utilities::Pointer::SmartPtr<Entity> &i_entity, const char*i_entityFile )
{
	FUNCTION_START;

	meshDatabase->push_back( new Mesh(i_entity, i_entityFile) );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			S_SIZE GetWindowSize( void )
	\brief		Get the size of current window
	\param		NONE
	\return		S_SIZE
 ****************************************************************************************************
*/
Utilities::S_SIZE &GameEngine::Renderer::GetWindowSize( void )
{
	FUNCTION_START;
	FUNCTION_FINISH;

	return g_windowSize;
}

/****************************************************************************************************
			Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void RemoveDeadSprites( void )
	\brief		Removes dead sprites from database
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::RemoveDeadSprites( void )
{
	FUNCTION_START;

	for( UINT32 i = 0; i < spriteDatabase->size(); ++i )
	{
		if( spriteDatabase->at(i)->m_entity->m_isDestroyed )
		{
			UINT32 u32QuadIndex = spriteDatabase->at(i)->m_u32QuadIndex;
			spriteDatabase->back()->m_u32QuadIndex = u32QuadIndex;
			quadToDraw->at(u32QuadIndex) = quadToDraw->back();
			quadToDraw->pop_back();

			spriteDatabase->at(i) = spriteDatabase->back();
			spriteDatabase->pop_back();
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void RemoveDeadAssets( void )
	\brief		Removes dead assets from database
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::RemoveDeadAssets( void )
{
	FUNCTION_START;

	for( UINT32 i = 0; i < meshDatabase->size(); ++i )
	{
		if( meshDatabase->at(i)->m_entity->m_isDestroyed )
		{
			UINT32 u32EntityIndex = meshDatabase->at(i)->m_u32EntityIndex;
			meshDatabase->back()->m_u32EntityIndex = u32EntityIndex;
			entityDatabase->at(u32EntityIndex) = entityDatabase->back();
			entityDatabase->pop_back();

			meshDatabase->at(i) = meshDatabase->back();
			meshDatabase->pop_back();
		}
	}

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Public sprite class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			Sprite( Pointer::SmartPtr<Entity> &i_entity, const D3DCOLOR &i_colour, const char *i_textureFile )
	\brief		Default constructor of Sprite
	\param		&i_entity pointer to common entity data
	\param		&i_colour colour of the sprite
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Renderer::Sprite::Sprite( Utilities::Pointer::SmartPtr<Entity> &i_entity, const D3DCOLOR &i_colour,
	const char *i_textureFile ) :
	m_entity( i_entity ),
	m_colour( i_colour ),
	m_hashedTextureFile( Utilities::StringHash("") )
{
	if( i_textureFile != NULL )
	{
		m_hashedTextureFile = Utilities::StringHash( i_textureFile );
		RendererEngine::CreateTexture( i_textureFile );
	}

	RendererEngine::S_QUAD_TO_DRAW newQuad;
	newQuad.size = m_entity->m_size;
	newQuad.position.x = m_entity->m_v3Position.X();
	newQuad.position.x = m_entity->m_v3Position.Y();
	newQuad.texture = m_hashedTextureFile;
	newQuad.colour = m_colour;
	m_u32QuadIndex = quadToDraw->size();
	quadToDraw->push_back( newQuad );
}

/**
 ****************************************************************************************************
	\fn			~Sprite( void )
	\brief		Destructor of Sprite class
 ****************************************************************************************************
*/
GameEngine::Renderer::Sprite::~Sprite( void )
{
}

/**
 ****************************************************************************************************
	\fn			void *operator new( size_t i_size )
	\brief		New operator of Sprite class
	\param		i_size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
void *GameEngine::Renderer::Sprite::operator new( size_t i_size )
{
	FUNCTION_START;

	assert( i_size == sizeof(Sprite) );
	assert( m_spritePool != NULL );

	FUNCTION_FINISH;
	return m_spritePool->Allocate( sizeof(Sprite) );
}

/**
 ****************************************************************************************************
	\fn			void operator delete( void *i_ptr )
	\brief		Delete operator of Sprite class
	\param		i_ptr pointer to be deleted
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::Sprite::operator delete( void *i_ptr )
{
	FUNCTION_START;

	assert( m_spritePool );
	m_spritePool->Deallocate( i_ptr );

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private sprite class implementation
****************************************************************************************************/

/****************************************************************************************************
			Public asset class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			Model( Pointer::SmartPtr<Entity> &i_entity, const std::string &i_fileName )
	\brief		Default constructor of Model
	\param		&i_entity pointer to common entity data
	\param		&i_fileName name of entity file
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Renderer::Mesh::Mesh( Utilities::Pointer::SmartPtr<Entity> &i_entity, const std::string &i_fileName ) :
	m_entity( i_entity ),
	_entityInput( i_fileName ),
	m_meshName( Utilities::StringHash(i_fileName.c_str()) )
{
	assert( i_fileName.size() > 0 );
	Load();
}

/**
 ****************************************************************************************************
	\fn			~Model( void )
	\brief		Destructor of Model class
 ****************************************************************************************************
*/
GameEngine::Renderer::Mesh::~Mesh( void )
{

}

/**
 ****************************************************************************************************
	\fn			void *operator new( size_t i_size )
	\brief		New operator of Model class
	\param		i_size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
void *GameEngine::Renderer::Mesh::operator new( size_t i_size )
{
	FUNCTION_START;

	assert( i_size == sizeof(Mesh) );
	assert( m_meshPool != NULL );

	FUNCTION_FINISH;
	return m_meshPool->Allocate( (UINT8)sizeof(Mesh) );
}

/**
 ****************************************************************************************************
	\fn			void operator delete( void *i_ptr )
	\brief		Delete operator of Model class
	\param		i_ptr pointer to be deleted
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Renderer::Mesh::operator delete( void *i_ptr )
{
	FUNCTION_START;

	assert( m_meshPool );
	m_meshPool->Deallocate( i_ptr );

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private asset class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void Load( void )
	\brief		Load the asset data
 ****************************************************************************************************
*/
void GameEngine::Renderer::Mesh::Load( void )
{
	FUNCTION_START;

	// The entity
	Utilities::EntityParser entityData( _entityInput.c_str() );
	RendererEngine::CreateEntity( m_meshName, Utilities::StringHash(entityData.m_meshData.c_str()),
		Utilities::StringHash(entityData.m_materialData.c_str()) );
	RendererEngine::CreateMesh( entityData.m_meshData.c_str() );

	// The material
	Utilities::MaterialParser materialData( entityData.m_materialData.c_str() );
	RendererEngine::CreateMaterial( Utilities::StringHash(entityData.m_materialData.c_str()), Utilities::StringHash(materialData.m_effectData.c_str()),
		Utilities::StringHash(materialData.m_diffuseColorTexture.c_str()), Utilities::StringHash(materialData.m_normalMapTexture.c_str()),
		materialData.m_transparency, materialData.m_shininess, materialData.m_reflectance );
	if( materialData.m_diffuseColorTexture.size() > 0 )
		RendererEngine::CreateTexture( materialData.m_diffuseColorTexture.c_str() );
	if( materialData.m_normalMapTexture.size() > 0 )
		RendererEngine::CreateNormalMap( materialData.m_normalMapTexture.c_str() );

	// The effect
	Utilities::EffectParser effectData( materialData.m_effectData.c_str() );
	RendererEngine::CreateEffect( Utilities::StringHash(materialData.m_effectData.c_str()),
		Utilities::StringHash(effectData.m_vertexShader.c_str()), Utilities::StringHash(effectData.m_fragmentShader->at(0).c_str()),
		effectData.m_renderState, effectData.m_textureMode );
	RendererEngine::CreateFragmentShader( effectData.m_fragmentShader->at((0)).c_str() );
	RendererEngine::CreateVertexShader( effectData.m_vertexShader.c_str() );

	// Add the model to entityDatabase
	RendererEngine::S_ENTITY_TO_DRAW newEntity;
	newEntity.entity = m_meshName;
	newEntity.position.x = m_entity->m_v3Position.X();
	newEntity.position.y = m_entity->m_v3Position.Y();
	newEntity.position.z = m_entity->m_v3Position.Z();
	newEntity.orientation = m_entity->m_orientation;
	m_u32EntityIndex = entityDatabase->size();
	entityDatabase->push_back( newEntity );

	FUNCTION_FINISH;
}
