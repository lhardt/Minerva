#include "gui.hpp"

extern "C" {
	#include "util.h"
	#include "assert.h"
};
#include <wx/spinctrl.h>

ListRoomsPane::ListRoomsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(250,250,250));

	m_rooms_list = new SearchableListPane(m_owner, this, wxID_ANY, wxDefaultPosition, wxSize(230,300));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * size_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_size);
	wxStaticText * active_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_active);
	wxNotebook   * notebook = new wxNotebook(this, wxID_ANY);
	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove);
	m_active_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_periods 	= new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_size_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));

	notebook->AddPage(m_periods, m_owner->m_lang->str_periods);

	m_periods->SetLabel(m_owner->m_lang->str_room_availability);
	ChoiceGrid * periods_grid = m_periods->GetGrid();
	periods_grid->AddState(m_owner->m_lang->str_adj__closed, wxColor(255,200,200));
	periods_grid->AddState(m_owner->m_lang->str_adj__open, wxColor(200,200,255));
	periods_grid->SetDefaultColumnLabel(m_owner->m_lang->str_day);
	periods_grid->SetDefaultRowLabel(m_owner->m_lang->str_period);

	periods_grid->GridRemake(school->n_days,school->n_periods_per_day);

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * fields_sz = new wxFlexGridSizer(4,5,5);
	wxSizer * fields_wrap = new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_basic_data);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);

	fields_sz->Add(name_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_name_text, 0, wxEXPAND);
	fields_sz->Add(size_label, 0, wxALIGN_BOTTOM | wxRIGHT | wxLEFT, 10);
	fields_sz->Add(m_size_text, 0, wxEXPAND);
	fields_sz->Add(active_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_active_text, 0, wxEXPAND);
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->Add(m_cancel_btn, 1, wxEXPAND);
	fields_sz->Add(m_edit_btn, 1, wxEXPAND);

	wxSizer * actions_sz = new wxStaticBoxSizer(wxHORIZONTAL, this, m_owner->m_lang->str_actions);
	actions_sz->Add(delete_btn, 0, wxALL, 5);

	fields_wrap->Add(fields_sz, 0, wxEXPAND | wxALL, 10);
	desc_sz->Add(fields_wrap, 0, wxEXPAND | wxBOTTOM, 5);
	desc_sz->Add(notebook, 1, wxEXPAND | wxBOTTOM, 5);
	desc_sz->Add(actions_sz, 0, wxEXPAND | wxBOTTOM, 5);

	sizer->Add(m_rooms_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(desc_sz, 1, wxEXPAND | wxRIGHT | wxTOP | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_edit_btn->Bind(wxEVT_BUTTON, &ListRoomsPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &ListRoomsPane::OnCancelButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListRoomsPane::OnDeleteButtonClicked, this);
	m_rooms_list->GetList()->Bind(wxEVT_LISTBOX, &ListRoomsPane::OnSelectionChanged, this);
	m_periods->GetCancelButton()->Bind(wxEVT_BUTTON, &ListRoomsPane::OnPeriodsCancelButtonClicked, this);
	m_periods->GetSaveButton()->Bind(wxEVT_BUTTON, &ListRoomsPane::OnPeriodsSaveButtonClicked, this);
	Bind(DATA_CHANGE_EVENT, &ListRoomsPane::OnDataChange, this);

	m_cancel_btn->Hide();
	m_name_text->Disable();
	m_size_text->Disable();
	m_active_text->Disable();

	ShowData();
}

void ListRoomsPane::ShowData(){
	int i;
	School * school = m_owner->m_school;
	m_cancel_btn->Hide();
	m_name_text->SetValue(wxT(""));
	m_size_text->SetValue(0);
	m_active_text->SetValue(false);
	m_name_text->Disable();
	m_size_text->Disable();
	m_active_text->Disable();
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	m_edit_btn->Show();
	m_rooms_list->Clear();
	if(school->n_rooms > 0){
		for(i = 0; i < school->n_rooms; ++i){
			wxString str = wxString::FromUTF8(school->rooms[i].name);
			if(school->rooms[i].active){
				m_rooms_list->AddItem(school->rooms[i].id, str);
			} else {
				m_rooms_list->AddItem(school->rooms[i].id, wxString::Format("(%s) %s", m_owner->m_lang->str_inactive, str));
			}
		}
	}
	m_periods->GetGrid()->SetAllActiveCellsState(1);
}

void ListRoomsPane::OnDataChange(wxNotifyEvent & evt) {
	ShowData();
}

void ListRoomsPane::OnPeriodsSaveButtonClicked(wxCommandEvent & evt) {
	int i_select = m_rooms_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int room_id = ((IntClientData*)m_rooms_list->GetList()->GetClientObject(i_select))->m_value;
		School * school = m_owner->m_school;
		ChoiceGrid * grid = m_periods->GetGrid();

		int * values = (int*) calloc(school->n_periods+1 , sizeof(int));
		for(int i = 0; i < school->n_periods; ++i){
			int state = grid->GetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day);
			values[i] = state >= 0 ? 1:0; /* State -1 is used for blocked cells */
		}
		values[school->n_periods] = -1;

		RoomAvailabilityUpdateAction * act = new RoomAvailabilityUpdateAction(m_owner, values, room_id);
		m_owner->Do(act);
		evt.Skip();
	}
}

void ListRoomsPane::OnPeriodsCancelButtonClicked(wxCommandEvent & evt) {
	School * school = m_owner->m_school;
	int i_select = m_rooms_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int id_room = ((IntClientData*)m_rooms_list->GetList()->GetClientObject(i_select))->m_value;
		Room * room = find_room_by_id(school, id_room);
		ChoiceGrid * periods_grid = m_periods->GetGrid();
		for(int i = 0; i < school->n_periods; ++i){
			if(school->periods[i]){
				periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, room->availability[i]?1:0);
			}
		}
	}
	evt.Skip();
}

void ListRoomsPane::OnEditButtonClicked(wxCommandEvent &){
	int i_select = m_rooms_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int id_room = ((IntClientData*)m_rooms_list->GetList()->GetClientObject(i_select))->m_value;
		if(m_name_text->IsEnabled()){
			m_cancel_btn->Hide();
			m_name_text->Disable();
			m_active_text->Disable();
			m_size_text->Disable();
			m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
			Room room = (Room){
				.name = copy_wx_string(m_name_text->GetValue()),
				.short_name = copy_wx_string(m_name_text->GetValue()),
				.size = m_size_text->GetValue(),
				.active = m_active_text->GetValue()
			};
			RoomBasicDataUpdateAction * act = new RoomBasicDataUpdateAction(m_owner, room, id_room);
			bool success = m_owner->Do(act);
			if(success){
				m_rooms_list->EditItem(id_room, wxString::FromUTF8(find_room_by_id(m_owner->m_school, id_room)->name));
			}
		} else {
			m_cancel_btn->Show();
			m_name_text->Enable();
			m_active_text->Enable();
			m_size_text->Enable();
			m_edit_btn->SetLabel(m_owner->m_lang->str_save);
		}
	}
}

void ListRoomsPane::OnCancelButtonClicked(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i_select = m_rooms_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int id_room = ((IntClientData*)m_rooms_list->GetList()->GetClientObject(i_select))->m_value;
		Room * sel_room = find_room_by_id(school, id_room);
		m_cancel_btn->Hide();
		m_name_text->Disable();
		m_active_text->Disable();
		m_size_text->Disable();
		m_edit_btn->SetLabel(m_owner->m_lang->str_edit);

		m_name_text->SetValue(wxString::FromUTF8(sel_room->name));
		m_size_text->SetValue(sel_room->size);
		m_active_text->SetValue(sel_room->active);
	}
}

void ListRoomsPane::OnDeleteButtonClicked(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i_select = m_rooms_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int id_room = ((IntClientData*)m_rooms_list->GetList()->GetClientObject(i_select))->m_value;
		if(can_remove_room(school, id_room)){
			RoomDeleteAction * act = new RoomDeleteAction(m_owner, get_room_index_by_id(m_owner->m_school, id_room));
			bool success = m_owner->Do(act);
			if(success){
				m_rooms_list->RemoveItem(id_room);
			} else {
				printf("Não foi possível apagar.");
			}
		} else {
			wxMessageDialog * dialog = new wxMessageDialog(nullptr, m_owner->m_lang->str_couldnt_delete_because_timetable, m_owner->m_lang->str_error, wxOK);
			dialog->ShowModal();
		}
	}
}

void ListRoomsPane::OnSelectionChanged(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i;
	int i_select = m_rooms_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int id_room = ((IntClientData*)m_rooms_list->GetList()->GetClientObject(i_select))->m_value;
		Room * room = find_room_by_id(school, id_room);
		m_name_text->SetValue(wxString::FromUTF8( room->name ));
		m_size_text->SetValue(room->size);
		m_active_text->SetValue(room->active);

		ChoiceGrid * periods_grid = m_periods->GetGrid();
		periods_grid->GridRemake(school->n_days,school->n_periods_per_day);
		for(i = 0; i < school->n_periods; ++i){
			if(school->periods[i] == false){
				periods_grid->SetCellLocked(i % school->n_periods_per_day, i / school->n_periods_per_day);
			} else {
				periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, room->availability[i] > 0? 1:0);
			}
		}

	}
	Layout();
}

ListRoomsPane::~ListRoomsPane(){

}
