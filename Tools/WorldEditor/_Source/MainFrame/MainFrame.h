/**
 ****************************************************************************************************
 * \file		MainFrame.h
 * \brief		MainFrame class declaration
 ****************************************************************************************************
*/

#ifndef _MAIN_FRAME_H_
#define _MAIN_FRAME_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

class GamePanel;
class WorkingPanel;

// Define a new frame type: this is going to be our main frame
class MainFrame : public wxFrame
{
public:
	// ctor(s)
	MainFrame( const wxString &i_title, const wxPoint &i_position, const wxSize &i_size );
	~MainFrame( void );

	wxTextCtrl *_infoWindow;

private:
	GamePanel *_gamePanel;
	WorkingPanel *_workingPanel;
	std::string *_sceneFile;

	// event handlers (these functions should _not_ be virtual)
	void OnFileOpenFBX( wxCommandEvent &i_event );
	void OnFileOpenSCN( wxCommandEvent &i_event );
	void OnFileExport( wxCommandEvent &i_event );
	void OnFileExportOctree( wxCommandEvent &i_event );

	void OnQuit( wxCommandEvent &i_event );
	void OnAbout( wxCommandEvent &i_event );

	// any class wishing to process wxWidgets events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif	// #ifndef _MAIN_FRAME_H_