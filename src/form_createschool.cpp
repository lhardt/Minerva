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
	wxTextCtrl * m_school_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,105), wxSize(200,25));

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

	// wxScrolledWindow * m_days_window = new wxScrolledWindow(this,wxID_ANY, wxPoint(30,270), wxSize(400, 300));
	// m_days_window->SetBackgroundColour(wxColor(0xC5, 0xC5, 0xC5));

	m_grid = new wxGrid(this, wxID_ANY, wxPoint(30,270), wxSize(400,200));
	m_grid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &CreateSchoolForm::OnGridLeftClick, this);
	m_grid->HideColLabels();
	m_grid->HideRowLabels();
	m_grid->CreateGrid(1,1);
	GridRemake();

	m_button_create = new wxButton(this, wxID_ANY, wxT("Criar"), wxPoint(350, 500), wxSize(80,30));
	m_button_back = new wxButton(this, wxID_ANY, wxT("Voltar"), wxPoint(260, 500), wxSize(80,30));

	m_button_create->Bind(wxEVT_BUTTON, &CreateSchoolForm::OnCreateClicked, this);
	m_button_back->Bind(wxEVT_BUTTON, &CreateSchoolForm::OnBackClicked, this);

	Refresh();
}

CreateSchoolForm::~CreateSchoolForm(){

}

void CreateSchoolForm::OnGridLeftClick(wxGridEvent & evt){
	if(evt.GetCol() > 0 && evt.GetRow() > 0){
		if(m_grid->GetCellValue(evt.GetRow(), evt.GetCol()).IsSameAs("Aberta")){
			m_grid->SetCellBackgroundColour(evt.GetRow(), evt.GetCol(), wxColor(250,200,200));
			m_grid->SetCellValue(evt.GetRow(), evt.GetCol(), wxT("Fechada"));
		} else {
			m_grid->SetCellBackgroundColour(evt.GetRow(), evt.GetCol(), wxColor(200,200,250));
			m_grid->SetCellValue(evt.GetRow(), evt.GetCol(), wxT("Aberta"));
		}
		evt.Skip();
		m_grid->Refresh();
	}
}

void CreateSchoolForm::GridRemake(){
	int i,j;

	int n_i = 1+ m_number_of_periods_text->GetValue();
	int n_j = 1+ m_number_of_days_text->GetValue();

	int old_n_i = m_grid->GetNumberRows();
	int old_n_j = m_grid->GetNumberCols();

	m_grid->SetCellBackgroundColour(0,0,wxColor(200,200,200));

	if(n_i > old_n_i){
		m_grid->AppendRows(n_i - old_n_i);
		for(i = old_n_i; i < n_i; ++i){
			m_grid->SetCellValue(i, 0, wxString::Format(wxT("Período %d"), i));
			for(j = 1; j < old_n_j; ++j){
				m_grid->SetCellBackgroundColour(i, j, wxColor(200,200,250));
				m_grid->SetCellValue(i, j, wxT("Aberta"));
				m_grid->SetReadOnly(i,j,true);
			}
		}
	} else if(n_i < old_n_i) {
		m_grid->DeleteRows(n_i, old_n_i - n_i);
	}



	if(n_j > old_n_j){
		m_grid->AppendCols(n_j - old_n_j);
		for(j = old_n_j; j < n_j; ++j){
			m_grid->SetCellValue(0, j, wxString::Format(wxT("Dia %d"), j));
			for(i = 1; i < n_i; ++i){
				m_grid->SetCellBackgroundColour(i, j, wxColor(200,200,250));
				m_grid->SetCellValue(i, j, wxT("Aberta"));
				m_grid->SetReadOnly(i,j,true);
			}
		}
	} else if(n_j < old_n_j){
		m_grid->DeleteCols(n_j, old_n_j - n_j);
	}

	m_grid->Refresh();
}

void CreateSchoolForm::OnGridSizeUpdated(wxSpinEvent & ev){
	GridRemake();
}

void CreateSchoolForm::OnBackClicked(wxCommandEvent & ev){
	m_owner->SwitchForm(FORM_WELCOME);
	this->Destroy();
}

void CreateSchoolForm::OnCreateClicked(wxCommandEvent & ev){
	m_owner->SwitchForm(FORM_MAIN_MENU);
	this->Destroy();
}
