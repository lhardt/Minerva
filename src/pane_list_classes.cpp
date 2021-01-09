#include "gui.hpp"

#include <wx/spinctrl.h>

extern "C" {
	#include "loader.h"
	#include "util.h"
};

ListClassesPane::ListClassesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(250,250,250));
	int i = 0;

	/* Layout */
	wxNotebook  * notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	/* Declaration of members */
	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * size_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_size);
	wxStaticText * free_periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_free_periods);
	wxStaticText * entry_period_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_entry_period);
	wxStaticText * exit_period_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_exit_period);
	wxStaticText * active_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_active);
	wxStaticText * composite_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_is_superclass);
	wxStaticText * m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));
	m_classes_list = new SearchableListPane(m_owner, this, wxID_ANY, wxDefaultPosition, wxSize(230,250));
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150,-1));
	m_active_text = new wxCheckBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
	m_size_text = new wxSpinCtrl(this, wxID_ANY);
	m_free_periods_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_composite_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_entry_period_text = new wxChoice(this, wxID_ANY);
	m_exit_period_text = new wxChoice(this, wxID_ANY);
	m_subjects_text = new wxStaticText(this, wxID_ANY, wxT(""));
	wxButton * delete_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_delete);
	m_basic_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	m_basic_edit_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_edit);

	m_periods = new ScoreGridPane(m_owner,notebook, wxID_ANY);
	m_superclasses = new ScoreGridPane(m_owner,notebook, wxID_ANY);
	m_rooms = new ScoreGridPane(m_owner,notebook, wxID_ANY);

	wxVector<wxString> all_subject_names = wxVector<wxString>();
	wxString subject_col_name = m_owner->m_lang->str_periods;
	for(i = 0; i < school->n_subjects; ++i){
		all_subject_names.push_back(wxString::FromUTF8(school->subjects[i].name));
	}
	m_assignments = new PosIntGridPane(m_owner, notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, subject_col_name, all_subject_names);

	wxVector<wxString> all_sgroup_names = wxVector<wxString>();
	wxString sgroup_col_name = m_owner->m_lang->str_max_number_of_periods_per_day;
	for(i = 0; i < school->n_subject_groups; ++i){
		all_sgroup_names.push_back(wxString::FromUTF8(school->subject_group_names[i]));
	}
	m_groups = new PosIntGridPane(m_owner, notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, sgroup_col_name, all_sgroup_names);

	m_err_msg->SetFont(*m_owner->m_small_font);

	/* Sizers */
	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * description_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * box_field_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_basic_data);
	wxSizer * fields_sizer = new wxFlexGridSizer(4,5,5);

	sizer->Add(m_classes_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(description_sizer, 1, wxEXPAND|wxALL, 15);

	fields_sizer->Add(name_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sizer->Add(m_name_text, 0, wxEXPAND);
	fields_sizer->Add(size_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sizer->Add(m_size_text, 0, wxEXPAND);
	fields_sizer->Add(entry_period_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sizer->Add(m_entry_period_text, 0, wxEXPAND);
	fields_sizer->Add(exit_period_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sizer->Add(m_exit_period_text, 0, wxEXPAND);
	fields_sizer->Add(free_periods_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sizer->Add(m_free_periods_text, 0, wxEXPAND);
	fields_sizer->Add(active_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sizer->Add(m_active_text, 0, wxEXPAND);
	fields_sizer->Add(composite_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sizer->Add(m_composite_text, 0, wxEXPAND);
	fields_sizer->Add(m_basic_cancel_btn, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sizer->Add(m_basic_edit_btn, 0, wxEXPAND);
	box_field_sizer->Add(fields_sizer, 0, wxALL, 5);
	description_sizer->Add(box_field_sizer, 0, wxEXPAND | wxBOTTOM, 15);
	description_sizer->Add(notebook, 1, wxEXPAND | wxBOTTOM, 15);
	description_sizer->Add(btn_sizer, 0, wxEXPAND);
	description_sizer->Add(delete_btn, 0, wxEXPAND);
	description_sizer->Add(m_err_msg, 0, wxEXPAND);

	notebook->InsertPage(0, m_periods, m_owner->m_lang->str_availability);
	notebook->InsertPage(1, m_assignments, m_owner->m_lang->str_subjects);
	notebook->InsertPage(2, m_rooms, m_owner->m_lang->str_rooms);
	notebook->InsertPage(3, m_groups, m_owner->m_lang->str_subject_groups);
	notebook->InsertPage(4, m_superclasses, m_owner->m_lang->str_group);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	/* Functionality and Information */
	m_basic_edit_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnEditButtonClicked, this);
	m_basic_cancel_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnCancelButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnRemoveButtonClicked, this);
	m_classes_list->GetList()->Bind(wxEVT_LISTBOX, &ListClassesPane::OnSelectionChanged, this);
	Bind(DATA_CHANGE_EVENT, &ListClassesPane::OnDataChange, this);

	// AVAILABILITY PANE CODE
	ChoiceGrid * periods_grid = m_periods->GetGrid();
	periods_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	periods_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	periods_grid->SetDefaultColumnLabel(m_owner->m_lang->str_day);
	periods_grid->SetDefaultRowLabel(m_owner->m_lang->str_period);
	periods_grid->GridRemake(school->n_days, school->n_periods_per_day);
	for(i = 0; i < school->n_periods; ++i){
		periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, school->periods[i]?0:-1);
	}

	// ROOM PANE CODE
	ChoiceGrid * rooms_grid = m_rooms->GetGrid();
	rooms_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	rooms_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	rooms_grid->GridRemake(1, school->n_rooms);
	rooms_grid->SetColLabel(0,m_owner->m_lang->str_name);
	for(i = 0; i < school->n_rooms; ++i){
		rooms_grid->SetRowLabel(i, wxString::FromUTF8(school->rooms[i].name));
	}

	if(school->n_classes > 0){
		for(i = 0; i < school->n_classes; ++i){
			m_classes_list->AddItem(school->classes[i].id,wxString::FromUTF8(school->classes[i].name));
		}
	}
	for(i = 0; i < school->n_periods_per_day; ++i){
		m_entry_period_text->Insert(wxString::FromUTF8(school->daily_period_names[i]), i, new IntClientData(i));
		m_exit_period_text->Insert(wxString::FromUTF8(school->daily_period_names[i]), i, new IntClientData(i));
	}

	m_name_text->Disable();
	m_active_text->Disable();
	m_size_text->Disable();
	m_entry_period_text->Disable();
	m_exit_period_text->Disable();
	m_free_periods_text->Disable();
	m_basic_cancel_btn->Hide();

}

void ListClassesPane::OnDataChange(wxNotifyEvent &){
	printf("Data change!\n");
}

void ListClassesPane::OnCancelButtonClicked(wxCommandEvent & ev){
	m_name_text->Disable();
	m_active_text->Disable();
	m_size_text->Disable();
	m_entry_period_text->Disable();
	m_exit_period_text->Disable();
	m_free_periods_text->Disable();
	m_basic_cancel_btn->Hide();
	m_basic_edit_btn->SetLabel(m_owner->m_lang->str_edit);
}

void ListClassesPane::OnEditButtonClicked(wxCommandEvent & ev){
	if(m_basic_cancel_btn->IsShown()){
		m_name_text->Disable();
		m_active_text->Disable();
		m_size_text->Disable();
		m_entry_period_text->Disable();
		m_exit_period_text->Disable();
		m_free_periods_text->Disable();
		m_basic_cancel_btn->Hide();
		m_basic_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	} else {
		m_name_text->Enable();
		m_active_text->Enable();
		m_size_text->Enable();
		m_entry_period_text->Enable();
		m_exit_period_text->Enable();
		m_free_periods_text->Enable();
		m_basic_cancel_btn->Show();
		m_basic_edit_btn->SetLabel(m_owner->m_lang->str_save);
	}
}

void ListClassesPane::OnSelectionChanged(wxCommandEvent & ev){
	int i;
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		Class * c = find_class_by_id(school, class_id); // &school->classes[m_classes_list->GetSelection()];
		m_name_text->SetValue(wxString::FromUTF8(c->name));
		m_size_text->SetValue(c->size);
		m_free_periods_text->SetValue(c->can_have_free_periods_flag);
		m_active_text->SetValue(c->active);
		m_free_periods_text->SetLabel(c->can_have_free_periods_flag? m_owner->m_lang->str_yes : m_owner->m_lang->str_no );
		m_active_text->SetLabel(c->active? m_owner->m_lang->str_yes : m_owner->m_lang->str_no );
		m_entry_period_text->SetSelection(c->maximal_entry_period);
		m_exit_period_text->SetSelection(c->minimal_exit_period);
		m_subjects_text->SetLabel(wxString::FromUTF8(""));
		if(c->assignments != NULL){
			for(i = 0; i < school->n_assignments && c->assignments[i] != NULL; ++i){
				int i_subject = get_subject_index_by_id(school, c->assignments[i]->subject->id);
				m_assignments->GetGrid()->SetCellValue(i_subject,0,wxString::Format("%d",c->assignments[i]->amount));
			}
		}
		for(i = 0; i < school->n_periods; ++i){
			if(school->periods[i]){
				m_periods->GetGrid()->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day,c->period_scores[i] > 0 ? 0:1);
			}
		}
		if(c->max_per_day_subject_group){
			for(i = 0; i < school->n_subject_groups; ++i){
				m_groups->GetGrid()->SetCellValue(i,0, wxString::Format("%d", c->max_per_day_subject_group[i]));
			}
		}
	}
}

void ListClassesPane::OnRemoveButtonClicked(wxCommandEvent & ev){
	bool success = false;
	School * school = m_owner->m_school;

	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int del_id =  ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		int del_i = get_class_index_by_id(school, del_id);
		bool can_delete = can_remove_class(school, del_id);
		if(can_delete){
			success = remove_class(stdout, m_owner->m_database, del_id);
			if(success) {
				/* TODO: substitute for an Action*/
				school_class_remove(school, del_i, true);
				m_classes_list->RemoveItem(del_id);

				m_name_text->SetLabel(wxT(""));
				m_size_text->SetLabel(wxT(""));
				m_free_periods_text->SetLabel(wxT(""));
				m_entry_period_text->SetLabel(wxT(""));
				m_exit_period_text->SetLabel(wxT(""));
				m_subjects_text->SetLabel(wxT(""));
				m_periods->GetGrid()->SetAllCellsState(0);

				m_owner->NotifyNewUnsavedData();
			} else {
				printf("Não foi possível deletar.\n");
			}
		} else {
		   wxMessageDialog * dialog = new wxMessageDialog(nullptr, wxT("(TODO lang) Error! Could not remove, because it is in a timetable already"), m_owner->m_lang->str_error, wxOK);
		   dialog->ShowModal();
		}
	}
}

ListClassesPane::~ListClassesPane(){

}
