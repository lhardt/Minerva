#include "gui.hpp"

#include <wx/spinctrl.h>
#include <wx/combobox.h>

extern "C" {
	#include "types.h"
	#include "loader.h"
	#include "util.h"
};


AddRoomPane::AddRoomPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i;

	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(250,250,250));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * capacity_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_room_size_in_students);
	wxStaticText * grid_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_room_availability);
	wxButton * button_go = new wxButton(this,wxID_ANY, m_owner->m_lang->str_add_room, wxDefaultPosition, wxSize(200,30));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	name_label->SetFont(*m_owner->m_small_font);
	capacity_label->SetFont(*m_owner->m_small_font);
	grid_label->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,90), wxSize(200,-1));
	m_capacity_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxPoint(30,150), wxSize(200,-1));
	m_grid = new ChoiceGrid(m_owner,this, wxID_ANY);

	m_grid->AddState(m_owner->m_lang->str_adj__closed, wxColor(255,200,200));
	m_grid->AddState(m_owner->m_lang->str_adj__open, wxColor(200,200,255));

	m_grid->SetDefaultColumnLabel(m_owner->m_lang->str_day);
	m_grid->SetDefaultRowLabel(m_owner->m_lang->str_period);

	m_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);

	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i] == false){
			m_grid->SetCellLocked(i % school->n_periods_per_day, i / school->n_periods_per_day);
		}
	}
	m_grid->SetAllActiveCellsState(1);

	button_go->Bind(wxEVT_BUTTON, &AddRoomPane::OnCreateButtonClicked, this);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
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
		m_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, school->periods[i] ? 1 : ChoiceGrid::CELL_STATE_LOCKED);
	}
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
			int state = m_grid->GetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day);
			room.availability[i] = state >= 0? state:0; /* State -1 is for blocked cells */
		}
		room.availability[school->n_periods] = -1;
		RoomInsertAction * act = new RoomInsertAction(m_owner, room);
		if(m_owner->Do(act)){
			ClearInsertedData();
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}
