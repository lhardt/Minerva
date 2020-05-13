#include "gui.hpp"

#include <wx/wx.h>

WelcomeForm::WelcomeForm(Application * owner) : wxFrame(nullptr, wxID_ANY, "Horário Escolar Minerva", wxPoint(30,30), wxSize(800,600)) {
	m_owner = owner;

	SetMinSize(wxSize(800,600));
	SetBackgroundColour(wxColour(0x25,0x75,0xb0));

	wxPanel * m_footer = new wxPanel(this, wxID_ANY, wxPoint(0,575), wxSize(800,25));
	m_footer->SetBackgroundColour(wxColor(0x25,0x75,0xb0));
	wxStaticText * m_footer_text = new wxStaticText(m_footer, wxID_ANY,wxT("Copyright (C) Léo Hardt 2019-2020. This program is free software protected by Mozilla Public License v2.0."),
				wxPoint(0,5), wxSize(800,20), wxST_NO_AUTORESIZE |wxALIGN_CENTRE | wxALIGN_CENTRE_HORIZONTAL);
	m_footer_text->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_footer_text->SetFont(*m_owner->m_small_font);

	wxStaticText * m_title = new wxStaticText(this, wxID_ANY,wxT("Horário Escolar Minerva"), wxPoint(20,140), wxSize(300,40));
	m_title->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_title->SetFont(*m_owner->m_title_font);

	wxStaticText * m_description = new wxStaticText(this, wxID_ANY,
		wxT("Comece agora a criar seu próprio horário escolar.\nSobre qual escola estamos falando hoje?"),
			wxPoint(30,190), wxSize(300,50));
	m_description->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_description->SetFont(*m_owner->m_text_font);

	m_image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(*owner->m_island_image), wxPoint(400,50));
	m_dropdown = new wxComboBox(this, wxID_ANY, "Projetos Anteriores", wxPoint(50,260), wxSize(240,30));
	m_button_open = new wxButton(this, wxID_ANY, "Abrir", wxPoint(300, 260), wxSize(55,30));
	m_button_import = new wxButton(this, wxID_ANY, "Importar", wxPoint(50, 305), wxSize(95,30));
	m_button_delete = new wxButton(this, wxID_ANY, "Apagar", wxPoint(155, 305), wxSize(95,30));
	m_button_create = new wxButton(this, wxID_ANY, "Criar", wxPoint(260, 305), wxSize(95,30));

	m_button_open->Bind(wxEVT_BUTTON, &WelcomeForm::OnOpenClicked, this);
	m_button_import->Bind(wxEVT_BUTTON, &WelcomeForm::OnImportClicked, this);
	m_button_delete->Bind(wxEVT_BUTTON, &WelcomeForm::OnDeleteClicked, this);
	m_button_create->Bind(wxEVT_BUTTON, &WelcomeForm::OnCreateClicked, this);

	int number_of_schools = 4;
	for(int i = 0; i < number_of_schools; i ++){
		m_dropdown->Append(wxT("School"));
	}

	this->Refresh();
}

void WelcomeForm::OnCreateClicked(wxCommandEvent & ev){
	m_owner->SwitchForm(FORM_CREATE_SCHOOL);
	this->Destroy();
	ev.Skip();
}

void WelcomeForm::OnOpenClicked(wxCommandEvent & ev){
	printf("Not Implemented\n");
	ev.Skip();
}

void WelcomeForm::OnImportClicked(wxCommandEvent & ev){
	printf("Not Implemented\n");
	ev.Skip();
}

void WelcomeForm::OnDeleteClicked(wxCommandEvent & ev){
	printf("Not Implemented\n");
	ev.Skip();
}

WelcomeForm::~WelcomeForm(){

}
