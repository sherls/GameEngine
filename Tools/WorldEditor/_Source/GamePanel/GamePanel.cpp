/**
 ****************************************************************************************************
 * \file		GamePanel.cpp
 * \brief		GamePanel class implementation
 ****************************************************************************************************
*/

#include "GamePanel.h"
#include "../MainFrame/MainFrame.h"
#include "../WorkingPanel/WorkingPanel.h"

#include <libconfig.hpp>

#include <fstream>

// Utilities header
#include <UtilitiesTypes.h>
#include <Parser/MeshParser/MeshParser.h>
#include <Parser/SceneParser/SceneParser.h>

// Tools/FbxLoader
#include <OctreeGenerator/OctreeGenerator.h>

// GameEngine header
#include <GameEngine.h>
#include <World/World.h>
#include <Camera/Camera.h>
#include <GameEngineDefault.h>
#include <DebugMenu/DebugMenu.h>
#include <Collision/Collision.h>
#include <Utilities/IDCreator/IDCreator.h>
#include <Utilities/Configuration/Configuration.h>

// RendererEngine header
#include <RendererEngine.h>

#include "../FBXLoader/FBXLoader.h"
#include "../Controller/CameraController.h"

WorkingPanel *GamePanel::_workingPanel = NULL;

/****************************************************************************************************
		Public static member declaration
****************************************************************************************************/
std::map<wxString, GamePanel::S_TRIGGER_BOX> *GamePanel::m_triggerBoxList = NULL;
std::map<UINT32, GameEngine::AI::S_WAY_POINT> *GamePanel::m_wayPointList = NULL;
std::set<GameEngine::AI::S_WAY_POINT_LINK, GameEngine::AI::S_WAY_POINT_COMPARE> *GamePanel::m_wayPointLinkList = NULL;
wxString GamePanel::m_currTriggerBox;
UINT32 GamePanel::m_u32CurrWayPointID;
bool GamePanel::m_bEntitySelected = false;
bool GamePanel::m_bEnableCameraControl = true;
bool GamePanel::m_bAddingLink = false;
Utilities::MemoryPool *GamePanel::SelectableEntity::m_selectableEntityPool = NULL;

/****************************************************************************************************
		Private static member declaration
****************************************************************************************************/
UINT32 GamePanel::_u32TriggerBoxCtr = 0;
UINT32 GamePanel::_u32WayPointCtr = 0;

/****************************************************************************************************
		Static function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void AddTriggerBox( void )
	\brief		Add new trigger box to trigger box list
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::AddTriggerBox( void )
{
	D3DXVECTOR3 cameraPos = g_world::Get().m_camera->GetPosition();
	D3DXVECTOR3 cameraDir = g_world::Get().m_camera->GetViewDirection();
	D3DXVec3Normalize( &cameraDir, &cameraDir );

	D3DXVECTOR3 centre = cameraPos + (cameraDir * 100);
	Utilities::S_SIZE size = { 50.0f, 50.0f, 50.0f };
	m_currTriggerBox = wxString::Format( "TriggerBox%d", _u32TriggerBoxCtr++ );

	S_TRIGGER_BOX newTriggerBox = { centre, size };
	std::pair<wxString, S_TRIGGER_BOX> insertingPair( m_currTriggerBox, newTriggerBox );
	m_triggerBoxList->insert( insertingPair );

	_workingPanel->SetTriggerBoxProperty( m_currTriggerBox, centre, size );
}

/**
 ****************************************************************************************************
	\fn			void UpdateCurrentTriggerBox( wxString &i_name, const D3DXVECTOR3 &i_centre, const Utilities::S_SIZE &i_size )
	\brief		Update current trigger box property
	\param		i_name new name
	\param		i_centre new centre
	\param		i_size new size
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::UpdateCurrentTriggerBox( wxString &i_name, const D3DXVECTOR3 &i_centre, const Utilities::S_SIZE &i_size )
{
	std::map<wxString, S_TRIGGER_BOX>::iterator iter;
	S_TRIGGER_BOX replacingTriggerBox = { i_centre, i_size };

	// Remove the old from the list
	iter = m_triggerBoxList->find( m_currTriggerBox );
	m_triggerBoxList->erase( iter );

	// Add the new one to the list
	m_currTriggerBox = i_name;
	std::pair<wxString, S_TRIGGER_BOX> insertingPair( i_name, replacingTriggerBox );
	m_triggerBoxList->insert( insertingPair );
}

/**
 ****************************************************************************************************
	\fn			void AddWayPoint( void )
	\brief		Add new way point to way point list
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::AddWayPoint( void )
{
	D3DXVECTOR3 cameraPos = g_world::Get().m_camera->GetPosition();
	D3DXVECTOR3 cameraDir = g_world::Get().m_camera->GetViewDirection();
	D3DXVec3Normalize( &cameraDir, &cameraDir );

	D3DXVECTOR3 centre = cameraPos + (cameraDir * 100);
	m_u32CurrWayPointID = _u32WayPointCtr++;

	GameEngine::AI::S_WAY_POINT newWayPoint = { centre, DEFAULT_WAY_POINT_RADIUS };
	std::pair<UINT32, GameEngine::AI::S_WAY_POINT> insertingPair( m_u32CurrWayPointID, newWayPoint );
	m_wayPointList->insert( insertingPair );

	_workingPanel->SetWayPointProperty( m_u32CurrWayPointID, centre );
}

/**
 ****************************************************************************************************
	\fn			void RemoveWayPoint( const UINT32 &i_u32ID )
	\brief		Remove way point
	\param		i_u32ID ID of the way point to be removed
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::RemoveWayPoint( const UINT32 &i_u32ID )
{
	m_wayPointList->erase( m_wayPointList->find(i_u32ID) );

	std::set<GameEngine::AI::S_WAY_POINT_LINK, GameEngine::AI::S_WAY_POINT_COMPARE>::iterator iter;
	std::set<GameEngine::AI::S_WAY_POINT_LINK, GameEngine::AI::S_WAY_POINT_COMPARE>::iterator linkToDelete;

	iter = m_wayPointLinkList->begin();
	for( ; iter != m_wayPointLinkList->end(); ++iter )
	{
		if( (iter->u32From == i_u32ID) || (iter->u32To == i_u32ID) )
		{
			linkToDelete = iter;
			++iter;
			m_wayPointLinkList->erase( linkToDelete );
		}
	}
}

/**
 ****************************************************************************************************
	\fn			void UpdateCurrentWayPoint( UINT32 &i_u32ID, const GameEngine::AI::S_WAY_POINTE &i_updatedNode )
	\brief		Update current way point property
	\param		i_u32ID new ID
	\param		i_updatedWayPoint updated way point
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::UpdateCurrentWayPoint( const UINT32 &i_u32ID, const GameEngine::AI::S_WAY_POINT &i_updatedNode )
{
	std::map<UINT32, GameEngine::AI::S_WAY_POINT>::iterator iter;

	// Remove the old from the list
	iter = m_wayPointList->find( m_u32CurrWayPointID );
	m_wayPointList->erase( iter );

	// Add the new one to the list
	m_u32CurrWayPointID = i_u32ID;
	std::pair<UINT32, GameEngine::AI::S_WAY_POINT> insertingPair( m_u32CurrWayPointID, i_updatedNode );
	m_wayPointList->insert( insertingPair );
}

/**
 ****************************************************************************************************
	\fn			bool AddWayPointLink( const UINT32 &i_u32FromID, const UINT32 &i_u32ToID )
	\brief		Add way point link
	\param		i_u32FromID the ID from originated way point
	\param		i_u32ToID the ID to destination way point
	\return		BOOLEAN
	\retval		SUCCESS if the link doesn't exist
	\retval		FAIL if the link already exists
 ****************************************************************************************************
*/
bool GamePanel::AddWayPointLink( const UINT32 &i_u32FromID, const UINT32 &i_u32ToID )
{
	std::set<GameEngine::AI::S_WAY_POINT_LINK, GameEngine::AI::S_WAY_POINT_COMPARE>::const_iterator iter;
	for( iter = m_wayPointLinkList->begin(); iter != m_wayPointLinkList->end(); ++iter )
	{
		if( (iter->u32From == i_u32FromID) && (iter->u32To == i_u32ToID) )
			return FAIL;
	}

	GameEngine::AI::S_WAY_POINT_LINK newLink = { i_u32FromID, i_u32ToID };
	m_wayPointLinkList->insert( newLink );

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool RemoveWayPointLink( const UINT32 &i_u32FromID, const UINT32 &i_u32Index )
	\brief		Remove specific way point
	\param		i_u32FromID the from ID
	\param		i_u32Index the index of the item
	\return		BOOLEAN
	\retval		SUCCESS if the link exists
	\retval		FAIL if the link doesn't exist
 ****************************************************************************************************
*/
bool GamePanel::RemoveWayPointLink( const UINT32 &i_u32FromID, const UINT32 &i_u32Index )
{
	UINT32 u32Index;
	std::set<GameEngine::AI::S_WAY_POINT_LINK, GameEngine::AI::S_WAY_POINT_COMPARE>::iterator iter;

	if( i_u32Index > m_wayPointLinkList->size() )
		return FAIL;

	iter = m_wayPointLinkList->begin();
	for( u32Index = 0; iter != m_wayPointLinkList->end(); ++iter )
	{
		if( iter->u32From == i_u32FromID )
		{
			if( u32Index == i_u32Index )
				break;
			++u32Index;
		}
	}

	m_wayPointLinkList->erase( iter );

	return SUCCESS;
}

/****************************************************************************************************
		Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			GamePanel( wxWindow *i_parent, WorkingPanel *i_workingPanel )
	\brief		GamePaneL class constructor
	\param		i_parent parent window
	\param		i_workingPanel pointer to working panel
	\return		NONE
 ****************************************************************************************************
*/
GamePanel::GamePanel( wxWindow *i_parent, WorkingPanel *i_workingPanel ) :
	wxPanel( i_parent ),
	_selectableEntities( NULL ),
	_selectedEntityName( NULL ),
	_mainFrame( NULL ),
	_hWnd( 0 ),
	_u32SelectedEntityIndex( 0 )
{
	m_bEntitySelected = false;
	m_bEnableCameraControl = true;
	_workingPanel = i_workingPanel;

	_selectedEntityName = new std::string();

	m_triggerBoxList = new std::map<wxString, S_TRIGGER_BOX>();
	m_wayPointList = new std::map<UINT32, GameEngine::AI::S_WAY_POINT>();
	m_wayPointLinkList = new std::set<GameEngine::AI::S_WAY_POINT_LINK, GameEngine::AI::S_WAY_POINT_COMPARE>();

	_hWnd = this->GetHWND();
	_mainFrame = (MainFrame*) i_parent;
	_selectableEntities = new std::vector<Utilities::Pointer::SmartPtr<SelectableEntity>>();
}

/**
 ****************************************************************************************************
	\fn			~GamePanel( void )
	\brief		GamePaneL class destructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GamePanel::~GamePanel( void )
{
	if( _mainFrame )
	{
		_mainFrame = NULL;
	}

	if( _selectableEntities )
	{
		_selectableEntities->clear();
		delete _selectableEntities;
		_selectableEntities = NULL;
	}

	if( SelectableEntity::m_selectableEntityPool )
	{
		delete SelectableEntity::m_selectableEntityPool;
		SelectableEntity::m_selectableEntityPool = NULL;
	}

	if( m_triggerBoxList )
	{
		m_triggerBoxList->clear();
		delete m_triggerBoxList;
		m_triggerBoxList = NULL;
	}

	if( m_wayPointList )
	{
		m_wayPointList->clear();
		delete m_wayPointList;
		m_wayPointList = NULL;
	}

	if( m_wayPointLinkList )
	{
		m_wayPointLinkList->clear();
		delete m_wayPointLinkList;
		m_wayPointLinkList = NULL;
	}

	GameEngine::ShutDown();
}

/**
 ****************************************************************************************************
	\fn			bool Load( void )
	\brief		Load game related setup
	\param		NONE
	\return		bool
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool GamePanel::Load( void )
{
	bool bResult;

	if( !GameEngine::StartUp( "../../External/master.cfg") )
	{
		GameEngine::ShutDown();
		_mainFrame->_infoWindow->WriteText( "\n<<<ERROR>>> Could not set up game engine. <<<ERROR>>>\n\nCheck the master.cfg in Resources");
		return FAIL;
	}

	bResult = GameEngine::Initialize( "World Editor", 1080, 640, 0, 1, _hWnd );
	if( !bResult )
		_mainFrame->_infoWindow->WriteText( "\n<<<ERROR>>> Could not loading game panel window <<<ERROR>>>\n\n");

	bResult = CreateControl();
	if( !bResult )
		_mainFrame->_infoWindow->WriteText( "\n<<<ERROR>>> Failed to create control <<<ERROR>>>\n\n" );

	// Create point light
	D3DXVECTOR3 position( 1000.0f, 1000.0f, 1000.0f );
	D3DCOLOR colour = D3DCOLOR_XRGB( 255, 255, 255 );
	D3DCOLOR ambient = D3DCOLOR_XRGB( 25, 25, 25 );
	g_world::Get().CreatePointLight( position, colour, ambient, 0.5f, 100.0f, 5.0f );

	// Create directional light
	Utilities::S_SIZE size;
	D3DXVECTOR3 direction( -1500.0f, 0.0f, 0.0f );
	D3DXVECTOR3 lookAt( 0.1f, -0.9f, 0.0f );
	colour = D3DCOLOR_XRGB( 128, 128, 128 );
	g_world::Get().CreateDirectionalLight( direction, lookAt, colour, size, 0.0f, 10000.0f, 0.2f );

	GameEngine::SetDebugMenu( true );

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void OpenScene( const std::string &i_filename )
	\brief		Open scene file
	\param		i_filename name of the scene to be opened
	\return		NONE
 ****************************************************************************************************
*/
bool GamePanel::OpenScene( const std::string &i_filename )
{
	if( SelectableEntity::m_selectableEntityPool == NULL )
		SelectableEntity::m_selectableEntityPool = Utilities::MemoryPool::Create( sizeof(SelectableEntity), Utilities::DEFAULT_MEMORY_POOL_SIZE );

	bool bResult = CreateEntities( i_filename );
	if( !bResult )
	{
		wxMessageBox( wxString::Format("<<<ERROR>>> Cannot open %s file!", i_filename.c_str()) , \
			"Invalid scene file", wxOK | wxICON_ERROR, _mainFrame );

		return FAIL;
	}

	LoadWorldConfiguration();

	_workingPanel->Enable( true );

	return SUCCESS;
}

/****************************************************************************************************
		Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void Export( void )
	\brief		Export current world editor setup
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::Export( void )
{
	std::ofstream outFile;
	outFile.open( "../../External/worldConfiguration.dat", std::ios::binary );
	if( outFile.fail() )
		return;

	// Save the trigger data
	UINT32 u32TriggerBoxSize = m_triggerBoxList->size();
	UINT32 u32TriggerBoxNameLength = 0;
	std::map<wxString, S_TRIGGER_BOX>::const_iterator triggerBoxIter = m_triggerBoxList->begin();
	outFile.write( reinterpret_cast<const char*>(&u32TriggerBoxSize), sizeof(u32TriggerBoxSize) );
	for( ; triggerBoxIter != m_triggerBoxList->end(); ++triggerBoxIter )
	{
		u32TriggerBoxNameLength = triggerBoxIter->first.length();
		outFile.write( reinterpret_cast<const char*>(&u32TriggerBoxNameLength), sizeof(u32TriggerBoxNameLength) );
		outFile.write( triggerBoxIter->first.c_str(), sizeof(char) * u32TriggerBoxNameLength );
		outFile.write( reinterpret_cast<const char*>(&triggerBoxIter->second), sizeof(triggerBoxIter->second) );
	}

	// Save the way point data
	UINT32 u32WayPointSize = m_wayPointList->size();
	outFile.write( reinterpret_cast<const char*>(&u32WayPointSize), sizeof(u32WayPointSize) );
	std::map<UINT32, GameEngine::AI::S_WAY_POINT>::const_iterator wayPointIter = m_wayPointList->begin();
	for( ; wayPointIter != m_wayPointList->end(); ++wayPointIter )
	{
		outFile.write( reinterpret_cast<const char*>(&wayPointIter->first), sizeof(wayPointIter->first) );
		outFile.write( reinterpret_cast<const char*>(&wayPointIter->second), sizeof(wayPointIter->second) );
	}

	// Save the way point link data
	UINT32 u32WayPointLinkSize = m_wayPointLinkList->size();
	outFile.write( reinterpret_cast<const char*>(&u32WayPointLinkSize), sizeof(u32WayPointLinkSize) );
	std::set<GameEngine::AI::S_WAY_POINT_LINK, GameEngine::AI::S_WAY_POINT_COMPARE>::const_iterator wayPointLinkIter;
	for( wayPointLinkIter = m_wayPointLinkList->begin(); wayPointLinkIter != m_wayPointLinkList->end(); ++wayPointLinkIter )
	{
		outFile.write( reinterpret_cast<const char*>(&wayPointLinkIter->u32From), sizeof(wayPointLinkIter->u32From) );
		outFile.write( reinterpret_cast<const char*>(&wayPointLinkIter->u32To), sizeof(wayPointLinkIter->u32To) );
	}

	// Add the tag, if it is exist to the existing mesh file
	std::vector< Utilities::Pointer::SmartPtr<SelectableEntity> >::const_iterator selectableEntityIter;
	for( selectableEntityIter = _selectableEntities->begin(); selectableEntityIter != _selectableEntities->end(); ++selectableEntityIter )
	{
		if( (*selectableEntityIter)->m_entity->m_tag \
			&& ((*selectableEntityIter)->m_entity->m_tag->length() > 0) \
			&& ((*selectableEntityIter)->m_entity->m_tag[0] != '\0')
			)
		{
			Utilities::MeshParser meshParser( (*selectableEntityIter)->m_name->c_str() );
			Utilities::S_NORMAL_MAP_VERTEX_DATA* vertexData;
			uint16_t *indexData;

			// Replace it
			std::string fileName = "../../External/Data/Meshes/";
			fileName += (*selectableEntityIter)->m_name->c_str();
			std::ofstream meshFile;
			meshFile.open( fileName.c_str(), std::ios::binary );
			if( meshFile.fail() )
				break;

			vertexData = new Utilities::S_NORMAL_MAP_VERTEX_DATA[meshParser.m_u32TotalVertices];
			indexData = new uint16_t[meshParser.m_u32TotalPrimitives*3];
			meshParser.GetVertexData( vertexData );
			meshParser.GetIndexData( indexData );

			meshFile.write( reinterpret_cast<const char*> (&meshParser.m_u32TotalVertices), sizeof(meshParser.m_u32TotalVertices) );
			meshFile.write( reinterpret_cast<const char*> (&meshParser.m_u32TotalPrimitives), sizeof(meshParser.m_u32TotalPrimitives) );
			for( UINT32 i = 0; i < meshParser.m_u32TotalVertices; ++i )
				meshFile.write( reinterpret_cast<const char*> (&vertexData[i]), sizeof(Utilities::S_NORMAL_MAP_VERTEX_DATA) );
			for( UINT32 i = 0; i < (meshParser.m_u32TotalPrimitives * 3); ++i )
				meshFile.write( reinterpret_cast<const char*>(&indexData[i]), sizeof(uint16_t) );

			wxString tagName( (*selectableEntityIter)->m_entity->m_tag->c_str() );
			tagName.Replace( " ", "_", true );
			UINT32 u32TagNameLength = tagName.length();
			meshFile.write( reinterpret_cast<const char*>(&u32TagNameLength), sizeof(UINT32) );
			meshFile.write( tagName.c_str(), sizeof(char) * u32TagNameLength );

			meshFile.close();
			delete [] vertexData;
			delete [] indexData;
		}
	}

	outFile.close();
}

/**
 ****************************************************************************************************
	\fn			void ExportOctree( void )
	\brief		Export current world editor setup as octree
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::ExportOctree( const std::string &i_sceneFile )
{
	Tools::OctreeGenerator::Initialize();

	Utilities::SceneParser scene( i_sceneFile.c_str() );

	for( UINT32 i = 0; i < scene.m_u32TotalEntity; ++i )
	{
		Utilities::MeshParser mesh( scene.m_entity[i].file.c_str() );
		Utilities::S_NORMAL_MAP_VERTEX_DATA *vertexData = new Utilities::S_NORMAL_MAP_VERTEX_DATA[mesh.m_u32TotalVertices];
		Utilities::StringHash emptyHash = Utilities::StringHash( "" );
		uint16_t *indices = new uint16_t[mesh.m_u32TotalPrimitives * 3];
		mesh.GetVertexData( vertexData );
		mesh.GetIndexData( indices );

		UINT32 u32CurrIndex = 0;
		for( UINT32 j = 0; j < mesh.m_u32TotalPrimitives; ++j )
		{
			D3DXVECTOR3 a, b, c;

			Utilities::StringHash hashedTag = emptyHash;

			a = vertexData[indices[u32CurrIndex++]].position;
			b = vertexData[indices[u32CurrIndex++]].position;
			c = vertexData[indices[u32CurrIndex++]].position;

			Utilities::Pointer::SmartPtr<GameEngine::Entity> tempEntity;
			tempEntity = g_world::Get().GetEntityByName( scene.m_entity[i].file.c_str() );
			if( tempEntity )
			{
				if( tempEntity->m_tag && (tempEntity->m_tag->length() > 0) && (tempEntity->m_tag[0] != '\0') )
					hashedTag = Utilities::StringHash( tempEntity->m_tag->c_str() );
			}

			Tools::OctreeGenerator::AddTriangle( a, b, c, hashedTag );
		}
	}

	std::string *octreeTargetStr = new std::string( i_sceneFile );
	*octreeTargetStr = octreeTargetStr->substr( 0, octreeTargetStr->length() - 4 );
	UINT32 startPos = octreeTargetStr->find_last_of("\\" );
	*octreeTargetStr = octreeTargetStr->substr(startPos+1, octreeTargetStr->size());
	Tools::OctreeGenerator::GenerateOctree( octreeTargetStr->c_str() );

	Tools::OctreeGenerator::ShutDown();
}

/**
 ****************************************************************************************************
	\fn			void OnQuit( wxCommandEvent &i_event )
	\brief		On quit event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::OnQuit( wxCommandEvent &i_event )
{
	GameEngine::ShutDown();
}

/**
 ****************************************************************************************************
	\fn			void OnIdle( wxIdleEvent &i_event )
	\brief		On idle event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::OnIdle( wxIdleEvent &i_event )
{
	GameEngine::BeginUpdate();

	std::map<wxString, S_TRIGGER_BOX>::const_iterator triggerBoxIter = m_triggerBoxList->begin();
	for( ; triggerBoxIter != m_triggerBoxList->end(); ++triggerBoxIter )
		g_debugMenu::Get().DrawRectangle( triggerBoxIter->second.centre, triggerBoxIter->second.size );

	std::map<UINT32, GameEngine::AI::S_WAY_POINT>::const_iterator wayPointIter = m_wayPointList->begin();
	for( ; wayPointIter != m_wayPointList->end(); ++wayPointIter )
		g_debugMenu::Get().DrawSphere( wayPointIter->second.centre, wayPointIter->second.radius );

	std::set<GameEngine::AI::S_WAY_POINT_LINK, GameEngine::AI::S_WAY_POINT_COMPARE>::const_iterator wayPointLinkIter = m_wayPointLinkList->begin();
	for( ; wayPointLinkIter != m_wayPointLinkList->end(); ++wayPointLinkIter )
	{
		GameEngine::AI::S_WAY_POINT fromWayPoint = (m_wayPointList->find(wayPointLinkIter->u32From))->second;
		GameEngine::AI::S_WAY_POINT toWayPoint = (m_wayPointList->find(wayPointLinkIter->u32To))->second;
		g_debugMenu::Get().DrawLine( fromWayPoint.centre, toWayPoint.centre );
	}

	if( !m_bEnableCameraControl )
		g_debugMenu::Get().Log( "Camera control disabled" );

	GameEngine::Update();

	GameEngine::EndUpdate();

	i_event.RequestMore( true );
}

/**
 ****************************************************************************************************
	\fn			void OnLeftClick( wxMouseEvent &i_event )
	\brief		On left click event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::OnLeftClick( wxMouseEvent &i_event )
{
	if( !_selectableEntities )
	{
		wxMessageBox( "<<<WARNING>>> No object to be selected.\n Make sure that your scene or FBX file has a valid object." , \
			"No selectable object", wxOK | wxICON_WARNING, _mainFrame );
		return;
	}

	//Utilities::Pointer::SmartPtr<GameEngine::Entity> prevSelectedEntity;
	UINT32 u32PrevSelectedIndex = _selectableEntities->size();

	if( m_bEntitySelected )
		u32PrevSelectedIndex = _u32SelectedEntityIndex;

	UnselectEntity();

	D3DXMATRIXA16 viewMatrix, projectionMatrix, worldMatrix;
	D3DXVECTOR3 startPoint, endPoint;
	D3DVIEWPORT9 viewport;

	RendererEngine::GetTransform( D3DTS_WORLD, worldMatrix );
	RendererEngine::GetViewPort( viewport );
	viewMatrix = g_world::Get().m_camera->m_worldToViewMatrix;
	projectionMatrix = g_world::Get().m_camera->GetViewToProjectedTransform();

	startPoint.x = i_event.GetX();
	startPoint.y = i_event.GetY();
	startPoint.z = 0.0f;

	endPoint.x = i_event.GetX();
	endPoint.y = i_event.GetY();
	endPoint.z = 1;

	// Change the point to projection to world space
	D3DXVec3Unproject( &startPoint, &startPoint, &viewport, &projectionMatrix, &viewMatrix, &worldMatrix );
	D3DXVec3Unproject( &endPoint, &endPoint, &viewport, &projectionMatrix, &viewMatrix, &worldMatrix );

	std::vector< Utilities::Pointer::SmartPtr<SelectableEntity> >::iterator iter;
	UINT32 u32Index = 0;
	float prevCollisionDistance = 0.0f;
	bool bRayTracingResult = false;
	bool bSelectableEntityFound = false;
	float collisionDistance = 0.0f;
	for( iter = _selectableEntities->begin(); iter != _selectableEntities->end(); ++iter )
	{
		D3DXVECTOR3 collisionPoint;
		bRayTracingResult = false;
		UINT32 u32TotalIndices = (*iter)->m_u32TotalPrimitives * 3;
		for( UINT32 i = 0; i < u32TotalIndices; i += 3 )
		{
			bRayTracingResult |= Utilities::Math::RayTracing( startPoint, endPoint,\
				(*iter)->m_vertices[(*iter)->m_u16Indices[i]],\
				(*iter)->m_vertices[(*iter)->m_u16Indices[i+1]],\
				(*iter)->m_vertices[(*iter)->m_u16Indices[i+2]],\
				bSelectableEntityFound, collisionPoint, collisionDistance );
			if( bRayTracingResult )
			{
				if( !bSelectableEntityFound )
					bSelectableEntityFound = true;
			}
		}

		if( bRayTracingResult )
		{
			if( Utilities::Math::AreRelativelyEqual(prevCollisionDistance, 0.0f) )
			{
				prevCollisionDistance = collisionDistance;
				_u32SelectedEntityIndex = u32Index;
				_selectedEntityName = (*iter)->m_name;
			}
			else if( prevCollisionDistance > collisionDistance )
			{
				prevCollisionDistance = collisionDistance;
				_u32SelectedEntityIndex = u32Index;
				_selectedEntityName = (*iter)->m_name;
			}
		}
		u32Index++;
	}

	// If select the same object, then un-select it
	if( u32PrevSelectedIndex == _u32SelectedEntityIndex )
		_u32SelectedEntityIndex = 0;
	else if( _u32SelectedEntityIndex < _selectableEntities->size() )
	{
		m_bEntitySelected = true;
		float originalPosition = _selectableEntities->at(_u32SelectedEntityIndex)->m_entity->m_v3Position.Y();
		_selectableEntities->at(_u32SelectedEntityIndex)->m_entity->m_v3Position.Y( originalPosition + 50.0f );

		// Fill in the entity property page
		_workingPanel->SetEntityName( *_selectedEntityName );
		if( _selectableEntities->at(_u32SelectedEntityIndex)->m_entity->m_tag )
		{
			wxString tag = _selectableEntities->at(_u32SelectedEntityIndex)->m_entity->m_tag->c_str();
			if( !tag.IsEmpty() )
				_workingPanel->m_tagName->WriteText( tag );
		}
		_workingPanel->m_tagName->Enable( true );
	}
	else
		_u32SelectedEntityIndex = 0;
}

/**
 ****************************************************************************************************
	\fn			void OnRightClick( wxMouseEvent &i_event )
	\brief		On right click event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::OnRightClick( wxMouseEvent &i_event )
{
	m_bEnableCameraControl = false;
}

/**
 ****************************************************************************************************
	\fn			void LoadTriggerBoxData( void )
	\brief		Load trigger box data from worldConfiguration.dat
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::LoadWorldConfiguration( void )
{
	UINT32 u32TotalTriggerBox;
	std::ifstream inFile;

	inFile.open( "../../External/worldConfiguration.dat", std::ios::binary );

	if( inFile.fail() )
		return;

	// Read trigger boxes
	inFile.seekg( 0, std::ios::beg );
	inFile.read( (char*)&u32TotalTriggerBox, sizeof(u32TotalTriggerBox) );
	for( UINT32 i = 0; i < u32TotalTriggerBox; ++i )
	{
		char *triggerBoxName;
		UINT32 u32TriggerBoxNameLength = 0;
		S_TRIGGER_BOX newTriggerBox;

		inFile.read( (char*)&u32TriggerBoxNameLength, sizeof(u32TriggerBoxNameLength) );
		triggerBoxName = new char[u32TriggerBoxNameLength + 1];
		inFile.read( triggerBoxName, u32TriggerBoxNameLength * sizeof(char) );
		triggerBoxName[u32TriggerBoxNameLength] = '\0';
		inFile.read( (char*)&newTriggerBox, sizeof(newTriggerBox) );

		wxString wxTriggerBoxName( triggerBoxName );
		std::pair<wxString, S_TRIGGER_BOX> insertingPair( wxTriggerBoxName, newTriggerBox );
		m_triggerBoxList->insert( insertingPair );
		_workingPanel->AddInitialTriggerBox( wxTriggerBoxName );

		delete [] triggerBoxName;
	}

	// Read way points
	UINT32 u32TotalWayPoint = 0;
	inFile.read( (char*)&u32TotalWayPoint, sizeof(u32TotalWayPoint) );
	for( UINT32 i = 0; i < u32TotalWayPoint; ++i )
	{
		UINT32 u32ID = 0;
		GameEngine::AI::S_WAY_POINT newWayPoint;
		inFile.read( (char*)&u32ID, sizeof(u32ID) );
		inFile.read( (char*)&newWayPoint, sizeof(newWayPoint) );
		std::pair<UINT32, GameEngine::AI::S_WAY_POINT> insertingPair( u32ID, newWayPoint );
		m_wayPointList->insert( insertingPair );
		_workingPanel->AddInitialWayPoint( u32ID );
		++_u32WayPointCtr;
	}

	// Read way point links
	UINT32 u32TotalWayPointLink = 0;
	inFile.read( (char*)&u32TotalWayPointLink, sizeof(u32TotalWayPointLink) );
	for( UINT32 i = 0; i < u32TotalWayPointLink; ++i )
	{
		GameEngine::AI::S_WAY_POINT_LINK newWayPointLink;
		inFile.read( (char*)&newWayPointLink, sizeof(newWayPointLink) );
		m_wayPointLinkList->insert( newWayPointLink );
	}

	inFile.close();
}

/**
 ****************************************************************************************************
	\fn			void CreateControl( void )
	\brief		Create the representation control
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
bool GamePanel::CreateControl( void )
{
	D3DXVECTOR3 position( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 lookAt( 0.0f, 0.0f, -1.0f );
	Utilities::S_SIZE size;

	size.width = 0.0f;
	size.height = 160.0f;
	size.depth = 0.0f;

	Utilities::Pointer::SmartPtr<GameEngine::Entity> control = GameEngine::Entity::Create( \
		position , new CameraController(), "Control" );
	control->m_applyPhysics = true;
	control->m_u8EntityID = g_IDCreator::Get().GetID( "Control" );
	control->m_size = size;
	control->m_vLookAt = lookAt;

	g_world::Get().AddEntity( control );
	g_world::Get().SetFriction( control, 0.0f );

	CreateCamera( control );
	CreateDirectionalLight();

	return SUCCESS;
}

void GamePanel::CreateDirectionalLight( void )
{
	libconfig::Config &config = GameEngine::Configuration::GetMasterConfig();
	libconfig::Setting *gameSettings = config.lookup( "CaptureTheFlag" );
	const libconfig::Setting &root = config.getRoot();

	Utilities::S_SIZE size;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 lookAt;
	D3DCOLOR colour;
	int red;
	int blue;
	int green;
	float intensity;
	float nearView;
	float farView;

	FUNCTION_START;

	if( !gameSettings )
	{
		FUNCTION_FINISH;
		return;
	}

	// Size
	// {
	const libconfig::Setting &directionalLightSize = root["CaptureTheFlag"]["directionalLight"]["size"];
	if( !directionalLightSize.lookupValue("width", size.width) )
	{
		DBG_CONFIG_ERROR( "directional light size", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightSize.lookupValue("height", size.height) )
	{
		DBG_CONFIG_ERROR( "directional light size", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}
	// }

	// Orientation
	// {
	const libconfig::Setting &directionalLightOrientation = root["CaptureTheFlag"]["directionalLight"]["direction"];
	if( !directionalLightOrientation.lookupValue("x", direction.x) )
	{
		DBG_CONFIG_ERROR( "directional light orientation", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightOrientation.lookupValue("y", direction.y) )
	{
		DBG_CONFIG_ERROR( "directional light orientation", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightOrientation.lookupValue("z", direction.z) )
	{
		DBG_CONFIG_ERROR( "directional light orientation", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	// }

	// Look at
	// {
	const libconfig::Setting &directionalLightLookAt = root["CaptureTheFlag"]["directionalLight"]["lookAt"];
	if( !directionalLightLookAt.lookupValue("x", lookAt.x) )
	{
		DBG_CONFIG_ERROR( "directional light look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightLookAt.lookupValue("y", lookAt.y) )
	{
		DBG_CONFIG_ERROR( "directional light look at", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightLookAt.lookupValue("z", lookAt.z) )
	{
		DBG_CONFIG_ERROR( "directional light look at", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	// }

	// Colour
	// {
	const libconfig::Setting &pointLightColour = root["CaptureTheFlag"]["directionalLight"]["colour"];
	if( !pointLightColour.lookupValue("r", red) )
	{
		DBG_CONFIG_ERROR( "point light red", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !pointLightColour.lookupValue("g", green) )
	{
		DBG_CONFIG_ERROR( "point light green", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !pointLightColour.lookupValue("b", blue) )
	{
		DBG_CONFIG_ERROR( "point light blue", "CaptureTheFlag");

		FUNCTION_FINISH;
		return;
	}
	colour = D3DCOLOR_RGBA( red, blue, green, 255 );
	// }

	const libconfig::Setting &directionalLightSetting = root["CaptureTheFlag"]["directionalLight"];
	if( !directionalLightSetting.lookupValue("intensity", intensity) )
	{
		DBG_CONFIG_ERROR( "directional light intensity", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightSetting.lookupValue("nearView", nearView) )
	{
		DBG_CONFIG_ERROR( "directional light near view", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	if( !directionalLightSetting.lookupValue("farView", farView) )
	{
		DBG_CONFIG_ERROR( "directional light far view", "CaptureTheFlag" );

		FUNCTION_FINISH;
		return;
	}

	g_world::Get().CreateDirectionalLight( direction, lookAt, colour, size, nearView, farView, intensity );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			CreateCamera( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity )
	\brief		Create camera
	\param		i_entity the entity to be followed
	\return		NONE
 ****************************************************************************************************
*/
bool GamePanel::CreateCamera( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity )
{
	D3DXVECTOR3 position( 0.0f, 0.0f, 15.0f );
	D3DXVECTOR3 lookAtPoint( 0.0f, 0.0f, -1.0f );

	g_world::Get().CreateCamera( i_entity, position, lookAtPoint, 45.0f, -1.0f, 0.1f, 10000.0f );
	g_world::Get().m_camera->m_backDistance = 0.0f;
	g_world::Get().m_camera->m_upDistance = 0.0f;

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool CreateEntities( const std::string &i_filename )
	\brief		Create entities
	\param		i_filename name of the file
	\return		NONE
 ****************************************************************************************************
*/
bool GamePanel::CreateEntities( const std::string &i_filename )
{
	Utilities::SceneParser scene( i_filename.c_str() );

	GameEngine::Math::Vector3 position( scene.m_entity[0].position.x, scene.m_entity[0].position.y, scene.m_entity[0].position.z );

	for( UINT32 i = 0; i < scene.m_u32TotalEntity; ++i )
	{
		Utilities::Pointer::SmartPtr<GameEngine::Entity> tempEntity = GameEngine::Entity::Create( \
			position, NULL, scene.m_entity[i].file.c_str() );
		tempEntity->m_applyPhysics = false;
		tempEntity->m_u32CollisionMask = 0;
		tempEntity->m_orientation = scene.m_entity[i].orientation;
		tempEntity->m_v3Position = position;

		Utilities::MeshParser mesh( scene.m_entity[i].file.c_str() );
		if( mesh.m_tag )
		{
			tempEntity->m_u8EntityID = g_IDCreator::Get().GetID( mesh.m_tag->c_str() );
			tempEntity->m_tag = new std::string( mesh.m_tag->c_str() );
		}
		else
		{
			tempEntity->m_u8EntityID = g_IDCreator::Get().GetID( "Model" );
		}
		_selectableEntities->push_back( new SelectableEntity(tempEntity, scene.m_entity[i].file.c_str()) );
		g_world::Get().AddEntity( tempEntity );
		g_world::Get().CreateMesh( tempEntity, scene.m_entity[i].file.c_str() );
	}

	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void UnselectEntity( void )
	\brief		Unselect entity
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::UnselectEntity( void )
{
	if( m_bEntitySelected )
	{
		// Set the tag to the selected entity
		wxString wxTag = _workingPanel->m_tagName->GetValue();
		wxTag.Trim();
		_selectableEntities->at(_u32SelectedEntityIndex)->m_entity->m_tag = new std::string( wxTag.c_str() );

		// Reset the entity property page
		_workingPanel->ResetEntityPropertyPage();

		// Unselect it
		float originalPosition = _selectableEntities->at(_u32SelectedEntityIndex)->m_entity->m_v3Position.Y();
		_selectableEntities->at(_u32SelectedEntityIndex)->m_entity->m_v3Position.Y( originalPosition - 50.0f );
		_u32SelectedEntityIndex = 0;
		m_bEntitySelected = false;
	}
}

/****************************************************************************************************
			CollisionEntity class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			SelectableEntity( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity, const char *i_collisionFile )
	\brief		Default constructor of SelectableEntity
	\param		i_entity pointer to common entity data
	\param		i_collisionFile the file contain the information of the collision
	\return		NONE
 ****************************************************************************************************
*/
GamePanel::SelectableEntity::SelectableEntity( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity,
	const char *i_collisionFile ) :
	m_collisionPoint( GameEngine::D3DXVECTOR3_ZERO ),
	m_entity( i_entity ),
	m_u16Indices( NULL ),
	m_vertices( NULL ),
	m_u32TotalPrimitives( 0 ),
	m_u32TotalVertices( 0 ),
	m_collisionDistance( 0.0f )
{
	assert( i_collisionFile != NULL );
	assert( i_collisionFile[0] != '\0' );

	m_name = new std::string( i_collisionFile );

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

/**
 ****************************************************************************************************
	\fn			~SelectableEntity( void )
	\brief		Default destructor of SelectableEntity
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GamePanel::SelectableEntity::~SelectableEntity( void )
{
	m_entity = NULL;

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

	if( m_name )
	{
		delete m_name;
		m_name = NULL;
	}
}

/**
 ****************************************************************************************************
	\fn			void *operator new( size_t i_size )
	\brief		New operator of SelectableEntity class
	\param		i_size size to be created
	\return		Pointer to allocated memory
 ****************************************************************************************************
*/
void *GamePanel::SelectableEntity::operator new( size_t i_size )
{
	assert( i_size == sizeof(SelectableEntity) );
	assert( m_selectableEntityPool );

	return m_selectableEntityPool->Allocate( sizeof(SelectableEntity) );
}

/**
 ****************************************************************************************************
	\fn			void operator delete( void *i_ptr )
	\brief		Delete operator of SelectableEntity class
	\param		i_ptr pointer to be deleted
	\return		NONE
 ****************************************************************************************************
*/
void GamePanel::SelectableEntity::operator delete( void *i_ptr )
{
	assert( m_selectableEntityPool );

	if( i_ptr )
		m_selectableEntityPool->Deallocate( i_ptr );
}
