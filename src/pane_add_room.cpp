#include "gui.hpp"

#include <wx/spinctrl.h>
#include <wx/combobox.h>

extern "C" {
	#include "types.h"
	#include "loader.h"
	#include "preprocess.h"
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
	wxStaticText * features_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_room_add_features_with_scores);

	name_label->SetFont(*m_owner->m_small_font);
	capacity_label->SetFont(*m_owner->m_small_font);
	grid_label->SetFont(*m_owner->m_small_font);
	features_label->SetFont(*m_owner->m_small_font);

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

	wxButton * button_go = new wxButton(this,wxID_ANY, m_owner->m_lang->str_add_room, wxDefaultPosition, wxSize(200,30));
	m_features = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(180,30));
	m_score_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(120,30));
	wxButton * add_feature = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_feature, wxDefaultPosition, wxSize(180,30));
	m_added_features = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));
	wxButton * remove_feature = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(180,30));
	wxButton * edit_feature = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit, wxDefaultPosition, wxSize(180,30));

	button_go->Bind(wxEVT_BUTTON, &AddRoomPane::OnCreateButtonClicked, this);

	for(i = 0; (i < m_owner->m_school->n_features) && (NULL != m_owner->m_school->feature_names[i]) ; ++i){
		IntClientData * data = new IntClientData(i);
		m_features->Insert(wxString::FromUTF8(m_owner->m_school->feature_names[i]), i, data);
	}

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * feature_hor_sz = new wxBoxSizer(wxHORIZONTAL);

	wxSizer * feature_ver_sz = new wxBoxSizer(wxVERTICAL);
	add_sizer->Add(m_features,0,wxRIGHT,10);
	add_sizer->Add(m_score_text,0,wxRIGHT,10);
	add_sizer->Add(add_feature,0,wxRIGHT,10);

	feature_hor_sz->Add(m_added_features, 0, wxALL, 0);
	feature_hor_sz->Add(feature_ver_sz, 0, 0);

	feature_ver_sz->Add(remove_feature, 0, wxLEFT | wxBOTTOM, 10);
	feature_ver_sz->Add(edit_feature, 0, wxLEFT, 10);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0,  wxLEFT | wxTOP, 15);
	sizer->Add(m_name_text, 0, wxLEFT , 15);
	sizer->Add(capacity_label, 0, wxLEFT | wxTOP, 15);
	sizer->Add(m_capacity_text, 0, wxLEFT , 15);
	sizer->Add(grid_label, 0, wxLEFT | wxTOP, 15);
	sizer->Add(m_grid, 1, wxLEFT, 15);
	sizer->Add(features_label, 0, wxLEFT | wxTOP, 15);
	sizer->Add(add_sizer, 0,  wxLEFT, 15);
	sizer->Add(feature_hor_sz, 0, wxLEFT | wxTOP, 15);
	sizer->Add(button_go, 0,  wxALL, 15);
	sizer->Add(m_err_msg, 0,  wxLEFT, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
    Layout();

	add_feature->Bind(wxEVT_BUTTON, &AddRoomPane::OnAddFeatureClicked, this);
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
	m_features->SetSelection(wxNOT_FOUND);
	m_added_features->Clear();
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
		for(i = 0; i < m_features->GetCount(); ++i){
			IntPairClientData * data = (IntPairClientData*) m_features->GetClientObject(i);
			room.room_features = (int*)calloc(school->n_features + 11 - (school->n_features % 10), sizeof(int));
			room.room_features[data->m_v1] = data->m_v2;
		}
		room.room_features[m_features->GetCount()] = -1;

		room.disponibility = (int*)calloc(school->n_periods + 1, sizeof(int));
		for(i = 0; i < school->n_periods; ++i){
			room.disponibility[i] =
					(m_grid->GetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day))==m_owner->m_lang->str_adj__open ? 1:0);
		}
		room.disponibility[school->n_periods] = -1;

		int id = insert_room(stdout, m_owner->m_database, &room, school);
		if(id != -1){
			room.id = id;
			school_room_add(m_owner->m_school, &room);
			m_err_msg->SetLabel(m_owner->m_lang->str_success);
			ClearInsertedData();
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}

}

void AddRoomPane::OnAddFeatureClicked(wxCommandEvent & ev){
	if(m_features->GetSelection() != wxNOT_FOUND){
		//m_feature_values[ m_features->GetSelection() ] = m_score_text->GetValue();
		IntPairClientData* data = new IntPairClientData(m_features->GetSelection(), m_score_text->GetValue());
		wxString to_be_inserted = wxString::FromUTF8(m_owner->m_school->feature_names[  m_features->GetSelection() ]) << wxT(": ") << m_score_text->GetValue();
		m_added_features->Insert(to_be_inserted, m_added_features->GetCount(), data);
		m_score_text->SetValue(0);
	}
}
