/**
 ****************************************************************************************************
 * \file		main.cpp
 * \brief		Main procedure
 ****************************************************************************************************
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

// Utilities header
#include <Utilities.h>
#include <UtilitiesTypes.h>

#include "IDControl.h"
#include "GamePanel\GamePanel.h"
#include "MainFrame\MainFrame.h"
#include "WorkingPanel\WorkingPanel.h"

/****************************************************************************************************
		Private class
****************************************************************************************************/
// Define a new application type, each program should derive a class from wxApp
class MainApp : public wxApp
{
public:
	// override base class virtuals
	// ----------------------------

	// This one is called on application startup and is a good place for the app
	// initialization (doing it here and not in the ctor allows to have an error
	// return: if OnInit() returns false, the application terminates)
	virtual bool OnInit( void );

	virtual int OnExit( void );
};

/****************************************************************************************************
		Event tables and other macros for wxWidgets
****************************************************************************************************/
// The event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE( MainFrame, wxFrame )
	// File menu
	EVT_MENU( WorldEditor_OPEN_FBX, MainFrame::OnFileOpenFBX )
	EVT_MENU( WorldEditor_OPEN_SCN, MainFrame::OnFileOpenSCN )
	EVT_MENU( WorldEditor_EXPORT, MainFrame::OnFileExport )
	EVT_MENU( WorldEditor_EXPORT_OCTREE, MainFrame::OnFileExportOctree )

	// Help
	EVT_MENU( WorldEditor_QUIT,  MainFrame::OnQuit )
	EVT_MENU( WorldEditor_ABOUT, MainFrame::OnAbout )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( GamePanel, wxPanel )
	EVT_IDLE( GamePanel::OnIdle )
	EVT_LEFT_DOWN( GamePanel::OnLeftClick )
	EVT_RIGHT_DOWN( GamePanel::OnRightClick )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( WorkingPanel, wxPanel )
	EVT_LEFT_DOWN( WorkingPanel::OnClick )
	// Trigger box
	EVT_BUTTON( WorldEditor_TRIGGER_BOX_ADD, WorkingPanel::OnAddTriggerBox )
	EVT_BUTTON( WorldEditor_TRIGGER_BOX_REMOVE, WorkingPanel::OnRemoveTriggerBox )
	EVT_BUTTON( WorldEditor_TRIGGER_BOX_UPDATE, WorkingPanel::OnUpdateTriggerBox )
	EVT_LIST_ITEM_SELECTED( WorldEditor_TRIGGER_BOX_LIST, WorkingPanel::OnSelectTriggerBox )
	EVT_LIST_ITEM_DESELECTED( WorldEditor_TRIGGER_BOX_LIST, WorkingPanel::OnDeselectTriggerBox )
	// Path node
	EVT_BUTTON( WorldEditor_WAY_POINT_ADD, WorkingPanel::OnAddWayPoint )
	EVT_BUTTON( WorldEditor_WAY_POINT_REMOVE, WorkingPanel::OnRemoveWayPoint )
	EVT_BUTTON( WorldEditor_WAY_POINT_UPDATE, WorkingPanel::OnUpdateWayPoint )
	EVT_LIST_ITEM_SELECTED( WorldEditor_WAY_POINT_LIST, WorkingPanel::OnSelectWayPoint )
	EVT_LIST_ITEM_DESELECTED( WorldEditor_WAY_POINT_LIST, WorkingPanel::OnDeselectWayPoint )
	// Path node link
	EVT_BUTTON( WorldEditor_WAY_POINT_LINK_ADD, WorkingPanel::OnAddWayPointLink )
	EVT_BUTTON( WorldEditor_WAY_POINT_LINK_REMOVE, WorkingPanel::OnRemoveWayPointLink )
	EVT_LIST_ITEM_SELECTED( WorldEditor_WAY_POINT_LINK_LIST, WorkingPanel::OnSelectWayPointLink )
	EVT_LIST_ITEM_DESELECTED( WorldEditor_WAY_POINT_LINK_LIST, WorkingPanel::OnDeselectWayPointLink )
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP( MainApp )

/****************************************************************************************************
		Private class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool OnInit( void )
	\brief		Initialize MainApp
	\param		NONE
	\return		boolean
	\retval		1 SUCCESS
	\retval		0 FAIL
 ****************************************************************************************************
*/
bool MainApp::OnInit( void )
{
	// Call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if ( !wxApp::OnInit() )
		return FAIL;

	if( !Utilities::Initialize() )
		return FAIL;

	// Create the main application window
	MainFrame *frame = new MainFrame( "World Editor", wxPoint(0, 0), wxSize(1380, 820) );

	// and show it (the frames, unlike simple controls, are not shown when
	// created initially)
	frame->Show( true );

	// SUCCESS: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned false here, the
	// application would exit immediately.
	return SUCCESS;
}

int MainApp::OnExit( void )
{
	Utilities::ShutDown();

	return 0;
}