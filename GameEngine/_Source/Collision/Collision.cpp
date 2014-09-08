/**
 ****************************************************************************************************
 * \file		Collision.cpp
 * \brief		Collision engine implementation and control
 ****************************************************************************************************
*/

#include <math.h>
#include <vector>
#include <float.h>
#include <assert.h>

// Utilities header
#include <Time/Time.h>
#include <SmartPtr/SmartPtr.h>
#include <MemoryPool/MemoryPool.h>
#include <Parser/MeshParser/MeshParser.h>

#include "Collision.h"
#include "../World/Entity.h"
#include "../Camera/Camera.h"
#include "../Renderer/Renderer.h"
#include "../GameEngineDefault.h"
#include "../Math/Matrix/Matrix.h"
#include "../DebugMenu/DebugMenu.h"
#include "../Utilities/Octree/Octree.h"
#include "../Utilities/GameEngineTypes.h"
#include "../Utilities/Profiler/Profiler.h"
#include "../Utilities/IDCreator/IDCreator.h"

/****************************************************************************************************
			Class / variable declaration
****************************************************************************************************/
namespace GameEngine
{
	namespace Collision
	{
		class CollisionEntity
		{
		public:
			static Utilities::MemoryPool					*m_collisionEntityPool;

			D3DXVECTOR3														m_downCollisionPoint;
			D3DXVECTOR3														m_forwardCollisionPoint;
			Utilities::Pointer::SmartPtr<Entity>	m_downCollidedEntity;
			Utilities::Pointer::SmartPtr<Entity>	m_forwardCollidedEntity;
			Utilities::Pointer::SmartPtr<Entity>	m_entity;
			CollisionHandler											*m_collisionHandler;
			Octree																*m_octree;
			uint16_t															*m_u16Indices;
			D3DXVECTOR3														*m_vertices;
			UINT32																m_u32TotalPrimitives;
			UINT32																m_u32TotalVertices;
			float																	m_downCollisionDistance;
			float																	m_forwardCollisionDistance;

			CollisionEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity, const char *i_collisionFile );

			~CollisionEntity( void );

			void *operator new( const size_t &i_size );
			void operator delete( void *i_ptr );

		private:
			CollisionEntity( void ) {}

			// Make it non-copyable
			CollisionEntity( CollisionEntity const &i_other );
			CollisionEntity &operator=( const CollisionEntity &i_other );

			// Make it incomparable
			bool &operator==( const CollisionEntity &i_other ) const;
		};

		bool bShowCollisionWireframe;

		static std::vector< Utilities::Pointer::SmartPtr<CollisionEntity> > *collisionEntityDatabase;
		static E_COLLISION_BY eCollisionBy = E_COLLISION_MAX;
		static D3DXVECTOR3 startPoint;
		static D3DXVECTOR3 endPoint;
		void RemoveDeadEntities( void );
		void CheckCollision( CollisionEntity &i_A, CollisionEntity &i_B, const bool i_bForwardDirection, Utilities::StringHash &o_hashedTag = Utilities::StringHash("") );
	}	// namespace Collision	
}	// namespace GameEngine

Utilities::MemoryPool *GameEngine::Collision::CollisionEntity::m_collisionEntityPool = NULL;

/****************************************************************************************************
			Global functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize collision
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::Collision::Initialize( void )
{
	FUNCTION_START;

	assert( CollisionEntity::m_collisionEntityPool == NULL );

	CollisionEntity::m_collisionEntityPool = Utilities::MemoryPool::Create( sizeof(CollisionEntity), Utilities::DEFAULT_MEMORY_POOL_SIZE );
	collisionEntityDatabase = new std::vector< Utilities::Pointer::SmartPtr<CollisionEntity> >;
	
	assert( CollisionEntity::m_collisionEntityPool );

#ifdef ENABLE_COLLISION_WIREFRAME
	g_debugMenu::Get().AddCheckBox( "Show collision meshes", bShowCollisionWireframe );
#endif// #ifdef ENABLE_COLLISION_WIREFRAME

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin updating collision 
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Collision::BeginUpdate( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update collision
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Collision::Update( void )
{
	PROFILE_UNSCOPED( "Collision" );
	float frameTime_ms = Utilities::Time::GetTimeElapsedThisFrame_ms();
	std::vector< Utilities::Pointer::SmartPtr<CollisionEntity> >::iterator iterA;
	std::vector< Utilities::Pointer::SmartPtr<CollisionEntity> >::iterator iterB;
	Utilities::StringHash hashedTag;

	FUNCTION_START;

	if( collisionEntityDatabase->size() == 0 )
	{
		FUNCTION_FINISH;
		return;
	}

	if( !bShowCollisionWireframe && (collisionEntityDatabase->size() == 1) )
	{
		FUNCTION_FINISH;
		return;
	}

	iterA = collisionEntityDatabase->begin();

	if( Camera::m_bOnDebugCamera )
	{	
		g_debugMenu::Get().DrawLine( startPoint, endPoint, Utilities::BLACK );
	}
	else
	{
		startPoint = D3DXVECTOR3( (*iterA)->m_entity->m_v3Position.X(), (*iterA)->m_entity->m_v3Position.Y(), (*iterA)->m_entity->m_v3Position.Z() );
		D3DXVECTOR3 newEndPoint( (*iterA)->m_entity->m_v3ProjectedPosition.X(), (*iterA)->m_entity->m_v3ProjectedPosition.Y(), (*iterA)->m_entity->m_v3ProjectedPosition.Z() );
		if( (*iterA)->m_entity->m_v3Velocity != Math::Vector3::Zero )
		{
			D3DXVECTOR3 velocity( (*iterA)->m_entity->m_v3Velocity.X(), (*iterA)->m_entity->m_v3Velocity.Y(), (*iterA)->m_entity->m_v3Velocity.Z() );
			endPoint = startPoint + 100 * velocity;
		}
		if( bShowCollisionWireframe )
			g_debugMenu::Get().DrawLine( startPoint, endPoint, Utilities::BLACK );
	}

	for( iterA = collisionEntityDatabase->begin(); iterA != collisionEntityDatabase->end(); ++iterA )
	{
		if( (*iterA)->m_collisionHandler && !(*iterA)->m_entity->m_isDestroyed )
		{
			// Check for forward direction collision
			iterB = iterA;
			for( ++iterB; iterB != collisionEntityDatabase->end(); ++iterB )
				CheckCollision( **iterA, **iterB, true );

			// Forward ray casting resolution
			if( (*iterA)->m_entity->m_u8EntityID == g_IDCreator::Get().GetID("Camera") )
			{
				do 
				{
					if( (*iterA)->m_forwardCollidedEntity && !Camera::m_bOnDebugCamera )
						(*iterA)->m_collisionHandler->HandleCollision( (*iterA)->m_entity, (*iterA)->m_forwardCollidedEntity, 0.0f, (*iterA)->m_forwardCollisionPoint );

					(*iterA)->m_forwardCollidedEntity = NULL;
					(*iterA)->m_forwardCollisionDistance = 0.0f;
					(*iterA)->m_forwardCollisionPoint = D3DXVECTOR3_ZERO;

					iterB = iterA;
					for( ++iterB; iterB != collisionEntityDatabase->end(); ++iterB )
						CheckCollision( **iterA, **iterB, true );
				} while( (*iterA)->m_forwardCollidedEntity );

				(*iterA)->m_collisionHandler->OnLeavingCollision( (*iterA)->m_entity, (*iterA)->m_entity );
			}
			else
			{
				if( (*iterA)->m_forwardCollidedEntity && !Camera::m_bOnDebugCamera )
					(*iterA)->m_collisionHandler->HandleCollision( (*iterA)->m_entity, (*iterA)->m_forwardCollidedEntity, 0.0f, (*iterA)->m_forwardCollisionPoint );
			}

			// Check for down direction collision
			iterB = iterA;
			for( ++iterB; iterB != collisionEntityDatabase->end(); ++iterB )
				CheckCollision( **iterA, **iterB, false, hashedTag );

			// Do not do collision resolution if in debug-camera-mode
			if( Camera::m_bOnDebugCamera )
				continue;

			// If down collision found
			if( (*iterA)->m_downCollidedEntity )
			{
				(*iterA)->m_collisionHandler->HandleCollision( (*iterA)->m_entity, (*iterA)->m_downCollidedEntity, 1.0f, \
					(*iterA)->m_downCollisionPoint, hashedTag );
			}
			else
			{
				UINT8 ctr = 0;
				do
				{
					(*iterA)->m_collisionHandler->HandleCollision( (*iterA)->m_entity, (*iterA)->m_entity, 1.0f, Math::Vector3::Zero );

					iterB = iterA;
					for( ++iterB; iterB != collisionEntityDatabase->end(); ++iterB )
						CheckCollision( **iterA, **iterB, false, hashedTag );

					if( (*iterA)->m_downCollidedEntity )
					{
						(*iterA)->m_collisionHandler->HandleCollision( (*iterA)->m_entity, (*iterA)->m_downCollidedEntity, 1.0f, \
							(*iterA)->m_downCollisionPoint, hashedTag );
						ctr = 2;
					}
				} while( ctr > 2 );
			}
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End updating collision
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Collision::EndUpdate( void )
{
	FUNCTION_START;

	RemoveDeadEntities();

	std::vector< Utilities::Pointer::SmartPtr<CollisionEntity> >::iterator iterA;
	for( iterA = collisionEntityDatabase->begin(); iterA != collisionEntityDatabase->end(); iterA++ )
	{
		(*iterA)->m_downCollidedEntity = NULL;
		(*iterA)->m_downCollisionDistance = 0.0f;
		(*iterA)->m_downCollisionPoint = D3DXVECTOR3_ZERO;
		(*iterA)->m_forwardCollidedEntity = NULL;
		(*iterA)->m_forwardCollisionDistance = 0.0f;
		(*iterA)->m_forwardCollisionPoint = D3DXVECTOR3_ZERO;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shutdown collision
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Collision::ShutDown( void )
{
	FUNCTION_START;

	if( collisionEntityDatabase )
	{
		delete collisionEntityDatabase;
		collisionEntityDatabase = NULL;
	}

	if( CollisionEntity::m_collisionEntityPool )
	{
		delete CollisionEntity::m_collisionEntityPool;
		CollisionEntity::m_collisionEntityPool = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			UINT8 AddCollisionEntity( Pointer::SmartPtr<Entity> &i_entity, const char *i_collisionFile )
	\brief		Add collision entity to collisionEntityDatabase
	\param		*i_entity entity to be added
	\return		UINT8
	\retval		index of the new added collision entity in database
 ****************************************************************************************************
*/
UINT8 GameEngine::Collision::AddCollisionEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity, const char *i_collisionFile )
{
	FUNCTION_START;

	collisionEntityDatabase->push_back( new CollisionEntity(i_entity, i_collisionFile) );
	collisionEntityDatabase->at(collisionEntityDatabase->size()-1)->m_collisionHandler = NULL;

	FUNCTION_FINISH;
	return collisionEntityDatabase->size() - 1;
}

/**
 ****************************************************************************************************
	\fn			void SetCollisionHandle( const UINT8 &i_index, CollisionHandler *i_collisionHandler )
	\brief		Set collision entity to collisionEntityDatabase
	\param		*i_entity entity to be added
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Collision::SetCollisionHandler( const UINT8 &i_index, CollisionHandler *i_collisionHandler )
{
	FUNCTION_START;

	if( i_index < collisionEntityDatabase->size() )
		collisionEntityDatabase->at(i_index)->m_collisionHandler = i_collisionHandler;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void SetCollisionType( const E_COLLISION_BY &i_collisionBy )
	\brief		Set collision detection type
	\param		i_collisionBy collision detection type
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Collision::SetCollisionDetectionType( const E_COLLISION_BY &i_collisionBy )
{
	eCollisionBy = i_collisionBy;
}

/****************************************************************************************************
			Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void RemoveDeadEntities( void )
	\brief		Remove dead entities
	\param		*i_entity entity to be added
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Collision::RemoveDeadEntities( void )
{
	FUNCTION_START;

	for( UINT8 i = 0; i < collisionEntityDatabase->size(); ++i )
	{
		collisionEntityDatabase->at(i)->m_downCollisionDistance = 0.0f;
		collisionEntityDatabase->at(i)->m_forwardCollisionDistance = 0.0f;
		collisionEntityDatabase->at(i)->m_downCollidedEntity = NULL;
		collisionEntityDatabase->at(i)->m_forwardCollidedEntity = NULL;

		if( collisionEntityDatabase->at(i)->m_entity->m_isDestroyed )
		{
			collisionEntityDatabase->back()->m_entity->m_u8CollisionEntityIndex = collisionEntityDatabase->at(i)->m_entity->m_u8CollisionEntityIndex;
			collisionEntityDatabase->at(i) = collisionEntityDatabase->back();
			collisionEntityDatabase->pop_back();
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void CheckCollision( CollisionEntity &i_A, CollisionEntity &i_B, bool &i_bForwardDirection )
	\brief		Move entities within boundary
	\param		i_A first entity
	\param		i_B second entity
	\param		i_bForwardDirection
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Collision::CheckCollision( CollisionEntity &i_A, CollisionEntity &i_B, const bool i_bForwardDirection, \
	Utilities::StringHash &o_hashedTag )
{
	D3DXVECTOR3 downCollisionPoint;
	D3DXVECTOR3 forwardCollisionPoint;
	bool AIsMoving = false;
	bool BIsMoving = false;
	float downCollisionDistance;
	float forwardCollisionDistance;

	FUNCTION_START;

	// If i_A do not have collision with other object or if either of the object is to be destroyed in the end 
	// of this frame do not do collision detection
	if( (i_A.m_entity->m_u32CollisionMask == 0) || i_A.m_entity->m_isDestroyed || i_B.m_entity->m_isDestroyed )
	{
		FUNCTION_FINISH;
		return;
	}

	if( i_A.m_entity->m_v3Velocity.SquaredLength() > 0.0f )
		AIsMoving = true;

	if( i_B.m_entity->m_v3Velocity.SquaredLength() > 0.0f )
		BIsMoving = true;

	if( i_A.m_entity->m_u32CollisionMask & g_IDCreator::Get().IDtoBitMask(i_B.m_entity->m_u8EntityID) )
	{
		bool bPrevDownRayCollided = false;
		bool bPrevForwardRayCollided = false;
		bool bThisDownRayTracingResult = false;
		bool bThisForwardRayTracingResult = false;

		D3DXVECTOR3 startPoint;
		D3DXVECTOR3 endPoint;
		D3DXVECTOR3 downPoint( i_A.m_entity->m_v3ProjectedPosition.X(),
			i_A.m_entity->m_v3ProjectedPosition.Y() - i_A.m_entity->m_size.height, i_A.m_entity->m_v3ProjectedPosition.Z() );

		if( i_bForwardDirection )
		{
			startPoint = D3DXVECTOR3( i_A.m_entity->m_v3Position.X(), i_A.m_entity->m_v3Position.Y(), i_A.m_entity->m_v3Position.Z() );
			endPoint = D3DXVECTOR3( i_A.m_entity->m_v3ProjectedPosition.X(), i_A.m_entity->m_v3ProjectedPosition.Y(),
				i_A.m_entity->m_v3ProjectedPosition.Z() );

			if( !Utilities::Math::AreRelativelyEqual(i_A.m_forwardCollisionDistance, 0.0f) )
				forwardCollisionDistance = i_A.m_forwardCollisionDistance;

			if( i_A.m_forwardCollidedEntity != NULL )
				bPrevForwardRayCollided = true;
		}
		else
		{
			startPoint = D3DXVECTOR3( i_A.m_entity->m_v3ProjectedPosition.X(), i_A.m_entity->m_v3ProjectedPosition.Y(),
				i_A.m_entity->m_v3ProjectedPosition.Z() );
			if( !Utilities::Math::AreRelativelyEqual(i_A.m_downCollisionDistance, 0.0f) )
				downCollisionDistance = i_A.m_downCollisionDistance;

			if( i_A.m_downCollidedEntity != NULL )
				bPrevDownRayCollided = true;
		}

		if( eCollisionBy == E_COLLISION_BY_MESH )
		{
			UINT32 u32TotalIndices = i_B.m_u32TotalPrimitives * 3;
			for( UINT32 i = 0; i < u32TotalIndices; i += 3 )
			{
				if( i_bForwardDirection )
				{
					bThisForwardRayTracingResult |= Utilities::Math::RayTracing( startPoint, endPoint,
						i_B.m_vertices[i_B.m_u16Indices[i]], i_B.m_vertices[i_B.m_u16Indices[i+1]], i_B.m_vertices[i_B.m_u16Indices[i+2]],
						bPrevForwardRayCollided, forwardCollisionPoint, forwardCollisionDistance );
					if( bThisForwardRayTracingResult )
					{
						if( !bPrevForwardRayCollided )
							bPrevForwardRayCollided = true;
					}
				}
				else
				{
					bThisDownRayTracingResult |= Utilities::Math::RayTracing( startPoint, downPoint,
						i_B.m_vertices[i_B.m_u16Indices[i]], i_B.m_vertices[i_B.m_u16Indices[i+1]], i_B.m_vertices[i_B.m_u16Indices[i+2]],
						bPrevDownRayCollided, downCollisionPoint, downCollisionDistance );
					if( bThisDownRayTracingResult )
					{
						if( !bPrevDownRayCollided )
							bPrevDownRayCollided = true;
					}
				}
			}
		}
		else
		{
			std::vector<Utilities::S_TRIANGLE> collisionTriangle;
			if( i_bForwardDirection )
				i_B.m_octree->GetTriangleData( startPoint, endPoint, collisionTriangle );
			else
				i_B.m_octree->GetTriangleData( startPoint, downPoint, collisionTriangle );

			UINT32 u32TotalTriangleData = collisionTriangle.size();
			for( UINT32 i = 0; i < u32TotalTriangleData; ++i )
			{
				if( bShowCollisionWireframe )
				{
					g_debugMenu::Get().DrawLine( collisionTriangle[i].a, collisionTriangle[i].b );
					g_debugMenu::Get().DrawLine( collisionTriangle[i].a, collisionTriangle[i].c );
					g_debugMenu::Get().DrawLine( collisionTriangle[i].b, collisionTriangle[i].c );
				}

				if( i_bForwardDirection )
				{
					bThisForwardRayTracingResult |= Utilities::Math::RayTracing( startPoint, endPoint,
						collisionTriangle[i].a, collisionTriangle[i].b, collisionTriangle[i].c,
						bPrevForwardRayCollided, forwardCollisionPoint, forwardCollisionDistance );
					if( bThisForwardRayTracingResult )
					{
						o_hashedTag = collisionTriangle[i].hashedTag;
						if( !bPrevForwardRayCollided )
							bPrevForwardRayCollided = true;
					}
				}
				else
				{
					bThisDownRayTracingResult |= Utilities::Math::RayTracing( startPoint, downPoint,
						collisionTriangle[i].a, collisionTriangle[i].b, collisionTriangle[i].c,
						bPrevDownRayCollided, downCollisionPoint, downCollisionDistance );
					if( bThisDownRayTracingResult )
					{
						if( collisionTriangle[i].hashedTag != Utilities::StringHash("") )
							o_hashedTag = collisionTriangle[i].hashedTag;
						if( !bPrevDownRayCollided )
							bPrevDownRayCollided = true;
					}
				}
			}
		}

		// Do not do collision resolution if in debug-camera-mode
		if( Camera::m_bOnDebugCamera )
		{
			FUNCTION_FINISH;
			return;
		}

		if( bThisForwardRayTracingResult )
		{
			if( (i_A.m_forwardCollidedEntity != i_B.m_entity) || (i_A.m_forwardCollisionDistance > forwardCollisionDistance) )
			{
				i_A.m_forwardCollidedEntity = i_B.m_entity;
				i_A.m_forwardCollisionDistance = forwardCollisionDistance;
				i_A.m_forwardCollisionPoint = forwardCollisionPoint;
			}

			if( i_B.m_forwardCollidedEntity != i_A.m_entity )
			{
				i_B.m_forwardCollidedEntity = i_A.m_entity;
				i_B.m_forwardCollisionDistance = forwardCollisionDistance;
				i_B.m_forwardCollisionPoint = forwardCollisionPoint;
			}
		}

		if( bThisDownRayTracingResult )
		{
			if( i_A.m_downCollidedEntity != i_B.m_entity )
			{
				i_A.m_downCollidedEntity = i_B.m_entity;
				i_A.m_downCollisionDistance = downCollisionDistance;
				i_A.m_downCollisionPoint = downCollisionPoint;
			}

			if( i_B.m_downCollidedEntity != i_A.m_entity )
			{
				i_B.m_downCollidedEntity = i_A.m_entity;
				i_B.m_downCollisionDistance = downCollisionDistance;
				i_B.m_downCollisionPoint = downCollisionPoint;
			}
		}
	}

	FUNCTION_FINISH;
}

/****************************************************************************************************
			CollisionEntity class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			CollisionEntity( Pointer::SmartPtr<Entity> &i_entity, const char *i_collisionFile )
	\brief		Default constructor of CollisionEntity
	\param		i_entity pointer to common entity data
	\param		i_collisionFile the file contain the information of the collision
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Collision::CollisionEntity::CollisionEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity,
	const char *i_collisionFile ) :
	m_downCollisionPoint( D3DXVECTOR3_ZERO ),
	m_forwardCollisionPoint( D3DXVECTOR3_ZERO ),
	m_downCollidedEntity( NULL ),
	m_forwardCollidedEntity( NULL ),
	m_entity( i_entity ),
	m_collisionHandler( NULL ),
	m_octree( NULL ),
	m_u16Indices( NULL ),
	m_vertices( NULL ),
	m_u32TotalPrimitives( 0 ),
	m_u32TotalVertices( 0 ),
	m_downCollisionDistance( 0.0f ),
	m_forwardCollisionDistance( 0.0f )
{
	assert( i_collisionFile != NULL );
	assert( i_collisionFile[0] != '\0' );

	if( eCollisionBy == E_COLLISION_BY_MESH )
	{
		Utilities::S_NORMAL_MAP_VERTEX_DATA* vertexData;
		Utilities::MeshParser meshData( i_collisionFile );

		m_u32TotalPrimitives = meshData.m_u32TotalPrimitives;
		m_u32TotalVertices = meshData.m_u32TotalVertices;
		vertexData = new Utilities::S_NORMAL_MAP_VERTEX_DATA[m_u32TotalVertices];
		m_u16Indices = new uint16_t[m_u32TotalPrimitives * 3];
		m_vertices = new D3DXVECTOR3[m_u32TotalVertices];
		meshData.GetVertexData( vertexData );
		meshData.GetIndexData( m_u16Indices );
		for( UINT32 i = 0; i < m_u32TotalVertices; ++i )
			m_vertices[i] = vertexData[i].position;

		delete [] vertexData;
	}
	else
	{
		m_octree = new Octree( i_collisionFile );
	}
}

/**
 ****************************************************************************************************
	\fn			~CollisionEntity( void )
	\brief		Default destructor of CollisionEntity
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Collision::CollisionEntity::~CollisionEntity( void )
{
	FUNCTION_START;

	m_entity = NULL;
	m_forwardCollidedEntity = NULL;

	if( m_octree )
	{
		delete m_octree;
		m_octree = NULL;
	}

	if( m_u16Indices )
	{
		delete [] m_u16Indices;
		m_u16Indices = NULL;
	}

	if( m_vertices )
	{
		delete [] m_vertices;
		m_vertices = NULL;
	}

	if( m_collisionHandler )
	{
		delete m_collisionHandler;
		m_collisionHandler = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void *operator new( size_t i_size )
	\brief		New operator of CollisionEntity class
	\param		i_size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
void *GameEngine::Collision::CollisionEntity::operator new( const size_t &i_size )
{
	FUNCTION_START;

	assert( i_size == sizeof(CollisionEntity) );
	assert( m_collisionEntityPool );

	FUNCTION_FINISH;
	return m_collisionEntityPool->Allocate( sizeof(CollisionEntity) );
}

/**
 ****************************************************************************************************
	\fn			void operator delete( void *i_ptr )
	\brief		Delete operator of CollisionEntity class
	\param		i_ptr pointer to be deleted
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Collision::CollisionEntity::operator delete( void *i_ptr )
{
	FUNCTION_START;

	assert( m_collisionEntityPool );

	if( i_ptr )
		m_collisionEntityPool->Deallocate( i_ptr );

	FUNCTION_FINISH;
}