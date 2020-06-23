#include "gui.hpp"

#include <wx/wx.h>
#include <sqlite3.h>

extern "C" {
	#include "loader.h"
};

WelcomeForm::WelcomeForm(Application * owner) : wxFrame(nullptr, wxID_ANY, wxString::FromUTF8("Horário Escolar Minerva"), wxPoint(30,30), wxSize(800,600)) {
	int i = 0;
	int * school_ids;
	char ** school_names;

	#ifdef __WXMSW__
		SetIcon(wxICON(aaaaaaaa));
	#endif

	m_owner = owner;

	SetMinSize(wxSize(800,600));
	SetBackgroundColour(wxColour(0x25,0x75,0xb0));

	wxPanel * m_footer = new wxPanel(this, wxID_ANY, wxPoint(0,575), wxSize(800,25));
	m_footer->SetBackgroundColour(wxColor(0x25,0x75,0xb0));
	wxStaticText * m_footer_text = new wxStaticText(m_footer, wxID_ANY,wxT("Copyright (C) Léo Hardt 2020. Licença a ser definida."),
				wxPoint(0,5), wxSize(800,20), wxST_NO_AUTORESIZE |wxALIGN_CENTRE | wxALIGN_CENTRE_HORIZONTAL);
	m_footer_text->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_footer_text->SetFont(*m_owner->m_small_font);

	wxStaticText * m_title = new wxStaticText(this, wxID_ANY,wxString::FromUTF8("Horário Escolar Minerva"), wxPoint(50,140), wxSize(300,40));
	m_title->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_title->SetFont(*m_owner->m_title_font);

	wxStaticText * m_description = new wxStaticText(this, wxID_ANY,
		wxT("Comece agora a criar seu próprio horário escolar.\nSobre qual escola estamos falando hoje?"),
			wxPoint(50,190), wxSize(300,50));
	m_description->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_description->SetFont(*m_owner->m_text_font);

	school_names = select_all_school_names(stdout, m_owner->m_database, &school_ids);

	m_image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(*owner->m_island_image), wxPoint(400,50));
	m_dropdown = new wxChoice(this, wxID_ANY, wxPoint(50,260), wxSize(240,30));

	for(i = 0; school_names[i] != NULL; ++i){
		m_dropdown->Append(wxString(school_names[i]), new IntClientData(school_ids[i], wxString::FromUTF8(school_names[i])));
	}

	m_dropdown->SetSelection(0);

	m_button_open = new wxButton(this, wxID_ANY, wxT("Abrir"), wxPoint(300, 260), wxSize(55,30));
	m_button_import = new wxButton(this, wxID_ANY, wxT("Ajuda"), wxPoint(50, 305), wxSize(95,30));
	m_button_settings = new wxButton(this, wxID_ANY, wxT("Configurações"), wxPoint(155, 305), wxSize(95,30));
	m_button_create = new wxButton(this, wxID_ANY, wxT("Criar"), wxPoint(260, 305), wxSize(95,30));

	if(i == 0){
		m_button_open->Disable();
	}

	m_button_open->Bind(wxEVT_BUTTON, &WelcomeForm::OnOpenClicked, this);
	m_button_import->Bind(wxEVT_BUTTON, &WelcomeForm::OnHelpClicked, this);
	m_button_settings->Bind(wxEVT_BUTTON, &WelcomeForm::OnSettingsClicked, this);
	m_button_create->Bind(wxEVT_BUTTON, &WelcomeForm::OnCreateClicked, this);

	this->Refresh();

	for(i = 0; school_names[i] != NULL; ++i){
		free(school_names[i]);
	}
	free(school_names);
	free(school_ids);
}

void WelcomeForm::OnCreateClicked(wxCommandEvent & ev){
	m_owner->SwitchForm(FORM_CREATE_SCHOOL);
	this->Destroy();
	ev.Skip();
}

void WelcomeForm::OnOpenClicked(wxCommandEvent & ev){
	if( wxNOT_FOUND == m_dropdown->GetCurrentSelection()){
		printf("No item selected\n");
	} else {
		IntClientData * data = (IntClientData *) m_dropdown->GetClientObject( m_dropdown->GetSelection());
		m_owner->m_school = select_school_by_id( stdout, m_owner->m_database, data->m_value);
		if(m_owner->m_school == NULL){
			printf("Could not load school.\n");
		} else {
			m_owner->SwitchForm(FORM_MAIN_MENU);
			this->Destroy();
		}
	}
	ev.Skip();
}

void WelcomeForm::OnHelpClicked(wxCommandEvent & ev){
	if(m_owner->m_window_manual == nullptr){
		m_owner->m_window_manual = new ManualWindow(m_owner);
		m_owner->m_window_manual->Show();
	}
}

void WelcomeForm::OnSettingsClicked(wxCommandEvent & ev){
	m_owner->SwitchForm(FORM_SETTINGS);
	Destroy();
}

WelcomeForm::~WelcomeForm(){

}
