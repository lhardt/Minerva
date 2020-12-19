#include "gui.hpp"

#include <wx/wx.h>
#include <sqlite3.h>

extern "C" {
	#include "loader.h"
};

WelcomeForm::WelcomeForm(Application * owner) : wxFrame(nullptr, wxID_ANY, owner->m_lang->str_minerva_school_timetables, wxPoint(30,30), wxSize(800,600)) {
	int i = 0;
	int * school_ids;
	char ** school_names;

	#ifdef __WXMSW__
		SetIcon(wxICON(aaaaaaaa));
	#endif

	m_owner = owner;

	SetFont(*m_owner->m_text_font);
	wxPanel * panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(800,600), wxTAB_TRAVERSAL);
	wxPanel * buttons_wrap = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	panel->SetBackgroundColour(wxColour(0x25,0x75,0xb0));
	buttons_wrap->SetBackgroundColour(wxColour(0xFF,0xFF,0xFF));

	wxStaticText * m_footer_text = new wxStaticText(panel, wxID_ANY,m_owner->m_lang->str_copyright_notice,wxDefaultPosition, wxSize(800,-1), wxST_NO_AUTORESIZE |wxALIGN_CENTRE | wxALIGN_CENTRE_HORIZONTAL);
	m_footer_text->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_footer_text->SetFont(*m_owner->m_small_font);

	wxStaticText * m_title = new wxStaticText(panel, wxID_ANY,m_owner->m_lang->str_minerva_school_timetables, wxDefaultPosition, wxDefaultSize);
	m_title->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_title->SetFont(*m_owner->m_title_font);

	wxStaticText * m_description = new wxStaticText(panel, wxID_ANY, m_owner->m_lang->str_minerva_welcome_description, wxDefaultPosition, wxDefaultSize);
	m_description->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_description->SetFont(*m_owner->m_text_font);
	school_names = select_all_school_names(stdout, m_owner->m_database, &school_ids);

	m_image = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(*owner->m_island_image), wxDefaultPosition);
	m_dropdown = new wxChoice(buttons_wrap, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	for(i = 0; school_names[i] != NULL; ++i){
		m_dropdown->Append(wxString::FromUTF8(school_names[i]), new IntClientData(school_ids[i], wxString::FromUTF8(school_names[i])));
	}

	m_dropdown->SetSelection(0);

	m_button_open = new wxButton(buttons_wrap, wxID_ANY, m_owner->m_lang->str_open);
	m_button_help = new wxButton(buttons_wrap, wxID_ANY, m_owner->m_lang->str_help);
	m_button_settings = new wxButton(buttons_wrap, wxID_ANY, m_owner->m_lang->str_settings);
	m_button_create = new wxButton(buttons_wrap, wxID_ANY, m_owner->m_lang->str_create);

	if(i == 0){
		m_button_open->Disable();
	}

	m_button_open->Bind(wxEVT_BUTTON, &WelcomeForm::OnOpenClicked, this);
	m_button_help->Bind(wxEVT_BUTTON, &WelcomeForm::OnHelpClicked, this);
	m_button_settings->Bind(wxEVT_BUTTON, &WelcomeForm::OnSettingsClicked, this);
	m_button_create->Bind(wxEVT_BUTTON, &WelcomeForm::OnCreateClicked, this);


	for(i = 0; school_names[i] != NULL; ++i){
		free(school_names[i]);
	}
	free(school_names);
	free(school_ids);

	wxSizer * frame_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * center_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * left_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * buttons_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * button_sz1 = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * button_sz2 = new wxBoxSizer(wxHORIZONTAL);

	panel->SetSizer(sizer);

	sizer->Add(center_sz, 1, wxCENTER|wxALL, 15);
	sizer->Add(m_footer_text, 0, wxCENTER | wxALL, 5);

	button_sz1->Add(m_dropdown, 1, wxEXPAND | wxALL, 5);
	button_sz1->Add(m_button_open, 0, wxEXPAND | wxALL, 5);

	button_sz2->Add(m_button_help, 0, wxEXPAND | wxALL, 5);
	button_sz2->Add(m_button_settings, 0, wxEXPAND | wxALL, 5);
	button_sz2->Add(m_button_create, 0, wxEXPAND | wxALL, 5);

	buttons_sz->Add(button_sz1, 0,wxEXPAND);
	buttons_sz->Add(button_sz2, 0,wxEXPAND);

	buttons_wrap->SetSizer(buttons_sz);

	left_sz->Add(m_title, 0, wxALL,  15);
	left_sz->Add(m_description, 0, wxALL, 15);
	left_sz->Add(buttons_wrap, 0, wxALL, 15);

	center_sz->Add(left_sz, 0, wxCENTER |wxRIGHT, 15);
	center_sz->Add(m_image, 0, wxCENTER |wxLEFT, 15);

	SetSize(wxSize(800,600));
	frame_sz->Add(panel,1,wxEXPAND,0);
	SetSizerAndFit(frame_sz);
	SetSize(wxSize(800,600));
}

void WelcomeForm::OnCreateClicked(wxCommandEvent & ev){
	m_owner->SwitchForm(FORM_CREATE_SCHOOL);
	ev.Skip();
}

void WelcomeForm::OnOpenClicked(wxCommandEvent & ev){
	if( wxNOT_FOUND != m_dropdown->GetCurrentSelection()){
		IntClientData * data = (IntClientData *) m_dropdown->GetClientObject( m_dropdown->GetSelection());
		m_owner->m_school = select_school_by_id( stdout, m_owner->m_database, data->m_value);
		if(m_owner->m_school != NULL){
			m_owner->SwitchForm(FORM_MAIN_MENU);
		} else {
			/* TODO proper error handling */
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
}

WelcomeForm::~WelcomeForm(){

}
