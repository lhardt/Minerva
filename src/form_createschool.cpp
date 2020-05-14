#include "gui.hpp"
#include <wx/grid.h>
#include <wx/spinctrl.h>
CreateSchoolForm::CreateSchoolForm(Application * owner)  : wxFrame(nullptr, wxID_ANY, "Horário Escolar Minerva", wxPoint(30,30), wxSize(800,600)){
	m_owner = owner;

	SetMinSize(wxSize(800,600));
	SetBackgroundColour(wxColour(240,240,240));

	wxPanel * m_right_pane = new wxPanel(this, wxID_ANY, wxPoint(500,0), wxSize(300,575));
	m_right_pane->SetBackgroundColour(wxColor(0x29, 0x80, 0xb9));

	wxPanel * m_footer = new wxPanel(this, wxID_ANY, wxPoint(0,575), wxSize(800,25));
	m_footer->SetBackgroundColour(wxColor(0x25,0x75,0xb0));


	wxStaticText * m_footer_text = new wxStaticText(m_footer, wxID_ANY,wxT("Copyright (C) Léo Hardt 2019-2020. This program is free software protected by Mozilla Public License v2.0."),
	wxPoint(0,5), wxSize(800,20), wxST_NO_AUTORESIZE |wxALIGN_CENTRE | wxALIGN_CENTRE_HORIZONTAL);
	m_footer_text->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_footer_text->SetFont(*m_owner->m_small_font);

	wxStaticText * m_title = new wxStaticText(this, wxID_ANY, wxT("Cadastrar Escola"), wxPoint(30,50), wxSize(100,25));
	m_title->SetFont(*m_owner->m_page_title_font);
	m_title->SetForegroundColour(wxColor(0x00, 0x00, 0x00));

	wxStaticText * m_school_name_label = new wxStaticText(this, wxID_ANY, wxT("Nome da Escola"), wxPoint(30,90), wxSize(200,15));
	m_school_name_label->SetFont(*m_owner->m_small_font);
	m_school_name_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));
	m_school_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,105), wxSize(200,25));

	wxStaticText * m_number_of_days_label = new wxStaticText(this, wxID_ANY, wxT("Número de Dias em um Ciclo"), wxPoint(30,145), wxSize(200,15));
	m_number_of_days_label->SetFont(*m_owner->m_small_font);
	m_number_of_days_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));
	m_number_of_days_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxPoint(30,160), wxSize(200,25));

	wxStaticText * m_number_of_periods_label = new wxStaticText(this, wxID_ANY, wxT("Número de Periodos por Dia"), wxPoint(30,200), wxSize(200,15));
	m_number_of_periods_label->SetFont(*m_owner->m_small_font);
	m_number_of_periods_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));
	m_number_of_periods_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxPoint(30,215), wxSize(200,25));

	m_number_of_periods_text->Bind(wxEVT_SPINCTRL, &CreateSchoolForm::OnGridSizeUpdated, this);
	m_number_of_days_text->Bind(wxEVT_SPINCTRL, &CreateSchoolForm::OnGridSizeUpdated, this);

	wxStaticText * m_names_label = new wxStaticText(this, wxID_ANY, wxT("A Escola Funciona em Quais Períodos?"), wxPoint(30,255), wxSize(400,15));
	m_names_label->SetFont(*m_owner->m_small_font);
	m_names_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));

	m_grid = new ChoiceGrid(this, wxID_ANY, wxPoint(30,270), wxSize(400,200));

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(wxT("Aberta"));
	grid_values.push_back(wxT("Fechada"));
	m_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200)); 
	m_grid->SetBackgroundColors(grid_colors);

	m_grid->m_basic_col_name = wxT("Dia");
	m_grid->m_basic_row_name = wxT("Período");


	m_grid->GridRemake(m_number_of_days_text->GetValue(), m_number_of_periods_text->GetValue());



	m_button_create = new wxButton(this, wxID_ANY, wxT("Criar"), wxPoint(350, 500), wxSize(80,30));
	m_button_back = new wxButton(this, wxID_ANY, wxT("Voltar"), wxPoint(260, 500), wxSize(80,30));

	m_button_create->Bind(wxEVT_BUTTON, &CreateSchoolForm::OnCreateClicked, this);
	m_button_back->Bind(wxEVT_BUTTON, &CreateSchoolForm::OnBackClicked, this);

	Refresh();
}

CreateSchoolForm::~CreateSchoolForm(){

}

void CreateSchoolForm::OnGridSizeUpdated(wxSpinEvent & ev){
	m_grid->GridRemake(m_number_of_days_text->GetValue(),m_number_of_periods_text->GetValue());
}

void CreateSchoolForm::OnBackClicked(wxCommandEvent & ev){
	m_owner->SwitchForm(FORM_WELCOME);
	this->Destroy();
}

void CreateSchoolForm::OnCreateClicked(wxCommandEvent & ev){
	m_owner->SwitchForm(FORM_MAIN_MENU);
	this->Destroy();
}
