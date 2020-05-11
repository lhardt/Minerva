#include "gui.hpp"
#include <wx/grid.h>

CreateSchoolForm::CreateSchoolForm(Application * owner)  : wxFrame(nullptr, wxID_ANY, "Horário Escolar Minerva", wxPoint(30,30), wxSize(800,600)){
	m_owner = owner;

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
	wxTextCtrl * m_number_of_days_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,160), wxSize(200,25));

	wxStaticText * m_number_of_periods_label = new wxStaticText(this, wxID_ANY, wxT("Número de Periodos por Dia"), wxPoint(30,200), wxSize(200,15));
	m_number_of_periods_label->SetFont(*m_owner->m_small_font);
	m_number_of_periods_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));
	wxTextCtrl * m_number_of_periods_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,215), wxSize(200,25));

	wxStaticText * m_names_label = new wxStaticText(this, wxID_ANY, wxT("Altere os Nomes dos Dias e dos Períodos como Quiser"), wxPoint(30,255), wxSize(400,15));
	m_names_label->SetFont(*m_owner->m_small_font);
	m_names_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));

	// wxHVScrolledWindow * m_days_window = new wxHVScrolledWindow(this,wxID_ANY, wxPoint(30,270), wxSize(400, 300));
	// m_days_window->SetBackgroundColour(wxColor(0xC5, 0xC5, 0xC5));

	// wxGridSizer * sizer = new wxGridSizer(5,1,1);

	grid = new wxGrid(this, wxID_ANY, wxPoint(30,270), wxSize(400,300));
	grid->CreateGrid(10,10);
	grid->SetRowLabelSize(0);
	grid->SetColLabelSize(0);
	grid->SetCellBackgroundColour(0,0,wxColor(0,0,0));
	for(int i = 1; i < 10; i++){
		grid->SetCellValue(0,i, wxT("1° Dia"));
		grid->SetCellValue(i,0, wxT("1° Período"));
		for(int j = 1; j < 10; j++){
			grid->SetReadOnly(i,j,true);
			grid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &CreateSchoolForm::OnGridLeftClick, this);
		}
	}

	// for(int i = 0; i < 80; i++){
	// 	wxButton * b = new wxButton(m_days_window, wxID_ANY, "a");
		// sizer->Add(grid);
	// }
	// m_days_window->SetSizer(sizer);
	// m_days_window->FitInside();
	// m_days_window->SetScrollRate(5, 5);
	// wxTextCtrl * m_number_of_periods_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,215), wxSize(200,25));


	// wxStaticText * m_number_of_periods_label = new wxStaticText(this, wxID_ANY, wxT("Número de Periodos por Dia"), wxPoint(30,215), wxSize(200,15));
	// m_number_of_periods_label->SetFont(*m_owner->m_small_font);
	// m_number_of_periods_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));
	// wxTextCtrl * m_number_of_periods_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,230), wxSize(200,25));



	Refresh();
}

CreateSchoolForm::~CreateSchoolForm(){

}

void CreateSchoolForm::OnGridLeftClick(wxGridEvent & evt){
	printf("Hello!\n");
	if(evt.GetCol() > 0 && evt.GetRow() > 0){
		printf("Passed %d %d\n", evt.GetCol(), evt.GetRow());
		grid->SetCellBackgroundColour(evt.GetRow(), evt.GetCol(), wxColor(0,0,120));
		grid->SetCellValue(evt.GetRow(), evt.GetCol(),"ahoj");
		evt.Skip();
	}
	grid->Refresh();
}
