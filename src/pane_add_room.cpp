#include "gui.hpp"

#include <wx/spinctrl.h>
#include <wx/combobox.h>

AddRoomPane::AddRoomPane(Application * owner, wxWindow * parent, wxPoint pos)  : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Adicionar Sala"), wxPoint(30,30), wxSize(200,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, wxT("Nome da Sala"), wxPoint(30,75), wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,90), wxSize(200,30));


	wxStaticText * capacity_label = new wxStaticText(this, wxID_ANY, wxT("Capacidade, em número de alunos, da sala"), wxPoint(30,135), wxSize(200,15));
	capacity_label->SetFont(*m_owner->m_small_font);
	m_capacity_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxPoint(30,150), wxSize(200,30));


	wxStaticText * grid_label = new wxStaticText(this, wxID_ANY, wxT("A sala fica aberta em quais períodos?"), wxPoint(30,195), wxSize(250,15));
	grid_label->SetFont(*m_owner->m_small_font);
	m_grid = new ChoiceGrid(this, wxID_ANY, wxPoint(30,210), wxSize(500,200));

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

	m_grid->GridRemake(7,5);

	wxButton * button_go = new wxButton(this,wxID_ANY, wxT("Adicionar Sala"), wxPoint(30,425), wxSize(200,30));

	button_go->Bind(wxEVT_BUTTON, &AddRoomPane::OnCreateButtonClicked, this);

	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText * characteristics_label = new wxStaticText(this, wxID_ANY, wxT("Adicione Características dessa Sala, com suas respectivas \"notas\"."), wxDefaultPosition, wxSize(200,15));
	characteristics_label->SetFont(*m_owner->m_small_font);
	m_characteristics = new wxComboBox(this, wxID_ANY, wxT("Características"), wxDefaultPosition, wxSize(180,30));
	m_score_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(120,30));
	wxButton * add_characteristic = new wxButton(this, wxID_ANY, wxT("Adicionar Característica"), wxDefaultPosition, wxSize(180,30));
	add_sizer->Add(m_characteristics,0,wxRIGHT,10);
	add_sizer->Add(m_score_text,0,wxRIGHT,10);
	add_sizer->Add(add_characteristic,0,wxRIGHT,10);

	wxSizer * characteristics_hor_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * characteristics_ver_sz = new wxBoxSizer(wxVERTICAL);

	m_added_characteristics = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	characteristics_hor_sz->Add(m_added_characteristics, 0, wxALL, 0);
	characteristics_hor_sz->Add(characteristics_ver_sz, 0, 0);

	wxButton * remove_characteristic = new wxButton(this, wxID_ANY, wxT("Remover"), wxDefaultPosition, wxSize(180,30));
	wxButton * edit_characteristic = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(180,30));
	characteristics_ver_sz->Add(remove_characteristic, 0, wxLEFT | wxBOTTOM, 10);
	characteristics_ver_sz->Add(edit_characteristic, 0, wxLEFT, 10);


	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(title, 0, wxFIXED_MINSIZE | wxALL, 15);
	sizer->Add(name_label, 0, wxFIXED_MINSIZE | wxLEFT | wxTOP, 15);
	sizer->Add(m_name_text, 0, wxFIXED_MINSIZE | wxLEFT , 15);
	sizer->Add(capacity_label, 0, wxFIXED_MINSIZE | wxLEFT | wxTOP, 15);
	sizer->Add(m_capacity_text, 0, wxFIXED_MINSIZE | wxLEFT , 15);
	sizer->Add(grid_label, 0, wxFIXED_MINSIZE | wxLEFT | wxTOP, 15);
	sizer->Add(m_grid, 1, wxLEFT, 15);
	sizer->Add(characteristics_label, 0, wxFIXED_MINSIZE | wxLEFT | wxTOP, 15);
	sizer->Add(add_sizer, 0, wxFIXED_MINSIZE | wxLEFT, 15);
	sizer->Add(characteristics_hor_sz, 0, wxLEFT | wxTOP, 15);
	sizer->Add(button_go, 0, wxFIXED_MINSIZE | wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
    Layout();

}

AddRoomPane::~AddRoomPane(){

}

void AddRoomPane::OnCreateButtonClicked(wxCommandEvent & ev){
	printf("Button clicked!\n");
}
