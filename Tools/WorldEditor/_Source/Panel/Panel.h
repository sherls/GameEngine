/**
 ****************************************************************************************************
 * \file		Panel.h
 * \brief		Panel class declaration
 ****************************************************************************************************
*/

#ifndef _PANEL_H_
#define _PANEL_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif
#include <wx/filename.h>

class Panel : public wxPanel
{
public:
	static wxWindow* CreatePanel( wxWindow *i_parent )
	{
		return new Panel( i_parent );
	}

	wxString GetInfo( void ) const
	{
		return wxString::Format( "Checkbox value = %d", static_cast<int>(m_checkBox->GetValue()) );
	}

private:
	Panel( wxWindow *i_parent );
	~Panel( void );

	void OnCheckBox( wxCommandEvent &i_event)
	{
		m_button->Enable( i_event.IsChecked() );
	}

	void OnUpdateLabelUI( wxUpdateUIEvent &i_event )
	{
		wxFileDialog* const dialog = wxStaticCast( GetParent(), wxFileDialog );
		const wxString filename = dialog->GetCurrentlySelectedFilename();

		wxString msg;
		if( filename.empty() )
			msg = "Nothing";
		else if( wxFileName::FileExists(filename) )
			msg = "File";
		else if( wxFileName::DirExists(filename) )
			msg = "Directory";
		else
			msg = "Something else";

		i_event.SetText( msg + " selected" );
	}

	wxButton *m_button;
	wxCheckBox *m_checkBox;
	wxStaticText *m_label;
};

#endif	// #ifndef _PANEL_H_