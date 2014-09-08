/**
 ****************************************************************************************************
 * \file		MainFrame.cpp
 * \brief		MainFrame class implementation
 ****************************************************************************************************
*/

#include <wx/filefn.h>

// Utilities header
#include <Utilities.h>

// GameEngine header
#include <GameEngine.h>

#include "MainFrame.h"
#include "../Panel/Panel.h"

#include "../IDControl.h"
#include "../FBXLoader/FBXLoader.h"
#include "../GamePanel/GamePanel.h"
#include "../MainFrame/MainFrame.h"
#include "../WorkingPanel/WorkingPanel.h"

/****************************************************************************************************
		Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			MainFrame( const wxString &i_title )
	\brief		MainFrame constructor
	\param		i_title the title to be shown in frame
	\param		i_position position of the frame in the window
	\param		i_size size of the frame
	\return		NONE
 ****************************************************************************************************
*/
MainFrame::MainFrame( const wxString &i_title, const wxPoint &i_position, const wxSize &i_size ) :
	wxFrame( NULL, wxID_ANY, i_title, i_position, i_size ),
	_gamePanel( NULL ),
	_infoWindow( NULL ),
	_sceneFile( NULL )
{
	// set the frame icon
	SetIcon( wxICON(sample) );

	// Create a menu bar
	wxMenu *menuFile = new wxMenu();
	menuFile->Append( WorldEditor_OPEN_FBX, "&Open a FBX file\tCtrl-O", "Open a FBX file." );
	menuFile->Append( WorldEditor_OPEN_SCN, "&Open a SCN file\tAlt-O", "Open a SCN file." );
	menuFile->AppendSeparator();
	menuFile->Append( WorldEditor_EXPORT, "&Export\tCtrl-S", "Export current world set up" );
	menuFile->Append( WorldEditor_EXPORT_OCTREE, "&Export Octree\tCtrl-Alt-S", "Export current scene as octree" );
	menuFile->AppendSeparator();
	menuFile->Append( WorldEditor_QUIT, "E&xit\tAlt-F4", "Quit" );

	// The "About" item should be in the help menu
	wxMenu *menuHelp = new wxMenu();
	menuHelp->Append( WorldEditor_ABOUT, "&About\tF1", "Show about dialog" );

	// Now append the freshly created menu to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append( menuFile, "&File" );
	menuBar->Append( menuHelp, "&Help" );

	// ... and attach this menu bar to the frame
	SetMenuBar( menuBar );

	// create a status bar just for fun (by default with 1 panel only)
	CreateStatusBar( 2 );
	SetStatusText( "Welcome to World Editor" );

	// Create info text window
	_infoWindow = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxPoint(0, 640), wxSize(1080, 100), \
		wxTE_READONLY | wxTE_MULTILINE );

	// Create the working panel
	_workingPanel = new WorkingPanel( this );
	_workingPanel->SetSize( 1080, 0, 300, 820 );
	_workingPanel->Enable( false );

	// Create the game panel
	_gamePanel = new GamePanel( this, _workingPanel );
	_gamePanel->SetSize( 0 , 0, 1080, 640 );
	if( !_gamePanel->Load() )
	{
		wxCommandEvent closeEvent;
		OnQuit( closeEvent );
	}

	_workingPanel->InitializeTriggerBoxList();
	_workingPanel->InitializeWayPointList();
}

/**
 ****************************************************************************************************
	\fn			~MainFrame( void )
	\brief		MainFrame destructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
MainFrame::~MainFrame( void )
{
	Utilities::ShutDown();

	if( _gamePanel )
	{
		delete _gamePanel;
		_gamePanel = NULL;
	}

	if( _workingPanel )
	{
		delete _workingPanel;
		_workingPanel = NULL;
	}

	if( _infoWindow )
	{
		delete _infoWindow;
		_infoWindow = NULL;
	}

	if( _sceneFile )
	{
		delete _sceneFile;
		_sceneFile = NULL;
	}
}

/****************************************************************************************************
		Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void OnFileOpenFBX( wxCommandEvent &WXUNUSED(i_event) )
	\brief		Open FBX file
	\param		i_event event received
	\return		NONE
 ****************************************************************************************************
*/
void MainFrame::OnFileOpenFBX( wxCommandEvent &WXUNUSED(i_event) )
{
	if( _sceneFile )
	{
		wxMessageBox( "A scene had been presented.\nYou need to close this World Editor first to open another FBX file.",
			"Invalid operation", wxOK | wxICON_WARNING );
		return;
	}

	wxFileDialog openFileDialog(	this,
												wxT( "Open FBX file" ),
												wxEmptyString,
												wxEmptyString,
												wxT("FBX file (*.fbx)|*.fbx")
											);

	openFileDialog.SetExtraControlCreator( &Panel::CreatePanel );
	openFileDialog.CenterOnParent();
	openFileDialog.SetDirectory( wxGetHomeDir() );

	if( openFileDialog.ShowModal() == wxID_OK )
	{
		_infoWindow->WriteText( "Loading FBX file. This may takes a while. Please wait patiently.\n" );

		_sceneFile = new std::string( openFileDialog.GetFilename().c_str() );
		bool bResult = false;
		bResult = g_FBXLoader::Get().OpenFBXLoader( openFileDialog.GetPath().c_str(), _infoWindow, *_sceneFile );

		if( bResult )
			_infoWindow->WriteText( wxString::Format("Success generating %s scene file.\n", _sceneFile->c_str()) );
		else
			wxMessageBox( wxString::Format("<<<ERROR>>> Cannot open %s file!", openFileDialog.GetFilename().c_str()) , \
			"Invalid FBX file", wxOK | wxICON_ERROR, this );

		_gamePanel->OpenScene( *_sceneFile );
	}
}

/**
 ****************************************************************************************************
	\fn			void OnFileOpenSCN( wxCommandEvent &WXUNUSED(i_event) )
	\brief		Open FBX file
	\param		i_event event received
	\return		NONE
 ****************************************************************************************************
*/
void MainFrame::OnFileOpenSCN( wxCommandEvent &WXUNUSED(i_event) )
{
	if( _sceneFile )
	{
		wxMessageBox( "A scene had been presented.\nYou need to close this World Editor first to open another FBX file.",
			"Invalid operation", wxOK | wxICON_WARNING );
		return;
	}

	wxFileDialog openFileDialog(	this,
												wxT( "Open SCN file" ),
												wxEmptyString,
												wxEmptyString,
												wxT("Scene file (*.scn)|*.scn")
											);

	openFileDialog.SetExtraControlCreator( &Panel::CreatePanel );
	openFileDialog.CenterOnParent();
	openFileDialog.SetDirectory( wxGetHomeDir() );

	if( openFileDialog.ShowModal() == wxID_OK )
	{
		_sceneFile = new std::string( openFileDialog.GetFilename().c_str() );
		bool bResult = _gamePanel->OpenScene( *_sceneFile );
		if( bResult )
			_infoWindow->WriteText( wxString::Format("Success opening %s scene file.\n", _sceneFile->c_str()) );
		else
			wxMessageBox( wxString::Format("<<<ERROR>>> Cannot open %s file!", openFileDialog.GetFilename().c_str()) , \
				"Invalid scene file", wxOK | wxICON_ERROR, this );
	}
}

/**
 ****************************************************************************************************
	\fn			void OnSave( wxCommandEvent &WXUNUSED(i_event) )
	\brief		Save all the changes made in the world editor
	\param		i_event event received
	\return		NONE
 ****************************************************************************************************
*/
void MainFrame::OnFileExport( wxCommandEvent &WXUNUSED(i_event) )
{
	if( !_sceneFile )
	{
		wxMessageBox( "<<<WARNING>>> Please open a FBX or scene file first!", "No scene available", wxOK | wxICON_WARNING, this );
		return;
	}

	_gamePanel->Export();
}

/**
 ****************************************************************************************************
	\fn			void OnSave( wxCommandEvent &WXUNUSED(i_event) )
	\brief		Save all the changes made in the world editor
	\param		i_event event received
	\return		NONE
 ****************************************************************************************************
*/
void MainFrame::OnFileExportOctree( wxCommandEvent &WXUNUSED(i_event) )
{
	if( !_sceneFile )
	{
		wxMessageBox( "<<<WARNING>>> Please open a FBX or scene file first!", "No scene available", wxOK | wxICON_WARNING, this );
		return;
	}

	_gamePanel->ExportOctree( *_sceneFile );
}

/**
 ****************************************************************************************************
	\fn			void OnQuit( wxCommandEvent &WXUNUSED(i_event) )
	\brief		Quit app on receiving event
	\param		i_event event received
	\return		NONE
 ****************************************************************************************************
*/
void MainFrame::OnQuit( wxCommandEvent &WXUNUSED(i_event) )
{
	GameEngine::ShutDown();
	// true is to force the frame to close
	Close( true );
}

/**
 ****************************************************************************************************
	\fn			void OnAbout( wxCommandEvent &WXUNUSED(i_event) )
	\brief		Show about screen on receiving event
	\param		i_event event received
	\return		NONE
	****************************************************************************************************
*/
void MainFrame::OnAbout( wxCommandEvent &WXUNUSED(i_event) )
{
	wxMessageBox( "Welcome to Capture the Flag World Editor. \
								This program is written by Sherly Yunita (u0733109)",
								"About CTF World Editor",
								wxOK | wxICON_INFORMATION,
								this );
}
