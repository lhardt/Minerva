#include "gui.hpp"

DescSchoolPane::DescSchoolPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Detalhes da Escola"), wxDefaultPosition, wxSize(300,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * school_name_text = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("Nome da Escola: %s"),wxString::FromUTF8(school->name)), wxDefaultPosition, wxSize(300,20));
	wxStaticText * school_nppd_text = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("N° de Períodos por Dia: %d"),school->n_periods_per_day), wxDefaultPosition, wxSize(300,20));
	wxStaticText * school_nday_text = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("N° de Dias: %d"),school->n_days), wxDefaultPosition, wxSize(300,20));

	wxStaticText * grid_label = new wxStaticText(this, wxID_ANY, wxT("Períodos"), wxDefaultPosition, wxSize(250,15));
	grid_label->SetFont(*m_owner->m_small_font);

	ChoiceGrid * m_grid = new ChoiceGrid(this, wxID_ANY, wxDefaultPosition, wxSize(500,200));

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

	m_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);

	for(int i = 0; i < school->n_periods; ++i){
		if(school->periods[i] == false){
			m_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), wxT("Fechada"));
			m_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),wxColor(255,200,200));
		} else {
			m_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), wxT("Aberta"));
			m_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),wxColor(200,200,255));
		}
	}


	sizer->Add(title, 0, wxCENTER | wxALL, 15);
	sizer->Add(school_name_text, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);
	sizer->Add(school_nppd_text, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);
	sizer->Add(school_nday_text, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);
	sizer->Add(grid_label, 0, wxLEFT | wxRIGHT, 15);
	sizer->Add(m_grid, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
}

DescSchoolPane::~DescSchoolPane(){

}
