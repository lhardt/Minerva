#include "gui.hpp"

#include <wx/spinctrl.h>

#include <iostream>

extern "C" {
	#include "loader.h"
	#include "util.h"
	#include "assert.h"
	#include "logic.h"
};

ListClassesPane::ListClassesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(250,250,250));

	wxNotebook  * notebook = new wxNotebook(this, wxID_ANY);
	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * size_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_size);
	wxStaticText * free_periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_free_periods);
	wxStaticText * entry_period_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_entry_period);
	wxStaticText * exit_period_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_exit_period);
	wxStaticText * active_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_active);
	m_classes_list = new SearchableListPane(m_owner, this, wxID_ANY, wxDefaultPosition, wxSize(230,250));
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150,-1));
	m_active_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_size_text = new wxSpinCtrl(this, wxID_ANY);
	m_free_periods_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_entry_period_text = new wxChoice(this, wxID_ANY);
	m_exit_period_text = new wxChoice(this, wxID_ANY);
	m_subjects_text = new wxStaticText(this, wxID_ANY, wxT(""));
	wxButton * delete_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_delete);
	m_basic_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	m_basic_edit_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_edit);
	m_periods = new ScoreGridPane(m_owner,notebook, wxID_ANY);
	m_class_groups = new ScoreGridPane(m_owner,notebook, wxID_ANY);
	m_rooms = new ScoreGridPane(m_owner,notebook, wxID_ANY);
	wxString subject_col_name = m_owner->m_lang->str_periods;
	m_assignments = new PosIntGridPane(m_owner, notebook, school->n_subjects, wxID_ANY, wxDefaultPosition, wxDefaultSize, subject_col_name);
	wxString sgroup_col_name = m_owner->m_lang->str_max_number_of_periods_per_day;
	m_groups = new PosIntGridPane(m_owner, notebook, school->n_subject_groups, wxID_ANY, wxDefaultPosition, wxDefaultSize, sgroup_col_name);
	/* Sizers */
	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * description_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * box_field_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_basic_data);
	wxSizer * fields_sizer = new wxFlexGridSizer(4,5,5);
	wxSizer * actions_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, m_owner->m_lang->str_actions);

	sizer->Add(m_classes_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(description_sizer, 1, wxALL | wxTOP | wxBOTTOM | wxRIGHT, 15);

	actions_sizer->Add(delete_btn, 0, wxALL, 5);
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
	fields_sizer->AddStretchSpacer();
	fields_sizer->AddStretchSpacer();
	fields_sizer->Add(m_basic_cancel_btn, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sizer->Add(m_basic_edit_btn, 0, wxEXPAND);
	box_field_sizer->Add(fields_sizer, 0, wxALL, 5);
	description_sizer->Add(box_field_sizer, 0, wxEXPAND | wxBOTTOM, 15);
	description_sizer->Add(notebook, 1, wxEXPAND | wxBOTTOM, 15);
	description_sizer->Add(actions_sizer, 0, wxEXPAND);

	notebook->AddPage(m_periods, m_owner->m_lang->str_availability);
	notebook->AddPage(m_assignments, m_owner->m_lang->str_subjects);
	notebook->AddPage(m_rooms, m_owner->m_lang->str_rooms);
	notebook->AddPage(m_groups, m_owner->m_lang->str_subject_groups);
	notebook->AddPage(m_class_groups, m_owner->m_lang->str_group);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_periods->GetSaveButton()->Bind(wxEVT_BUTTON, &ListClassesPane::OnSavePeriods, this);
	m_periods->GetCancelButton()->Bind(wxEVT_BUTTON, &ListClassesPane::OnCancelPeriods, this);
	m_assignments->GetSaveButton()->Bind(wxEVT_BUTTON, &ListClassesPane::OnSaveSubjects, this);
	m_assignments->GetCancelButton()->Bind(wxEVT_BUTTON, &ListClassesPane::OnCancelSubjects, this);
	m_rooms->GetSaveButton()->Bind(wxEVT_BUTTON, &ListClassesPane::OnSaveRooms, this);
	m_rooms->GetCancelButton()->Bind(wxEVT_BUTTON, &ListClassesPane::OnCancelRooms, this);
	m_groups->GetSaveButton()->Bind(wxEVT_BUTTON, &ListClassesPane::OnSaveGroups, this);
	m_groups->GetCancelButton()->Bind(wxEVT_BUTTON, &ListClassesPane::OnCancelGroups, this);
	m_class_groups->GetSaveButton()->Bind(wxEVT_BUTTON, &ListClassesPane::OnSaveClassGroups, this);
	m_class_groups->GetCancelButton()->Bind(wxEVT_BUTTON, &ListClassesPane::OnSaveClassGroups, this);
	m_basic_edit_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnEditButtonClicked, this);
	m_basic_cancel_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnCancelButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnRemoveButtonClicked, this);
	m_classes_list->GetList()->Bind(wxEVT_LISTBOX, &ListClassesPane::OnSelectionChanged, this);
	Bind(DATA_CHANGE_EVENT, &ListClassesPane::OnDataChange, this);


	ChoiceGrid * cgroups_grid = m_class_groups->GetGrid();
	cgroups_grid->GridRemake(1,school->n_classes);
	cgroups_grid->AddState(m_owner->m_lang->str_no, wxColor(255,200,200));
	cgroups_grid->AddState(m_owner->m_lang->str_yes, wxColor(200,200,255));
	cgroups_grid->SetColLabel(0,m_owner->m_lang->str_belongs);
	ChoiceGrid * rooms_grid = m_rooms->GetGrid();
	rooms_grid->GridRemake(1, school->n_rooms);
	rooms_grid->SetColLabel(0,m_owner->m_lang->str_name);
	rooms_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	rooms_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	// AVAILABILITY PANE CODE
	ChoiceGrid * periods_grid = m_periods->GetGrid();
	periods_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	periods_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	periods_grid->SetDefaultColumnLabel(m_owner->m_lang->str_day);
	periods_grid->SetDefaultRowLabel(m_owner->m_lang->str_period);

	m_periods->SetLabel(m_owner->m_lang->str_class_availability);
	m_class_groups->SetLabel(m_owner->m_lang->str_participation_in_class_groups);
	m_groups->SetLabel(m_owner->m_lang->str_max_periods_per_day_subject_group);
	m_rooms->SetLabel(m_owner->m_lang->str_room_preference);
	ShowData();
}

void ListClassesPane::ShowData(){
	School * school = m_owner->m_school;
	int i;

	m_classes_list->Clear();
	for(i = 0; i < school->n_classes; ++i){
		wxString str = wxString::FromUTF8(school->classes[i].name);
		if(school->classes[i].active){
			m_classes_list->AddItem(school->classes[i].id, str);
		} else {
			m_classes_list->AddItem(school->classes[i].id, wxString::Format("(%s) %s", m_owner->m_lang->str_inactive, str));
		}
	}

	ChoiceGrid * rooms_grid = m_rooms->GetGrid();
	rooms_grid->GridRemake(1, school->n_rooms);
	for(i = 0; i < school->n_rooms; ++i){
		rooms_grid->SetRowLabel(i, wxString::FromUTF8(school->rooms[i].name));
	}

	ChoiceGrid * periods_grid = m_periods->GetGrid();
	periods_grid->GridRemake(school->n_days, school->n_periods_per_day);
	for(i = 0; i < school->n_periods; ++i){
		periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, school->periods[i]?0:-1);
	}

	m_entry_period_text->Clear();
	m_exit_period_text->Clear();
	for(i = 0; i < school->n_periods_per_day; ++i){
		m_entry_period_text->Insert(wxString::FromUTF8(school->daily_period_names[i]), i, new IntClientData(i));
		m_exit_period_text->Insert(wxString::FromUTF8(school->daily_period_names[i]), i, new IntClientData(i));
	}

	if(m_groups->GetGrid()->GetNumberRows() != school->n_subject_groups){
		m_groups->ResizeTable(school->n_subject_groups);
	}
	for(i = 0; i < school->n_subject_groups; ++i){
		m_groups->SetRowLabel(i, wxString::FromUTF8(school->subject_group_names[i]));
	}

	if(m_assignments->GetGrid()->GetNumberRows() != school->n_subjects){
		m_assignments->ResizeTable(school->n_subjects);
	}
	for(i = 0; i < school->n_subjects; ++i){
		m_assignments->SetRowLabel(i, wxString::FromUTF8(school->subjects[i].name));
	}

	ChoiceGrid * cgroups_grid = m_class_groups->GetGrid();
	cgroups_grid->GridRemake(1,school->n_classes);
	cgroups_grid->SetAllCellsState(0);
	for(int i = 0; i < school->n_classes; ++i){
		cgroups_grid->SetRowLabel(i, wxString::FromUTF8(school->classes[i].name));
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
	ShowData();
}

void ListClassesPane::OnCancelButtonClicked(wxCommandEvent & ev){
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		m_name_text->Disable();
		m_active_text->Disable();
		m_size_text->Disable();
		m_entry_period_text->Disable();
		m_exit_period_text->Disable();
		m_free_periods_text->Disable();
		m_basic_cancel_btn->Hide();
		m_basic_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	}
}

void ListClassesPane::OnEditButtonClicked(wxCommandEvent & ev){
	int i_select = m_classes_list->GetList()->GetSelection();
	School * school = m_owner->m_school;
	if(i_select != wxNOT_FOUND){
		if(m_basic_cancel_btn->IsShown()){
			int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
			Class * old_class = find_class_by_id(school, class_id);
			Class c = (Class){
				.name = copy_wx_string(m_name_text->GetValue()),
				.short_name = copy_wx_string(m_name_text->GetValue()),
				.size = m_size_text->GetValue(),
				.maximal_entry_period = m_entry_period_text->GetSelection(),
				.minimal_exit_period = m_exit_period_text->GetSelection(),
				.can_have_free_periods_flag = m_free_periods_text->GetValue(),
				.active = m_active_text->GetValue(),
			};
			if(  c.name == NULL || 0 == strcmp(c.name, "")
			  || c.short_name == NULL || 0 == strcmp(c.short_name, "")
			  || c.size < 1
			  || c.maximal_entry_period == wxNOT_FOUND
			  || c.minimal_exit_period == wxNOT_FOUND
			){
				printf("Invalid");
				wxMessageDialog * dialog = new wxMessageDialog(nullptr, wxT("Erro"), wxT("Dados inválidos"), wxOK);
				free(c.name);
				free(c.short_name);
				dialog->ShowModal();
				return;
			}

			Action* act = new ClassBasicDataUpdateAction(m_owner, old_class->id, c);
			if(m_owner->Do(act)){
				m_name_text->Disable();
				m_active_text->Disable();
				m_size_text->Disable();
				m_entry_period_text->Disable();
				m_exit_period_text->Disable();
				m_free_periods_text->Disable();
				m_basic_cancel_btn->Hide();
				m_basic_edit_btn->SetLabel(m_owner->m_lang->str_edit);
			}
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
}

void ListClassesPane::OnSelectionChanged(wxCommandEvent & ev){
	int i;
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		int class_i = get_class_index_by_id(school, class_id);
		Class * c = &school->classes[class_i];
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
				m_assignments->SetCellValue(i_subject,0,c->assignments[i]->amount);
			}
		}
		ChoiceGrid * periods_grid = m_periods->GetGrid();
		for(i = 0; i < school->n_periods; ++i){
			if(school->periods[i]){
				periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day,c->period_scores[i] > 0 ? 1:0);
			}
		}
		if(c->max_per_day_subject_group){
			for(i = 0; i < school->n_subject_groups; ++i){
				m_groups->SetCellValue(i,0,c->max_per_day_subject_group[i]);
			}
		} else {
			printf("Does not have. %d \n", c->id);
			for(i = 0; i < school->n_subject_groups; ++i){
				m_groups->SetCellValue(i,0,0);
			}
		}
		if(c->room_scores){
			ChoiceGrid * rooms_grid = m_rooms->GetGrid();
			for(i = 0; i < school->n_rooms; ++i){
				rooms_grid->SetCellState(i, 0, c->room_scores[i]);
			}
		}

		ChoiceGrid * cgroups_grid = m_class_groups->GetGrid();
		for(i = 0; i < school->n_classes; ++i){
			if(school->classes[i].id != class_id){
				int * this_sub = school->classes[i].subordinates;
				if(this_sub != NULL && this_sub[class_i] > 0){
					cgroups_grid->SetCellState(i,0,1);
				} else {
					cgroups_grid->SetCellState(i,0,0);
				}
			} else {
				cgroups_grid->SetCellState(i,0,-1);
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
				m_periods->GetGrid()->SetAllCellsState(1);

				m_owner->NotifyNewUnsavedData();
			} else {
				printf("Não foi possível deletar.\n");
			}
		} else {
		   wxMessageDialog * dialog = new wxMessageDialog(nullptr, m_owner->m_lang->str_couldnt_delete_because_timetable, m_owner->m_lang->str_error, wxOK);
		   dialog->ShowModal();
		}
	}
}

void ListClassesPane::OnSavePeriods(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		ChoiceGrid * periods_grid = m_periods->GetGrid();
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		int * scores = m_periods->GetValues();

		Action * act = new ClassAvailabilityUpdateAction(m_owner, class_id, scores);
		if(m_owner->Do(act)){
			evt.Skip();
		}
	}
}
void ListClassesPane::OnCancelPeriods(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		ChoiceGrid * periods_grid = m_periods->GetGrid();
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		Class * c = find_class_by_id(school, class_id);
		for(int i = 0; i < school->n_periods; ++i){
			if(school->periods[i]){
				periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, c->period_scores[i]);
			}
		}
	}
}

void ListClassesPane::OnSaveSubjects(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		Class * c = find_class_by_id(school, class_id);

		LMH_ASSERT(c != NULL);
		LMH_ASSERT(class_id > 0);

		int n_assignments = 0;
		int * subject_amount = m_assignments->GetValues();
		for(int i = 0; i < school->n_subjects; ++i){
			LMH_ASSERT(subject_amount[i] >= 0);
			if(subject_amount[i] > 0){
				++n_assignments;
			}
		}
		Assignment * assignments = (Assignment*) calloc(n_assignments, sizeof(Assignment));
		int i_assign = 0;
		for(int i = 0; i < school->n_subjects; ++i){
			if(subject_amount[i] > 0){
				assignments[i_assign] = {
					.id=0,
					.subject=&school->subjects[i],
					.m_class=c,
					.amount=subject_amount[i],
					.max_per_day=school->n_periods_per_day,
					.possible_teachers=NULL, /* Negative-terminated score list*/
				};
				Assignment * old_assignment = find_assignment_by_class_subj_id(school, class_id, school->subjects[i].id);
				if(old_assignment != NULL){
					assignments[i_assign].max_per_day = old_assignment->max_per_day;
					if(old_assignment != NULL){
						printf("Old possible teachers: ");
						print_int_list(stdout, old_assignment->possible_teachers);
						printf("\n");
						assignments[i_assign].possible_teachers = int_list_copy(old_assignment->possible_teachers);
					}
				}

				if(assignments[i_assign].possible_teachers == NULL){
					assignments[i_assign].possible_teachers = (int*)calloc(school->n_teachers + 1, sizeof(int));
					assignments[i_assign].possible_teachers[school->n_teachers] = -1;
					for(int j = 0; j < school->n_teachers; ++j){
						Teaches * t = find_teaches_by_teacher_subj_id(school, school->teachers[j].id, school->subjects[i].id);
						if(t != NULL){
							LMH_ASSERT(t->score > 0);
							assignments[i_assign].possible_teachers[j] = t->score;
						}
					}
				}
				++i_assign;
			}
		}

		printf("Class id is %d, List of assignments (%d):\n", c->id, n_assignments);
		for(int i = 0; i < n_assignments; ++i){
			printf("Assignment with amount %d idclass %d and idsubj %d. tscores %x\n", assignments[i].amount, assignments[i].m_class->id, assignments[i].subject->id, assignments[i].possible_teachers);
		}

		LMH_ASSERT(n_assignments == i_assign);

		Action * act = new ClassAssignmentsUpdateAction(m_owner, c->id, n_assignments, assignments);

		bool success = m_owner->Do(act);
		if(success){
			evt.Skip();
		}
	}
}
void ListClassesPane::OnCancelSubjects(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		Class * c = find_class_by_id(school, class_id);

	}
}

void ListClassesPane::OnSaveRooms(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;

		int * scores = (int*) calloc(school->n_rooms + 1, sizeof(int));
		ChoiceGrid *rooms_grid = m_rooms->GetGrid();
		for(int i = 0; i < school->n_rooms; ++i){
			scores[i] = rooms_grid->GetCellState(i,0);
			if(scores[i] == -1){
				scores[i] = 0;
			}
		}
		scores[school->n_rooms] = -1;

		Action * act = new ClassRoomsUpdateAction(m_owner, class_id, scores);

		if(m_owner->Do(act)){
			evt.Skip();
		} else {
			free(scores);
		}
	}
}

void ListClassesPane::OnCancelRooms(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		Class * c = find_class_by_id(school, class_id);

		ChoiceGrid * rooms_grid = m_rooms->GetGrid();
		if(c->room_scores != NULL){
			for(int i = 0; i < school->n_rooms; ++i){
				rooms_grid->SetCellState(i, 0,c->room_scores[i]);
			}
		} else {
			rooms_grid->SetAllCellsState(0);
		}
	}
}

void ListClassesPane::OnSaveGroups(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		Class * c = find_class_by_id(school, class_id);

		int * max = m_groups->GetValues();
		Action * act = new ClassSubjectGroupsUpdateAction(m_owner, class_id, max);

		if(m_owner->Do(act)){
			evt.Skip();
		}
	}
}
void ListClassesPane::OnCancelGroups(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		Class * c = find_class_by_id(school, class_id);

	}
}

void ListClassesPane::OnSaveClassGroups(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		Class * c = find_class_by_id(school, class_id);

	}
}
void ListClassesPane::OnCancelClassGroups(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i_select = m_classes_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int class_id = ((IntClientData*)m_classes_list->GetList()->GetClientObject(i_select))->m_value;
		Class * c = find_class_by_id(school, class_id);

	}
}

ListClassesPane::~ListClassesPane(){

}
