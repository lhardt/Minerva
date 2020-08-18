#include "gui.hpp"

#include <wx/spinctrl.h>
#include <wx/combobox.h>

extern "C" {
	#include "types.h"
	#include "loader.h"
	#include "preprocess.h"
	#include "util.h"
};


AddRoomPane::AddRoomPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i;

	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_room);
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * capacity_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_room_size_in_students);
	wxStaticText * grid_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_room_availibility);
	wxButton * button_go = new wxButton(this,wxID_ANY, m_owner->m_lang->str_add_room, wxDefaultPosition, wxSize(200,30));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	name_label->SetFont(*m_owner->m_small_font);
	capacity_label->SetFont(*m_owner->m_small_font);
	grid_label->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,90), wxSize(200,30));
	m_capacity_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxPoint(30,150), wxSize(200,30));
	m_grid = new ChoiceGrid(this, wxID_ANY, wxPoint(30,210), wxSize(500,200));

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(m_owner->m_lang->str_adj__open);
	grid_values.push_back(m_owner->m_lang->str_adj__closed);
	m_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200));
	m_grid->SetBackgroundColors(grid_colors);

	m_grid->m_basic_col_name = m_owner->m_lang->str_day;
	m_grid->m_basic_row_name = m_owner->m_lang->str_period;

	m_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);

	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i] == false){
			m_grid->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
		}
	}

	button_go->Bind(wxEVT_BUTTON, &AddRoomPane::OnCreateButtonClicked, this);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0,  wxLEFT | wxTOP, 15);
	sizer->Add(m_name_text, 0, wxLEFT , 15);
	sizer->Add(capacity_label, 0, wxLEFT | wxTOP, 15);
	sizer->Add(m_capacity_text, 0, wxLEFT , 15);
	sizer->Add(grid_label, 0, wxLEFT | wxTOP, 15);
	sizer->Add(m_grid, 1, wxLEFT, 15);
	sizer->Add(button_go, 0,  wxALL, 15);
	sizer->Add(m_err_msg, 0,  wxLEFT, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
    Layout();
}

void AddRoomPane::ClearInsertedData(){
	School * school = m_owner->m_school;
	int i;
	m_name_text->Clear();
	m_capacity_text->SetValue(0);
	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i]){
			m_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), m_owner->m_lang->str_adj__open);
			m_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), wxColor(200,200,255));
		} else {
			m_grid->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
		}
	}
	m_score_text->SetValue(0);
}

AddRoomPane::~AddRoomPane(){

}

void AddRoomPane::OnCreateButtonClicked(wxCommandEvent & ev){
	int i;
	School * school = m_owner->m_school;
	if(!m_name_text->GetValue().IsEmpty() && (m_capacity_text->GetValue() > 0)){
		Room room;
		room.name = copy_wx_string(m_name_text->GetValue());
		room.short_name = copy_wx_string(m_name_text->GetValue());
		room.size = m_capacity_text->GetValue();
		room.active = true;
		room.availability = (int*)calloc(school->n_periods + 1, sizeof(int));
		for(i = 0; i < school->n_periods; ++i){
			room.availability[i] =
					(m_grid->GetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day))==m_owner->m_lang->str_adj__open ? 1:0);
		}
		room.availability[school->n_periods] = -1;
		int id = insert_room(stdout, m_owner->m_database, &room, school);
		if(id != -1){
			room.id = id;
			school_room_add(m_owner->m_school, &room);
			m_err_msg->SetLabel(m_owner->m_lang->str_success);
			ClearInsertedData();
			m_owner->NotifyNewUnsavedData();
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}

}
