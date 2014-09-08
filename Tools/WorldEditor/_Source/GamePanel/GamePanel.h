/**
 ****************************************************************************************************
 * \file		GamePanel.h
 * \brief		GamePanel class declaration. Show the game and handle all action related to it.
 ****************************************************************************************************
*/

#ifndef _GAME_PANEL_H_
#define _GAME_PANEL_H_

#include <map>
#include <set>
#include <vector>

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// Utilities header
#include <SmartPtr/SmartPtr.h>

// GameEngine header
#include <World/Entity.h>
#include <AI/WayPoint.h>

class MainFrame;
class WorkingPanel;

class GamePanel : public wxPanel
{
public:
	class SelectableEntity
	{
		SelectableEntity( void ) {}

		// Make it non-copyable
		SelectableEntity( SelectableEntity const &i_other );
		SelectableEntity &operator=( const SelectableEntity &i_other );

		// Make it incomparable
		bool &operator==( const SelectableEntity &i_other ) const;

	public:
		static Utilities::MemoryPool			*m_selectableEntityPool;

		D3DXVECTOR3																			m_collisionPoint;
		Utilities::Pointer::SmartPtr<GameEngine::Entity>	m_entity;
		uint16_t																				*m_u16Indices;
		D3DXVECTOR3																			*m_vertices;
		std::string																			*m_name;
		UINT32																					m_u32TotalPrimitives;
		UINT32																					m_u32TotalVertices;
		float																						m_collisionDistance;

		SelectableEntity( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity, const char *i_collisionFile );
		~SelectableEntity( void );

		void *operator new( size_t i_size );
		void operator delete( void *i_ptr );
	};

	typedef struct s_trigger_box
	{
		D3DXVECTOR3 centre;
		Utilities::S_SIZE size;
	} S_TRIGGER_BOX;

	static std::map<wxString, S_TRIGGER_BOX> *m_triggerBoxList;
	static std::map<UINT32, GameEngine::AI::S_WAY_POINT> *m_wayPointList;
	static std::set<GameEngine::AI::S_WAY_POINT_LINK, GameEngine::AI::S_WAY_POINT_COMPARE> *m_wayPointLinkList;
	static wxString			m_currTriggerBox;
	static UINT32				m_u32CurrWayPointID;
	static bool					m_bEntitySelected;
	static bool					m_bEnableCameraControl;
	static bool					m_bAddingLink;

	// Trigger box control
	static void AddTriggerBox( void );
	static void UpdateCurrentTriggerBox( wxString &i_name, const D3DXVECTOR3 &i_centre, const Utilities::S_SIZE &i_size );

	// Path node control
	static void AddWayPoint( void );
	static void RemoveWayPoint( const UINT32 &i_u32ID );
	static void UpdateCurrentWayPoint( const UINT32 &i_u32ID, const GameEngine::AI::S_WAY_POINT &i_updatedPathNode );
	static bool AddWayPointLink( const UINT32 &i_u32FromID, const UINT32 &i_u32ToID );
	static bool RemoveWayPointLink( const UINT32 &i_u32FromID, const UINT32 &i_u32Index );

	GamePanel( wxWindow *i_parent, WorkingPanel *i_workingPanel );
	~GamePanel( void );

	bool Load( void );
	bool OpenScene( const std::string &i_filename );
	void Export( void );
	void ExportOctree( const std::string &i_sceneFile );

private:
	static UINT32 _u32TriggerBoxCtr;
	static UINT32 _u32WayPointCtr;
	static WorkingPanel *_workingPanel;

	std::vector<Utilities::Pointer::SmartPtr<SelectableEntity>> *_selectableEntities;
	std::string* _selectedEntityName;
	MainFrame *_mainFrame;
	HWND _hWnd;
	UINT32 _u32SelectedEntityIndex;

	void OnIdle( wxIdleEvent &i_event );
	void OnLeftClick( wxMouseEvent &i_event );
	void OnRightClick( wxMouseEvent &i_event );

	void OnQuit( wxCommandEvent &i_event );

	void LoadWorldConfiguration( void );
	bool CreateControl( void );
	void CreateDirectionalLight( void );
	bool CreateCamera( Utilities::Pointer::SmartPtr<GameEngine::Entity> &i_entity );
	bool CreateEntities( const std::string &i_filename );
	bool CreateCollisionEntities( void );

	void UnselectEntity( void );

	DECLARE_EVENT_TABLE();
};

#endif	// #ifndef _GAME_PANEL_H_