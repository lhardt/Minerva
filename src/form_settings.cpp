#include "gui.hpp"

SettingsForm::SettingsForm(Application * owner)  : wxFrame(nullptr, wxID_ANY, wxString::FromUTF8("Horário Escolar Minerva"), wxPoint(30,30), wxSize(800,600)){
	this->m_owner = owner;
	SetMinSize(wxSize(800,600));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Configurações do Sistema"), wxDefaultPosition, wxSize(400,30));
	title->SetFont(*m_owner->m_page_title_font);

	wxArrayString lang_names;
	lang_names.Add(wxT("Deutsch"));
	lang_names.Add(wxT("English"));
	lang_names.Add(wxT("Español"));
	lang_names.Add(wxT("Português"));

	wxStaticText * lang_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_language, wxDefaultPosition, wxSize(300,20));
	wxChoice * lang_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(300,30), lang_names);

	wxButton * save_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_save, wxDefaultPosition, wxSize(200,30));
	wxButton * back_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_back, wxDefaultPosition, wxSize(200,30));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(lang_label, 0, wxLEFT | wxRIGHT, 15);
	sizer->Add(lang_choice, 0, wxBOTTOM | wxLEFT | wxRIGHT, 15);

	wxSizer * buffer_sz = new wxBoxSizer(wxVERTICAL);
	buffer_sz->Add(sizer, 1, wxALL, 15);

	SetSizer(buffer_sz);
	Layout();
}

void SettingsForm::OnSaveButtonClicked(wxCommandEvent& evt){

}

SettingsForm::~SettingsForm(){

}
