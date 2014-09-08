/**
 ****************************************************************************************************
 * \file		AI.cpp
 * \brief		AI engine implementation
 ****************************************************************************************************
*/

#include <map>
#include <set>
#include <deque>
#include <assert.h>

// Utilities header
#include <Time/Time.h>
#include <Debug/Debug.h>
#include <UtilitiesDefault.h>
#include <SmartPtr/SmartPtr.h>
#include <MemoryPool/MemoryPool.h>

#include "AI.h"
#include "DebugMenu/DebugMenu.h"

/****************************************************************************************************
			Class / variable declaration
****************************************************************************************************/
namespace GameEngine
{
	namespace AI
	{
		typedef enum _e_AI_state_
		{
			E_AI_STATE_DEACTIVATE = 0,
			E_AI_STATE_GO_TO_TARGET_NODE,
			E_AI_STATE_ARRIVED_AT_TARGET_NODE,
			E_AI_STATE_PATH_FINDING,
			E_AI_STATE_MAX
		} E_AI_STATE;

		typedef struct _s_way_point_open_list_node_
		{
			UINT32 u32ID;
			UINT32 u32ParentID;
			float g;
			float h;
		} S_WAY_POINT_OPEN_LIST_NODE;

		typedef struct _s_way_point_open_list_node_by_f_less
		{
			bool operator()( const S_WAY_POINT_OPEN_LIST_NODE &i_lhs, const S_WAY_POINT_OPEN_LIST_NODE &i_rhs )
			{
				if( (i_lhs.g + i_lhs.h) < (i_rhs.g + i_rhs.h) )
					return TRUE;
				else if( (i_lhs.g + i_lhs.h) > (i_rhs.g + i_rhs.h) )
					return FALSE;
				else
					return i_lhs.u32ID < i_rhs.u32ID;
			}
		} S_WAY_POINT_OPEN_LIST_NODE_BY_F_LESS;

		typedef struct _s_way_point_open_list_node_by_number_less
		{
			bool operator()( const S_WAY_POINT_OPEN_LIST_NODE &i_lhs, const S_WAY_POINT_OPEN_LIST_NODE &i_rhs )
			{
				return i_lhs.u32ID < i_rhs.u32ID;
			}
		} S_WAY_POINT_OPEN_LIST_NODE_BY_NUMBER_LESS;

		class AIEntity
		{
		public:
			static Utilities::MemoryPool					*m_AIEntityPool;

			Utilities::Pointer::SmartPtr<Entity>	m_entity;
			std::deque<UINT32> *m_optimalPath;
			E_AI_STATE m_AIState;
			UINT32 m_u32TargetNodeID;

			AIEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity );
			~AIEntity( void );

			void *operator new( const size_t &i_size );
			void operator delete( void *i_ptr );

		private:
			std::set<UINT32> *_optimalPathNodeID;

			AIEntity( void ) {}

			// Make it non-copyable
			AIEntity( AIEntity const &i_other );
			AIEntity &operator=( const AIEntity &i_other );

			// Make it incomparable
			bool &operator==( const AIEntity &i_other ) const;
		};

		bool bShowAIWayPoint;
		bool bShowOptimalAIWayPoint;
		//bool bActivateAI;

		static std::vector< Utilities::Pointer::SmartPtr<AIEntity> > *AIEntityDatabase;
		void RemoveDeadEntities( void );
		bool FindOptimalPath( const UINT32 &i_u32FromNodeID, const UINT32 &i_u32ToNodeID, std::deque<UINT32> &o_path );

		std::map<UINT32, S_WAY_POINT> *wayPointList = NULL;
		std::set<S_WAY_POINT_LINK, S_WAY_POINT_COMPARE> *wayPointLinkList = NULL;
	}
}

Utilities::MemoryPool *GameEngine::AI::AIEntity::m_AIEntityPool = NULL;

/****************************************************************************************************
			Global functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize AI
	\param		NONE
	\return		BOOLEAN
	\retval		SUCCESS if success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool GameEngine::AI::Initialize( void )
{
	FUNCTION_START;

	assert( AIEntity::m_AIEntityPool == NULL );

	AIEntity::m_AIEntityPool = Utilities::MemoryPool::Create( sizeof(AIEntity), Utilities::DEFAULT_MEMORY_POOL_SIZE );
	AIEntityDatabase = new std::vector< Utilities::Pointer::SmartPtr<AIEntity> >;

	assert( wayPointList == NULL );
	wayPointList = new std::map<UINT32, S_WAY_POINT>();

	assert( wayPointLinkList == NULL );
	wayPointLinkList = new std::set<S_WAY_POINT_LINK, S_WAY_POINT_COMPARE>();

#ifdef ENABLE_WAY_POINT_DISPLAY
	g_debugMenu::Get().AddCheckBox( "Show AI way point node", bShowAIWayPoint );
	g_debugMenu::Get().AddCheckBox( "Show optimal AI way point node", bShowOptimalAIWayPoint );
	//g_debugMenu::Get().AddCheckBox( "Activate AI", bActivateAI );
#endif

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin updating AI
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::BeginUpdate( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update AI
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::Update( void )
{
	std::vector<Utilities::Pointer::SmartPtr<AIEntity>>::iterator iter;

	FUNCTION_START;

	if( bShowAIWayPoint )
	{
		std::map<UINT32, S_WAY_POINT>::const_iterator wayPointIter = wayPointList->begin();
		for( ; wayPointIter != wayPointList->end(); ++wayPointIter )
			g_debugMenu::Get().DrawSphere( wayPointIter->second.centre, wayPointIter->second.radius );

		std::set<S_WAY_POINT_LINK, S_WAY_POINT_COMPARE>::const_iterator wayPointLinkIter = wayPointLinkList->begin();
		for( ; wayPointLinkIter != wayPointLinkList->end(); ++wayPointLinkIter )
		{
			S_WAY_POINT fromWayPoint = (wayPointList->find(wayPointLinkIter->u32From))->second;
			S_WAY_POINT toWayPoint = (wayPointList->find(wayPointLinkIter->u32To))->second;
			g_debugMenu::Get().DrawLine( fromWayPoint.centre, toWayPoint.centre );
		}
	}

	for( iter = AIEntityDatabase->begin(); iter != AIEntityDatabase->end(); ++iter )
	{
		bool bMove = false;

		if( bShowOptimalAIWayPoint )
		{
			std::deque<UINT32>::const_iterator optimalPathIter = (*iter)->m_optimalPath->begin();
			for( ; optimalPathIter != (*iter)->m_optimalPath->end(); ++optimalPathIter )
			{
				D3DXVECTOR3 startPoint = wayPointList->find((*optimalPathIter))->second.centre;
				++optimalPathIter;
				D3DXVECTOR3 endPoint = startPoint;
				if( optimalPathIter != (*iter)->m_optimalPath->end() )
					 endPoint = wayPointList->find((*optimalPathIter))->second.centre;
				--optimalPathIter;
				g_debugMenu::Get().DrawLine( startPoint, endPoint, Utilities::WHITE );
			}
		}

		switch( (*iter)->m_AIState )
		{
		default:
		case E_AI_STATE_DEACTIVATE:
			break;

		case E_AI_STATE_GO_TO_TARGET_NODE:
			bMove = true;
			break;

		case E_AI_STATE_ARRIVED_AT_TARGET_NODE:
			if( !(*iter)->m_optimalPath->empty() )
			{
				UINT32 u32OldTargetNodeID = (*iter)->m_u32TargetNodeID;
				(*iter)->m_u32TargetNodeID = (*iter)->m_optimalPath->front();
				(*iter)->m_optimalPath->pop_front();

				// If previous target is the same as current, get a new one
				if( u32OldTargetNodeID == (*iter)->m_u32TargetNodeID && !(*iter)->m_optimalPath->empty() )
				{
					(*iter)->m_u32TargetNodeID = (*iter)->m_optimalPath->front();
					(*iter)->m_optimalPath->pop_front();
				}
				(*iter)->m_AIState = E_AI_STATE_GO_TO_TARGET_NODE;
				bMove = true;
			}
			else
			{
				(*iter)->m_AIState = E_AI_STATE_DEACTIVATE;
			}
			break;
		}

		if( bMove )
		{
			if( ((*iter)->m_u32TargetNodeID != Utilities::MAX_UINT32) && \
				((*iter)->m_u32TargetNodeID < wayPointList->size())
			)
			{
				std::map<UINT32, S_WAY_POINT>::const_iterator wayPoint = wayPointList->find((*iter)->m_u32TargetNodeID);
				D3DXVECTOR3 entityPosition = D3DXVECTOR3((*iter)->m_entity->m_v3Position.X(), \
					(*iter)->m_entity->m_v3Position.Y(), (*iter)->m_entity->m_v3Position.Z());

				D3DXVECTOR3 direction = wayPoint->second.centre - entityPosition;
				if( Utilities::Math::AreWithinRange(entityPosition, wayPoint->second.centre, Utilities::Time::GetTimeElapsedThisFrame_ms()) )
				{
					direction = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
					(*iter)->m_AIState = E_AI_STATE_ARRIVED_AT_TARGET_NODE;
				}
				else
					D3DXVec3Normalize( &direction, &direction );

				(*iter)->m_entity->m_v3Velocity = direction;
			}
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End updating AI
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::EndUpdate( void )
{
	FUNCTION_START;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shutdown AI
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::ShutDown( void )
{
	FUNCTION_START;

	if( AIEntityDatabase )
	{
		delete AIEntityDatabase;
		AIEntityDatabase = NULL;
	}

	if( AIEntity::m_AIEntityPool )
	{
		delete AIEntity::m_AIEntityPool;
		AIEntity::m_AIEntityPool = NULL;
	}

	if( wayPointList )
	{
		delete wayPointList;
		wayPointList = NULL;
	}

	if( wayPointLinkList )
	{
		delete wayPointLinkList;
		wayPointLinkList = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void AddWayPoint( const UINT32 &i_u32ID, const S_WAY_POINT &i_wayPoint )
	\brief		Add way point to the list
	\param		i_u32ID ID of the new way point
	\param		i_wayPoint way point ot be added
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::AddWayPoint( const UINT32 &i_u32ID, const S_WAY_POINT &i_wayPoint )
{
	FUNCTION_START;

	std::pair<UINT32, S_WAY_POINT> insertingPair( i_u32ID, i_wayPoint );
	wayPointList->insert( insertingPair );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void AddWayPointLink( const S_WAY_POINT_LINK &i_newWayPointLink )
	\brief		Add way point link
	\param		i_newWayPointLink new point link to be added
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::AddWayPointLink( const S_WAY_POINT_LINK &i_newWayPointLink )
{
	FUNCTION_START;

	wayPointLinkList->insert( i_newWayPointLink );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			UINT8 AddAIEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
	\brief		Add trigger box entity to AIEntityDatabase
	\param		*i_entity entity to be added
	\param		i_centre the center of the bounding box
	\return		UINT8
	\retval		index of the new added trigger box entity in database
 ****************************************************************************************************
*/
UINT8 GameEngine::AI::AddAIEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
{
	FUNCTION_START;

	AIEntityDatabase->push_back( new AIEntity(i_entity) );

	FUNCTION_FINISH;
	return AIEntityDatabase->size() - 1;
}

void GameEngine::AI::RemoveAIEntity( UINT8 &i_u8Index )
{
	if( i_u8Index < AIEntityDatabase->size() )
	{
		std::vector< Utilities::Pointer::SmartPtr<AIEntity> >::iterator AIEntityDatabaseIter;
		AIEntityDatabaseIter = AIEntityDatabase->begin();
		UINT8 i = 0;
		while( i < i_u8Index )
		{
			++i;
			++AIEntityDatabaseIter;
		}
		AIEntityDatabase->erase( AIEntityDatabaseIter );
	}
}

/**
 ****************************************************************************************************
	\fn			void UpdateAIDestinationTo( UINT8 &i_u8Index, const UINT8 &i_u8NodeID )
	\brief		Update destination for this AI
	\param		i_u8Index index of entity in AI entity
	\param		i_u8NodeID destination node ID
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::UpdateAIDestinationTo( const UINT8 &i_u8Index, const UINT8 &i_u8NodeID )
{
	FUNCTION_START;

	if( AIEntityDatabase->at(i_u8Index)->m_AIState != E_AI_STATE_DEACTIVATE )
		AbortAI( i_u8Index );

	if( i_u8Index < AIEntityDatabase->size() )
	{
		UINT32 u32ClosestNodeID = Utilities::MAX_UINT32;
		D3DXVECTOR3 entityPosition( AIEntityDatabase->at(i_u8Index)->m_entity->m_v3Position.X(),
			AIEntityDatabase->at(i_u8Index)->m_entity->m_v3Position.Y(),
			AIEntityDatabase->at(i_u8Index)->m_entity->m_v3Position.Z() );

		FindClosestNodeIDFromPosition( entityPosition, u32ClosestNodeID );
		if( u32ClosestNodeID < wayPointList->size() )
		{
			AIEntityDatabase->at(i_u8Index)->m_u32TargetNodeID = u32ClosestNodeID;
			AIEntityDatabase->at(i_u8Index)->m_AIState = E_AI_STATE_GO_TO_TARGET_NODE;
			FindOptimalPath( u32ClosestNodeID, i_u8NodeID, *(AIEntityDatabase->at(i_u8Index)->m_optimalPath) );
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void AbortAI( const UINT8 &i_u8Index )
	\brief		Abort AI
	\param		i_u8Index index of entity in AI entity
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::AbortAI( const UINT8 &i_u8Index )
{
	FUNCTION_START;

	if( i_u8Index < AIEntityDatabase->size() )
	{
		AIEntityDatabase->at(i_u8Index)->m_AIState = E_AI_STATE_DEACTIVATE;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool GetAIState( const UINT8 &i_u8Index )
	\brief		Get current AI state
	\param		i_u8Index index of entity in AI entity
	\return		BOOLEAN
	\retval		TRUE if activated
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::AI::GetAIState( const UINT8 &i_u8Index )
{
	FUNCTION_START;

	if( i_u8Index < AIEntityDatabase->size() )
	{
		if( AIEntityDatabase->at(i_u8Index)->m_AIState == E_AI_STATE_DEACTIVATE )
			return FALSE;
		return TRUE;
	}

	FUNCTION_FINISH;
	return FALSE;
}

/**
 ****************************************************************************************************
	\fn			void FindClosestNodeID( const D3DXVECTOR3 &i_vCurrPosition, UINT32 &o_u32NodeID )
	\brief		Find closest node ID
	\param		*i_vCurrPosition current entity position
	\param		o_u32NodeID the closest node ID
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::FindClosestNodeIDFromPosition( const D3DXVECTOR3 &i_vCurrPosition, UINT32 &o_u32NodeID )
{
	float currClosestDistance = 100000.0f;
	float testingDistance = 0.0f;
	D3DXVECTOR3 distanceVector;

	FUNCTION_START;

	std::map<UINT32, S_WAY_POINT>::const_iterator iter;
	for( iter = wayPointList->begin(); iter != wayPointList->end(); ++iter )
	{
		distanceVector = i_vCurrPosition - iter->second.centre;
		testingDistance = D3DXVec3Length( &distanceVector );
		if( testingDistance < currClosestDistance )
		{
			currClosestDistance = testingDistance;
			o_u32NodeID = iter->first;
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			float FindSquaredDistanceToNodeID( const D3DXVECTOR3 &i_vCurrPosition, const UINT32 &i_u32NodeID )
	\brief		Find squared distance from position to node ID
	\param		*i_vCurrPosition current entity position
	\param		i_u32NodeID the node ID
	\return		float
	\retval		Squared distance to the given node
 ****************************************************************************************************
*/
float GameEngine::AI::FindDistanceToNodeID( const D3DXVECTOR3 &i_vCurrPosition, const UINT32 &i_u32NodeID )
{
	UINT32 u32ClosestNodeID;
	float returnDistance = 0.0f;

	FUNCTION_START;

	FindClosestNodeIDFromPosition( i_vCurrPosition, u32ClosestNodeID );
	if( u32ClosestNodeID < wayPointList->size() )
	{
		D3DXVECTOR3 distance;
		distance = wayPointList->at(u32ClosestNodeID).centre - i_vCurrPosition;
		returnDistance = D3DXVec3Length( &distance );

		std::deque<UINT32> path;
		bool bResult = FindOptimalPath( u32ClosestNodeID, i_u32NodeID, path );

		if( bResult )
		{
			std::deque<UINT32>::const_iterator pathIter = path.begin();
			for( ; pathIter != path.end(); ++pathIter )
			{
				D3DXVECTOR3 startPoint = wayPointList->find((*pathIter))->second.centre;
				D3DXVECTOR3 endPoint = startPoint;
				++pathIter;
				if( pathIter != path.end() )
					endPoint = wayPointList->find((*pathIter))->second.centre;
				--pathIter;
				distance = endPoint - startPoint;
				returnDistance += D3DXVec3Length( &distance );
			}
		}
	}

	FUNCTION_FINISH;
	return returnDistance;
}

/****************************************************************************************************
			Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void RemoveDeadEntities( void )
	\brief		Remove dead entities
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::RemoveDeadEntities( void )
{
	FUNCTION_START;

	for( UINT8 i = 0; i < AIEntityDatabase->size(); ++i )
	{
		if( AIEntityDatabase->at(i)->m_entity->m_isDestroyed )
		{
			AIEntityDatabase->back()->m_entity->m_u8AIEntityIndex = AIEntityDatabase->at(i)->m_entity->m_u8AIEntityIndex;
			AIEntityDatabase->at(i) = AIEntityDatabase->back();
			AIEntityDatabase->pop_back();
		}
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool FindOptimalPath( const UINT32 &i_u32FromNodeID, const UINT32 &i_u32ToNodeID, std::deque<UINT32> &o_path )
	\brief		Find optimal path from one node ID to another node ID
	\param		i_u32FromNodeID start node ID
	\param		i_u32ToNodeID destination node ID
	\param		o_path returned path
	\return		BOOLEAN
	\retval		TRUE if success
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
bool GameEngine::AI::FindOptimalPath( const UINT32 &i_u32FromNodeID, const UINT32 &i_u32ToNodeID, std::deque<UINT32> &o_path )
{
	std::map<UINT32, UINT32> closedList;
	std::pair<UINT32, UINT32> closedListInsertingPair;
	std::map<UINT32, UINT32>::const_iterator closedListIter;

	std::set<S_WAY_POINT_OPEN_LIST_NODE, S_WAY_POINT_OPEN_LIST_NODE_BY_F_LESS> openListByF;
	std::set<S_WAY_POINT_OPEN_LIST_NODE, S_WAY_POINT_OPEN_LIST_NODE_BY_F_LESS>::iterator openListByFIter;

	std::set<S_WAY_POINT_OPEN_LIST_NODE, S_WAY_POINT_OPEN_LIST_NODE_BY_NUMBER_LESS> openListByNumber;
	std::set<S_WAY_POINT_OPEN_LIST_NODE, S_WAY_POINT_OPEN_LIST_NODE_BY_NUMBER_LESS>::iterator openListByNumberIter;

	std::map<UINT32, S_WAY_POINT>::const_iterator wayPointIter = wayPointList->find( i_u32FromNodeID );

	FUNCTION_START;

	if( wayPointIter == wayPointList->end() )
	{
		FUNCTION_FINISH;
		return FALSE;
	}

	S_WAY_POINT_OPEN_LIST_NODE currentNode;
	S_WAY_POINT_OPEN_LIST_NODE insertingNode;
	insertingNode.h = 0.0f;
	currentNode.g = 0.0f;
	currentNode.h = 0.0f;
	currentNode.u32ID = i_u32FromNodeID;
	currentNode.u32ParentID = Utilities::MAX_UINT32;
	openListByF.insert( currentNode );
	openListByNumber.insert( currentNode );
	std::set<S_WAY_POINT_LINK, S_WAY_POINT_COMPARE>::const_iterator wayPointLinkIter;
	S_WAY_POINT_LINK link;
	//std::map<UINT32, UINT32>::iterator locationInHeapIter;

	while( !openListByF.empty() )
	{
		currentNode = *openListByF.begin();
		if( currentNode.u32ID == i_u32ToNodeID )
			break;
		openListByF.erase( openListByF.begin() );
		openListByNumber.erase( currentNode );
		closedListInsertingPair.first = currentNode.u32ID;
		closedListInsertingPair.second = currentNode.u32ParentID;
		closedList.insert( closedListInsertingPair );
		link.u32From = currentNode.u32ID;
		link.u32To = 0;

		wayPointLinkIter = wayPointLinkList->lower_bound( link );

		if( wayPointLinkIter == wayPointLinkList->end() )
			continue;

		while( wayPointLinkIter->u32From == currentNode.u32ID )
		{
			// Is it on the closed list?
			if( closedList.find(wayPointLinkIter->u32To) != closedList.end() )
			{
				++wayPointLinkIter;
				continue;
			}

			insertingNode.u32ID = wayPointLinkIter->u32To;
			openListByNumberIter = openListByNumber.find( insertingNode );
			// Not in the open list yet
			if( openListByNumberIter == openListByNumber.end() )
			{
				insertingNode.g = currentNode.g + 0.0f;
				insertingNode.u32ID = wayPointLinkIter->u32To;
				insertingNode.u32ParentID = currentNode.u32ID;
				openListByNumber.insert( insertingNode );
				openListByF.insert( insertingNode );
			}
			// Already in the open list
			else
			{
				if( (currentNode.g + 0.0f + currentNode.h) < (openListByNumberIter->g + openListByNumberIter->h) )
				{
					insertingNode = *openListByNumberIter;
					openListByF.erase( insertingNode );
					openListByNumber.erase( openListByNumberIter );
					insertingNode.g = currentNode.g + 0.0f;
					openListByNumber.insert( insertingNode );
					openListByF.insert( insertingNode );
				}
			}
			++wayPointLinkIter;
		}
	}

	if( currentNode.u32ID == i_u32ToNodeID )
	{
		o_path.clear();
		o_path.push_front( i_u32ToNodeID );
		closedListIter = closedList.find( currentNode.u32ParentID );
		while( (closedListIter != closedList.end()) && (closedListIter->second != Utilities::MAX_UINT32) )
		{
			o_path.push_front( closedListIter->first );
			closedListIter = closedList.find( closedListIter->second );
			if( closedListIter == closedList.end() )
				return FALSE;
		}
		if( i_u32ToNodeID != i_u32FromNodeID )
			o_path.push_front( i_u32FromNodeID );
		return TRUE;
	}

	FUNCTION_FINISH;
	return FALSE;
}

/****************************************************************************************************
			AIEntity class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			AIEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
	\brief		Default constructor of AIEntity
	\param		i_entity pointer to common entity data
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::AI::AIEntity::AIEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity ) :
	m_entity( i_entity ),
	m_AIState(E_AI_STATE_DEACTIVATE),
	m_u32TargetNodeID(Utilities::MAX_UINT32)
{
	m_optimalPath = new std::deque<UINT32>();
	_optimalPathNodeID = new std::set<UINT32>();
}

/**
 ****************************************************************************************************
	\fn			~AIEntity( void )
	\brief		Default destructor of AIEntity
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::AI::AIEntity::~AIEntity( void )
{
	FUNCTION_START;

	m_entity = NULL;

	if( m_optimalPath )
	{
		delete m_optimalPath;
		m_optimalPath = NULL;
	}

	if( _optimalPathNodeID )
	{
		delete _optimalPathNodeID;
		_optimalPathNodeID = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void *operator new( const size_t &i_size )
	\brief		New operator of AIEntity class
	\param		i_size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
void *GameEngine::AI::AIEntity::operator new( const size_t &i_size )
{
	FUNCTION_START;

	assert( i_size == sizeof(AIEntity) );
	assert( m_AIEntityPool );

	FUNCTION_FINISH;	
	return m_AIEntityPool->Allocate( sizeof(AIEntity) );
}

/**
 ****************************************************************************************************
	\fn			void operator delete( void *i_ptr )
	\brief		Delete operator of AIEntity class
	\param		i_ptr pointer to be deleted
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::AI::AIEntity::operator delete( void *i_ptr )
{
	FUNCTION_START;

	assert( m_AIEntityPool );
				
	if( i_ptr )
		m_AIEntityPool->Deallocate( i_ptr );

	FUNCTION_FINISH;
}