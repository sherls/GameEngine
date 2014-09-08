/**
 ****************************************************************************************************
 * \file		TriggerBox.h
 * \brief		The header of TriggerBox function
 ****************************************************************************************************
*/

// Utilities header
#include <Time/Time.h>
#include <Debug/Debug.h>
#include <SmartPtr/SmartPtr.h>
#include <MemoryPool/MemoryPool.h>

#include "TriggerBox.h"
#include "BoundingBox.h"
#include "../World/Entity.h"
#include "../Utilities/Profiler/Profiler.h"
#include "../Utilities/IDCreator/IDCreator.h"

/****************************************************************************************************
			Class / variable declaration
****************************************************************************************************/
namespace GameEngine
{
	namespace TriggerBox
	{
		class TriggerBoxEntity
		{
		public:
			static Utilities::MemoryPool					*m_triggerBoxEntityPool;
			Utilities::Pointer::SmartPtr<Entity>	m_intersectedEntity;
			Utilities::Pointer::SmartPtr<Entity>	m_entity;
			TriggerBoxHandler											*m_triggerBoxHandler;
			float																	m_triggerBoxDistance;
			float																	m_enterTime;

			TriggerBoxEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity );
			~TriggerBoxEntity( void );

			void *operator new( const size_t &i_size );
			void operator delete( void *i_ptr );

		private:
			TriggerBoxEntity( void ) {}

			// Make it non-copyable
			TriggerBoxEntity( TriggerBoxEntity const &i_other );
			TriggerBoxEntity &operator=( const TriggerBoxEntity &i_other );

			// Make it incomparable
			bool &operator==( const TriggerBoxEntity &i_other ) const;
		};

		static std::vector< Utilities::Pointer::SmartPtr<TriggerBoxEntity> > *triggerBoxEntityDatabase;
		void RemoveDeadEntities( void );
		void CheckIntersection( TriggerBoxEntity &i_A, TriggerBoxEntity &i_B );

		// BoundingBox calculation
		float SeparationX( const BoundingBox &i_A, const BoundingBox &i_B );
		float SeparationY( const BoundingBox &i_A, const BoundingBox &i_B );
		float SeparationZ( const BoundingBox &i_A, const BoundingBox &i_B );
		bool Overlap( const BoundingBox &i_A, const BoundingBox &i_B );
		bool Overlap( const BoundingBox &i_A, const BoundingBox &i_B, Math::Vector3 &o_enterNormal );
		bool Overlap( const BoundingBox &i_A, const BoundingBox &i_B, Math::Matrix &i_transformation );
		bool MovingToStationary( const BoundingBox &i_stationary, const BoundingBox &i_moving, const Math::Vector3 &i_v3Velocity, \
			float &o_enterTime, Math::Vector3 &o_enterSurfaceNormal );
		bool MovingToStationary( const BoundingBox &i_stationary, const BoundingBox &i_moving, const Math::Vector3 &i_v3Velocity, \
			Math::Matrix &i_transformation, float &o_enterTime, Math::Vector3 &o_enterSurfaceNormal );
		bool MovingCheckInAxis( const float &i_min, const float &i_max, const float i_velocity, const float &i_start, \
			float &o_enterTime, float &o_normalDirection );

	}	// namesapce TriggerBox
}	// namespace GameEngine

Utilities::MemoryPool *GameEngine::TriggerBox::TriggerBoxEntity::m_triggerBoxEntityPool = NULL;

/****************************************************************************************************
			Global functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize trigger box
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool GameEngine::TriggerBox::Initialize( void )
{
	FUNCTION_START;

	assert( TriggerBoxEntity::m_triggerBoxEntityPool == NULL );
	
	TriggerBoxEntity::m_triggerBoxEntityPool = Utilities::MemoryPool::Create( sizeof(TriggerBoxEntity), Utilities::DEFAULT_MEMORY_POOL_SIZE );
	triggerBoxEntityDatabase = new std::vector< Utilities::Pointer::SmartPtr<TriggerBoxEntity> >;
	
	assert( TriggerBoxEntity::m_triggerBoxEntityPool );

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin updating trigger box
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::TriggerBox::BeginUpdate( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update trigger box
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::TriggerBox::Update( void )
{
	PROFILE_UNSCOPED( "Trigger box" );
	std::vector< Utilities::Pointer::SmartPtr<TriggerBoxEntity> >::iterator iterA;
	float frameTime_ms = static_cast<float>( Utilities::Time::GetTimeElapsedThisFrame_ms() );

	FUNCTION_START;

	if( triggerBoxEntityDatabase->size() == 1 )
	{
		FUNCTION_FINISH;
		return;
	}

	std::vector< Utilities::Pointer::SmartPtr<TriggerBoxEntity> >::iterator iterB;
	for( iterA = triggerBoxEntityDatabase->begin(); iterA != triggerBoxEntityDatabase->end(); ++iterA )
	{
		iterB = iterA;
		for( ++iterB; iterB != triggerBoxEntityDatabase->end(); ++iterB )
		{
			CheckIntersection( **iterA, **iterB );
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End updating trigger box
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::TriggerBox::EndUpdate( void )
{
	FUNCTION_START;

	RemoveDeadEntities();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shutdown trigger box
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::TriggerBox::ShutDown( void )
{
	FUNCTION_START;

	if( triggerBoxEntityDatabase )
	{
		delete triggerBoxEntityDatabase;
		triggerBoxEntityDatabase = NULL;
	}

	if( TriggerBoxEntity::m_triggerBoxEntityPool )
	{
		delete TriggerBoxEntity::m_triggerBoxEntityPool;
		TriggerBoxEntity::m_triggerBoxEntityPool = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			UINT8 AddTriggerBoxEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
	\brief		Add trigger box entity to triggerBoxEntityDatabase
	\param		*i_entity entity to be added
	\param		i_centre the center of the bounding box
	\return		UINT8
	\retval		index of the new added trigger box entity in database
 ****************************************************************************************************
*/
UINT8 GameEngine::TriggerBox::AddTriggerBoxEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
{
	FUNCTION_START;

	triggerBoxEntityDatabase->push_back( new TriggerBoxEntity(i_entity) );
	triggerBoxEntityDatabase->at(triggerBoxEntityDatabase->size()-1)->m_triggerBoxHandler = NULL;

	FUNCTION_FINISH;
	return triggerBoxEntityDatabase->size() - 1;
}

/**
 ****************************************************************************************************
	\fn			void SetTriggerBoxHandle( const UINT8 &i_index, TriggerBoxHandler *i_TriggerBoxHandler )
	\brief		Set TriggerBox entity to triggerBoxEntityDatabase
	\param		*i_entity entity to be added
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::TriggerBox::SetTriggerBoxHandler( const UINT8 &i_index, TriggerBoxHandler *i_TriggerBoxHandler )
{
	FUNCTION_START;

	if( i_index < triggerBoxEntityDatabase->size() )
		triggerBoxEntityDatabase->at(i_index)->m_triggerBoxHandler = i_TriggerBoxHandler;

	FUNCTION_FINISH;
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
void GameEngine::TriggerBox::RemoveDeadEntities( void )
{
	FUNCTION_START;

	for( UINT8 i = 0; i < triggerBoxEntityDatabase->size(); i++ )
	{
		if( triggerBoxEntityDatabase->at(i)->m_entity->m_isDestroyed )
		{
			triggerBoxEntityDatabase->back()->m_entity->m_u8TriggerBoxEntityIndex = triggerBoxEntityDatabase->at(i)->m_entity->m_u8TriggerBoxEntityIndex;
			triggerBoxEntityDatabase->at(i) = triggerBoxEntityDatabase->back();
			triggerBoxEntityDatabase->pop_back();
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			float SeparationX( const BoundingBox &i_A, const BoundingBox &i_B )
	\brief		Calculate the distance between A and B in x axis
	\param		i_A first BoundingBox
	\param		i_B second BoundingBox
	\return		float
	\retval		the distance between i_A and i_B in x axis
 ****************************************************************************************************
*/
float GameEngine::TriggerBox::SeparationX( const BoundingBox &i_A, const BoundingBox &i_B )
{
	FUNCTION_START;

	FUNCTION_FINISH;
	return fabs( i_A.Center().X() - i_B.Center().X() ) - (i_A.m_halfW + i_B.m_halfW);
}

/**
 ****************************************************************************************************
	\fn			float SeparationY( const BoundingBox &i_A, const BoundingBox &i_B )
	\brief		Calculate the distance between A and B in y axis
	\param		i_A first BoundingBox
	\param		i_B second BoundingBox
	\return		float
	\retval		the distance between i_A and i_B in y axis
 ****************************************************************************************************
*/
float GameEngine::TriggerBox::SeparationY( const BoundingBox &i_A, const BoundingBox &i_B )
{
	FUNCTION_START;
	
	FUNCTION_FINISH;
	return fabs( i_A.Center().Y() - i_B.Center().Y() ) - (i_A.m_halfH + i_B.m_halfH );
}

/**
 ****************************************************************************************************
	\fn			float SeparationZ( const BoundingBox &i_A, const BoundingBox &i_B )
	\brief		Calculate the distance between A and B in z axis
	\param		i_A first BoundingBox
	\param		i_B second BoundingBox
	\return		float
	\retval		the distance between i_A and i_B in z axis
 ****************************************************************************************************
*/
float GameEngine::TriggerBox::SeparationZ( const BoundingBox &i_A, const BoundingBox &i_B )
{
	FUNCTION_START;

	FUNCTION_FINISH;
	return fabs( i_A.Center().Z() - i_B.Center().Z() ) - (i_A.m_halfD + i_B.m_halfD );
}

/**
 ****************************************************************************************************
	\fn			bool Overlap( const BoundingBox &i_A, const BoundingBox &i_B, Math::Vector3 &o_enterNormal )
	\brief		Check whether 2 objects are overlapping
	\param		i_A first BoundingBox
	\param		i_B second BoundingBox
	\param		o_enterNormal enter surface normal
	\return		bool
	\retval		TRUE if both objects are overlapping
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::TriggerBox::Overlap( const BoundingBox &i_A, const BoundingBox &i_B, Math::Vector3 &o_v3EnterNormal )
{
	float enterX = -SeparationX( i_A, i_B );
	float enterY = -SeparationY( i_A, i_B );
	float enterZ = -SeparationZ( i_A, i_B );

	FUNCTION_START;

	if( (-o_v3EnterNormal.X() > 0.0f) || (-o_v3EnterNormal.Y() > 0.0f) || (-o_v3EnterNormal.Z() > 0.0f) )
	{
		FUNCTION_FINISH;
		return false;
	}
	else
	{
		if( enterX < enterY )
		{
			if( enterX < enterZ )
			{
				if( i_B.Center().X() > i_A.Center().X() )
					o_v3EnterNormal = Math::Vector3::Right;
				else
					o_v3EnterNormal = Math::Vector3::Left;
			}
			else
			{
				if( i_B.Center().Z() > i_A.Center().Z() )
					o_v3EnterNormal = Math::Vector3::Forward;
				else
					o_v3EnterNormal = Math::Vector3::Backward;
			}
		}
		else if( enterY < enterZ )
		{
			if( i_B.Center().Y() > i_A.Center().Y() )
				o_v3EnterNormal = Math::Vector3::Up;
			else
				o_v3EnterNormal = Math::Vector3::Down;
		}
		else
		{
			if( i_B.Center().Z() > i_A.Center().Z() )
				o_v3EnterNormal = Math::Vector3::Forward;
			else
				o_v3EnterNormal = Math::Vector3::Backward;
		}

		FUNCTION_FINISH;
		return true;
	}
}

/**
 ****************************************************************************************************
	\fn			bool Overlap( const BoundingBox &i_A, const BoundingBox &i_B )
	\brief		Check whether 2 objects are overlapping
	\param		i_A first BoundingBox
	\param		i_B second BoundingBox
	\return		bool
	\retval		TRUE if both objects are overlapping
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::TriggerBox::Overlap( const BoundingBox &i_A, const BoundingBox &i_B )
{
	FUNCTION_START;

	if( (SeparationX(i_A, i_B) > 0.0f) || (SeparationY(i_A, i_B) > 0.0f) || (SeparationZ(i_A, i_B) > 0.0f) )
	{
		FUNCTION_FINISH;
		return false;
	}
	else
	{
		FUNCTION_FINISH;
		return true;
	}
}

/**
 ****************************************************************************************************
	\fn			void CheckTriggerBox( TriggerBoxEntity &i_A, TriggerBoxEntity &i_B )
	\brief		Move entities within boundary
	\param		i_index index of entity in interest
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::TriggerBox::CheckIntersection( TriggerBoxEntity &i_A, TriggerBoxEntity &i_B )
{
	bool bIntersection = false;
	float enterTime = 0.0f;
	Math::Vector3 enterNormal = Math::Vector3::Zero;
	BoundingBox i_ABB( i_A.m_entity->m_v3Position, i_A.m_entity->m_size );
	BoundingBox i_BBB( i_B.m_entity->m_v3Position, i_B.m_entity->m_size );

	FUNCTION_START;

	// If i_A do not have collision with other object or if either of the object is to be destroyed in the end 
	// of this frame do not do collision detection
	if( (i_A.m_entity->m_u32CollisionMask == 0) || i_A.m_entity->m_isDestroyed || i_B.m_entity->m_isDestroyed )
	{
		FUNCTION_FINISH;
		return;
	}

	if( i_A.m_entity->m_u32CollisionMask & g_IDCreator::Get().IDtoBitMask(i_B.m_entity->m_u8EntityID) )
	{
		// Both objects are moving
		if( (i_A.m_entity->m_v3Velocity.SquaredLength() > 0.0f) && (i_B.m_entity->m_v3Velocity.SquaredLength() > 0.0f) )
		{
			Math::Vector3 velocityBA = i_B.m_entity->m_v3Velocity - i_A.m_entity->m_v3Velocity;

			if( Utilities::Math::AreRelativelyEqual(velocityBA.SquaredLength(), 0.0f) )
				bIntersection = Overlap( i_ABB, i_BBB );
			else
				bIntersection = MovingToStationary( i_ABB, i_BBB, velocityBA, enterTime, enterNormal );
		
		}
		// A is moving, B is stationary
		else if( i_A.m_entity->m_v3Velocity.SquaredLength() > 0.0f )
		{
			bIntersection = MovingToStationary( i_BBB, i_ABB, i_A.m_entity->m_v3Velocity, enterTime, enterNormal );
		}
		// B is moving, A is stationary
		else if( i_B.m_entity->m_v3Velocity.SquaredLength() > 0.0f )
		{
			bIntersection = MovingToStationary( i_ABB, i_BBB, i_B.m_entity->m_v3Velocity, enterTime, enterNormal );
		}
		// Both are stationary
		else
		{
			bIntersection = Overlap( i_ABB, i_BBB );
		}
	}

	if( bIntersection )
	{
		if( i_A.m_triggerBoxHandler && !i_A.m_entity->m_isDestroyed && !i_B.m_entity->m_isDestroyed )
		{
			i_A.m_intersectedEntity = i_B.m_entity;
			i_A.m_triggerBoxHandler->HandleIntersection( i_A.m_entity, i_B.m_entity, enterTime, enterNormal );
		}
	}
	else
	{
		if( i_A.m_intersectedEntity && (i_A.m_intersectedEntity == i_B.m_entity) )
		{
			if( i_A.m_triggerBoxHandler != NULL )
				i_A.m_triggerBoxHandler->OnLeavingTriggerBox( i_A.m_entity, i_B.m_entity );
			i_A.m_intersectedEntity = NULL;
		}
		i_A.m_enterTime = 0.0f;

		if( i_B.m_intersectedEntity && (i_B.m_intersectedEntity == i_A.m_entity) )
		{
			if( i_B.m_triggerBoxHandler != NULL )
				i_B.m_triggerBoxHandler->OnLeavingTriggerBox( i_B.m_entity, i_A.m_entity );
			i_B.m_intersectedEntity = NULL;
		}
		i_B.m_enterTime = 0.0f;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool MovingToStationary( const BoundingBox &i_stationary, const BoundingBox &i_moving, const Math::Vector3 &i_velocity, float &o_enterTime, Math::Vector3 &o_enterSurfaceNormal )
	\brief		Check the TriggerBox of stationary object agains moving object
	\param		i_stationary stationary object
	\param		i_moving moving object
	\param		i_velocity velocity of the moving object
	\param		o_enterTime output of entering time
	\param		o_enterSurfaceNormal normal of enter plane
	\return		bool
	\retval		TRUE if both objects are overlapping
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::TriggerBox::MovingToStationary( const BoundingBox &i_stationary, const BoundingBox &i_moving, \
	const Math::Vector3 &i_v3Velocity, float &o_enterTime, Math::Vector3 &o_enterSurfaceNormal )
{
	float enterTimeX;
	float normalDirectionX;
	float enterTimeY;
	float normalDirectionY;
	float enterTimeZ;
	float normalDirectionZ;

	FUNCTION_START;

	assert( i_v3Velocity.SquaredLength() > 0.0f );

	BoundingBox newStationary( i_stationary.Center(), i_stationary.m_halfW + i_moving.m_halfW, i_stationary.m_halfH + i_moving.m_halfH, i_stationary.m_halfD + i_moving.m_halfD );
	BoundingBox::S_EXTENDS stationaryExtends = newStationary.GetExtends();

	if( !MovingCheckInAxis(stationaryExtends.minX, stationaryExtends.maxX, i_v3Velocity.X(), i_moving.Center().X(), enterTimeX, normalDirectionX) )
	{
		//DEBUG_MSG( DBG_TriggerBox, D_INFO, "Not colliding in X\n" );
		FUNCTION_FINISH;
		return false;
	}

	if( !MovingCheckInAxis(stationaryExtends.minY, stationaryExtends.maxY, i_v3Velocity.Y(), i_moving.Center().Y(), enterTimeY, normalDirectionY) )
	{
		//DEBUG_MSG( DBG_TriggerBox, D_INFO, "Not colliding in Y\n" );
		FUNCTION_FINISH;
		return false;
	}

	if( !MovingCheckInAxis(stationaryExtends.minZ, stationaryExtends.maxZ, i_v3Velocity.Z(), i_moving.Center().Z(), enterTimeZ, normalDirectionZ) )
	{
		//DEBUG_MSG( DBG_TriggerBox, D_INFO, "Not colliding in Z\n" );
		FUNCTION_FINISH;
		return false;
	}

	if( enterTimeX < enterTimeY )
	{
		if( enterTimeY < enterTimeZ )
		{
			o_enterTime = enterTimeZ;
			o_enterSurfaceNormal = Math::Vector3::Forward * normalDirectionZ;
		}
		else
		{
			o_enterTime = enterTimeY;
			o_enterSurfaceNormal = Math::Vector3::Up * normalDirectionY;
		}
	}
	else if( enterTimeX < enterTimeZ )
	{
		o_enterTime = enterTimeZ;
		o_enterSurfaceNormal = Math::Vector3::Forward * normalDirectionZ;
	}
	else
	{
		o_enterTime = enterTimeX;
		o_enterSurfaceNormal = Math::Vector3::Right * normalDirectionX;
	}

	FUNCTION_FINISH;
	return true;
}

/**
 ****************************************************************************************************
	\fn			bool MovingCheckInAxis( const float &i_min, const float &i_max, const float i_velocity, const float &i_start, float &o_enterTime, float &o_normalDirection )
	\brief		Check the TriggerBox of moving object in distinguished axis
	\param		i_min minimum axis
	\param		i_max maximum axis
	\param		i_velocity velocity in the axis
	\param		i_start start position in the axis
	\param		o_enterTime enter time
	\param		o_normalDirection normal direction of enterred plane
	\return		bool
	\retval		TRUE if both objects are overlapping
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::TriggerBox::MovingCheckInAxis( const float &i_min, const float &i_max, \
	const float i_velocity, const float &i_start, float &o_enterTime, float &o_normalDirection )
{
	o_enterTime = 0.0f;
	float leaveTime = 0.0f;
	float frameTime_ms = static_cast<float>( Utilities::Time::GetTimeElapsedThisFrame_ms() );

	//DEBUG_MSG( DBG_TriggerBox, D_INFO, "Time %.5f\n", frameTime_ms );

	FUNCTION_START;

	if( !Utilities::Math::AreRelativelyEqual(i_velocity, 0.0f) )
	{
		o_enterTime = (i_min - i_start) / i_velocity;
		leaveTime = (i_max - i_start) / i_velocity;

		if( leaveTime < o_enterTime )
		{
			std::swap( o_enterTime, leaveTime );
			o_normalDirection = 1.0f;
		}
		else
			o_normalDirection = -1.0f;

		//DEBUG_MSG( DBG_TriggerBox, D_INFO, "min %.5f max %.5f start %.5f velocity %.5f enter time: %.5f leave time: %.5f\n", i_min, i_max, i_start, i_velocity, o_enterTime, leaveTime );

		if( ((o_enterTime >= 0.0f) && (o_enterTime <= frameTime_ms))
			|| ((o_enterTime < 0.0f) && (leaveTime > 0.0f))
			)
		{
			//DEBUG_MSG( DBG_TriggerBox, D_INFO, "Case 2" );
			FUNCTION_FINISH;
			return true;
		}
	}
	else
	{
		//DEBUG_MSG( DBG_TriggerBox, D_INFO, "No velocity\n" );
		//DEBUG_MSG( DBG_TriggerBox, D_INFO, "min %.5f max %.5f start %.5f velocity %.5f\n", i_min, i_max, i_start, i_velocity );

		if( (i_start >= i_min) && (i_start <= i_max) )
		{
			o_enterTime = 0;

			FUNCTION_FINISH;
			return true;
		}
	}

	FUNCTION_FINISH;
	return false;
}

/****************************************************************************************************
			TriggerBoxEntityHandler class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void HandleTriggerBox( Pointer::SmartPtr<Entity> &i_entity, Pointer::SmartPtr<Entity> &i_other, const float i_TriggerBoxTime, const Math::Vector3 &i_TriggerBoxNormal )
	\brief		Activate trigger box action when TriggerBox happen
	\param		i_entity main entity
	\param		i_other the other entity that colliding
	\param		i_triggerBoxTime collided time
	\param		i_v3TriggerBoxNormal enter surface normal
	\return		float
	\retval		total elapsed time in seconds
 ****************************************************************************************************
*/
void GameEngine::TriggerBox::TriggerBoxHandler::HandleIntersection( Utilities::Pointer::SmartPtr<Entity> &i_entity, Utilities::Pointer::SmartPtr<Entity> &i_other,
	const float i_triggerBoxTime, const Math::Vector3 &i_v3TriggerBoxNormal )
{
	FUNCTION_START;

	if( i_entity->m_u32CollisionMask & g_IDCreator::Get().IDtoBitMask(i_other->m_u8EntityID) )
	{
		if( _u32CollidedObjectID == 0 || !(_u32CollidedObjectID & g_IDCreator::Get().IDtoBitMask(i_other->m_u8EntityID)) )
		{
			_u32CollidedObjectID |= g_IDCreator::Get().IDtoBitMask( i_other->m_u8EntityID );
			OnEnter( i_other, i_triggerBoxTime, i_v3TriggerBoxNormal );
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void OnLeavingCollision( Pointer::SmartPtr<Entity> &i_entity, Pointer::SmartPtr<Entity> &i_other )
	\brief		Activate trigger box action on leaving TriggerBox
	\param		i_entity main entity
	\param		i_other the other entity that was colliding
	\return		float
	\retval		total elapsed time in seconds
 ****************************************************************************************************
*/
void GameEngine::TriggerBox::TriggerBoxHandler::OnLeavingTriggerBox( Utilities::Pointer::SmartPtr<Entity> &i_entity, Utilities::Pointer::SmartPtr<Entity> &i_other )
{
	FUNCTION_START;

	if( _u32CollidedObjectID & g_IDCreator::Get().IDtoBitMask(i_other->m_u8EntityID) )
	{
		_u32CollidedObjectID ^= g_IDCreator::Get().IDtoBitMask( i_other->m_u8EntityID );
		OnLeave( i_other );
	}

	FUNCTION_FINISH;
}

/****************************************************************************************************
			TriggerBoxEntity class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			TriggerBoxEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
	\brief		Default constructor of TriggerBoxEntity
	\param		i_entity pointer to common entity data
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::TriggerBox::TriggerBoxEntity::TriggerBoxEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity ) :
	m_intersectedEntity( NULL ),
	m_entity( i_entity ),
	m_triggerBoxHandler( NULL ),
	m_triggerBoxDistance( 0.0f ),
	m_enterTime( 0.0f )
{
}

/**
 ****************************************************************************************************
	\fn			~TriggerBoxEntity( void )
	\brief		Default destructor of TriggerBoxEntity
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::TriggerBox::TriggerBoxEntity::~TriggerBoxEntity( void )
{
	FUNCTION_START;

	m_entity = NULL;
	m_intersectedEntity = NULL;

	if( m_triggerBoxHandler )
	{
		delete m_triggerBoxHandler;
		m_triggerBoxHandler = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void *operator new( const size_t &i_size )
	\brief		New operator of TriggerBoxEntity class
	\param		i_size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
void *GameEngine::TriggerBox::TriggerBoxEntity::operator new( const size_t &i_size )
{
	FUNCTION_START;

	assert( i_size == sizeof(TriggerBoxEntity) );
	assert( m_triggerBoxEntityPool );

	FUNCTION_FINISH;	
	return m_triggerBoxEntityPool->Allocate( sizeof(TriggerBoxEntity) );
}

/**
 ****************************************************************************************************
	\fn			void operator delete( void *i_ptr )
	\brief		Delete operator of TriggerBoxEntity class
	\param		i_ptr pointer to be deleted
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::TriggerBox::TriggerBoxEntity::operator delete( void *i_ptr )
{
	FUNCTION_START;

	assert( m_triggerBoxEntityPool );
				
	if( i_ptr )
		m_triggerBoxEntityPool->Deallocate( i_ptr );

	FUNCTION_FINISH;
}