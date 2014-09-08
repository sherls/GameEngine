#ifndef _WORKING_PANEL_H_
#define _WORKING_PANEL_H_

#include <vector>

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/generic/propdlg.h>
#include <wx/listctrl.h>

#include "../IDControl.h"

#include "UtilitiesTypes.h"

class MainFrame;
class GamePanel;
class TriggerBoxList;

class WorkingPanel : public wxPanel
{
public:
	// Entity page
	wxTextCtrl *m_tagName;

	WorkingPanel( wxWindow *i_parent );
	~WorkingPanel( void );

	// Entity page
	void SetEntityName( const std::string &i_entityName );
	void ResetEntityPropertyPage( void );

	// Trigger box
	void InitializeTriggerBoxList( void );
	void AddInitialTriggerBox( const wxString &i_name );
	void SetTriggerBoxProperty( const wxString &i_triggerBoxName, const D3DXVECTOR3 &i_centre, const Utilities::S_SIZE &i_size );
	void ClearCurrentTriggerBoxProperty( void );

	// Path node
	void InitializeWayPointList( void );
	void AddInitialWayPoint( const UINT32 &i_u32ID );
	void SetWayPointProperty( const UINT32 &i_u32PathNodeID, const D3DXVECTOR3 &i_centre );
	void ClearCurrentWayPointProperty( void );

private:
	wxBoxSizer *_sizerFrame;
	wxBookCtrlBase *_bookCtrl;

	// Entity page
	wxPanel *_entityPropertyPanel;
	wxTextCtrl *_entityName;
	wxTextCtrl *_entityColourR;
	wxTextCtrl *_entityColourG;
	wxTextCtrl *_entityColourB;

	// Trigger box
	wxTextCtrl *_triggerBoxName;
	wxTextCtrl *_triggerBoxCentreX;
	wxTextCtrl *_triggerBoxCentreY;
	wxTextCtrl *_triggerBoxCentreZ;
	wxTextCtrl *_triggerBoxSizeX;
	wxTextCtrl *_triggerBoxSizeY;
	wxTextCtrl *_triggerBoxSizeZ;
	wxListCtrl *_triggerBoxList;
	UINT32 _u32TriggerBoxListSize;
	UINT32 _u32TriggerBoxListCurrIndex;

	// Path node
	wxTextCtrl *_wayPointID;
	wxTextCtrl *_wayPointCentreX;
	wxTextCtrl *_wayPointCentreY;
	wxTextCtrl *_wayPointCentreZ;
	wxListCtrl *_wayPointList;
	wxListCtrl *_wayPointLinkList;
	UINT32 _u32WayPointListSize;
	UINT32 _u32WayPointListCurrIndex;
	UINT32 _u32WayPointLinkListSize;
	UINT32 _u32WayPointIndexToBeLinked;
	UINT32 _u32WayPointLinkListCurrIndex;

	wxPanel *CreateEntityPropertyPage( wxBookCtrlBase *i_parent );
	wxPanel *CreateTriggerBoxPage( wxBookCtrlBase *i_parent );
	wxPanel *CreateWayPointPage( wxBookCtrlBase *i_parent );

	void OnClick( wxMouseEvent &i_event );

	// Trigger box control
	void OnAddTriggerBox( wxCommandEvent &i_event );
	void OnRemoveTriggerBox( wxCommandEvent &i_event );
	void OnUpdateTriggerBox( wxCommandEvent &i_event );
	void OnSelectTriggerBox( wxListEvent &i_event );
	void OnDeselectTriggerBox( wxListEvent &i_event );

	// Way point control
	void OnAddWayPoint( wxCommandEvent &i_event );
	void OnRemoveWayPoint( wxCommandEvent &i_event );
	void OnUpdateWayPoint( wxCommandEvent &i_event );
	void OnSelectWayPoint( wxListEvent &i_event );
	void OnDeselectWayPoint( wxListEvent &i_event );

	// Way point link control
	void OnAddWayPointLink( wxCommandEvent &i_event );
	void OnRemoveWayPointLink( wxCommandEvent &i_event );
	void OnSelectWayPointLink( wxListEvent &i_event );
	void OnDeselectWayPointLink( wxListEvent &i_event );

	DECLARE_EVENT_TABLE();
};

#endif	// #ifndef _WORKING_PANEL_H_