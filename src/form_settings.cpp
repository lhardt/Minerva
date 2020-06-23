#include "gui.hpp"

SettingsForm::SettingsForm(Application * owner)  : wxFrame(nullptr, wxID_ANY, wxString::FromUTF8("Horário Escolar Minerva"), wxPoint(30,30), wxSize(800,600)){
	this->m_owner = owner;
	SetMinSize(wxSize(800,600));
	SetFont(*m_owner->m_text_font);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_system_config, wxDefaultPosition, wxSize(400,30));
	title->SetFont(*m_owner->m_page_title_font);

	wxArrayString lang_names;
	lang_names.Add(wxT("Deutsch"));
	lang_names.Add(wxT("English"));
	lang_names.Add(wxT("Español"));
	lang_names.Add(wxT("Português"));

	wxStaticText * lang_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_language, wxDefaultPosition, wxSize(300,20));
	m_lang_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(300,30), lang_names);

	wxArrayString size_names;
	size_names.Add(m_owner->m_lang->str_small_font);
	size_names.Add(m_owner->m_lang->str_medium_font);
	size_names.Add(m_owner->m_lang->str_large_font);

	wxStaticText * font_size_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_font_sz, wxDefaultPosition, wxSize(300,20));
	m_font_size_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(300,30), size_names);

	wxButton * save_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_save, wxDefaultPosition, wxSize(150,30));
	wxButton * back_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_back, wxDefaultPosition, wxSize(150,30));

	wxSizer * btn_sz = new wxBoxSizer(wxHORIZONTAL);
	btn_sz->Add(save_btn, 0, wxRIGHT, 5);
	btn_sz->Add(back_btn, 0, wxRIGHT, 5);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(lang_label, 0, wxLEFT | wxRIGHT, 15);
	sizer->Add(m_lang_choice, 0, wxBOTTOM | wxLEFT | wxRIGHT, 15);
	sizer->Add(font_size_label, 0, wxLEFT | wxRIGHT, 15);
	sizer->Add(m_font_size_choice, 0, wxBOTTOM | wxLEFT | wxRIGHT, 15);
	sizer->Add(btn_sz, 0, wxBOTTOM | wxLEFT | wxRIGHT, 15);

	wxSizer * buffer_sz = new wxBoxSizer(wxVERTICAL);
	buffer_sz->Add(sizer, 1, wxALL, 15);

	save_btn->Bind(wxEVT_BUTTON, &SettingsForm::OnSaveButtonClicked, this);
	back_btn->Bind(wxEVT_BUTTON, &SettingsForm::OnBackButtonClicked, this);


	switch(m_owner->m_font_sz){
		case 10: {
			m_font_size_choice->SetSelection(0);
			break;
		}
		case 12: {
			m_font_size_choice->SetSelection(1);
			break;
		}
		case 14: {
			m_font_size_choice->SetSelection(2);
			break;
		}
	}

	if(m_owner->m_lang == &LANG_DEU){
		m_lang_choice->SetSelection(0);
	} else if(m_owner->m_lang == &LANG_ENG){
		m_lang_choice->SetSelection(1);
	} else if(m_owner->m_lang == &LANG_SPA){
		m_lang_choice->SetSelection(2);
	} else if(m_owner->m_lang == &LANG_POR){
		m_lang_choice->SetSelection(3);
	}

	if(m_owner->m_lang == NULL){
		printf("wtf man;");
	}

	SetSizer(buffer_sz);
	Layout();
}

void SettingsForm::OnSaveButtonClicked(wxCommandEvent& evt){
	if(m_lang_choice->GetSelection() != wxNOT_FOUND){
		switch(m_lang_choice->GetSelection()){
			case 0: {
				m_owner->m_lang = &LANG_DEU;
				break;
			}
			case 1: {
				m_owner->m_lang = &LANG_ENG;
				break;
			}
			case 2: {
				m_owner->m_lang = &LANG_SPA;
				break;
			}
			case 3: {
				m_owner->m_lang = &LANG_POR;
				break;
			}
		}
	}
	if(m_font_size_choice->GetSelection() != wxNOT_FOUND){
		switch(m_font_size_choice->GetSelection()){
			case 0: {
				m_owner->m_font_sz = 10;
				break;
			}
			case 1: {
				m_owner->m_font_sz = 12;
				break;
			}
			case 2: {
				m_owner->m_font_sz = 14;
				break;
			}
		}
	}
	m_owner->OnConfigUpdate();
	m_owner->SwitchForm(FORM_SETTINGS);
	this->Destroy();
}

void SettingsForm::OnBackButtonClicked(wxCommandEvent & evt){
	m_owner->SwitchForm(FORM_WELCOME);
	this->Destroy();
}

SettingsForm::~SettingsForm(){

}
