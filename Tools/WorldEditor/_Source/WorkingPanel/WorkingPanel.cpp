#include "WorkingPanel.h"

#include <wx/bookctrl.h>

#include "../GamePanel/GamePanel.h"

/****************************************************************************************************
		WorkingPanel public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			WorkingPanel( wxWindow *i_parent )
	\brief		WorkingPanel class constructor
	\param		i_parent parent window
	\return		NONE
 ****************************************************************************************************
*/
WorkingPanel::WorkingPanel( wxWindow *i_parent ) :
	wxPanel( i_parent ),
	_bookCtrl( NULL )
{
	SetExtraStyle( wxFRAME_EX_CONTEXTHELP );

	_sizerFrame = new wxBoxSizer( wxVERTICAL );

	_bookCtrl = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, \
		wxBK_TOP | wxNB_MULTILINE | wxNB_NOPAGETHEME );

	if( !_bookCtrl )
		return;

	_bookCtrl->Hide();

	wxPanel *panel = CreateEntityPropertyPage( _bookCtrl );
	_bookCtrl->AddPage( panel, "Entity Property", false, -1 );

	panel = CreateTriggerBoxPage( _bookCtrl );
	_bookCtrl->AddPage( panel, "Trigger box", false, -1 );

	panel = CreateWayPointPage( _bookCtrl );
	_bookCtrl->AddPage( panel, "Way point", false, -1 );

	_bookCtrl->SetSelection( 0 );

	_sizerFrame->Insert( 0, _bookCtrl, wxSizerFlags(5).Expand().Border(wxALL, 15) );
	_sizerFrame->Show( _bookCtrl );
	_sizerFrame->Layout();

	this->SetSizer( _sizerFrame );
	this->Layout();
}

/**
 ****************************************************************************************************
	\fn			~WorkingPanel( void )
	\brief		WorkingPanel class destructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
WorkingPanel::~WorkingPanel( void )
{
	if( _sizerFrame )
	{
		//delete _sizerFrame;
		_sizerFrame = NULL;
	}

	if( _bookCtrl )
	{
		//delete _bookCtrl;
		_bookCtrl = NULL;
	}

	if( _entityName )
	{
		//delete _entityName;
		_entityName = NULL;
	}

	if( m_tagName )
	{
		//delete _tagName;
		m_tagName = NULL;
	}
}

/**
 ****************************************************************************************************
	\fn			void SetEntityName( const std::string &i_entityName )
	\brief		Set entity name
	\param		i_entityName the entity name
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::SetEntityName( const std::string &i_entityName )
{
	_entityName->WriteText( wxString(i_entityName.c_str()) );
}

/**
 ****************************************************************************************************
	\fn			void ResetEntityPropertyPage( void )
	\brief		Reset the entity property page
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::ResetEntityPropertyPage( void )
{
	_entityName->Clear();
	m_tagName->Clear();
	m_tagName->Enable( false );
}

/**
 ****************************************************************************************************
	\fn			void InitializeTriggerBoxList( void )
	\brief		Initialize trigger box list
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::InitializeTriggerBoxList( void )
{
	_u32TriggerBoxListSize = 0;
	std::map<wxString, GamePanel::S_TRIGGER_BOX>::const_iterator iter = GamePanel::m_triggerBoxList->begin();
	for( ; iter != GamePanel::m_triggerBoxList->end(); ++iter )
	{
		_triggerBoxList->InsertItem( _u32TriggerBoxListSize, iter->first );
		_u32TriggerBoxListSize++;
	}
}

/**
 ****************************************************************************************************
	\fn			void AddInitialTriggerBox( const wxString &i_name )
	\brief		Add initial trigger box
	\param		i_name name of the trigger box
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::AddInitialTriggerBox( const wxString &i_name )
{
	_triggerBoxList->InsertItem( _u32TriggerBoxListSize, i_name.c_str() );
	_u32TriggerBoxListSize++;
}

/**
 ****************************************************************************************************
	\fn			void SetTriggerBoxProperty( const wxString &i_triggerBoxName, const D3DXVECTOR3 &i_centre, const Utilities::S_SIZE &i_size )
	\brief		Set the property of current trigger box
	\param		i_triggerBoxName name of the trigger box
	\param		i_centre center of the trigger box
	\param		i_size size of the trigger box
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::SetTriggerBoxProperty( const wxString &i_triggerBoxName, const D3DXVECTOR3 &i_centre, const Utilities::S_SIZE &i_size )
{
	// Name
	_triggerBoxName->WriteText( i_triggerBoxName.c_str() );
	_triggerBoxName->Enable( true );

	// Centre
	_triggerBoxCentreX->WriteText( wxString::Format("%.4f", i_centre.x) );
	_triggerBoxCentreX->Enable( true );
	_triggerBoxCentreY->WriteText( wxString::Format("%.4f", i_centre.y) );
	_triggerBoxCentreY->Enable( true );
	_triggerBoxCentreZ->WriteText( wxString::Format("%.4f", i_centre.z) );
	_triggerBoxCentreZ->Enable( true );

	// Size
	_triggerBoxSizeX->WriteText( wxString::Format("%.4f", i_size.width) );
	_triggerBoxSizeX->Enable( true );
	_triggerBoxSizeY->WriteText( wxString::Format("%.4f", i_size.height) );
	_triggerBoxSizeY->Enable( true );
	_triggerBoxSizeZ->WriteText( wxString::Format("%.4f", i_size.depth) );
	_triggerBoxSizeZ->Enable( true );
}

/**
 ****************************************************************************************************
	\fn			void InitializeWayPointList( void )
	\brief		Initialize way point list
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::InitializeWayPointList( void )
{
	_u32WayPointListSize = 0;
	std::map<UINT32, GameEngine::AI::S_WAY_POINT>::const_iterator wayPointIter = GamePanel::m_wayPointList->begin();
	for( ; wayPointIter != GamePanel::m_wayPointList->end(); ++wayPointIter )
	{
		_wayPointList->InsertItem( _u32WayPointListSize, wxString::Format("Node %d", wayPointIter->first) );
		_u32WayPointListSize++;
	}

	_u32WayPointLinkListSize = 0;
}

/**
 ****************************************************************************************************
	\fn			void AddInitialWayPoint( const UINT32 &i_u32ID )
	\brief		Add initial way point
	\param		i_u32ID ID of the way point
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::AddInitialWayPoint( const UINT32 &i_u32ID )
{
	_wayPointList->InsertItem( _u32WayPointListSize, wxString::Format( "Node %d", i_u32ID) );
	_u32WayPointListSize++;
}

/**
 ****************************************************************************************************
	\fn			void SetWayPointProperty( const UINT32 &i_u32WayPointID, const D3DXVECTOR3 &i_centre )
	\brief		Set the property of current way point
	\param		i_u32WayPointID ID of the way point
	\param		i_centre center of the way point
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::SetWayPointProperty( const UINT32 &i_u32WayPointID, const D3DXVECTOR3 &i_centre )
{
	// Name
	_wayPointID->WriteText( wxString::Format("%d", i_u32WayPointID) );
	//_wayPointID->Enable( true );

	// Centre
	_wayPointCentreX->WriteText( wxString::Format("%.4f", i_centre.x) );
	_wayPointCentreX->Enable( true );
	_wayPointCentreY->WriteText( wxString::Format("%.4f", i_centre.y) );
	_wayPointCentreY->Enable( true );
	_wayPointCentreZ->WriteText( wxString::Format("%.4f", i_centre.z) );
	_wayPointCentreZ->Enable( true );

	// Set the way point link
	_u32WayPointIndexToBeLinked = GamePanel::m_wayPointList->size();
	std::set<GameEngine::AI::S_WAY_POINT_LINK, GameEngine::AI::S_WAY_POINT_COMPARE>::const_iterator iter;
	for( iter =  GamePanel::m_wayPointLinkList->begin(); iter != GamePanel::m_wayPointLinkList->end(); ++iter )
	{
		if( iter->u32From == i_u32WayPointID )
		{
			wxString nodeID = wxString::Format( "Node %d", iter->u32To );
			_wayPointLinkList->InsertItem( _u32WayPointLinkListSize, nodeID.c_str() );
			++_u32WayPointLinkListSize;
		}
	}
	_u32WayPointLinkListCurrIndex = _u32WayPointLinkListSize;
}

/****************************************************************************************************
		WorkingPanel private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			wxPanel *CreateEntityPropertyPage( wxBookCtrlBase *i_parent )
	\brief		Create entity property page
	\param		i_parent the parent of this panel
	\return		wxPanel the created engine panel
 ****************************************************************************************************
*/
wxPanel* WorkingPanel::CreateEntityPropertyPage( wxBookCtrlBase *i_parent )
{
	wxPanel *panel = new wxPanel( i_parent );
	_entityPropertyPanel = panel;

	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *item = new wxBoxSizer( wxVERTICAL );

	wxStaticText *text = new wxStaticText( panel, wxID_ANY, "Name:" );
	item->Add( text, 0, wxGROW | wxALL, 5 );
	_entityName = new wxTextCtrl( panel, wxID_ANY, "" );
	_entityName->Enable( false );
	item->Add( _entityName, 0, wxGROW | wxALL, 5 );

	text = new wxStaticText( panel, wxID_ANY, "Tag:" );
	item->Add( text, 0, wxGROW | wxALL, 5 );
	m_tagName = new wxTextCtrl( panel, WorldEditor_TAG_NAME, "" );
	item->Add( m_tagName, 0, wxGROW | wxALL, 5 );

	// Centre
	wxStaticBoxSizer *colourBox = new wxStaticBoxSizer( wxVERTICAL, panel, "Colour" );
	// X
	text = new wxStaticText( panel, wxID_ANY, "R:" );
	colourBox->Add( text, 0, wxTILE, 5 );
	_entityColourR = new wxTextCtrl( panel, wxID_ANY, "" );
	_entityColourR->Enable( false );
	colourBox->Add( _entityColourR, 0, wxTILE, 5 );
	// Y
	text = new wxStaticText( panel, wxID_ANY, "G:");
	colourBox->Add( text, 0, wxTILE, 5 );
	_entityColourG = new wxTextCtrl( panel, wxID_ANY, "" );
	_entityColourG->Enable( false );
	colourBox->Add( _entityColourG, 0, wxTILE, 5 );
	// Z
	text = new wxStaticText( panel, wxID_ANY, "B:" );
	colourBox->Add( text, 0, wxTILE, 5 );
	_entityColourB = new wxTextCtrl( panel, wxID_ANY, "" );
	_entityColourB->Enable( false );
	colourBox->Add( _entityColourB, 0, wxTILE, 5 );
	item->Add( colourBox );

	if( !GamePanel::m_bEntitySelected )
		m_tagName->Enable( false );

	topSizer->Add( item, 1, wxGROW | wxALIGN_CENTER | wxALL, 5 );
	topSizer->AddSpacer( 5 );
	panel->SetSizer( topSizer );

	return panel;
}

/**
 ****************************************************************************************************
	\fn			wxPanel *CreateTriggerBoxPage( wxBookCtrlBase *i_parent )
	\brief		Create trigger box page
	\param		i_parent the parent of this panel
	\return		wxPanel the created trigger box page
 ****************************************************************************************************
*/
wxPanel* WorkingPanel::CreateTriggerBoxPage( wxBookCtrlBase *i_parent )
{
	wxPanel *panel = new wxPanel( i_parent );

	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *item = new wxBoxSizer( wxVERTICAL );

	// Add button
	wxButton *button = new wxButton( panel, WorldEditor_TRIGGER_BOX_ADD, "Add trigger box" );
	item->Add( button, 0, wxGROW | wxALL, 5 );

	// Name
	wxStaticText *text = new wxStaticText( panel, wxID_ANY, "Trigger box name:" );
	item->Add( text, 0, wxGROW | wxALL, 5 );
	_triggerBoxName = new wxTextCtrl( panel, wxID_ANY, "" );
	_triggerBoxName->Enable( false );
	item->Add( _triggerBoxName, 0, wxGROW | wxALL, 5 );

	wxBoxSizer *insideSizer = new wxBoxSizer( wxHORIZONTAL );

	// Centre
	wxStaticBoxSizer *centreBox = new wxStaticBoxSizer( wxVERTICAL, panel, "Centre" );
	// X
	text = new wxStaticText( panel, wxID_ANY, "X:" );
	centreBox->Add( text, 0, wxTILE, 5 );
	_triggerBoxCentreX = new wxTextCtrl( panel, wxID_ANY, "" );
	_triggerBoxCentreX->Enable( false );
	centreBox->Add( _triggerBoxCentreX, 0, wxTILE, 5 );
	// Y
	text = new wxStaticText( panel, wxID_ANY, "Y:");
	centreBox->Add( text, 0, wxTILE, 5 );
	_triggerBoxCentreY = new wxTextCtrl( panel, wxID_ANY, "" );
	_triggerBoxCentreY->Enable( false );
	centreBox->Add( _triggerBoxCentreY, 0, wxTILE, 5 );
	// Z
	text = new wxStaticText( panel, wxID_ANY, "Z:" );
	centreBox->Add( text, 0, wxTILE, 5 );
	_triggerBoxCentreZ = new wxTextCtrl( panel, wxID_ANY, "" );
	_triggerBoxCentreZ->Enable( false );
	centreBox->Add( _triggerBoxCentreZ, 0, wxTILE, 5 );
	insideSizer->Add( centreBox, 0, wxALL, 5 );
	insideSizer->SetItemMinSize( centreBox, centreBox->GetStaticBox()->GetBestSize() );

	// Size
	wxStaticBoxSizer *sizeBox = new wxStaticBoxSizer( wxVERTICAL, panel, "Size" );
	// Width
	text = new wxStaticText( panel, wxID_ANY, "Width:" );
	sizeBox->Add( text, 0, wxTILE, 5 );
	_triggerBoxSizeX = new wxTextCtrl( panel, wxID_ANY, "" );
	_triggerBoxSizeX->Enable( false );
	sizeBox->Add( _triggerBoxSizeX, 0, wxTILE, 5 );
	// Height
	text = new wxStaticText( panel, wxID_ANY, "Height:");
	sizeBox->Add( text, 0, wxTILE, 5 );
	_triggerBoxSizeY = new wxTextCtrl( panel, wxID_ANY, "" );
	_triggerBoxSizeY->Enable( false );
	sizeBox->Add( _triggerBoxSizeY, 0, wxTILE, 5 );
	// Depth
	text = new wxStaticText( panel, wxID_ANY, "Depth:" );
	sizeBox->Add( text, 0, wxTILE, 5 );
	_triggerBoxSizeZ = new wxTextCtrl( panel, wxID_ANY, "" );
	_triggerBoxSizeZ->Enable( false );
	sizeBox->Add( _triggerBoxSizeZ, 0, wxTILE, 5 );
	insideSizer->Add( sizeBox, 0, wxALL, 5 );
	insideSizer->SetItemMinSize( sizeBox, sizeBox->GetStaticBox()->GetBestSize() );

	item->Add( insideSizer, 0, 0, 0 );

	// Update button
	button = new wxButton( panel, WorldEditor_TRIGGER_BOX_UPDATE, "Update current trigger box", \
		wxPoint(0, 320) );
	item->Add( button, 0, wxGROW | wxALL, 5 );

	// List
	text = new wxStaticText( panel, wxID_ANY, "List of trigger boxes:" );
	item->Add( text, 0, wxGROW | wxALL, 5 );
	_triggerBoxList = new wxListCtrl( panel, WorldEditor_TRIGGER_BOX_LIST, wxDefaultPosition, wxDefaultSize, wxLC_LIST );
	item->Add( _triggerBoxList, 0, wxGROW | wxALL, 5 );

	// Remove button
	button = new wxButton( panel, WorldEditor_TRIGGER_BOX_REMOVE, "Remove trigger box", \
		wxPoint(125, 0) );
	item->Add( button, 0, wxGROW | wxALL, 5 );

	topSizer->Add( item, 1, wxGROW | wxALIGN_CENTER | wxALL, 5 );
	topSizer->AddSpacer( 5 );
	panel->SetSizer( topSizer );

	return panel;
}

/**
 ****************************************************************************************************
	\fn			wxPanel *CreateWayPointPage( wxBookCtrlBase *i_parent )
	\brief		Create pah node page
	\param		i_parent the parent of this panel
	\return		wxPanel the created way point page
 ****************************************************************************************************
*/
wxPanel* WorkingPanel::CreateWayPointPage( wxBookCtrlBase *i_parent )
{
	wxPanel *panel = new wxPanel( i_parent );

	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *item = new wxBoxSizer( wxVERTICAL );

	// Add button
	wxButton *button = new wxButton( panel, WorldEditor_WAY_POINT_ADD, "Add way point" );
	item->Add( button, 0, wxGROW | wxALL, 5 );

	// Name
	wxStaticText *text = new wxStaticText( panel, wxID_ANY, "Way point ID:" );
	item->Add( text, 0, wxGROW | wxALL, 5 );
	_wayPointID = new wxTextCtrl( panel, wxID_ANY, "" );
	_wayPointID->Enable( false );
	item->Add( _wayPointID, 0, wxGROW | wxALL, 5 );

	wxBoxSizer *insideSizer = new wxBoxSizer( wxHORIZONTAL );

	// Centre
	wxStaticBoxSizer *centreBox = new wxStaticBoxSizer( wxVERTICAL, panel, "Centre" );
	// X
	text = new wxStaticText( panel, wxID_ANY, "X:" );
	centreBox->Add( text, 0, wxTILE, 5 );
	_wayPointCentreX = new wxTextCtrl( panel, wxID_ANY, "" );
	_wayPointCentreX->Enable( false );
	centreBox->Add( _wayPointCentreX, 0, wxTILE, 5 );
	// Y
	text = new wxStaticText( panel, wxID_ANY, "Y:");
	centreBox->Add( text, 0, wxTILE, 5 );
	_wayPointCentreY = new wxTextCtrl( panel, wxID_ANY, "" );
	_wayPointCentreY->Enable( false );
	centreBox->Add( _wayPointCentreY, 0, wxTILE, 5 );
	// Z
	text = new wxStaticText( panel, wxID_ANY, "Z:" );
	centreBox->Add( text, 0, wxTILE, 5 );
	_wayPointCentreZ = new wxTextCtrl( panel, wxID_ANY, "" );
	_wayPointCentreZ->Enable( false );
	centreBox->Add( _wayPointCentreZ, 0, wxTILE, 5 );
	insideSizer->Add( centreBox, 0, wxALL, 5 );
	insideSizer->SetItemMinSize( centreBox, centreBox->GetStaticBox()->GetBestSize() );

	item->Add( insideSizer, 0, 0, 0 );

	// Update button
	button = new wxButton( panel, WorldEditor_WAY_POINT_UPDATE, "Update current way point", \
		wxPoint(0, 320) );
	item->Add( button, 0, wxGROW | wxALL, 5 );

	// Link of this node
	text = new wxStaticText( panel, wxID_ANY, "List of link of this node:" );
	item->Add( text, 0, wxGROW | wxALL, 5 );
	_wayPointLinkList = new wxListCtrl( panel, WorldEditor_WAY_POINT_LINK_LIST, wxDefaultPosition, wxDefaultSize, wxLC_LIST );
	item->Add( _wayPointLinkList, 0, wxGROW | wxALL, 5 );

	// Add way point link
	button = new wxButton( panel, WorldEditor_WAY_POINT_LINK_ADD, "Add link to current way point", \
		wxPoint(0, 320) );
	item->Add( button, 0, wxGROW | wxALL, 5 );

	// Remove way point link
	button = new wxButton( panel, WorldEditor_WAY_POINT_LINK_REMOVE, "Remove current way point link", \
		wxPoint(0, 320) );
	item->Add( button, 0, wxGROW | wxALL, 5 );

	// Way point list
	text = new wxStaticText( panel, wxID_ANY, "List of way points:" );
	item->Add( text, 0, wxGROW | wxALL, 5 );
	_wayPointList = new wxListCtrl( panel, WorldEditor_WAY_POINT_LIST, wxDefaultPosition, wxDefaultSize, wxLC_LIST );
	item->Add( _wayPointList, 0, wxGROW | wxALL, 5 );

	// Remove button
	button = new wxButton( panel, WorldEditor_WAY_POINT_REMOVE, "Remove way point", \
		wxPoint(125, 0) );
	item->Add( button, 0, wxGROW | wxALL, 5 );

	topSizer->Add( item, 1, wxGROW | wxALIGN_CENTER | wxALL, 5 );
	topSizer->AddSpacer( 5 );
	panel->SetSizer( topSizer );

	return panel;
}

/**
 ****************************************************************************************************
	\fn			void ClearCurrentTriggerBoxProperty( void )
	\brief		Clear the property of current trigger box
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::ClearCurrentTriggerBoxProperty( void )
{
	// Name
	_triggerBoxName->Clear();
	_triggerBoxName->Enable( false );

	// Centre
	_triggerBoxCentreX->Clear();
	_triggerBoxCentreX->Enable( false );
	_triggerBoxCentreY->Clear();
	_triggerBoxCentreY->Enable( false );
	_triggerBoxCentreZ->Clear();
	_triggerBoxCentreZ->Enable( false );

	// Size
	_triggerBoxSizeX->Clear();
	_triggerBoxSizeX->Enable( false );
	_triggerBoxSizeY->Clear();
	_triggerBoxSizeY->Enable( false );
	_triggerBoxSizeZ->Clear();
	_triggerBoxSizeZ->Enable( false );
}

/**
 ****************************************************************************************************
	\fn			void ClearCurrentWayPointProperty( void )
	\brief		Clear the property of current way point
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::ClearCurrentWayPointProperty( void )
{
	// Name
	_wayPointID->Clear();
	_wayPointID->Enable( false );

	// Centre
	_wayPointCentreX->Clear();
	_wayPointCentreX->Enable( false );
	_wayPointCentreY->Clear();
	_wayPointCentreY->Enable( false );
	_wayPointCentreZ->Clear();
	_wayPointCentreZ->Enable( false );

	// Way point link
	if( _u32WayPointIndexToBeLinked == GamePanel::m_wayPointList->size() )
	{
		_wayPointLinkList->ClearAll();
		_u32WayPointLinkListSize = 0;
	}
}

/**
 ****************************************************************************************************
	\fn			void OnClick( wxMouseEvent &i_event )
	\brief		WorkingPanel OnClick event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnClick( wxMouseEvent &i_event )
{
	GamePanel::m_bEnableCameraControl = false;
}

/**
 ****************************************************************************************************
	\fn			void OnAddTriggerBox( wxCommandEvent &i_event )
	\brief		Adding trigger box event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnAddTriggerBox( wxCommandEvent &i_event )
{
	ClearCurrentTriggerBoxProperty();

	GamePanel::AddTriggerBox();

	_triggerBoxList->InsertItem( _u32TriggerBoxListSize, _triggerBoxName->GetValue() );
	if( _u32TriggerBoxListSize != 0 )
		_triggerBoxList->SetItemState( _u32TriggerBoxListCurrIndex, 0, wxLIST_STATE_SELECTED );
	_u32TriggerBoxListCurrIndex = _u32TriggerBoxListSize;
	_triggerBoxList->SetItemState( _u32TriggerBoxListCurrIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
	_u32TriggerBoxListSize++;
}

/**
 ****************************************************************************************************
	\fn			void OnRemoveTriggerBox( wxCommandEvent &i_event )
	\brief		Remove trigger box event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnRemoveTriggerBox( wxCommandEvent &i_event )
{
	if( (_u32TriggerBoxListCurrIndex == GamePanel::m_triggerBoxList->size())
		|| (GamePanel::m_triggerBoxList->size() == 0) )
	{
		wxMessageBox( "<<<ERROR>>> Create or select a trigger box first!", \
			"Cannot remove trigger box", wxOK | wxICON_ERROR, this );

		return;
	}
	else
	{
		wxString key = _triggerBoxName->GetValue();
		std::map<wxString, GamePanel::S_TRIGGER_BOX>::iterator iter = GamePanel::m_triggerBoxList->find( key );
		if( iter != GamePanel::m_triggerBoxList->end() )
		{
			GamePanel::m_triggerBoxList->erase( iter );
			_triggerBoxList->DeleteItem( _u32TriggerBoxListCurrIndex );
			_u32TriggerBoxListCurrIndex = GamePanel::m_triggerBoxList->size();
			--_u32TriggerBoxListSize;
		}
	}
}

/**
 ****************************************************************************************************
	\fn			void OnUpdateTriggerBox( wxCommandEvent &i_event )
	\brief		Updating trigger box event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnUpdateTriggerBox( wxCommandEvent &i_event )
{
	float x;
	float y;
	float z;
	wxString tempStr;
	D3DXVECTOR3 newCentre;

	if( (_u32TriggerBoxListCurrIndex == GamePanel::m_triggerBoxList->size())
		|| (GamePanel::m_triggerBoxList->size() == 0) )
	{
		wxMessageBox( "<<<ERROR>>> Create or select a trigger box first!", \
			"Cannot update trigger box", wxOK | wxICON_ERROR, this );

		return;
	}

	tempStr = _triggerBoxCentreX->GetValue();
	x = atof( tempStr.c_str() );

	tempStr = _triggerBoxCentreY->GetValue();
	y = atof( tempStr.c_str() );

	tempStr = _triggerBoxCentreZ->GetValue();
	z = atof( tempStr.c_str() );
	newCentre = D3DXVECTOR3( x, y, z );

	tempStr = _triggerBoxSizeX->GetValue();
	x = atof( tempStr.c_str() );

	tempStr = _triggerBoxSizeY->GetValue();
	y = atof( tempStr.c_str() );

	tempStr = _triggerBoxSizeZ->GetValue();
	z = atof( tempStr.c_str() );
	Utilities::S_SIZE newSize = { x, y, z };

	wxString newName = _triggerBoxName->GetValue();
	UINT32 u32CurrIndex = _u32TriggerBoxListCurrIndex;
	GamePanel::UpdateCurrentTriggerBox( _triggerBoxName->GetValue(), newCentre, newSize );
	_triggerBoxList->ClearAll();
	InitializeTriggerBoxList();
}

/**
 ****************************************************************************************************
	\fn			void OnSelectTriggerBox( wxListEvent &i_event )
	\brief		On selecting trigger box event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnSelectTriggerBox( wxListEvent &i_event )
{
	_u32TriggerBoxListCurrIndex = i_event.m_itemIndex;

	ClearCurrentTriggerBoxProperty();
	wxListItem info;
	info.m_itemId = _u32TriggerBoxListCurrIndex;
	info.m_col = 0;
	info.m_mask = wxLIST_MASK_TEXT;
	if( _triggerBoxList->GetItem(info) )
	{
		std::map<wxString, GamePanel::S_TRIGGER_BOX>::const_iterator iter = GamePanel::m_triggerBoxList->find( info.m_text );
		GamePanel::m_currTriggerBox = info.m_text;
		SetTriggerBoxProperty( info.m_text, iter->second.centre, iter->second.size );
	}
}

/**
 ****************************************************************************************************
	\fn			void OnDeselectTriggerBox( wxListEvent &i_event )
	\brief		On deselecting trigger box event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnDeselectTriggerBox( wxListEvent &i_event )
{
	ClearCurrentTriggerBoxProperty();
	_u32TriggerBoxListCurrIndex = GamePanel::m_triggerBoxList->size();
}

/**
 ****************************************************************************************************
	\fn			void OnAddWayPoint( wxCommandEvent &i_event )
	\brief		Adding way point event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnAddWayPoint( wxCommandEvent &i_event )
{
	ClearCurrentWayPointProperty();

	GamePanel::AddWayPoint();

	wxString nodeID = wxString::Format( "Node %s", _wayPointID->GetValue() );
	_wayPointList->InsertItem( _u32WayPointListSize, nodeID.c_str() );
	if( _u32WayPointListSize != 0 )
		_wayPointList->SetItemState( _u32WayPointListCurrIndex, 0, wxLIST_STATE_SELECTED );
	_u32WayPointListCurrIndex = _u32WayPointListSize;
	_wayPointList->SetItemState( _u32WayPointListCurrIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
	_u32WayPointListSize++;
}

/**
 ****************************************************************************************************
	\fn			void OnRemoveWayPoint( wxCommandEvent &i_event )
	\brief		Remove way point event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnRemoveWayPoint( wxCommandEvent &i_event )
{
	if( (_u32WayPointListCurrIndex == GamePanel::m_wayPointList->size())
		|| (GamePanel::m_wayPointList->size() == 0) )
	{
		wxMessageBox( "<<<ERROR>>> Create or select a way point first!", \
			"Cannot remove way point", wxOK | wxICON_ERROR, this );

		return;
	}
	else
	{
		wxString keyStr = _wayPointID->GetValue();
		UINT32 key = atoi( keyStr.c_str() );
		std::map<UINT32, GameEngine::AI::S_WAY_POINT>::iterator iter = GamePanel::m_wayPointList->find( key );
		if( iter != GamePanel::m_wayPointList->end() )
		{
			GamePanel::m_wayPointList->erase( iter );
			_wayPointList->DeleteItem( _u32WayPointListCurrIndex );
			_u32WayPointListCurrIndex = GamePanel::m_wayPointList->size();
			--_u32WayPointListSize;
		}
	}
}

/**
 ****************************************************************************************************
	\fn			void OnUpdateWayPoint( wxCommandEvent &i_event )
	\brief		Updating way point event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnUpdateWayPoint( wxCommandEvent &i_event )
{
	float x;
	float y;
	float z;
	wxString tempStr;
	D3DXVECTOR3 newCentre;
	GameEngine::AI::S_WAY_POINT updatedWayPoint;

	if( (_u32WayPointListCurrIndex == GamePanel::m_wayPointList->size())
		|| (GamePanel::m_wayPointList->size() == 0) )
	{
		wxMessageBox( "<<<ERROR>>> Create or select a way point first!", \
			"Cannot update trigger box", wxOK | wxICON_ERROR, this );

		return;
	}

	tempStr = _wayPointCentreX->GetValue();
	x = atof( tempStr.c_str() );

	tempStr = _wayPointCentreY->GetValue();
	y = atof( tempStr.c_str() );

	tempStr = _wayPointCentreZ->GetValue();
	z = atof( tempStr.c_str() );
	newCentre = D3DXVECTOR3( x, y, z );

	updatedWayPoint.centre = newCentre;
	updatedWayPoint.radius = DEFAULT_WAY_POINT_RADIUS;

	GamePanel::UpdateCurrentWayPoint( _u32WayPointListCurrIndex, updatedWayPoint );
}

/**
 ****************************************************************************************************
	\fn			void OnSelectWayPoint( wxListEvent &i_event )
	\brief		On selecting way point event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnSelectWayPoint( wxListEvent &i_event )
{
	if( GamePanel::m_bAddingLink )
	{
		_u32WayPointIndexToBeLinked = i_event.m_itemIndex;
	}
	else
	{
		_u32WayPointListCurrIndex = i_event.m_itemIndex;

		ClearCurrentWayPointProperty();
		wxListItem info;
		info.m_itemId = _u32WayPointListCurrIndex;
		info.m_col = 0;
		info.m_mask = wxLIST_MASK_TEXT;
		if( _wayPointList->GetItem(info) )
		{
			wxString keyStr = info.m_text;
			keyStr = keyStr.SubString( 5, keyStr.Length() );
			UINT32 key = atoi( keyStr.c_str() );
			std::map<UINT32, GameEngine::AI::S_WAY_POINT>::const_iterator iter = GamePanel::m_wayPointList->find( key );
			GamePanel::m_u32CurrWayPointID = key;
			SetWayPointProperty( key, iter->second.centre );
		}
	}
}

/**
 ****************************************************************************************************
	\fn			void OnDeselectWayPoint( wxListEvent &i_event )
	\brief		On deselecting way point event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnDeselectWayPoint( wxListEvent &i_event )
{
	if( _u32WayPointIndexToBeLinked == GamePanel::m_wayPointList->size() )
	{
		ClearCurrentWayPointProperty();
		_u32WayPointListCurrIndex = GamePanel::m_wayPointList->size();
	}
}

/**
 ****************************************************************************************************
	\fn			void OnAddWayPointLink( wxCommandEvent &i_event )
	\brief		Adding way point link event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnAddWayPointLink( wxCommandEvent &i_event )
{
	bool bRetval = GamePanel::AddWayPointLink(_u32WayPointListCurrIndex, _u32WayPointIndexToBeLinked);

	if( bRetval )
	{
		_wayPointList->SetItemState( _u32WayPointIndexToBeLinked, 0, wxLIST_STATE_SELECTED );
		//_wayPointList->SetItemState( _u32WayPointListCurrIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );

		wxString nodeID = wxString::Format( "Node %d", _u32WayPointIndexToBeLinked );
		_wayPointLinkList->InsertItem( _u32WayPointLinkListSize, nodeID.c_str() );
		//if( _u32WayPointLinkListSize != 0 )
		//	_wayPointLinkList->SetItemState( _u32WayPointLinkListCurrIndex, 0, wxLIST_STATE_SELECTED );
		_u32WayPointLinkListCurrIndex = _u32WayPointLinkListSize;
		//_wayPointLinkList->SetItemState( _u32WayPointLinkListCurrIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
		++_u32WayPointLinkListSize;
		_u32WayPointIndexToBeLinked = GamePanel::m_wayPointList->size();
	}
}

/**
 ****************************************************************************************************
	\fn			void OnRemoveWayPointLink( wxCommandEvent &i_event )
	\brief		Remove way point link event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnRemoveWayPointLink( wxCommandEvent &i_event )
{
	if( (_u32WayPointLinkListCurrIndex == GamePanel::m_wayPointLinkList->size())
		|| (GamePanel::m_wayPointLinkList->size() == 0) )
	{
		wxMessageBox( "<<<ERROR>>> Create or select a way point first!", \
			"Cannot remove way point", wxOK | wxICON_ERROR, this );

		return;
	}
	else
	{
		wxString keyStr = _wayPointID->GetValue();
		UINT32 key = atoi( keyStr.c_str() );
		bool bRetval = GamePanel::RemoveWayPointLink( key, _u32WayPointLinkListCurrIndex );
		if( bRetval )
		{
			_wayPointLinkList->DeleteItem( _u32WayPointLinkListCurrIndex );
			_u32WayPointLinkListCurrIndex = GamePanel::m_wayPointLinkList->size();
			--_u32WayPointLinkListSize;
		}
	}
}

/**
 ****************************************************************************************************
	\fn			void OnSelectWayPointLink( wxListEvent &i_event )
	\brief		On selecting way point link event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnSelectWayPointLink( wxListEvent &i_event )
{
	_u32WayPointLinkListCurrIndex = i_event.m_itemIndex;
}

/**
 ****************************************************************************************************
	\fn			void OnDeselectWayPointLink( wxListEvent &i_event )
	\brief		On deselecting way point link event
	\param		i_event input event
	\return		NONE
 ****************************************************************************************************
*/
void WorkingPanel::OnDeselectWayPointLink( wxListEvent &i_event )
{
	_u32WayPointLinkListCurrIndex = GamePanel::m_wayPointLinkList->size();
}
