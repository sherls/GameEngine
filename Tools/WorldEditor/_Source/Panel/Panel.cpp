#include "Panel.h"

Panel::Panel( wxWindow *i_parent ) :
	wxPanel( i_parent )
{
	m_button = new wxButton( this, -1, wxT("Custom Button") );
	m_button->Enable( false );
	m_checkBox = new wxCheckBox( this, -1, wxT("Enable Custom Button") );
	m_checkBox->Connect( wxEVT_CHECKBOX, wxCommandEventHandler(Panel::OnCheckBox), NULL, this );
	m_label = new wxStaticText( this, wxID_ANY, "Nothing selected" );
	m_label->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler(Panel::OnUpdateLabelUI), NULL, this );

	wxBoxSizer *sizerTop = new wxBoxSizer( wxHORIZONTAL );
	sizerTop->Add( m_checkBox, wxSizerFlags().Centre().Border() );
	sizerTop->AddStretchSpacer();
	sizerTop->Add( m_button, wxSizerFlags().Centre().Border() );
	sizerTop->AddStretchSpacer();
	sizerTop->Add( m_label, wxSizerFlags().Centre().Border() );

	SetSizerAndFit( sizerTop );
}

Panel::~Panel( void )
{
	if( m_button )
	{
		delete m_button;
		m_button = NULL;
	}

	if( m_checkBox )
	{
		delete m_checkBox;
		m_checkBox = NULL;
	}

	if( m_label )
	{
		delete m_label;
		m_label = NULL;
	}
}